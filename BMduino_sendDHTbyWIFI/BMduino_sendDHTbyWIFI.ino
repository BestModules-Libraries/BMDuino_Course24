//溫溼度上傳到MQTT Broker V3
#include "DHTLib.h"  // 引入自訂溫濕度感測模組的函式庫
#include "TCP.h"
#include "MQTTLib.h"  // http GET 使用的函式庫庫#include "TCP.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include <String.h>
#include "commlib.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include "LedButtonLib.h"

//-------自定義函式宣告區-----------
void initSensor();   // 初始化所有感測模組
void initAll() ;  // 初始化整體系統
void INITWIFI();  //系統INIT  WIFI網路
//-------全域變數宣告區-----------
String SSIDData ;//MAC Address全域變數
String IPData ;//MAC Address全域變數


// ------------------ 初始化 ------------------
void setup()
{
  initAll();   // 初始化整體系統函式             // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）
    delay(200); //延遲200ms
    ShowDHTInformation();   //印出溫溼度感測器產品所有資訊 
    INITWIFI();  //系統INIT  WIFI網路
  if  (Wifi.getStatus())
 {
    initMQTT(); //init MQTT Server
    Serial.println("WIFI OK") ;
    //SendtoClouding() ;    //傳送感測資料到雲端
 }
  Serial.println("Enter Loop()") ; 
}


// ------------------ 主迴圈 ------------------
void loop() 
{              
    // 從感測器讀取濕度數值並顯示
    HValue = readHumidity();        //讀取濕度數
    Serial.print("Humidity : ");
    Serial.print(HValue);       // 顯示濕度值
    Serial.print(" %    ");   

    // 從感測器讀取溫度數值並顯示
    TValue = readTemperature();        //讀取濕度數
    Serial.print("Temperature : ");
    Serial.print(BMht.readTemperature());    // 顯示溫度值
    Serial.println(" °C ");                 // 顯示溫度單位 °C

    //傳送溫溼度資訊到雲端  
    
    if  (Wifi.getStatus()!=2) //判斷網路連線不正常，可能已斷線
    { 
      //判斷網路連線不正常，可能已斷線
      INITWIFI();  //系統INIT  WIFI網路
      Serial.println("WIFI OK") ;
      MQTTPublish() ; //Send Data to MQTT Broker
    }
    else  //判斷網路連線正常，可以正常初始化MQTT
    {
      Serial.println("WIFI OK") ;
      MQTTPublish() ; //Send Data to MQTT Broker
    }   

     delay(5000) ;              // 每隔 2 秒讀取並更新一次溫濕度資料



}



void initSensor()//初始化所有模組
{
  initButton() ;  //init LedButton模組
  initDHT() ;// 初始化溫溼度感測器，啟動 I2C 通訊
}

void initAll()    // init system all configuration
{
    // 啟動序列埠 (USB Debug 用)
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
   initSensor() ;  // 初始化所有感測模組 
}

void INITWIFI()  //系統INIT  WIFI網路
{
    initWiFi() ; // 初始化 WiFi 自訂模組
   Serial.println("");      // 印出空行，純粹格式美觀
  Serial.println("---wifi access point----"); // 分隔線，美觀用途
  SSIDData = GetSSID(); //取得 SSID熱點字串
  Serial.println(SSIDData); //取得 SSID熱點字串
  // Serial.println(typeof(Wifi.SSID())) ; // 註解掉的除錯語句，用於查資料型別
    Serial.println("---Show IP Address----"); // 分隔線，美觀用途
  IPData = GetIP(); // 印出取得 連接上的SSID熱點之後閘道器IP位址
  Serial.println(IPData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
  MacData = GetMAC() ; //取得 MAC 位址字串
      Serial.println("---MAC Address----"); // 分隔線，美觀用途
  Serial.println(MacData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
  //initMQTT(); //init MQTT Server
}

