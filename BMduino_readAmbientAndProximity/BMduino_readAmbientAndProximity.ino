/*****************************************************************
File:         readAmbientAndProximity.ino
Description:  重複透過 I2C 通訊介面讀取環境光（Ambient Light）與接近感測值（Proximity），
              並將讀取到的數值顯示在序列埠監控視窗中。
補充說明：
BMS33M332 是一種結合環境光與接近感測功能的 I2C 感測模組。

readRawProximity()：回傳接近感測的原始數值，通常可用於距離偵測。

readRawAmbient()：回傳環境光感測器的原始亮度值，可用於亮度判斷（如自動調光等應用）
******************************************************************/

#include "BMS33M332.h"  // 引入 BMS33M332 感測模組的函式庫

// 建立一個 BMS33M332 物件 Alsps，並設定 INT (中斷腳位) 為第 8 腳

//BMS33M332  Alsps(8,&Wire);   // 連接到板子上的I2C腳位(A4/A5)* int Pin = D8
BMS33M332  Alsps(22,&Wire1);   //連接到板子上的BMCOM1(Wire1 int Pin = 22)
//BMS33M332  Alsps(25,&Wire2);   //連接到板子上的BMCOM2(Wire1 int Pin = 25)

// 宣告 16 位元整數變數，用來儲存感測器讀取到的環境光與接近數值
uint16_t alsValue;  // ALS：Ambient Light Sensor 環境光感測值
uint16_t psValue;   // PS：Proximity Sensor 接近感測值

void setup() 
{
   Serial.begin(9600);  // 初始化序列埠通訊，設定鮑率為 9600，用來在序列監控視窗輸出資料
   Alsps.begin();       // 初始化感測器模組，啟動 I2C 與感測功能
}

void loop()
{
   // 讀取接近感測器的原始資料，儲存到 psValue
   psValue = Alsps.readRawProximity();
   Serial.print("Data_PS : "); 
   Serial.print(psValue); // 在序列埠輸出接近感測數值

   // 讀取環境光感測器的原始資料，儲存到 alsValue
   alsValue = Alsps.readRawAmbient();
   Serial.print("   Data_ALS : ");
   Serial.print(alsValue); // 在序列埠輸出環境光感測數值

   Serial.println();  // 換行，方便下次輸出閱讀

   delay(1000); // 延遲 1000 毫秒（1 秒），每秒執行一次感測與輸出
}
