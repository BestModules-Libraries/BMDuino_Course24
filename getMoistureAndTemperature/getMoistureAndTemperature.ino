/*************************************************
File:             getMoistureAndTemperature.ino
Description:      Development board is connected with a single module 
                  to obtain the soil moisture detection value and temperature value of the module 
                  and display them on the serial port monitor
Note:               
**************************************************/
#include "BME34M101.h"

// BME34M101 mySoilMoistureSensor(5,4); // 如果不使用 Serial 請註解掉這一行程式碼
BME34M101 mySoilMoistureSensor(&Serial1); // 如果在 BMduino 上使用 Serial1，請註解掉這一行程式碼
// BME34M101 mySoilMoistureSensor(&Serial2); // 如果在 BMduino 上使用 Serial2，請註解掉這一行程式碼

void setup()
{
  Serial.begin(115200); // 設定 Serial 通訊速率為 115200
    
  mySoilMoistureSensor.begin(); // 初始化土壤濕度感測器
  Serial.println("檢查模組是否已連接，等待中...");  
  while(mySoilMoistureSensor.isConnected() == false) // 檢查模組是否連接
  {
    delay(1000); // 每 1 秒檢查一次
  }
  Serial.println("模組已連接。");
}

void loop() 
{
  Serial.print("取得土壤濕度值: ");
  Serial.println(mySoilMoistureSensor.getMoisture()); // 讀取並顯示土壤濕度值
  delay(100); // 延遲 100 毫秒

  Serial.print("取得模組溫度值: ");
  Serial.println(mySoilMoistureSensor.getTemperature()); // 讀取並顯示模組溫度值
  delay(1000); // 延遲 1 秒
}
