/*****************************************************************
File:        readTemperatureAndHumidityWithIIC
Description: 開機後透過 I2C 介面讀取一次裝置資訊（包含序號SN、產品ID PID、韌體版本VER），
             接著不斷重複讀取溫濕度資料，並透過序列埠輸出顯示。
******************************************************************/

#include "BM25S2021-1.h"  // 引入 BM25S2021-1 溫濕度感測模組的函式庫

// 建立一個 BM25S2021_1 的物件，命名為 BMht，並使用 I2C（Wire）通訊方式
BM25S2021_1 BMht(&Wire);

void setup() 
{
    Serial.begin(9600);         // 初始化序列埠，設定鮑率為 9600，用於與電腦通訊顯示資料
   BMht.begin();               // 初始化感測器，啟動 I2C 通訊
 
    // 顯示裝置的基本資訊：序號 SN、產品 ID、韌體版本
    Serial.println("=======DeviceInfo(HEX)========= ");
    Serial.print("SN : ");
    Serial.println(BMht.getSN(), HEX);       // 以十六進位格式顯示序號
    Serial.print("PID : ");
    Serial.println(BMht.getPID(), HEX);      // 顯示產品 ID
    Serial.print("VER : ");
    Serial.println(BMht.getFWVer(), HEX);    // 顯示韌體版本
    Serial.println("=============================== ");       

    delay(2000);                // 延遲 2 秒，讓使用者有時間閱讀裝置資訊
}

void loop() 
{
    // 從感測器讀取濕度數值並顯示
    Serial.print("Humidity : ");
    Serial.print(BMht.readHumidity());       // 顯示濕度值
    Serial.print(" %    ");                  // 顯示濕度的單位 %

    // 從感測器讀取溫度數值並顯示
    Serial.print("Temperature : ");
    Serial.print(BMht.readTemperature());    // 顯示溫度值
    Serial.println(" °C ");                  // 顯示溫度單位 °C

    delay(2000);                // 每隔 2 秒讀取並更新一次溫濕度資料
}
