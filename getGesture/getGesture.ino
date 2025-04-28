/*****************************************************************
File:         getGesture.ino
Description:  1.SoftwareSerial interface (BAUDRATE 9600)is used to communicate with BMS31M002.
              2.hardware Serial (BAUDRATE 9600) is used to communicate with Serial port monitor.
                When there is a gesture,read the proximity sensor status displayed on the serial port monitor.
              3.Slide the left and the serial port monitor prints "Swipe left".
                Swipe right and the serial monitor prints "Swipe right"
connection method： intPin:D3 rxPin:D5 txPin:D4
******************************************************************/
#include "BM32S3021-1.h" // 引入 BM32S3021-1 函式庫
//BM32S3021_1     myGesture(3,5,4); // intPin, rxPin, txPin，如果不使用軟體序列通訊請註解這行
BM32S3021_1     myGesture(22, &Serial1); // 如果使用 BMduino 的硬體序列通訊 1，請取消註解這行
//BM32S3021_1     myGesture(25, &Serial2); // 如果使用 BMduino 的硬體序列通訊 2，請取消註解這行
//BM32S3021_1     myGesture(3, &Serial3); // 如果使用 BMduino 的硬體序列通訊 3，請取消註解這行
//BM32S3021_1     myGesture(3, &Serial4); // 如果使用 BMduino 的硬體序列通訊 4，請取消註解這行

uint8_t irStatus = 0; // 用來儲存紅外線感測器的狀態

void setup() {
  myGesture.begin(); // 初始化手勢感測器
  Serial.begin(9600); // 設定序列監視器與 BMS31M002 之間的通訊速率為 9600
}

void loop() { 
  if (!myGesture.getINT()) { // 檢查中斷信號，為 0 時表示有新的感測數據
    irStatus = myGesture.getIRStatus(); // 取得接近感測狀態，0 表示未接近，1 表示接近
    if (!(irStatus & 0x08)) { // 校準完成時 BIT3 = 0，表示感測器已校準
      if (irStatus & 0x02) { // 判斷是否從左向右滑動
        Serial.println("Swipe right"); // 顯示「向右滑動」
      } else if (irStatus & 0x04) { // 判斷是否從右向左滑動
        Serial.println("Swipe left"); // 顯示「向左滑動」
      }
    }
  }
}
