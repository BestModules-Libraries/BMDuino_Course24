// ------- 全域變數宣告區 -----------
String SSIDData;   // 儲存 WiFi 熱點名稱 (SSID)
String IPData;     // 儲存 WiFi 分配到的 IP 位址
String MacData;     // 儲存 WiFi 分配到的 MAC Address
// MacData 在其他函式中宣告，作為全域變數

// ------- 感測模組函式與外部函式引用宣告區 -----------
#include <String.h>    // Arduino 內建字串處理函式庫
#include "commlib.h"   // 通訊相關的共用函式庫（可能包含封包處理、緩衝區管理等）

#include "OledLib.h"   // 自訂 OLED 顯示模組函式庫（提供 OLED 初始化、文字繪製、清屏等功能）
#include "TCP.h"       // TCP 通訊函式庫（包含 WiFi 初始化、MAC 取得等函式）

#include "MQTTLib.h"   // MQTT 函式庫，提供 MQTT 初始化、連線與發佈/訂閱訊息功能

#include "VocLib.h" //自訂語音自訂函式宣告

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
void INITtWIFI();                  // 初始化 WiFi 網路連線
void showTitleonOled(String ss,int row);   // 顯示標題文字於 OLED 第一列
void showIPonOled(String ss,int row);      // 顯示 IP 位址於 OLED
void showDeviceonOled(String ss,int row);  // 顯示裝置 Device ID 於 OLED



void setup() 
{
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
  INITtWIFI();     // 初始化 WiFi 網路，並取得 SSID、IP 與 MAC 資料

  if (Wifi.getStatus())   // 確認 WiFi 是否連線成功
  {
    initMQTT();           // 初始化 MQTT 伺服器連線
    Serial.println("WIFI OK"); 
    // SendtoClouding();  // （選用功能）傳送感測資料到雲端
  }

  //---------------------------------
  clearScreen();  // 清除 OLED 螢幕
  // 顯示 BEST MODULES 的 LOGO
  drawPicture(0, 0, BestModule_LOGO, 128, 64);
  delay(3000);    // LOGO 顯示 3 秒
  clearScreen();  // 再次清除螢幕

  // 顯示系統資訊於 OLED
  showTitleonOled(MacData,0);  // 在 OLED 顯示 MAC 位址
  showIPonOled(IPData,2);      // 在 OLED 顯示 IP 位址
  //----------------------------
  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()
//-----------------------
  // put your main code here, to run repeatedly:
    // sayTitle(VOC_13);  //撥放語音開始公告音
    // saySensor(VOC_18,"29.34",VOC_16);  //撥放語音開始公告音
    
}

void loop() 
{              
  // WiFi 斷線檢查與重連邏輯（目前註解掉）
  // 當 WiFi 斷線時可重新執行 INITtWIFI()

  // 從 MQTT Broker 接收資料
  Wifi.readIotData(&ReciveBuff, &ReciveBufflen, &topic);  // 讀取 Broker 下發的資料

  if (ReciveBufflen)   // 如果有接收到資料
  {
    Serial.print("read payload from mqtt broker:(");
    Serial.print(topic);
    Serial.print("/");
    Serial.print(ReciveBuff);
    Serial.print(")\n");

      // 解析接收到的 JSON 資料
    DeserializationError error = deserializeJson(doc, ReciveBuff);

    // 取出 JSON 中的欄位數值
    const char* device     = doc["Device"];      // 例: "48E729732158"
    float TValue        = doc["Temperature"]; // 例: 2
    float HValue      = doc["Humidity"];     // 例: "ON" 或 "OFF"

    // 將結果輸出到序列監視器
    Serial.print("Device: ");
    Serial.println(device);

    Serial.print("TValue: ");
    Serial.println(TValue);
    Serial.print("HValue: ");
    Serial.println(HValue);

    // 顯示在 OLED 上
    showMsgonOled("T:"+String(int(TValue))+".C "+"H:"+String(int(HValue))+"%",4);           // 顯示裝置 ID
     sayTitle(VOC_13);  //撥放語音開始公告音
    saySensor(VOC_14,String(TValue),VOC_16);  //撥放語音開始公告音
    saySensor(VOC_15,String(HValue),VOC_17);  //撥放語音開始公告音
 
  }
}

//---------------  自訂函式區 ------------------
// ------------------ 系統初始化區 ------------------

// 初始化所有感測模組
void initSensor()	// 初始化所有感測模組
{
  initOled();    // 初始化 OLED 12864 (0.96吋 OLED BMD31M090)
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
  initVoice();                   // 初始化語音模組
}

// 初始化整體系統
void initAll()	// 初始化整體系統
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor(); 
}

// 初始化 WiFi
void INITtWIFI()
{
  initWiFi();   // 初始化 WiFi 功能
  Serial.println("");
  Serial.println("---wifi access point----");

  SSIDData = GetSSID();   // 取得連線的 WiFi 熱點名稱
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得裝置分配到的 IP 位址
  Serial.println(IPData);

  MacData = GetMAC();     // 取得裝置的 MAC 位址
  Serial.println("---MAC Address----");
  Serial.println(MacData);
}
