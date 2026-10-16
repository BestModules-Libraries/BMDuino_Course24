#include "VocLib.h"


void setup() 
{
  Serial.begin(9600)  ;
  Serial.println("System Start....") ;
  initVoice();    //初始化語音感測模組

}

void loop() 
{
  keycode = getBoardKey() ; //取得語音感測模組之按鍵  
  Serial.print("Scan Board Key:(") ;
  Serial.print(keycode) ;
  Serial.print(")\n") ;
    delay(300) ;

}
