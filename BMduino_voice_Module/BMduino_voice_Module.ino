#include "VocLib.h" //自訂語音自訂函式宣告


void setup() 
{
  Serial.begin(9600)  ;
  Serial.println("System Start....") ;
  initVoice();    //初始化語音感測模組
  SpeakStringNumber(String(23467)); //念出一串數字
  delay(1000);
  SpeakStringNumber(String(245.678)); //念出一串數字
  delay(1000);
  SpeakStringNumber(String(-2467)); //念出一串數字
  delay(1000);
}

void loop() 
{
 
}
