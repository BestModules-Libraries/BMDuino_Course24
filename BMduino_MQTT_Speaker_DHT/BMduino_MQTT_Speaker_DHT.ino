/**********************************************************************************************
  程式名稱：IoT 溫溼度 MQTT 雲端監控 + OLED 顯示 + 語音播報整合系統
  設計目的：
      本程式示範如何使用 ESP32/BMduino WiFi 開發板，
      將 MQTT 雲端資料(溫度與濕度) 透過 WiFi 接收，
      並在 OLED 顯示、同時以語音模組播放感測結果。

  主要功能：
      (1) WiFi 初始化與網路資訊顯示
      (2) MQTT Broker 資料接收與 JSON 資料解析
      (3) OLED 顯示感測數據
      (4) 語音播報溫溼度結果（使用 VOC_XX 語音代碼）
      (5) 系統開機顯示 LOGO 與裝置資訊

  開發平台：Arduino IDE
  適用硬體：ESP32 / BMduino 系列 + BMD31M090 OLED + BMV31T001 語音模組
  作者：Bruce Tsao
  版權：All CopyRight Reserved by BruceTsao
***********************************************************************************************/


// -------------------------------------------------------------
// 【全域變數宣告區】
// -------------------------------------------------------------
String SSIDData;   // 儲存目前連線中的 WiFi 熱點名稱 (SSID)
String IPData;     // 儲存目前取得的 IP 位址
String MacData;    // 儲存目前模組的 MAC Address (作為 Device ID)
// 以上三個變數為全域變數，方便在整個程式中使用


// -------------------------------------------------------------
// 【外部函式庫引用區】
// -------------------------------------------------------------
#include <String.h>     // Arduino 內建的字串處理函式庫，提供 String 物件操作
#include "commlib.h"    // 共用通訊函式庫，可能包含資料封包組合、字串切割、緩衝區操作等功能
#include "OledLib.h"    // 自訂 OLED 顯示模組控制函式庫（支援顯示文字、圖像、清屏等功能）
#include "TCP.h"        // WiFi/TCP 相關通訊函式庫（含 WiFi 初始化、MAC/IP 取得等功能）
#include "MQTTLib.h"    // MQTT 函式庫，用於與雲端 MQTT Broker 進行連線、訂閱、發佈資料
#include "VocLib.h"     // 語音模組函式庫，用於語音初始化與播報控制（如 BMV31T001 模組）

// -------------------------------------------------------------
// 【自訂函式宣告區】
// -------------------------------------------------------------
void initSensor();                     // 初始化所有感測與顯示模組（OLED + 語音）
void initAll();                        // 初始化整體系統（序列埠、OLED、語音）
void INITtWIFI();                      // 初始化 WiFi 功能並取得網路資訊
void showTitleonOled(String ss, int row);   // 顯示標題文字於 OLED 指定列
void showIPonOled(String ss, int row);      // 顯示 IP 位址於 OLED 指定列
void showDeviceonOled(String ss, int row);  // 顯示裝置 ID 於 OLED 指定列


// -------------------------------------------------------------
// 【主程式初始化 setup()】
// -------------------------------------------------------------
void setup() 
{
  initAll();       // 執行整體系統初始化（序列埠啟動、OLED 與語音模組初始化）
  delay(200);      // 延遲 200ms，確保硬體穩定啟動
  INITtWIFI();     // 初始化 WiFi 連線，並取得 SSID / IP / MAC 資訊

  // -------------------------------
  // WiFi 連線狀態確認
  // -------------------------------
  if (Wifi.getStatus())   // 如果 WiFi 已成功連線
  {
    initMQTT();           // 初始化 MQTT 連線，準備與雲端 Broker 溝通
    Serial.println("WIFI OK"); 
    // SendtoClouding();  // 可選功能：上傳本地感測資料到雲端
  }

  // -------------------------------
  // OLED 顯示初始化 LOGO 畫面
  // -------------------------------
  clearScreen();  // 清除 OLED 螢幕顯示
  drawPicture(0, 0, BestModule_LOGO, 128, 64);  // 顯示 BEST MODULES 公司 LOGO 圖像
  delay(3000);    // 停留 3 秒讓 LOGO 顯示完畢
  clearScreen();  // 清除螢幕準備顯示系統資訊

  // -------------------------------
  // 顯示系統網路資訊於 OLED
  // -------------------------------
  showTitleonOled(MacData, 0);  // 顯示裝置的 MAC Address（Device ID）
  showIPonOled(IPData, 2);      // 顯示當前 IP Address

  Serial.println("Enter Loop()"); // 顯示提示訊息，進入主執行迴圈 loop()

  // -------------------------------
  // （可選）語音提示啟動
  // -------------------------------
  // sayTitle(VOC_13);                   // 撥放「系統啟動」語音提示音
  // saySensor(VOC_18,"29.34",VOC_16);   // 範例：撥放數值語音，如「目前溫度 29.34 度」
}



// -------------------------------------------------------------
// 【主執行迴圈 loop()】
// -------------------------------------------------------------
void loop() 
{              
  // -------------------------------
  // WiFi 連線狀態監控邏輯 (可選)
  // -------------------------------
  // 若偵測到 WiFi 斷線，可重新執行 INITtWIFI() 嘗試重連

  // -------------------------------
  // 接收 MQTT Broker 傳來的資料封包
  // -------------------------------
  Wifi.readIotData(&ReciveBuff, &ReciveBufflen, &topic);  
  // ↑ 從 MQTT Broker 讀取主題(topic)與內容(payload)，
  //   並存放於 ReciveBuff 變數中，同時計算資料長度 ReciveBufflen。

  if (ReciveBufflen)   // 若收到的資料長度不為 0，表示有有效訊息
  {
    // -------------------------------
    // 顯示接收到的原始資料內容
    // -------------------------------
    Serial.print("read payload from mqtt broker:(");
    Serial.print(topic);      // 顯示 MQTT 主題名稱
    Serial.print("/");
    Serial.print(ReciveBuff); // 顯示主題內的 JSON 資料內容
    Serial.print(")\n");

    // -------------------------------
    // 解析 JSON 資料結構
    // -------------------------------
    DeserializationError error = deserializeJson(doc, ReciveBuff);
    // 將接收到的 JSON 字串轉換成可解析的 JSON 物件

    // -------------------------------
    // 從 JSON 中取出個別欄位
    // -------------------------------
    const char* device = doc["Device"];     // 取得裝置代號，例如 "48E729732158"
    float TValue       = doc["Temperature"]; // 取得溫度值，例如 27.5
    float HValue       = doc["Humidity"];    // 取得濕度值，例如 55.3
/*
{
  "Device":"48E7294E6A10",
  "Card":"9723682696",
  "Result":"Find"
  }

*/
    // -------------------------------
    // 將資料印出於序列監視器 (Serial Monitor)
    // -------------------------------
    Serial.print("Device: ");
    Serial.println(device);
    Serial.print("TValue: ");
    Serial.println(TValue);
    Serial.print("HValue: ");
    Serial.println(HValue);

    // -------------------------------
    // 顯示於 OLED 螢幕
    // -------------------------------
    // 顯示格式範例："T:27.C  H:55%"
    showMsgonOled("T:" + String(int(TValue)) + ".C " + "H:" + String(int(HValue)) + "%", 4);

    // -------------------------------
    // 語音播報區 (需語音模組支援)
    // -------------------------------
    sayTitle(VOC_13);                          // 播放語音提示音「資料公告開始」
    saySensor(VOC_14, String(TValue), VOC_16); // 播放溫度數值（如：「目前溫度 27 度」）
    saySensor(VOC_15, String(HValue), VOC_17); // 播放濕度數值（如：「目前濕度 55%」）
  }
}



// -------------------------------------------------------------
// 【自訂函式區】
// -------------------------------------------------------------

// ------------------ 初始化所有感測與顯示模組 ------------------
void initSensor()
{
  initOled();       // 初始化 OLED 顯示模組 (例如 0.96 吋 BMD31M090)
  delay(2000);      // 延遲 2 秒，確保 OLED 啟動完成並穩定顯示
  initVoice();      // 初始化語音模組（例如 BMV31T001）
}


// ------------------ 初始化整體系統 ------------------
void initAll()
{
  Serial.begin(9600);                 // 啟動序列埠，設定通訊速率為 9600 bps
  Serial.println("System Start.....");// 在序列監視器顯示系統啟動訊息
  initSensor();                       // 執行感測模組初始化
}


// ------------------ 初始化 WiFi 功能 ------------------
void INITtWIFI()
{
  initWiFi();   // 執行 WiFi 初始化程序
  Serial.println("");
  Serial.println("---wifi access point----");

  SSIDData = GetSSID();   // 取得連線的 WiFi 熱點名稱
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得分配到的 IP 位址
  Serial.println(IPData);

  MacData = GetMAC();     // 取得裝置的 MAC Address
  Serial.println("---MAC Address----");
  Serial.println(MacData);
}
