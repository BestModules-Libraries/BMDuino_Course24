/*
================================================================================
【程式名稱】BMduino_readLux.ino
【功能概述】本程式為 BMduino 開發板搭配 BME82M131 環境光感測模組（ALS，
            Ambient Light Sensor）的範例程式。主要功能為初始化 VEML7700
            環境光感測器，並持續讀取周遭環境的光照度（Lux）數值，
            透過序列埠輸出顯示。

【整體運作流程解說】
--------------------------------------------------------------------------------
一、setup() 初始化階段（開機後僅執行一次）：
    (1) 啟動序列埠，速率設定為 9600 bps，用於輸出除錯與監控訊息。
    (2) 呼叫 ALS.begin() 初始化 BME82M131 環境光感測模組（內含 VEML7700）。
    (3) 透過 ALS.getNumber() 取得目前連接的感測模組數量，並印出
        「X modules are Connected!」訊息，確認感測器已正確連接。

二、loop() 主迴圈階段（每秒執行一次，週而復始）：
    (1) 透過序列埠印出 "Lux1:" 字樣作為標籤。
    (2) 呼叫 ALS.readLux(1) 讀取第 1 個感測器（sensor1）的環境光照度，
        並將數值透過序列埠印出。
    (3) 延遲 1000 毫秒（1 秒）後再次執行迴圈，達到每秒更新一次的效果。

【硬體與函式庫對應】
- BME82M131.h：BME82M131 環境光感測模組（ALS）官方函式庫，提供
               begin、getNumber、readLux 等方法。
- Wire1      ：BMduino 的第一組 I2C 介面（依開發板不同可能為 Wire、
               Wire1 或 Wire2）。
- Pin 22     ：BME82M131 模組的中斷接腳（intPin），本範例未實際使用中斷功能，
               但仍需於建構時指定。

【硬體接線與 I2C 選擇說明】
本範例提供三種 I2C 介面的建構方式（依 BMduino 開發板支援的介面而定）：
  (1) Wire  ：BME82M131 ALS(2, &Wire);   // 適用於標準 I2C 介面
  (2) Wire1 ：BME82M131 ALS(22, &Wire1); // 適用於第一組 I2C（本範例預設啟用）
  (3) Wire2 ：BME82M131 ALS(25, &Wire1); // 適用於第二組 I2C（註解中，需依
                                          // 實際開發板調整為 &Wire2）

【作者/版本】BMduino 官方範例 readLux.ino
================================================================================
*/

/*****************************************************************************************************
File:             readLux.ino
Description:      This example demonstrates how to initialize the VEML7700 and then get the ambient light lux.
Note:             
******************************************************************************************************/

#include "BME82M131.h"  // 引入 BME82M131 環境光感測模組（ALS）函式庫

//BME82M131 ALS(2, &Wire);     //intPin, Wire. Please comment out this line of code if you don't use Wire
BME82M131 ALS(22, &Wire1); //Please uncomment out this line of code if you use Wire1 on BMduino
// BME82M131 ALS(25, &Wire1); //Please uncomment out this line of code if you use Wire2 on BMduino
/*
  建立 BME82M131 環境光感測器物件，命名為 ALS。
  建構子參數：
    (1) intPin：中斷接腳編號（本範例未使用中斷功能，但仍需指定）。
    (2) &WireX：指定使用的 I2C 介面（Wire、Wire1 或 Wire2）。
  本範例預設使用 Wire1（第一組 I2C）與 Pin 22。
  註：第三行註解若改用 Wire2，應將 &Wire1 改為 &Wire2 才正確。
*/


//================================================================================
// setup()：系統初始化函式，開機後僅執行一次
//================================================================================
void setup()
{
  Serial.begin(9600);          // 啟動序列埠，速率 9600 bps，用於除錯與監控
  ALS.begin();                 // 初始化 BME82M131 環境光感測模組（VEML7700）

  Serial.print(ALS.getNumber());        // 取得目前連接的感測模組數量並印出
  Serial.print(" modules are ");        // 印出 " modules are " 字樣
  Serial.println("Connected!");         // 印出 "Connected!" 字樣，完成連接確認訊息
}


//================================================================================
// loop()：主迴圈函式，Arduino 開機後會不斷重複執行
//================================================================================
void loop()
{
  Serial.print("Lux1:");                // 印出 "Lux1:" 作為數值標籤
  Serial.println(ALS.readLux(1));       // 讀取第 1 個感測器的環境光照度並印出
  delay(1000);                          // 延遲 1000 毫秒（1 秒）後再次執行迴圈
}