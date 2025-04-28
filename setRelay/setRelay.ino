/*************************************************
File:             controlRelay.ino
Description:      A relay module is controlled by IIC as an LED switch to control the switching of the LED once per second.
Note:               
**************************************************/

#include <BMP75M131.h> // 引入 BMP75M131 的庫

//BMP75M131 myRelay(&Wire); // 使用 Wire（I2C 接口）作為通訊接口，若不使用 Wire 請註解掉這行
BMP75M131 myRelay(&Wire1); // 使用 Wire1（I2C 接口）作為通訊接口，若在 BMduino 上使用 Wire1 請取消註解
//BMP75M131 myRelay(&Wire2); // 使用 Wire2 作為通訊接口，若不使用 Wire 請註解掉這行

void setup() {
  Serial.begin(9600); // 設定序列監控的通訊速率為 9600（用於偵錯輸出）
  myRelay.begin(); // 初始化繼電器模組
}

void loop() 
{
  uint8_t relayStatus; // 定義變數儲存繼電器的狀態

  // 設定繼電器開關（第1路繼電器，1 表示關閉繼電器）
  myRelay.setRelaySwitch(1, 1);
  Serial.println("Close the relay switch"); // 顯示關閉繼電器的訊息

  // 取得第1路繼電器的狀態
  relayStatus = myRelay.getRelayStatus(1);
  Serial.print("Get the relay switch status value is: ");
  Serial.println(relayStatus); // 顯示繼電器的狀態值

  delay(1000); // 延遲 1 秒

  // 開啟繼電器開關（第1路繼電器，0 表示開啟繼電器）
  myRelay.setRelaySwitch(1, 0);
  Serial.println("Open the relay switch"); // 顯示開啟繼電器的訊息

  // 取得第1路繼電器的狀態
  relayStatus = myRelay.getRelayStatus(1);
  Serial.print("Get the relay switch status value is: ");
  Serial.println(relayStatus); // 顯示繼電器的狀態值

  delay(1000); // 延遲 1 秒
}
