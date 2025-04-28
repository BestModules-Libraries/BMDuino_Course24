/*****************************************************************************************************
File:             printString.ino
Description:      This example tests illuminating whole 4 letter strings on the 14-segment display.
Note:              
******************************************************************************************************/
#include "BMD11M134.h" // 引入 BMD11M134 顯示模組的庫

BMD11M134 Display(&Wire1); // 使用 Wire1（I2C 通訊）初始化 Display 顯示模組
char cdata[100]; // 宣告一個字符陣列，用於儲存字串
String sdata; // 宣告一個 String 變數，用於存放要顯示的字串

void setup()
{
  Serial.begin(9600); // 設定序列通訊速率為 9600
  Display.begin(); // 初始化顯示模組
  
  // 獲取並顯示已連接的顯示模組數量
  Serial.print(Display.getNumber());
  Serial.print(" modules are ");
  Serial.println("Connected!");

  // 設定要顯示的字串 "HOLTBEST"
  sdata = "HOLTBEST";
  
  // 將 String 轉換為字符陣列並存放至 cdata
  sdata.toCharArray(cdata, 9);

  // 將字串 cdata 顯示於顯示模組上
  Display.printStr(cdata);
}

void loop()
{
  // 空的 loop 函數，未執行任何操作
}
