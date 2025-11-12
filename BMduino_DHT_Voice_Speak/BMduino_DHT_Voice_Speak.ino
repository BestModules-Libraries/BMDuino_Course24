// ------- 感測模組函式與外部函式引用宣告區 -----------
#include <String.h>    // Arduino 內建字串處理函式庫
#include "commlib.h"   // 通訊相關的共用函式庫（可能包含封包處理、緩衝區管理等）

#include "VocLib.h" //自訂語音自訂函式宣告
#include "DHTLib.h"  // 自訂溫溼度模組函式庫

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
// ----end of --- 自定義函式宣告區 -----------


void setup() 
{
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定

  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()
}

void loop() 
{
    TValue = readTemperature(); // 讀取溫度值
    HValue = readHumidity(); // 讀取濕度值
    Serial.print("溫度:(") ;
    Serial.print(TValue) ;
    Serial.print(")℃  \n ") ;
    Serial.print("濕度:(") ;
    Serial.print(HValue) ;
    Serial.print(") % \n") ;
    SpeakStringNumber(String(TValue)) ;
    delay(1000) ;  
    SpeakStringNumber(String(HValue)) ;
    delay(1000) ;  

    delay(5000) ;  
}

//--------自訂函式區---------
// ------------------ 系統初始化區 ------------------
// 初始化所有感測模組
void initSensor() // 初始化所有感測模組
{
  initDHT() ;// 初始化溫溼度感測器，啟動 I2C 通訊
  ShowDHTInformation();   //印出溫溼度感測器產品所有資訊
  initVoice();    //初始化語音感測模組
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
}

// 初始化整體系統
void initAll()  // 初始化整體系統
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor();        // 呼叫初始化感測模組
}




