// =======================================================
// --------------- 全域變數宣告區 -------------------------
// =======================================================

String SSIDData;   // 儲存目前連線的 WiFi 熱點名稱 (SSID)
String IPData;     // 儲存目前裝置由路由器分配到的 IP 位址
String MacData;    // 儲存目前裝置的 MAC Address（硬體識別碼）
// 註：MacData 在其他函式亦會使用，因此放在全域變數區


// =======================================================
// ----- 感測模組函式與外部函式庫引用宣告區 ----------------
// =======================================================

#include <String.h>     // Arduino 內建字串處理函式庫
#include "commlib.h"    // 通訊功能函式庫（含封包處理、緩衝資料等）
#include "OledLib.h"    // OLED 128x64 顯示器函式庫（初始化/顯示圖示/顯示文字）
#include "TCP.h"        // TCP & WiFi 函式庫（負責 WiFi 初始化、SSID、IP、MAC 取得）
#include "MQTTLib.h"    // MQTT 函式庫，提供 Publish / Subscribe / MQTT 初始化等功能
#include "VocLib.h"     // 自訂語音播放模組函式庫，用於呼叫語音編號播放

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
void INITtWIFI();                  // 初始化 WiFi 網路連線
void ShowWiFiInformation();  //顯示wifi的基本參數
void showTitleonOled(String ss,int row);   // 顯示標題文字於 OLED 第一列
void showSSIDeonOled(String ss,int row); //列印SSID於OLED上
void showIPonOled(String ss,int row); //列印IP Address於OLED上
void showMsgonOled(String ss,int row); //列印Message於OLEDvoid showIPonOled(String ss,int row);      // 顯示 IP 位址於 OLED

// =======================================================
// ======================= setup() 區 ======================
// =======================================================
void setup() 
{
  initAll();        // 初始化整體系統（包含 OLED、語音模組、序列埠等）
  delay(200);       // 等待 200ms，讓硬體穩定
 
  // ----- 操作 OLED 顯示 -----
  clearScreen();  // 清除 OLED 螢幕
  Serial.println("Clear OLED Screen");

  // 在 OLED 顯示 BEST MODULES 的 LOGO
  drawPicture(0, 0, BestModule_LOGO, 128, 64);
  Serial.println("Draw LOGO on OLED");

  delay(3000);   // LOGO 顯示 3 秒
  clearScreen(); // 再次清除 OLED
  Serial.println("Clear OLED Screen");
//-----------------
 INITtWIFI();      // 初始化 WiFi（包含 SSID、IP、MAC 顯示）

  // ----- 檢查 WiFi 連線狀態 -----
  if (Wifi.getStatus())   // 若 WiFi 連線成功
  {
    initMQTT();           // 初始化 MQTT Broker 連線
    Serial.println("WIFI OK");
    // SendtoClouding(); //（選用）可將數據送到雲端
  }

//-----------------------
  // 顯示 WiFi 與系統資訊
  showTitleonOled(MacData,0); // 第 0 行顯示 MAC Address
  showSSIDeonOled(SSIDData,2); // 顯示 SSID
  showIPonOled(IPData,4); // 顯示 IP

  Serial.println("Enter Loop()");
}


// =======================================================
// ======================== loop() ========================
// =======================================================
void loop() 
{
    // WiFi 斷線處理（目前為註解，可在連線斷線時呼叫 INITtWIFI()）
    // 從 MQTT Broker 接收訊息
    Wifi.readIotData(&ReciveBuff, &ReciveBufflen, &topic);

  if (ReciveBufflen) // 若收到資料（ReciveBufflen > 0）
  {
    Serial.print("read payload from mqtt broker:(");
    Serial.print(topic);
    Serial.print("/");
    Serial.print(ReciveBuff);
    Serial.println(")");

    // ----- 處理 MQTT JSON Payload -----
    DeserializationError error = deserializeJson(doc, ReciveBuff);

    // 取得 JSON 欄位
    const char* device = doc["Device"];       // → 裝置 ID，例如："48E729732158"
    const char* loc    = doc["Location"];     // → 位置，例如："前門"
    const char* status = doc["Status"];       // → 狀態，例如："Active"

    // ----- 將解析內容輸出到序列埠 -----
    Serial.print("Device: ");   Serial.println(device);
    Serial.print("Location: "); Serial.println(loc);
    Serial.print("Status: ");   Serial.println(status);

    // ===================================================
    // ============ 語音播放與 OLED 顯示邏輯 ===============
    // ===================================================

     // 正確寫法應為：strcmp(status, "Active") == 0
    if (strcmp(status, "Active") == 0)  // 注意：這裡是“指定運算子”，會永遠為真！
    {
      SayString(VOC_33); // 播放一段提示語（例如：偵測到異常）
      // ---------- 判斷位置 loc =前門 ----------
      if (strcmp(loc, "前門") == 0)
      {
        showMsgonOled("FrontDoor",6);
        SayString(VOC_35, VOC_34);  // 播放「前門入侵」警告
      }

      // ---------- 判斷位置 loc =客廳 ----------
      if (strcmp(loc, "客廳") == 0)
      {
        showMsgonOled("Living room",6);
        SayString(VOC_36, VOC_34);  // 客廳入侵警示
      }

      // ---------- 判斷位置 loc =倉庫 ----------
      if (strcmp(loc, "倉庫") == 0)
      {
        showMsgonOled("Warehouse",6);
        SayString(VOC_37, VOC_34);  // 倉庫入侵警示
      }

      // ---------- 判斷位置 loc =臥室 ----------
      if (strcmp(loc, "臥室") == 0)
      {
        showMsgonOled("Rest Room",6);
        SayString(VOC_38, VOC_34);  // 臥室入侵警示
      }
    }
    else
    {
      showMsgonOled("Detective",6); // 若非 Active 顯示待命狀態
    }
  }
}


// =======================================================
// ====================== 自訂函式區 =======================
// =======================================================

// ------------------ 初始化感測模組 -----------------------
void initSensor() //初始化感測模組
{
  initOled();    // 初始化 0.96 吋 OLED 顯示模組
  delay(2000);   // 等待 OLED 穩定

  initVoice();   // 初始化語音播放模組
  delay(1000);   // 讓語音模組穩定
}

// ------------------ 初始化完整系統 ------------------------
void initAll()  //初始化完整系統
{
  Serial.begin(9600); // 啟動序列埠 9600bps
  Serial.println("System Start.....");

  initSensor();       // 呼叫感測模組初始化
}

// ------------------ 初始化 WiFi -------------------------
void INITtWIFI()  //初始化 WiFi
{
  initWiFi();             // 連線 WiFi
  ShowWiFiInformation();  // 顯示 MAC / SSID / IP
}


// ----------------示 WiFi 資訊------------------
//**************************************************************
// 函式名稱：ShowWiFiInformation()
// 功能：顯示目前 WiFi 連線相關資訊（MAC、SSID、IP）
//**************************************************************
void ShowWiFiInformation()  // 顯示 WiFi 資訊
{
  MacData = GetMAC();   // 呼叫自訂函式 GetMAC()，取得目前裝置的 MAC Address
                        // 並將結果存入全域變數 MacData

  Serial.println("---MAC Address----");  // 在序列埠輸出標題文字
  Serial.println(MacData);               // 印出裝置的 MAC Address
  Serial.println("");                    // 空一行，使顯示格式更清楚

  Serial.println("---wifi access point----"); // 顯示目前使用的 WiFi AP 資訊標題
  SSIDData = GetSSID(); // 呼叫 GetSSID() 取得目前連線的 WiFi SSID
                        // 並儲存回全域變數 SSIDData

  Serial.println(SSIDData);  // 印出目前連線的 WiFi 熱點名稱 (SSID)

  Serial.println("---Show IP Address----"); // 顯示 IP 位址標題
  IPData = GetIP();     // 呼叫自訂函式 GetIP() 取得目前 WiFi 分配到的 IP 位址
                        // 並儲存回全域變數 IPData

  Serial.println(IPData);  // 在序列埠輸出 IPAddress，例如：192.168.1.105
}
