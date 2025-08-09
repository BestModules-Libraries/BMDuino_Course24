/*************************************************
File:             geyButtonStatus.ino
Description:      本範例為開發板連接按鈕模組的應用，
                  當接收到中斷訊號時，會透過 I2C 通訊方式
                  讀取按鈕的觸發狀態（短按或長按）。
Note:             
**************************************************/

#include <BMK22M131.h>   // 引入 BMK22M131 按鈕模組的函式庫
#include <Wire.h>        // 引入 I2C（TWI）通訊的標準函式庫

#define intPin 22
// 建立一個 BMK22M131 類別的物件 myButton，使用中斷腳 D2 與 I2C 傳輸
//BMK22M131 myButton(intPin, &Wire);
BMK22M131 myButton(intPin, &Wire1);
//BMK22M131 myButton(intPin, &Wire2);

// 宣告變數 int_flag 作為中斷觸發的旗標（flag），初始值為 0
uint8_t int_flag = 0;

// 宣告變數用來儲存按鈕狀態（短按或長按）
uint8_t buttonStatus;

void setup() {
  Serial.begin(9600);  // 啟動序列埠通訊，設定傳輸速率為 9600（用於除錯輸出）

  // 將數位腳位 D2 設為外部中斷輸入腳，當發生「下降沿」（FALLING）時觸發中斷函數 ButtonInt()
  attachInterrupt(digitalPinToInterrupt(intPin), ButtonInt, FALLING);

  // 啟動按鈕模組（初始化 I2C 傳輸）
  myButton.begin();

  // 啟用按鈕模組的 LED 顯示模式（參數 1 表示開啟 LED 模式）
  myButton.ledButtonMode(1);

  // 檢查模組是否成功連線
  Serial.println("Check whether the module is connected, waiting...");
  if (myButton.isConnected() == true) 
  {
    Serial.println("The module is connecting");  // 顯示模組連線成功
  } // end of  if (myButton.isConnected() == true) 
}

void loop() 
{
  // 如果中斷旗標被設為 1（表示中斷事件發生）
  if (int_flag) 
  {
    int_flag = 0;  // 重設旗標，避免重複處理

    // 呼叫函式讀取按鈕的觸發狀態，參數 1 表示按鈕通道（或ID）
    buttonStatus = myButton.getButtonStatus(1);

    // 判斷按鈕狀態並印出訊息
    if (buttonStatus == 1) 
    {
      Serial.println("The button is short pressed.");  // 短按
    } // end of  if (buttonStatus == 1) 
    else 
    {
      Serial.println("The button is long pressed.");   // 長按
    } // end of else  if (buttonStatus == 1) 
  } //  end of if (int_flag) 
}

// 外部中斷服務函數，當按鈕腳位偵測到下降沿時執行
void ButtonInt() 
{
  int_flag = 1;  // 設定旗標為 1，通知主程式有中斷事件
}
