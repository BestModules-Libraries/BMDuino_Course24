/*****************************************************************
File:         readWheelAndKeyValue.ino
Description:  Connect the IIC interface of BMK56T004 to Wire of Arduino
              and connect the INT interface to PIN2 at the same time. 
              When a button is pressed, Arduino will output the 
              Key+Wheel status through the Serial monitor
******************************************************************/
#include "BMK56T004.h"  // 引入 BMK56T004 觸控模組的函式庫
BMK56T004 BMK56(2, &Wire);  // 宣告一個 BMK56T004 物件，使用 intPin 為 2，並使用 Wire 進行 I2C 通訊
// BMK56T004 BMK56(22, &Wire1);  // 若使用 BMduino 並使用 Wire1 通訊，請取消註解此行
// BMK56T004 BMK56(25, &Wire2);  // 若使用 BMduino 並使用 Wire2 通訊，請取消註解此行

void setup() 
{
  Serial.begin(9600);  // 設置序列通訊，波特率為 9600
  BMK56.begin();       // 初始化 BMK56T004 觸控模組
}

void loop() 
{
  // 檢查觸控模組的中斷引腳是否為低電平（表示觸控事件發生）
  if(BMK56.getINT() == 0)
  {
      // 讀取觸控模組的旋鈕值，並將結果顯示在序列監視器上
      Serial.print("wheelValue:");
      Serial.println(BMK56.readWheelValue());            

      // 讀取觸控模組的按鍵值，並將結果顯示在序列監視器上
      Serial.print("keyValue:");
      Serial.println(BMK56.readKeyValue());       
  }
}
