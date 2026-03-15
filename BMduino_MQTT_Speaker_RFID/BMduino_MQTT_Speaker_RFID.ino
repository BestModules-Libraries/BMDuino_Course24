// ============================================================
// 程式名稱：IoT + WiFi + MQTT + OLED + 語音公告系統範例
// 程式說明：
// 本程式示範如何整合 WiFi、MQTT、OLED 與語音模組。
// 主要功能包含：
// 1. 初始化感測器與顯示模組（OLED）。
// 2. 透過 WiFi 連線取得 SSID、IP、MAC。
// 3. 顯示系統資訊於 OLED。
// 4. 連線至 MQTT Broker 並接收控制訊息。
// 5. 解析 MQTT 接收到的 JSON 訊息內容。
// 6. 透過語音模組播報接收到的卡號與驗證結果。
// 作者：Bruce Tsao
// ============================================================


// ------- 全域變數宣告區 ------------------------------------
String SSIDData;   // 儲存目前連線的 WiFi 熱點名稱 (SSID)
String IPData;     // 儲存由 WiFi 分配到的 IP 位址
String MacData;    // 儲存模組的 MAC Address（用於辨識裝置 ID）
// 以上三個變數皆設為全域變數，方便在多個函式間共用


// ------- 外部函式庫引用區 -----------------------------------
#include <String.h>      // Arduino 內建字串函式庫，用於處理 String 類型
#include "commlib.h"     // 共用通訊函式庫（可能包含序列緩衝、字串解析、封包處理等）
#include "OledLib.h"     // 自訂 OLED 函式庫，提供顯示、清屏、畫圖等功能
#include "TCP.h"         // TCP 通訊函式庫，包含 WiFi 初始化與網路資訊取得
#include "MQTTLib.h"     // MQTT 函式庫，提供 MQTT 初始化、連線、發佈與訂閱功能
#include "VocLib.h"      // 語音模組函式庫，提供語音初始化與語音播放控制


// ------- 自訂函式宣告區 -------------------------------------
void initSensor();                     // 初始化所有感測模組（OLED、語音）
void initAll();                        // 初始化整體系統（序列埠 + 感測器）
void INITtWIFI();                      // 初始化 WiFi 並取得網路資訊
void showTitleonOled(String ss,int row);   // 在 OLED 上顯示標題資訊
void showIPonOled(String ss,int row);      // 在 OLED 上顯示 IP 資訊
void showDeviceonOled(String ss,int row);  // 在 OLED 上顯示 Device ID


// ============================================================
// 主程式初始化區（setup）
// ============================================================
void setup() 
{
  initAll();       // 初始化整體系統（包含序列埠與感測器）
  delay(200);      // 延遲 200ms，確保硬體模組上電穩定
  INITtWIFI();     // 初始化 WiFi，並讀取 SSID、IP、MAC

  if (Wifi.getStatus())   // 如果 WiFi 連線成功
  {
    initMQTT();           // 初始化 MQTT Broker 連線
    Serial.println("WIFI OK"); 
    // SendtoClouding();  // 可選功能：傳送感測資料到雲端平台
  }

  //---------------------------------
  clearScreen();  // 清除 OLED 顯示內容
  drawPicture(0, 0, BestModule_LOGO, 128, 64); // 顯示 BestModules 品牌 LOGO 圖示
  delay(3000);    // LOGO 顯示 3 秒鐘
  clearScreen();  // 再次清屏，準備顯示系統資訊

  // 顯示系統網路資訊
  showTitleonOled(MacData, 0);  // 在 OLED 第 0 列顯示 MAC Address
  showIPonOled(IPData, 2);      // 在 OLED 第 2 列顯示 IP Address

  Serial.println("Enter Loop()"); // 在序列監視器提示進入主迴圈 loop()
  //-----------------------
  // 下列語音函式目前被註解掉，可依需求啟用
  // sayTitle(VOC_13);                   // 播放系統啟動語音提示音
  // saySensor(VOC_18,"29.34",VOC_16);   // 播放感測數值語音（例如溫度）
}


// ============================================================
// 主程式迴圈區（loop）
// ============================================================
void loop() 
{              
  // （可選）WiFi 連線監測機制：
  // 若偵測到 WiFi 斷線，可呼叫 INITtWIFI() 重新連線。

  // 從 MQTT Broker 接收雲端資料
  Wifi.readIotData(&ReciveBuff, &ReciveBufflen, &topic);  
  // ↑ 從 Broker 接收訊息並存入緩衝區 ReciveBuff

  if (ReciveBufflen)   // 若接收到的資料長度不為零，表示有新訊息
  {
    // 顯示 MQTT 主題與內容
    Serial.print("read payload from mqtt broker:(");
    Serial.print(topic);
    Serial.print("/");
    Serial.print(ReciveBuff);
    Serial.print(")\n");

    // 將接收到的 JSON 字串反序列化
    DeserializationError error = deserializeJson(doc, ReciveBuff);

    // 從 JSON 資料中取出對應欄位值
    const char* device = doc["Device"];   // 裝置代號，例如 "48E729732158"
    const char* card   = doc["Card"];     // 卡片編號，例如 RFID 卡號 2
    const char* rt     = doc["Result"];   // 驗證結果，例如 "ON" 或 "OFF"

    // 將解析後的結果輸出到序列監視器
    Serial.print("Device: ");  
    Serial.println(device);      // 印出裝置編號
    Serial.print("Card: ");      
    Serial.println(card);        // 印出卡號
    Serial.print("Result: ");    
    Serial.println(rt);          // 印出卡號驗證結果（ON/OFF）

    // 將結果顯示到 OLED 上
    showMsgonOled(String(card), 2);   // 在第 2 列顯示卡號
    showMsgonOled(String(rt), 4);     // 在第 4 列顯示驗證結果

    // 播放語音公告（需 VocLib 支援）
    sayTitle(VOC_13);                      // 播放語音提示「系統開始公告」
    saySensor(VOC_20, String(card), VOC_25);  // 播報 RFID 卡號
    saySensor(VOC_19, VOC_19, VOC_25);        // 播報驗證結果（例如 ON/OFF）
  }
}


// ============================================================
// 自訂函式實作區
// ============================================================

// ------------------ 系統初始化 ------------------
void initSensor()
{
  initOled();        // 初始化 OLED 顯示模組 (BMD31M090, I2C介面)
  delay(2000);       // 延遲 2 秒，確保畫面穩定
  initVoice();       // 初始化語音模組（BMV31T001 或類似模組）
}


// 初始化整體系統（序列埠 + 感測模組）
void initAll()
{
  Serial.begin(9600);                      // 啟動序列埠通訊，設定速率為 9600 bps
  Serial.println("System Start.....");     // 顯示系統啟動訊息
  initSensor();                            // 執行感測模組初始化
}


// ------------------ WiFi 初始化 ------------------
void INITtWIFI()
{
  initWiFi();   // 初始化 WiFi 模組
  Serial.println("");
  Serial.println("---wifi access point----");

  SSIDData = GetSSID();   // 取得目前連線的 WiFi 熱點名稱
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得分配的 IP 位址
  Serial.println(IPData);

  MacData = GetMAC();     // 取得裝置的 MAC Address
  Serial.println("---MAC Address----");
  Serial.println(MacData);
}
