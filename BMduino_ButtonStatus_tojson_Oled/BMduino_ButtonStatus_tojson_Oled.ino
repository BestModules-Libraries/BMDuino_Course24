#include "LedButtonLib.h"
#include "OledLib.h"
#include "MQTTLib.h"

//----------------------------------
//-------列印按鈕資料於OLED上---------------
void showButtononOled(String ss); //列印資訊於OLED上
void initSensor();   // 初始化所有感測模組
void initAll();   // 初始化整體系統函式
//-----------------------------------------------
String BTNCMD ;//取得按鈕內容



//-----------------------------------------------
void setup() 
{
  initAll();   // 初始化整體系統函式             // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）
  delay(200); //延遲200ms
//--------------------
 //---------------------------------
  clearScreen()  ;  //清除螢幕
// 畫BEST MODULE LOGO
// drawPicture(0,0,BestModule_LOGO,128, 64) ; //畫BEST MODULE LOGO
// delay(2000) ;
// clearScreen()  ;  //清除螢幕

}

void loop() 
{
  if (int_flag) 
  {
    BTNCMD = getAllButtonStatus() ;
    showButtononOled(BTNCMD) ;  //列印資訊於OLED上
    genJSON(BTNCMD) ; //將按鈕內容轉成json內容
  }
} 
//----------初始化所有感測模組------------
void initSensor()   // 初始化所有感測模組
{
    initOled(); //初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
    delay(2000) ;
    intLedButton() ;// 初始化溫溼度感測器，啟動 I2C 通訊
    void initMQTT(); //init MQTT Server
}


//---------初始化整體系統函式------
void initAll()   // 初始化整體系統函式
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
   initSensor() ;  // 初始化所有感測模組 
}

//-------列印按鈕資料於OLED上---------------
void showButtononOled(String ss) //列印資訊於OLED上
{
      printText(0,4,"              ")  ;  //清空
    printText(0,4,ss)  ;  //轉換string字串到char array
    Serial.print("Title on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

void genJSON(String cc) //將按鈕內容轉成json內容
{
  String tt;
  if (cc.length() >0)
  {
      int le = cc.length() ;
      for(int i = 0 ; i <le;i++)
      {
          tt = cc.substring(i,i+1) ;
          Serial.print("Decode btn:(") ;
          Serial.print(i) ;
          Serial.print("/") ;
          Serial.print(tt) ;
          Serial.print(")\n") ;
          
          fillPayload(MacData,i+1,GetCmd(tt)) ;
      }
  }
}

String GetCmd(String c) //將按鈕內容轉成json Command內容
{
  if (c == "0")
    {
      return "OFF" ;     
    }
    else
    {
      return "ON" ;     
    }
}