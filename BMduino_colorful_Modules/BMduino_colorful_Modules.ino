
#include "RGBLEDLib.h"  //RGBLED自訂模組
/*
這段程式會使 LED 燈以流動的方式顯示不同的 RGB 顏色，
並根據 flow 變數改變色彩效果，
達到持續變換的視覺效果。
*/
// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統


void setup() 
{      
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
}



void loop()
{
  
  setRGBMatrix(RGBLight) ;
  Serial.println("RGBLight") ;
  delay(4000) ;
  setRGBMatrix(RGBBlank) ;
    Serial.println("RGBBlank") ;
  delay(4000) ;
  
}
//---------------  主程式自訂函式區 ------------------


//-----初始化所有感測模組----------
void initSensor()	// 初始化所有感測模組
{
  initRGBLED() ;  //起始RGBLED模組
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定

}

// ------------------ 系統初始化區 ------------------
// 初始化整體系統
void initAll()	// 初始化整體系統
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor(); 
}
