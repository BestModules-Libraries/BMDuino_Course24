/*************************************************
檔案名稱：controlRelay.ino
功能描述：
1. 透過 I2C（Wire/Wire1/Wire2）介面控制 BMP75M131 繼電器模組。
2. 每秒切換一次繼電器（類似開關 LED）。
3. 並將狀態輸出到序列埠監控視窗（Serial Monitor）。
注意事項：
- 適用於使用 BMduino 平台與 BMP75M131 模組連接。
- 須依據實際接線選擇 Wire/Wire1/Wire2。

myRelay.begin()	初始化繼電器模組
setRelaySwitch(channel, state)	控制第 channel 號繼電器的開關，state = 1 為開啟，0 為關閉
getRelayStatus(channel)	讀取第 channel 號繼電器的目前狀態（回傳 0 或 1）

如果你有 多顆繼電器 模組（BMP75M131 支援多通道），可以使用 setRelaySwitch(2, 1) 控制第 2 號等等。

**************************************************/

#include <BMP75M131.h>  // 引入 BMP75M131 繼電器模組專用函式庫

// 建立一個 BMP75M131 繼電器物件，指定使用 Wire1 I2C 通訊（適用於 BMduino）
// BMP75M131 myRelay(&Wire);   // 如果使用預設 I2C（Wire），取消這行註解
BMP75M131 myRelay(&Wire1);     // 如果使用 Wire1（例如 BMduino 上的第二組 I2C 腳位）
 // BMP75M131 myRelay(&Wire2); // 如果使用 Wire2，可啟用這行

void setup() {
  Serial.begin(9600);       // 啟動序列埠，波特率設為 9600，便於除錯輸出

  myRelay.begin();          // 初始化 BMP75M131 繼電器模組，啟用 I2C 通訊
}

void loop() 
{
  uint8_t relyaStatus;  // 宣告變數用來儲存目前繼電器的狀態（0 = 關，1 = 開）

  // === 開啟繼電器開關 ===
  myRelay.setRelaySwitch(1, 1);  // 控制第 1 號繼電器，設定為 "1"（開啟）
  Serial.println("Close the relay switch");  // 輸出訊息：關閉開關（其實這代表開啟繼電器）

  relyaStatus = myRelay.getRelayStatus(1);  // 取得第 1 號繼電器的狀態
  Serial.print("Get the relay switch status valus is: ");
  Serial.println(relyaStatus);              // 印出目前繼電器狀態（0 或 1）

  delay(1000);  // 延遲 1 秒（1000 毫秒）

  // === 關閉繼電器開關 ===
  myRelay.setRelaySwitch(1, 0);  // 控制第 1 號繼電器，設定為 "0"（關閉）
  Serial.println("Open the relay switch");  // 輸出訊息：開啟開關（代表關閉繼電器）

  relyaStatus = myRelay.getRelayStatus(1);  // 再次取得第 1 號繼電器狀態
  Serial.print("Get the relay switch status valus is: ");
  Serial.println(relyaStatus);              // 印出目前繼電器狀態

  delay(1000);  // 再延遲 1 秒
}
