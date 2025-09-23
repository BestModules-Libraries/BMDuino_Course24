#include <String.h>              // 引入字串處理函式庫
#include "commlib.h"             // 引入 TCP 控制邏輯相關函式與變數定義（包含 WiFi 初始化、MAC 取得等）
#include "TCP.h"                 // 引入 TCP 通訊協定相關程式庫
#include "LedButtonLib.h"        // 引入 LED 與按鈕控制函式庫
#include "OledLib.h"             // 引入 OLED 顯示模組函式庫
#include "MQTTLib.h"             // 引入 MQTT 通訊函式庫

//----------------------------------
//-------函式前置宣告區---------------
void showButtononOled(String ss); // 在 OLED 上列印按鈕狀態
void initSensor();                // 初始化所有感測模組
void initAll();                   // 初始化整體系統（序列埠、感測器等）
void showTitleonOled(String ss);  // 在 OLED 上列印裝置抬頭（MAC Address）
void showIPonOled(String ss);     // 在 OLED 上列印 IP 位址
void INITtWIFI();                 // 系統初始化 WiFi 網路

//----------------------------------
//-------全域變數宣告區---------------
String SSIDData ;   // 儲存 WiFi SSID
String IPData ;     // 儲存 IP 位址
String BTNCMD ;     // 儲存取得的按鈕狀態（例如 0/1）
                    // MacData 在其他檔案（commlib.h）定義為全域變數

//----------------------------------
//------- Arduino 主程式進入點 -------

void setup() 
{
  initAll();        // 初始化整體系統（序列埠、OLED、按鈕等）
  delay(200);       // 延遲 200 毫秒，讓硬體穩定

  INITtWIFI();      // 初始化 WiFi，並取得 SSID、IP、MAC 資訊
  delay(2000) ;     // 延遲 2000 毫秒，讓硬體穩定
  //initMQTT();  // 初始化 MQTT
  if  (Wifi.getStatus())   // 檢查 WiFi 是否連線成功
  {
    initMQTT();            // 初始化 MQTT Server 並嘗試連線
    Serial.println("WIFI OK") ;
    // SendtoClouding();   // 傳送感測資料到雲端（此功能已被註解）
  }

  //---------------------------------
  clearScreen();   // 清除 OLED 螢幕

  // 顯示開機 Logo
  drawPicture(0,0,BestModule_LOGO,128,64);  // 在 (0,0) 畫出 128x64 的 BEST MODULE LOGO
  delay(2000);    // 停留 2 秒
  clearScreen();   // 再次清除螢幕

  // 在 OLED 顯示裝置資訊
  showTitleonOled(MacData); // 第一行顯示 MAC Address
  showIPonOled(IPData);     // 第二行顯示 IP Address

  Serial.println("Enter Loop()"); // 提示即將進入 loop()
}

void loop() 
{
  if (int_flag)  // 當有按鈕事件發生時（來自 LedButtonLib 的中斷旗標）
  {
    BTNCMD = getAllButtonStatus();   // 取得所有按鈕的狀態字串，例如 "0101"
    showButtononOled(BTNCMD);        // 將按鈕狀態顯示在 OLED 上
    genJSON(BTNCMD);                 // 轉換成 JSON 並傳送到 MQTT Broker
  }
} 

//----------------------------------
//----------初始化所有感測模組------------
void initSensor()
{
    intLedButton();   // 初始化 LED 與按鈕控制模組
    initOled();       // 初始化 OLED 顯示模組 (BMD31M090)
    delay(2000);      
}

//---------初始化整體系統函式------
void initAll()
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps，用於除錯與監控
  initSensor();       // 初始化所有感測模組
}

//-------------------------------------
//----------初始化 WiFi 網路------------
void INITtWIFI()  
{
  initWiFi();   // 初始化 WiFi 連線
  Serial.println("");      
  Serial.println("---wifi access point----"); // 顯示連線中的 AP 資訊

  SSIDData = GetSSID();   // 取得目前連線的 WiFi SSID
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得分配到的 IP 位址
  Serial.println(IPData);

  MacData = GetMAC();     // 取得裝置的 MAC Address
  Serial.println("---MAC Address----");
  Serial.println(MacData);
  // initMQTT();           // 這裡可以直接初始化 MQTT（目前先註解）
}

//----------------------------------
//-------顯示 MAC Address 抬頭於 OLED----
void showTitleonOled(String ss)
{
  printText(0,0,"              ");  // 清除第 0 行
  printText(0,0,ss);                // 顯示字串
  Serial.print("Title on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//-------顯示 IP Address 於 OLED---------
void showIPonOled(String ss)
{
  printText(0,2,"              ");  // 清除第 2 行
  printText(0,2,ss);                // 顯示字串
  Serial.print("IP Address on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//-------顯示按鈕狀態於 OLED-------------
void showButtononOled(String ss)
{
  printText(0,4,"              ");  // 清除第 4 行
  printText(0,4,ss);                // 顯示字串
  Serial.print("Button on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//----------------------------------
//-------將按鈕內容轉成 JSON 發送-------
void genJSON(String cc)
{
  String tt;
  if (cc.length() > 0)  // 確認有按鈕資料
  {
      int le = cc.length();
      for(int i = 0; i < le; i++)
      {
          tt = cc.substring(i,i+1);   // 擷取單一按鈕狀態
          Serial.print("Decode btn:(");
          Serial.print(i);
          Serial.print("/");
          Serial.print(tt);
          Serial.print(")\n");
          
          // 將按鈕資料組裝成 JSON Payload
          fillPayload(MacData,i+1,GetCmd(tt));

          // 檢查 WiFi 狀態，若斷線則重新連線
          if (Wifi.getStatus() != 2)
          { 
            INITtWIFI();   // 重新連線 WiFi
            Serial.println("WIFI OK");
            MQTTPublish(); // 發送資料到 MQTT Broker
          }
          else
          {
            Serial.println("WIFI OK");
            MQTTPublish(); // 發送資料到 MQTT Broker
          }  
      }
  }
}

//----------------------------------
//-------按鈕值轉換成對應命令字串---------
String GetCmd(String c)
{
  if (c == "0")
  {
    return "OFF";  // 若按鈕值為 0，則代表 OFF
  }
  else
  {
    return "ON";   // 其他值代表 ON
  }
}
