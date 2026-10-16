// ------- 感測模組函式與外部函式引用宣告區 -----------
#include <String.h>    // Arduino 內建字串處理函式庫
#include "commlib.h"   // 通訊相關的共用函式庫（可能包含封包處理、緩衝區管理等）

#include "VocLib.h" //自訂語音自訂函式宣告

#include "OledLib.h"   // 自訂 OLED 顯示模組函式庫（提供 OLED 初始化、文字繪製、清屏等功能）

// -----end of -- 感測模組函式與外部函式引用宣告區 -----------

void setup() 
{
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
  //---------------------------------
  clearScreen();  // 清除 OLED 螢幕
  // 顯示 BEST MODULES 的 LOGO
  drawPicture(0, 0, BestModule_LOGO, 128, 64);
  delay(3000);    // LOGO 顯示 3 秒
  clearScreen();  // 再次清除螢幕
  showTitleonOled("My Voice Speak",0); //列印抬頭於OLED上
  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()

}

void loop() 
{
  SayString(VOC_33);  //播放一段文字或歡迎字   
  SayString(VOC_35,VOC_34);  //播放二段文字或歡迎字  
   delay(100000) ;  
}

//--------自訂函式區---------
// ------------------ 系統初始化區 ------------------
// 初始化所有感測模組
void initSensor() // 初始化所有感測模組
{
  initOled() ;//初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
  //delay(2000);   // 延遲 2 秒，等待顯示模組穩定

  initVoice();    //初始化語音感測模組
  delay(1000);   // 延遲 1 秒，等待顯示模組穩定
}

// 初始化整體系統
void initAll()  // 初始化整體系統
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor();        // 呼叫初始化感測模組
}
