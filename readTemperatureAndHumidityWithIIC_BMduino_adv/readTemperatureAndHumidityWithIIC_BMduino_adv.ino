#include "DHTLib.h"  // 引入自訂溫濕度感測模組的函式庫

//----------自定義函式區宣告--------------
void initSensor() ;  // 初始化所有感測模組
void initAll() ;  // 初始化整體系統函式

//----------自定義函式區宣告--------------



void setup() 
{
    initAll() ;  // 初始化整體系統    
    ShowDHTInformation();   //印出溫溼度感測器產品所有資訊   
    delay(2000);                // 延遲 2 秒，讓使用者有時間閱讀裝置資訊
}

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

    delay(2000);                // 每隔 2 秒讀取並更新一次溫濕度資料
}

//----------初始化所有感測模組------------
void initSensor()   // 初始化所有感測模組
{
  initDHT() ;// 初始化溫溼度感測器，啟動 I2C 通訊
}


//---------初始化整體系統函式------
void initAll()   // 初始化整體系統函式
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
   initSensor() ;  // 初始化所有感測模組 
}
