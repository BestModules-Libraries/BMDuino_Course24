/********************* 模組／函式庫引用 *************************/
// Wi-Fi/TCP 連線功能（內含連線、HTTP、狀態檢查等封裝）
#include "TCP.h"
// OLED 128x64 顯示模組控制（清屏、畫圖、印字串等）
#include "OledLib.h"
// 血氧／心率感測器模組（BMH83M002；封裝 begin、量測、讀封包等）
#include "OxygenLib.h"
// 雲端上傳封裝（HTTP/MQTT 相關；含 SendtoClouding() 等）
#include "clouding.h"

#include <String.h>  // Arduino 內建字串類別（動態字串處理）
// #include "commlib.h" // 若有共用通訊工具（封包/緩衝區）可再開啟

/********************* 自訂函式宣告（介面） *************************/
// ── 系統／感測初始化 ──
void initSensor();                 // 初始化顯示器等「感測/周邊模組」
void initAll();                    // 初始化整體系統（序列埠 + 感測等）
void INITtWIFI();                  // 初始化 Wi-Fi 連線並讀回 SSID/IP/MAC

// ── OLED 顯示輔助：把指定字串印在指定列(row) ──
//   row：第幾列（由各自 OledLib 的列高而定，常見 0~7）
//   ss ：要顯示的字串
void showDeviceonOled(String ss, int row);  // 顯示「Device ID」
void showTitleonOled(String ss, int row);   // 顯示抬頭／任意標題
void showIPonOled(String ss, int row);      // 顯示 IP Address
void showMACeonOled(String ss, int row);    // 顯示 MAC Address
void showSSIDeonOled(String ss, int row);   // 顯示 Wi-Fi SSID

/********************* 全域變數（與網路相關） *************************/
String SSIDData;   // 目前連線的 Wi-Fi 熱點名稱
String IPData;     // 由 AP 分配給本機的 IPv4 位址
// String MacData; // MAC 在其它檔或外部已宣告為全域（此處不重複宣告）

/* 其他與血氧模組／雲端上傳相關的全域變數與函式（於其它檔案）：
   - extern uint8_t Mode, readok;         // 由 OxygenLib 範例或你先前程式定義
   - void initOxygen(), readOxygen();     // 血氧模組初始化／啟動量測
   - void Mode_ask(), Mode_continuous_timing(); // 查詢／連續模式讀取
   - void SendtoClouding();               // 雲端上傳（clouding.h）
*/

/********************* Arduino 生命週期：setup() *************************/
void setup()
{
  initAll();        // 1) 啟動序列埠、初始化感測/顯示等
  delay(200);       // 小延遲，讓硬體穩定（電壓/時鐘就緒）
  INITtWIFI();      // 2) 啟動 Wi-Fi、取得 SSID/IP/MAC 等資訊

  // ── 開機歡迎畫面（OLED 顯示廠牌 LOGO） ──
  clearScreen();                            // 清除 OLED 畫面
  drawPicture(0, 0, BestModule_LOGO, 128, 64); // 顯示 LOGO（全螢幕 128x64）
  delay(3000);                              // LOGO 停留 3 秒
  clearScreen();                            // 再清一次，準備顯示狀態資訊

  // ── 顯示網路基本資訊 ──
  showTitleonOled(MacData, 0);  // 第 0 列：顯示裝置 MAC（可當裝置 ID）
  showIPonOled(IPData, 2);      // 第 2 列：顯示本機 IP（便於除錯/辨識）

  // ── 血氧模組初始化與第一次量測設定 ──
  initOxygen();                         // 初始化血氧感測器（設定模式/間隔）
  Serial.println("Enter Loop()");       // 提示即將進入主迴圈
  delay(2000);                          // 預留時間讓使用者把手指放上感測器
  readOxygen();                         // 啟動量測並判定模式（查詢/連續）
}

/********************* Arduino 生命週期：loop() *************************/
void loop()
{
  // Mode 值由 readOxygen()/getModeConfig() 取得（0：連續、1：查詢）
  switch (Mode)
  {
    case 1:  // 查詢模式（主動要求一次資料）
      Mode_ask();                      // 向感測器索取資料封包
      if (readok != 0)                // 本輪有成功讀到有效資料
        {
          showMsgonOled("Spo2:"+String(oxyvalue)+"%,"+"HB:"+String(hbvalue),4) ;//show Spo2+Heart Beat on OLED
          SendtoClouding();             // 送上雲（HTTP/MQTT 由 clouding.h 處理）
          readok = 0 ;  //重設定為0，避免重複傳送端資料
        }
      break;

    default: // 其他一律視為連續模式（裝置主動持續吐資料）
      Mode_continuous_timing();       // 若緩衝區有資料就讀出並解析
      // 可依需要在此判斷 readok 上傳雲端，避免過度頻繁
  }
}

/********************* 系統初始化相關 *************************/
// 初始化感測/周邊（此處先做 OLED；如未來加入蜂鳴器/繼電器也可放這）
void initSensor()
{
  initOled();   // 初始化 OLED 控制器（I2C/SPI、字型、座標系統等）
  delay(2000);  // 等待顯示器電源穩定與初始化完成
}

// 初始化整體系統（必做：序列埠、周邊）
void initAll()
{
  Serial.begin(9600); // 串列監控鮑率（和 PC 端監控視窗設定一致）
  initSensor();       // 先把顯示器就緒，後續印狀態/錯誤訊息較方便
}

// 初始化 Wi-Fi（連線、讀取基本資料）
void INITtWIFI()
{
  initWiFi();   // 由 TCP.h 封裝的 Wi-Fi 初始化／連線流程
  Serial.println();
  Serial.println("---wifi access point----");

  // 取得目前連線的 AP 名稱（SSID）
  SSIDData = GetSSID();
  Serial.println(SSIDData);

  // 取得本機 IP（用於同網段除錯、Web 連線、MQTT 鑒別等）
  Serial.println("---Show IP Address----");
  IPData = GetIP();
  Serial.println(IPData);

  // 取得本機 MAC（通常當作裝置唯一 ID，上雲或資料庫主鍵很常用）
  MacData = GetMAC();
  Serial.println("---MAC Address----");
  Serial.println(MacData);

  // 也可選擇把 SSID/MAC 印到 OLED（視畫面需求與列高決定）
  // showSSIDeonOled(SSIDData, 4);
  // showMACeonOled(MacData, 6);
}