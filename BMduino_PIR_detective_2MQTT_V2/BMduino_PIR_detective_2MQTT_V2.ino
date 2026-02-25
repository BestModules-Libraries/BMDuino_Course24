// ================================================================
// =============== 全域變數宣告區 (Global Variables) ===============
// ================================================================

// ------- 感測模組函式與外部函式引用宣告區 -----------
#include <String.h>    // Arduino 內建字串處理函式庫
#include "commlib.h"   // 通訊相關的共用函式庫（可能包含封包處理、緩衝區管理等）
// 宣告三個全域變數，用於儲存 WiFi 模組相關資訊
String MacData;   // 儲存目前裝置的 WiFi MAC Address（網路卡的唯一識別碼）
String SSIDData;  // 儲存目前連線的 WiFi 熱點名稱 (SSID)
String IPData;    // 儲存 WiFi 連線後由路由器分配的 IP 位址
#define PirLocation "前門"    //定義發生觸動PIR感測器位置

#include "TCP.h"       // TCP 通訊函式庫（包含 WiFi 初始化、MAC 取得等函式）

#include "MQTTLib.h"   // MQTT 函式庫，提供 MQTT 初始化、連線與發佈/訂閱訊息功能

#include "PIRLib.h" //PIR偵測模組(BMCOM) BMA46M422 自訂函式區


// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
void INITtWIFI();                  // 初始化 WiFi 網路連線
void ShowWiFiInformation();  //顯示wifi的基本參數
void alarmled(); //根據 flag 狀態控制 LED 行為
// ------- 自定義函式宣告區 -----------

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

   //--------修正setup() 主程式： 進入loop()之前----------
  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()
  flag = 0; 
}

void loop() 
{

  /***************************************
   * 當感測器輸出 HIGH（偵測到物體）
   * 且 flag 尚未進入警報狀態（!= 1）
   * → 則觸發警報事件
   ***************************************/
  if (readPIR() == HIGH && flag != 1) { 
    Serial.println("Alarm! an object passes by");  // 印出警報訊息
    flag = 1;  
    alarmled(); //根據 flag 狀態控制 LED 行為 
    if  (Wifi.getStatus()!=2) //判斷網路連線不正常，可能已斷線
    { 
      //判斷網路連線不正常，可能已斷線
      INITtWIFI();  //系統INIT  WIFI網路
      Serial.println("WIFI OK") ;
      MQTTPublish() ; //Send Data to MQTT Broker
    }
    else  //判斷網路連線正常，可以正常初始化MQTT
    {
      Serial.println("WIFI OK") ;
      MQTTPublish() ; //Send Data to MQTT Broker
    }   
    flag = 0;                                  // 設定為警報狀態
  }

  /***************************************
   * 當感測器恢復 LOW（無物體）
   * 且 flag 不為 0（表示剛解除警報）
   * → 模組恢復正常
   ***************************************/
  if (flag != 1 && readPIR() == LOW) 
  {
    flag = 0;   
    alarmled(); //根據 flag 狀態控制 LED 行為                                    // 重設旗標
    Serial.println("Module normal; no alarm");     // 印出正常訊息
  }

  delay(2000) ; //delay 2 sec
}


//-----------自訂函式實體程式區--------------

//根據 flag 狀態控制 LED 行為
void alarmled() //根據 flag 狀態控制 LED 行為
{
    /***************************************
   * 根據 flag 狀態控制 LED 行為
   * case 0 → 正常模式：每 1 秒閃爍一次
   * case 1 → 警報模式：LED 長亮（每秒亮一次）
   ***************************************/
  switch (flag) 
  {

    case 0:                                // 正常狀態（無偵測）

      digitalWrite(13, LOW);               // 熄滅
      break;

    case 1:                                // 警報狀態（偵測到物體）
      digitalWrite(13, HIGH);              // LED 持續亮
      break;
  }
}

// ------------------ 系統初始化區 ------------------
// 初始化所有感測模組
void initSensor()	// 初始化所有感測模組
{
  initPIR();  //初始化PIR偵測模組(BMCOM) BMA46M422
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
}

//-------- 初始化整體系統-----
void initAll()	// 初始化整體系統
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor(); 
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

