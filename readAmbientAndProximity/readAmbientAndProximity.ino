 /*****************************************************************
File:         readAmbientAndProximity.ino
Description:  repeatedly obtain the Ambient and Proximity value 
              through IIC and display the value in the serial port.
******************************************************************/
#include "BMS33M332.h" // 引入 BMS33M332 感測器的程式庫

BMS33M332  Alsps(8);   // 使用 Pin8 作為中斷腳位 (INTPIN)
uint16_t alsValue;     // 宣告變數 alsValue 儲存光感測數值
uint16_t psValue;      // 宣告變數 psValue 儲存接近感測數值

void setup() 
{
   Serial.begin(9600);               // 初始化序列埠通訊，設定傳輸速率為 9600 bps
   Alsps.begin();                    // 初始化 BMS33M332 感測器
}

void loop()
{
   psValue = Alsps.readRawProximity(); // 讀取原始接近感測值
   Serial.print("Data_PS : ");        // 顯示接近感測數值標題
   Serial.print(psValue);             // 顯示接近感測的數值
   alsValue = Alsps.readRawAmbient(); // 讀取原始環境光感測值
   Serial.print("   Data_ALS : ");    // 顯示環境光感測數值標題
   Serial.print(alsValue);            // 顯示環境光感測的數值
   Serial.println();                  // 換行
   delay(1000);                       // 延遲 1000 毫秒（1 秒）
}
