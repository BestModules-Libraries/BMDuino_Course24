/*****************************************************************
File:         TCP.ino
Description:  
1. 使用硬體序列埠 Serial1（或其他，如 Serial2）與 WiFi 模組 BMC81M001 通訊，通訊速率為 115200 bps
2. 使用主序列埠 Serial 作為監控序列埠，印出 BMC81M001 的狀態資訊
3. 開機後會自動執行 WiFi 模組的連網動作
4. 當主序列埠（USB傳送）有資料輸入時，可直接透過模組轉送給 TCP 伺服器
******************************************************************/
#include "TCP.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）

void setup() 
{
  digitalWrite(LED, LOW);  // 將 LED 腳位設定為 LOW（熄滅），可視為開機狀態指示

  Serial.begin(9600);      // 啟動主機序列埠（USB）通訊，速率為 9600 bps，作為監控輸出
  initWiFi();              // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）

  Serial.println("");      // 印出空行，純粹格式美觀
  Serial.println(GetMAC()); // 印出 WiFi 模組的 MAC 位址（用於識別裝置）
  Serial.println("----------------"); // 分隔線，美觀用途
  Serial.println(GetIP()); // 印出 WiFi 模組的 MAC 位址（用於識別裝置）
  Serial.println("----------------"); // 分隔線，美觀用途


}


void loop() 
{
  // 目前未定義任何行為。可擴充如下：
  // - 接收 Serial 輸入後轉送 TCP
  // - 定時與伺服器互動
  // - 接收伺服器資料後解析
}


