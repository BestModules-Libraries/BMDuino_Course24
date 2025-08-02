#include "DHTLib.h"  // 引入自訂溫濕度感測模組的函式庫
#include "TCP.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include "clouding.h"  // 傳送溫溼度資料到雲端自訂函式庫;http GET 使用的函式庫
#include "OledLib.h"  // OLED 顯示模組自訂函式庫


//-------自定義函式宣告區-----------
void initSensor();   // 初始化所有感測模組
void initAll() ;  // 初始化整體系統
void showTitleonOled(String ss); //列印抬頭於OLED上
void showIPonOled(String ss); //列印IP Address於OLED上
void showTemperatureonOled(float ss); //列印溫度於OLED上
void showHumidityonOled(float ss);  //列印濕度於OLED上-

//-------自定義函式宣告區-----------
String SSIDData ;//MAC Address全域變數
String IPData ;//MAC Address全域變數


//---------------
void setup() 
{
    initAll() ;
    delay(200);
    ShowDHTInformation();   //印出溫溼度感測器產品所有資訊 
 // ------ wifi process--------------------
      initWiFi();              // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）
  Serial.println("");      // 印出空行，純粹格式美觀
  Serial.println("---wifi access point----"); // 分隔線，美觀用途
  SSIDData = GetSSID(); //取得 SSID熱點字串
  Serial.println(SSIDData); //取得 SSID熱點字串
  // Serial.println(typeof(Wifi.SSID())) ; // 註解掉的除錯語句，用於查資料型別
    Serial.println("---Show IP Address----"); // 分隔線，美觀用途
  IPData = GetIP() ; // 印出取得 連接上的SSID熱點之後閘道器IP位址
  Serial.println(IPData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
  MacData = GetMAC() ; //取得 MAC 位址字串
      Serial.println("---MAC Address----"); // 分隔線，美觀用途
  Serial.println(MacData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
//-------------------------------  
    clearScreen()  ;  //清除螢幕
    // 畫BEST MODULE LOGO
    drawPicture(0,0,BestModule_LOGO,128, 64) ; //畫BEST MODULE LOGO
    delay(2000) ;
    clearScreen()  ;  //清除螢幕
    showTitleonOled(MacData) ; //秀第一列字        
    showIPonOled(IPData) ; //秀第一列字        
} 

void loop() {
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

    showTemperatureonOled(TValue); //列印溫度於OLED上
    showHumidityonOled(HValue);  //列印濕度於OLED上-   
    if  (Wifi.getStatus())
    {
        Serial.println("WIFI OK") ;
        SendtoClouding() ;    //傳送感測資料到雲端
    }  
    delay(8000);                // 每隔 2 秒讀取並更新一次溫濕度資料

}


//----------初始化所有感測模組------------
void initSensor()   // 初始化所有感測模組
{
    initOled(); //初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
    delay(2000) ;
    initDHT() ;// 初始化溫溼度感測器，啟動 I2C 通訊

}


//---------初始化整體系統函式------
void initAll()   // 初始化整體系統函式
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
   initSensor() ;  // 初始化所有感測模組 
}

//-------列印抬頭於OLED上---------------
void showTitleonOled(String ss) //列印抬頭於OLED上
{
      printText(0,0,"              ")  ;  //清空
    printText(0,0,ss)  ;  //轉換string字串到char array
    Serial.print("Title on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}
//-------列印IP Address於OLED上---------------
void showIPonOled(String ss) //列印IP Address於OLED上
{
      printText(0,2,"              ")  ;  //清空
    printText(0,2,ss)  ;  //轉換string字串到char array
    Serial.print("IP Address on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印溫度於OLED上---------------
void showTemperatureonOled(float ss) //列印溫度於OLED上
{
      printText(0,4,"              ")  ;  //清空
    printText(0,4,String(ss)+".C")  ;  //轉換string字串到char array
    Serial.print("Temperature on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印濕度於OLED上---------------
void showHumidityonOled(float ss)  //列印濕度於OLED上-
{
   printText(62,4,"         ")  ;  //清空
    printText(62,4,String(ss)+"%")  ;  //轉換string字串到char array
    Serial.print("Humidity on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}






