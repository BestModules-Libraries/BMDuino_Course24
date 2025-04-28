/*****************************************************************
File:        readTemperatureAndHumidityWithIIC
Description: Power on to obtain the equipment information once, 
             including (SN/PID/VER), and then repeatedly obtain 
             the temperature and humidity value through IIC and 
             display the value in the serial port.
******************************************************************/
#include "BM25S2021-1.h"   // 引入BM25S2021-1感測器的函式庫
//BM25S2021_1 BMht(&Wire); // 如果使用Wire接口可以使用這一行
BM25S2021_1 BMht(&Wire1);  // 使用Wire1接口初始化BM25S2021-1物件

void setup() 
{
    BMht.begin();           // 初始化感測器
    Serial.begin(9600);     // 設定串口通訊，傳輸速率為9600
    Serial.println("=======DeviceInfo(HEX)========= ");
    
    Serial.print("SN : ");  // 顯示裝置序號（SN）
    Serial.println(BMht.getSN(), HEX);  // 取得並顯示感測器的序號，格式為16進位
    Serial.print("PID : "); // 顯示裝置產品ID（PID）
    Serial.println(BMht.getPID(), HEX); // 取得並顯示產品ID，格式為16進位
    Serial.print("VER : "); // 顯示韌體版本（VER）
    Serial.println(BMht.getFWVer(), HEX); // 取得並顯示韌體版本，格式為16進位
    
    Serial.println("=============================== ");
    delay(2000);            // 延遲2秒
}

void loop() 
{
    Serial.print("Humidity : ");         // 顯示濕度標籤
    Serial.print(BMht.readHumidity());   // 讀取並顯示濕度數值
    Serial.print(" %    ");              // 顯示濕度的百分比單位
    Serial.print("Temperature : ");      // 顯示溫度標籤
    Serial.print(BMht.readTemperature()); // 讀取並顯示溫度數值
    Serial.println(" °C ");              // 顯示溫度的攝氏單位
    delay(2000);                         // 延遲2秒後再次讀取
}
