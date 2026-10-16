/********************* 程式整體運作解說 *************************/
/*
 * ============================================================================
 * 【檔案名稱】course1017V1.ino
 * ============================================================================
 *
 * 【程式用途】
 * ----------------------------------------------------------------------------
 * 本程式為一套「血氧／心率資料採集器」的 Arduino 主程式（Sketch），
 * 主要負責整合以下四大功能模組，並透過 Wi-Fi 將量測結果上傳雲端：
 *     1. TCP.h        ：Wi-Fi／TCP 連線功能封裝（含連線、HTTP、狀態檢查）。
 *     2. OxygenLib.h  ：血氧／心率感測器（BMH83M002）控制封裝。
 *     3. clouding.h   ：雲端上傳封裝（HTTP/MQTT，含 SendtoClouding()）。
 *     4. String.h     ：Arduino 內建字串類別（動態字串處理）。
 *
 * 【整體運作流程】
 * ----------------------------------------------------------------------------
 * 一、啟動階段（setup）
 *   1. 呼叫 initAll()：
 *      - 啟動序列埠（Serial，9600 bps）。
 *      - 呼叫 initSensor() 初始化周邊（此處先初始化血氧模組）。
 *   2. delay(200)：讓硬體（電壓、時鐘）穩定。
 *   3. 呼叫 INITtWIFI()：
 *      - 由 TCP.h 的 initWiFi() 進行 Wi-Fi 連線。
 *      - 透過 GetSSID()、GetIP()、GetMAC() 取得目前連線的
 *        SSID、本機 IP、MAC 位址，並儲存於全域變數。
 *   4. 再次呼叫 initSensor()：
 *      - 初始化血氧感測器（initOxygen）。
 *      - delay(2000) 讓使用者有時間把手指放上感測器。
 *      - 呼叫 readOxygen() 啟動第一次量測，並決定後續運作模式
 *        （Mode = 0 連續模式；Mode = 1 查詢模式）。
 *   5. 印出「Enter Loop()」，提示即將進入主迴圈。
 *
 * 二、主迴圈階段（loop）
 *   依 Mode 值判斷運作模式：
 *   - Mode == 1（查詢模式）：
 *       a. 呼叫 Mode_ask() 向感測器主動索取一次資料封包。
 *       b. 若 readok != 0（本輪成功讀到有效資料），
 *          呼叫 SendtoClouding() 將資料上傳雲端。
 *   - 其他（視為連續模式）：
 *       a. 呼叫 Mode_continuous_timing()，若緩衝區有資料就讀出並解析。
 *       b. 可依需要判斷 readok 上傳雲端（避免過度頻繁）。
 *
 * 【模組與全域變數對應】
 * ----------------------------------------------------------------------------
 * - TCP.h        ：提供 initWiFi()、GetSSID()、GetIP()、GetMAC() 等。
 * - OxygenLib.h  ：提供 initOxygen()、readOxygen()、Mode_ask()、
 *                  Mode_continuous_timing()，以及全域變數 Mode、readok。
 * - clouding.h   ：提供 SendtoClouding()，將資料以 HTTP/MQTT 上傳。
 * - 全域變數：
 *     SSIDData ：目前連線的 Wi-Fi 熱點名稱（SSID）。
 *     IPData   ：由 AP 分配給本機的 IPv4 位址。
 *     MacData  ：本機 MAC 位址（通常在 TCP.h 或外部已宣告為全域）。
 *
 * 【硬體與感測器說明】
 * ----------------------------------------------------------------------------
 * - 血氧感測器型號：BMH83M002（透過 OxygenLib.h 封裝控制）。
 * - 序列埠鮑率：9600 bps（與 PC 端序列監控視窗設定一致）。
 * - 量測模式：
 *     0 = 連續模式（裝置主動持續吐資料）。
 *     1 = 查詢模式（程式主動向感測器索取一次資料）。
 *
 * 【注意事項】
 * ----------------------------------------------------------------------------
 * 1. 若要使用 OLED 顯示 SSID／MAC，可開啟 initAll() 或 INITtWIFI() 中的
 *    showSSIDeonOled()、showMACeonOled() 註解。
 * 2. 若未來加入蜂鳴器、繼電器等周邊，可於 initSensor() 中擴充。
 * 3. MacData 若在 TCP.h 或其它檔案已宣告為全域，本檔不需重複宣告。
 * ============================================================================
 */

/********************* 模組／函式庫引用 *************************/

#include <String.h>  // Arduino 內建字串類別（動態字串處理）
// #include "commlib.h" // 若有共用通訊工具（封包/緩衝區）可再開啟
// Wi-Fi/TCP 連線功能（內含連線、HTTP、狀態檢查等封裝）
#include "TCP.h"
// 血氧／心率感測器模組（BMH83M002；封裝 begin、量測、讀封包等）
#include "OxygenLib.h"
// 雲端上傳封裝（HTTP/MQTT 相關；含 SendtoClouding() 等）
#include "clouding.h"

/********************* 自訂函式宣告（介面） *************************/
// ── 系統／感測初始化 ──
void initSensor();                 // 初始化顯示器等「感測/周邊模組」
void initAll();                    // 初始化整體系統（序列埠 + 感測等）
void INITtWIFI();                  // 初始化 Wi-Fi 連線並讀回 SSID/IP/MAC

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

  initSensor();     // 3) 初始化感測/周邊（血氧模組）
  Serial.println("Enter Loop()");       // 提示即將進入主迴圈
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
        SendtoClouding();             // 送上雲（HTTP/MQTT 由 clouding.h 處理）
      break;

    default: // 其他一律視為連續模式（裝置主動持續吐資料）
      Mode_continuous_timing();       // 若緩衝區有資料就讀出並解析
      // 可依需要在此判斷 readok 上傳雲端，避免過度頻繁
  }
}

/********************* 系統初始化相關 *************************/
// 初始化感測/周邊（此處先做 OLED；如未來加入蜂鳴器/繼電器也可放這）
void initSensor() //初始化感測/周邊
{
  // ── 血氧模組初始化與第一次量測設定 ──
  initOxygen();                         // 初始化血氧感測器（設定模式/間隔）
  delay(2000);                          // 預留時間讓使用者把手指放上感測器
  readOxygen();                         // 啟動量測並判定模式（查詢/連續）
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