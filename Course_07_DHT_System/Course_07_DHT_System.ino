#include "DHTLib.h"  // 引入自訂溫濕度感測模組的函式庫
#include "OledLib.h"  // OLED 顯示模組自訂函式庫

//-------自定義函式宣告區-----------
void initSensor();   // 初始化所有感測模組
void initAll() ;  // 初始化整體系統
void showTitleonOled(String ss); //列印抬頭於OLED上
void showTemperatureonOled(float ss); //列印溫度於OLED上
void showHumidityonOled(float ss);  //列印濕度於OLED上-

//-------自定義函式宣告區-----------

void setup() 
{
    initAll() ;
    delay(200);
    ShowDHTInformation();   //印出溫溼度感測器產品所有資訊   

    // 畫BEST MODULE LOGO
    drawPicture(0,0,BestModule_LOGO,128, 64) ; //畫BEST MODULE LOGO
    delay(2000) ;
    clearScreen()  ;  //清除螢幕
    showTitleonOled("Temp & Humid SyS") ; //秀第一列字


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

      showTemperatureonOled(TValue) ;//
      showHumidityonOled(HValue) ;//    
    delay(2000);                // 每隔 2 秒讀取並更新一次溫濕度資料

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

//-------列印溫度於OLED上---------------
void showTemperatureonOled(float ss) //列印溫度於OLED上
{
      printText(0,2,"              ")  ;  //清空
    printText(0,2,String(ss))  ;  //轉換string字串到char array
    Serial.print("Temperature on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印濕度於OLED上---------------
void showHumidityonOled(float ss)  //列印濕度於OLED上-
{
      printText(0,4,"              ")  ;  //清空
    printText(0,4,String(ss))  ;  //轉換string字串到char array
    Serial.print("Humidity on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}






