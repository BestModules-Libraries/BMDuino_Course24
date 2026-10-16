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
#include "IRTempLib.h"       // IR Temperature紅外線溫度模組自訂函式

#include "clouding.h"       // REST Ful API 基本模組

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
}

void loop()
{
  bodytemp = readIRTemperature(); //讀取紅外線模組取得量測溫度
  Serial.print("讀取到溫度：(");
  Serial.print(bodytemp);
  Serial.print(") °C \n");
//------------處理顯示OLED問題--------------
  showMsgonOled("Temp:("+String(bodytemp)+").C",4); //列印Message於OLED上
  //------------雲端資料傳送問題--------------
  SendtoClouding() ;    //傳送感測資料到雲端
  delay(6000) ; //延遲六秒鐘
}

// ------------------ 系統初始化區 ------------------

// 初始化所有感測模組
void initSensor()
{
  initOled();    // 初始化 OLED 12864 (0.96吋 OLED BMD31M090)
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
  initIRTemperature() ;   //初始化紅外線模組
}

// 初始化整體系統
void initAll()
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  initSensor();        // 呼叫初始化感測模組
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









