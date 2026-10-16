// 溫溼度上傳到 BMduino_sendDHTbyWIFI
#include "DHTLib.h"      // 引入溫濕度感測模組的函式庫 (DHT11/DHT22 等)
#include "TCP.h"         // 引入 TCP 通訊相關函式庫 (WiFi 初始化、網路連線)
#include "MQTTLib.h"     // 引入 MQTT 函式庫 (負責與 MQTT Broker 連線、發布/訂閱訊息)
#include <String.h>      // 引入 Arduino 內建的字串處理函式庫
#include "commlib.h"     // 引入自訂的通訊函式庫 (可能包含共用網路/資料處理函式)
#include "LedButtonLib.h"// 引入 LED 與按鍵控制模組的函式庫


// ------- 自定義函式宣告區 -----------
void initSensor();    // 初始化所有感測模組 (溫溼度、按鍵等)
void initAll();       // 初始化整體系統 (序列埠 + 感測器)
void INITWIFI();      // 初始化 WiFi 網路 (連線、顯示 SSID / IP / MAC 等資訊)


// ------- 全域變數宣告區 -----------
String SSIDData;   // 儲存 WiFi 熱點 SSID
String IPData;     // 儲存 WiFi 連線後取得的 IP 位址
//String MacData;    // 儲存 WiFi 裝置的 MAC 位址


// ------------------ 初始化 ------------------
void setup()
{
  initAll();                // 初始化整體系統 (序列埠、感測模組)
  delay(200);               // 延遲 200ms，避免模組還沒準備好
  ShowDHTInformation();     // 顯示 DHT 溫溼度感測器的產品資訊
  INITWIFI();               // 啟動 WiFi 連線程序

  // 檢查 WiFi 是否成功連線
  if (Wifi.getStatus())
  {
    initMQTT();             // 初始化 MQTT Server (設定 Broker IP/Port、Topic)
    Serial.println("WIFI OK");
    // SendtoClouding();    // (可選) 傳送感測資料到雲端，先註解掉
  }

  Serial.println("Enter Loop()");  // 提示程式已進入主迴圈
}


// ------------------ 主迴圈 ------------------
void loop()
{
  // 讀取濕度數值
  HValue = readHumidity();  
  Serial.print("Humidity : ");
  Serial.print(HValue);        // 顯示濕度數值
  Serial.print(" %    ");

  // 讀取溫度數值
  TValue = readTemperature();  
  Serial.print("Temperature : ");
  Serial.print(BMht.readTemperature());  // 顯示溫度數值
  Serial.println(" °C ");                // 顯示單位 °C

  // 檢查 WiFi 連線狀態，並傳送資料到 MQTT Broker
  if (Wifi.getStatus() != 2)  // 狀態 2 代表連線異常 (可能斷線)
  { 
    INITWIFI();               // 嘗試重新連線 WiFi
    Serial.println("WIFI OK");
    MQTTPublish();            // 傳送資料到 MQTT Broker
  }
  else                        // WiFi 連線正常
  {
    Serial.println("WIFI OK");
    MQTTPublish();            // 傳送資料到 MQTT Broker
  }

  delay(5000);   // 每隔 5 秒讀取與上傳一次溫溼度資料
}


// ------------------ 初始化模組函式 ------------------

// 初始化所有感測模組
void initSensor()
{
  initButton();   // 初始化 LED 與按鈕模組
  initDHT();      // 初始化 DHT 溫溼度感測器
}

// 初始化整體系統
void initAll()
{
  Serial.begin(9600);  // 啟動序列埠，設定速率為 9600 bps
  initSensor();        // 初始化所有感測器模組
}

// 初始化 WiFi 網路
void INITWIFI()
{
  initWiFi();         // 初始化 WiFi 模組
  Serial.println("");
  Serial.println("---wifi access point----");

  SSIDData = GetSSID();   // 取得 WiFi 熱點名稱 (SSID)
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得 WiFi 連線後的 IP 位址
  Serial.println(IPData);

  MacData = GetMAC();     // 取得裝置 MAC 位址
  Serial.println("---MAC Address----");
  Serial.println(MacData);

  // initMQTT();          // (可選) 初始化 MQTT Server，可在這裡或 setup() 中呼叫
}
