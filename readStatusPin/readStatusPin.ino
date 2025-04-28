/*****************************************************************
File:         readStatusPin     
Description:  LED will have different actions for different states of the module.
              a. The module is normal: flag 0. There are no intrusion alerts. Led13 lights up for 100 milliseconds.
              b. Module alarm: sign 1, object/person passing is detected, and led 13 keeps 3S
              When a module status send change is detected, the status information will be printed on the serial port monitor.
******************************************************************/ 
#include "BM22S4221-1.h" // 引入 BM22S4221-1 的庫
uint8_t flag; // 定義旗標變數，用於偵測狀態變化
uint8_t STATUS = 5; // 設定狀態腳位為 5

//BM22S4221_1 PIR(STATUS,6,7);// 使用軟體序列（intPin 5, rxPin 6 , txPin 7），若不使用軟體序列請註解掉這行
BM22S4221_1 PIR(22, &Serial1); // 使用硬體序列1（HW Serial1），若使用 BMduino 的硬體序列1，請取消這行的註解
//BM22S4221_1 PIR(29, &Serial2); // 使用硬體序列2，若使用 BMduino 的硬體序列2，請取消這行的註解
//BM22S4221_1 PIR(STATUS, &Serial3); // 使用硬體序列3，若使用 BMduino 的硬體序列3，請取消這行的註解
//BM22S4221_1 PIR(STATUS, &Serial4); // 使用硬體序列4，若使用 BMduino 的硬體序列4，請取消這行的註解

void setup() {
  Serial.begin(9600); // 設定序列通訊，鮑率為 9600
  PIR.begin(); // 初始化 PIR 模組
  pinMode(STATUS, INPUT); // 將 STATUS 腳位設為輸入
  pinMode(13, OUTPUT); // 設定腳位 13 為輸出（用於控制 LED）
}

void loop() {
  // 當偵測到物體並且 flag 不為 1 時觸發警報
  if(PIR.getSTATUS() == HIGH && flag != 1)
  { 
    Serial.println("Alarm! an object passes by"); // 顯示警報訊息
    flag = 1; // 設定旗標為 1，表示警報狀態
  }

  // 當狀態恢復正常並且 flag 不為 0 時顯示正常訊息
  if(flag != 0 && PIR.getSTATUS() == LOW)
  {
    flag = 0; // 重置旗標
    Serial.println("Module normal; no alarm"); // 顯示無警報訊息
  }

  // 根據 flag 狀態改變 LED 的閃爍模式
  switch(flag)
  {
    case 0: // 無警報狀態
      digitalWrite(13, HIGH); // 快速閃爍一次
      delay(100);
      digitalWrite(13, LOW);
      delay(900);
      break;

    case 1: // 警報狀態
      digitalWrite(13, HIGH); // LED 長亮
      delay(1000);
      break;
  }
}
