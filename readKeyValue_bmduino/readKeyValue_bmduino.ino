/*
BMK52T016 模組：通常是 BestModules 公司推出的 I2C 按鍵矩陣模組，支援中斷觸發。

getINT()：檢查中斷腳是否為低電位，代表有按鍵事件。

readKeyValue()：讀取當前按下的按鍵數值（可能是一個 16 鍵矩陣，每個鍵對應一個值）。

BIN：代表將數值以二進位格式印出，便於觀察每個 bit 狀態。

*/
#include "BMK52T016.h"  // 引入 BMK52T016 模組所需的函式庫

// 宣告一個 BMK52T016 物件 BMK52，並指定中斷腳位為 2，並使用 I2C 介面（Wire）
// 若不使用 Wire（I2C），請將這一行註解掉
BMK52T016 BMK52(2, &Wire);

// 若使用 BMduino 開發板上的 Wire1 或 Wire2，可改用以下方式初始化模組
//BMK52T016 BMK52(22, &Wire1); // 使用 Wire1，對應的中斷腳位為 22
//BMK52T016 BMK52(25, &Wire2); // 使用 Wire2，對應的中斷腳位為 25

void setup() 
{
    BMK52.begin();         // 初始化 BMK52 模組
    Serial.begin(9600);    // 啟動序列埠，並設定傳輸速率為 9600 bps
}

void loop() 
{  
    // 檢查是否有按鍵事件發生（中斷訊號為 LOW，即為 0）
    if(BMK52.getINT() == 0)
    {
         Serial.print("keyValue:");                     // 印出提示字串
         Serial.println(BMK52.readKeyValue(), BIN);     // 讀取按鍵數值並以二進位格式輸出
    }  
}
