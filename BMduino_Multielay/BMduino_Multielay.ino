#include "RelayLib.h"   // 引入繼電器模組的函式庫，內含繼電器控制函式（如 TurnonRelay、TurnoffRelay）
#include "OledLib.h"    // 引入 OLED 顯示模組的函式庫，內含 OLED 初始化與顯示控制函式

//--------------------------------
// 函式宣告區（提前告訴編譯器有哪些自訂函式）
void initSensor();   // 初始化所有感測模組的函式（OLED、繼電器）
void initAll();      // 初始化整體系統的函式（序列埠、感測模組）

//---------------------------------
void setup() 
{
  initAll();   // 初始化整體系統（包含序列埠與感測模組）
               // 其中 WiFi 模組的初始化與連線在其他檔案中定義（TCP.h / BMC81M001.h）
  delay(200);  // 延遲 200 毫秒，確保硬體模組有足夠時間完成上電穩定化

  //---------------------------------
  clearScreen();   // 清除 OLED 螢幕內容，讓顯示畫面為空白狀態
}

void loop() 
{
    TurnonRelay(1);              // 開啟第 1 號繼電器，讓接在 Relay1 的裝置通電
    Serial.println("Relay1 is on");   // 在序列埠監控視窗輸出狀態資訊：Relay1 已開啟
    delay(2000);                 // 延遲 2 秒，維持 Relay1 開啟狀態

    TurnonRelay(2);              // 開啟第 2 號繼電器
    Serial.println("Relay2 is on");   // 在序列埠監控視窗輸出狀態資訊：Relay2 已開啟
    delay(2000);                 // 延遲 2 秒，維持 Relay2 開啟狀態

    TurnoffRelay(1);             // 關閉第 1 號繼電器，讓接在 Relay1 的裝置斷電
    Serial.println("Relay1 is off");  // 在序列埠監控視窗輸出狀態資訊：Relay1 已關閉
    delay(2000);                 // 延遲 2 秒，維持 Relay1 關閉狀態

    TurnoffRelay(2);             // 關閉第 2 號繼電器
    Serial.println("Relay2 is off");  // 在序列埠監控視窗輸出狀態資訊：Relay2 已關閉
    delay(2000);                 // 延遲 2 秒，維持 Relay2 關閉狀態
}

//----------初始化所有感測模組------------
void initSensor()   // 初始化所有感測模組
{
    initOled();     // 初始化 OLED12864 顯示模組（BMD31M090，0.96 吋 I2C OLED）
    delay(2000);    // 延遲 2 秒，確保 OLED 上電並完成初始化
    initRealy();    // 初始化繼電器模組（設定控制腳位與預設狀態）
}

//---------初始化整體系統函式------
void initAll()   // 初始化整體系統的函式
{
  Serial.begin(9600);  // 啟動序列埠，並設定鮑率為 9600 bps
                       // 主要用於監控裝置狀態與除錯訊息輸出
  initSensor();        // 呼叫 initSensor()，完成感測模組的初始化
}
