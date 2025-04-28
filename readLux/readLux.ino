/*****************************************************************************************************
File:             readLux.ino
Description:      This example demonstrates how to initialize the VEML7700 and then get the ambient light lux.
Note:程式碼會在每秒顯示一次感測器1的光照度數值。             
******************************************************************************************************/
#include "BME82M131.h"   // 引入BME82M131感測器的函式庫

BME82M131 ALS;           // 建立一個BME82M131物件，用來控制光照度感測器

void setup()
{
  Serial.begin(9600);    // 設定串口通訊，傳輸速率為9600
  ALS.begin();           // 初始化光照度感測器
  Serial.print(ALS.getNumber());   // 取得並顯示感測器的編號數量
  Serial.print(" modules are ");
  Serial.println("Connected!");   // 顯示「模組已連接」訊息
  Serial.println("modle is Connected!"); // 顯示「模組已連接」訊息
}

void loop()
{
  Serial.print("Lux1:");   // 顯示感測器1的光照度值標籤
  Serial.println(ALS.readLux(1));   // 讀取並顯示感測器1的光照度值
  delay(1000);             // 等待1秒後再次讀取
}
