/*****************************************************************
File:         readIrStateAndControlMotor.ino
Description:  1.The delay of 0.5s after power-on is to ensure that the initialization of the BMS31M001 is completed.
              2.SoftwareSerial interface (BAUDRATE 9600)is used to communicate with BMS31M001.
              3.hardware Serial (BAUDRATE 9600) is used to communicate with Serial Monitor. 
              4.Distance learning mode (85cm) : Please place the paper 85cm away from the module .
                When the module finishes learning, the calibration result will be printed.
              5.Loop to obtain proximity sensing state. If the induction is successful, print the signal value
                on the serial monitor. If the reading of the signal value fails, print "communication fail". 
connection method： rxPin:D2 txPin:D3
******************************************************************/
#include "BM32S2031-1.h" // 引入 BM32S2031-1 感測模組的庫

//BM32S2031_1 BMS31(2, 3); // 使用軟體序列 (SW Serial)，接收引腳為 2，傳輸引腳為 3，若不使用軟體序列請註解掉這行
BM32S2031_1 BMS31(&Serial1); // 使用硬體序列 Serial1，若使用 BMduino 上的硬體 Serial1，請取消註解
//BMS31M001 BMS31(&Serial2); // 使用硬體序列 Serial2，若使用 BMduino 上的硬體 Serial2，請取消註解
//BMS31M001 BMS31(&Serial3); // 使用硬體序列 Serial3，若使用 BMduino 上的硬體 Serial3，請取消註解
//BMS31M001 BMS31(&Serial4); // 使用硬體序列 Serial4，若使用 BMduino 上的硬體 Serial4，請取消註解

uint8_t irStatus = 0; // 定義變數儲存紅外線偵測的狀態
int16_t signalValue = 0; // 定義變數儲存信號值
unsigned long startTime = 0; // 定義變數儲存距離學習的開始時間
uint8_t flag = 1; // 定義旗標變數，用於記錄學習狀態

void setup() 
{  
    BMS31.begin(); // 初始化 BMS31 感測模組
    Serial.begin(9600); // 設定序列通訊速率為 9600（用於 BMS31M001 和序列監控之間）
    startTime = millis(); // 獲取距離學習的開始時間
    flag = BMS31.distanceLearning(); // 開始距離學習，並將結果儲存至 flag

    // 判斷距離學習是否成功
    if(flag == 0) // 距離學習成功
    {
      Serial.println("Distance learning success");
    }  
    if(flag == 1) // 距離學習失敗
    {
      Serial.println("Distance learning fail");
    }  
}

void loop() 
{
   // 取得接近感應的狀態，0 表示未接近，1 表示接近
   if(BMS31.getIRStatus()) 
   {
     // 取得紅外線信號的強度值
     signalValue = BMS31.readIRSignalValue();
     Serial.print(" SignalValue:");  
     Serial.println(signalValue); // 輸出信號值
   } 
}
