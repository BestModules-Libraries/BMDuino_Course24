#include "OledLib.h"   // 自訂 OLED 顯示模組函式庫（提供 OLED 初始化、文字繪製、清屏等功能）
#include "TCP.h"       // TCP 通訊函式庫（包含 WiFi 初始化、MAC 取得等函式）
#include <String.h>    // Arduino 內建字串處理函式庫

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
void INITtWIFI();                  // 初始化 WiFi 網路連線
void showMAConOled(String ss); //列印MAC Address於OLED上
void showSSIDonOled(String ss); //列印SSID NAME於OLED上
void showIPonOled(String ss); //列印IP Address於OLED上

// ------- 全域變數宣告區 -----------
String MacData;   // 儲存 WiFi Mac Address Data
String SSIDData;   // 儲存 WiFi 熱點名稱 (SSID)
String IPData;     // 儲存 WiFi 分配到的 IP 位址
// MacData 在其他函式中宣告，作為全域變數

// ------------------ 初始化 ------------------
void setup()
{
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
  INITtWIFI();     // 初始化 WiFi 網路，並取得 SSID、IP 與 MAC 資料

 //---------OLED Show-------------
  clearScreen();  // 清除 OLED 螢幕
  // 顯示 BEST MODULES 的 LOGO
  drawPicture(0, 0, BestModule_LOGO, 128, 64);
  delay(1500);    // LOGO 顯示 3 秒
  clearScreen();  // 再次清除螢幕

  // 顯示系統資訊於 OLED
  showMAConOled(MacData);  // 在 OLED 顯示 MAC 位址
  showSSIDonOled(SSIDData);      // 在 OLED 顯示 IP 位
  showIPonOled(IPData);      // 在 OLED 顯示 IP 位址
  //----------------------------
  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()
}

// ------------------ 主迴圈 ------------------
void loop() 
{              
}

// ------------------ 系統初始化區 ------------------

// 初始化所有感測模組
void initSensor()
{
  initOled();    // 初始化 OLED 12864 (0.96吋 OLED BMD31M090)
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
//-------列印MAC Address於OLED上---------------
void showMAConOled(String ss) //列印MAC Address於OLED上
{
      printText(0,0,"              ")  ;  //清空
    printText(0,0,ss)  ;  //轉換string字串到char array
    Serial.print("MAC Address on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印SSID NAME於OLED上---------------
void showSSIDonOled(String ss) //列印SSID NAME於OLED上
{
      printText(0,2,"              ")  ;  //清空
    printText(0,2,ss)  ;  //轉換string字串到char array
    Serial.print("SSID NAME on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}


//-------列印IP Address於OLED上---------------
void showIPonOled(String ss) //列印IP Address於OLED上
{
      printText(0,4,"              ")  ;  //清空
    printText(0,4,ss)  ;  //轉換string字串到char array
    Serial.print("IP Address on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

