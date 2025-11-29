//溫溼度上傳到MQTT Broker V3
#include "OledLib.h"  // OLED 顯示模組自訂函式
#include "TCP.h"
#include "MQTTLib.h"  // http GET 使用的函式庫庫#include "TCP.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include <String.h>
#include "commlib.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include "LedButtonLib.h"

//-------自定義函式宣告區-----------
void initSensor();   // 初始化所有感測模組
void initAll() ;  // 初始化整體系統
void INITtWIFI();  //系統INIT  WIFI網路
void showTitleonOled(String ss); //列印抬頭於OLED上
void showIPonOled(String ss); //列印IP Address於OLED上
void showTemperatureonOled(float ss); //列印溫度於OLED上
void showHumidityonOled(float ss);  //列印濕度於OLED上-
//-------全域變數宣告區-----------
String SSIDData ;//MAC Address全域變數
String IPData ;//MAC Address全域變數


// ------------------ 初始化 ------------------
void setup()
{
  initAll();   // 初始化整體系統函式             // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）
    delay(200); //延遲200ms
    INITtWIFI();  //系統INIT  WIFI網路
  if  (Wifi.getStatus())
 {
    initMQTT(); //init MQTT Server
    Serial.println("WIFI OK") ;
    //SendtoClouding() ;    //傳送感測資料到雲端
 }
 //---------------------------------
  clearScreen()  ;  //清除螢幕
// 畫BEST MODULE LOGO
drawPicture(0,0,BestModule_LOGO,128, 64) ; //畫BEST MODULE LOGO
delay(3000) ;
clearScreen()  ;  //清除螢幕
showTitleonOled(MacData) ; //秀第一列字        
showIPonOled(IPData) ; //秀第一列字      
//----------------------------
 Serial.println("Enter Loop()") ;
}

// ------------------ 主迴圈 ------------------
void loop() 
{              
  //  if  (Wifi.getStatus()!=2) //判斷網路連線不正常，可能已斷線
  //   { 
  //     //判斷網路連線不正常，可能已斷線
  //     INITtWIFI();  //系統INIT  WIFI網路
  //     Serial.println("WIFI OK") ;
  //   if  (Wifi.getStatus())
  //     {
  //     initMQTT(); //init MQTT Server
  //     }
  //   }
    // if  (Wifi.getStatus()!=2) //判斷網路連線不正常，可能已斷線
    // { 
    //   //判斷網路連線不正常，可能已斷線
    //   INITtWIFI();  //系統INIT  WIFI網路
    //   Serial.println("WIFI OK") ;
    //   //MQTTPublish() ; //Send Data to MQTT Broker
    // }
    // else  //判斷網路連線正常，可以正常初始化MQTT
    // {
    //   Serial.println("WIFI OK") ;
    //   //MQTTPublish() ; //Send Data to MQTT Broker
    // }   

    // delay(5000) ;              // 每隔 2 秒讀取並更新一次溫濕度資料
        if (Wifi.getStatus() == 2)
        {
          Serial.println("enter read io") ;
        Wifi.readIotData(&ReciveBuff,&ReciveBufflen,&topic);             //监听模块接收的数据
          if(ReciveBufflen)
          {
            Serial.print("read payload from mqtt broker:(") ;
            Serial.print(ReciveBuff) ;
            Serial.print(")\n") ;
          }
        }
        else
        {
            Serial.print("wifi status:(") ;
            Serial.print(Wifi.getStatus()) ;
            Serial.print("/") ;
            Serial.print(millis()) ;
            Serial.print(")\n") ;
        }
          // DeserializationError error = deserializeJson(doc, ReciveBuff); // 解析 JSON

          // if (error) 
          // {
          //   Serial.print("JSON parsing failed: ");
          //   Serial.println(error.c_str());
          //   return ;
          // }

          // // 從 JSON 中提取欄位
          // const char* device = doc["Device"];
          // const char* Temperature = doc["Temperature"];
          // const char* Humidity = doc["Humidity"];
          // Serial.print("Device:(") ;
          // Serial.print(device) ;
          // Serial.print(")\n") ;
          // Serial.print("Temperature:(") ;
          // Serial.print(Temperature) ;
          // Serial.print(")\n") ;
          // Serial.print("Humidity:(") ;
          // Serial.print(Humidity) ;
          // Serial.print(")\n") ;
                   
        //}
 }



void initSensor()//初始化所有模組
 {
  initButton() ;  //init LedButton模組
    initOled(); //初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
    delay(2000) ;
}

void initAll()    // init system all configuration
{
    // 啟動序列埠 (USB Debug 用)
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
   initSensor() ;  // 初始化所有感測模組 
}

void INITtWIFI()  //系統INIT  WIFI網路
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

