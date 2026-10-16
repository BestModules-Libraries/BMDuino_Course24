/*
================================================================================
  LuxLib.h ── BME82M131 環境光感測模組（ALS）函式庫封裝
================================================================================

【程式目的】
  本程式為一個 Arduino C/C++ 的「標頭檔（Header File）」，目的是將
  BME82M131 環境光感測模組（Ambient Light Sensor, ALS）的初始化與讀值
  功能包裝成簡單易用的自訂函式（initLux、readLux、readLuxs），
  讓主程式（.ino）只需呼叫這幾個函式，即可完成環境光照度的初始化與讀取，
  並將讀到的光照度儲存在全域變數 LValue 中，方便後續上傳雲端或做其他應用。

【硬體與通訊說明】
  1. BME82M131 是一顆可量測環境光照度（Lux）的感測器模組。
  2. 本程式使用 I2C 介面與感測器通訊，Arduino 端支援 Wire、Wire1、Wire2
     三組 I2C 匯流排（依開發板而定，例如 BMduino 提供多組 I2C）。
  3. 建構子 BME82M131 ALS(intPin, &WireX) 的兩個參數：
       (1) intPin：中斷（Interrupt）接腳編號。本範例未使用中斷功能，
                   但仍必須指定一個接腳編號。
       (2) &WireX：指定使用的 I2C 介面物件（&Wire、&Wire1 或 &Wire2）。
  4. 本範例預設使用 Wire1（第一組 I2C）與 Pin 22；
     若讀者使用 Wire（預設 I2C）或 Wire2，請依註解切換對應的程式行。

【程式流程】
  1. 引入 BME82M131.h 函式庫。
  2. 建立一個名為 ALS 的全域 BME82M131 物件，並指定中斷腳與 I2C 介面。
  3. 宣告三個自訂函式原型：
       - initLux()        ：初始化感測器。
       - readLux()        ：讀取第 1 個感測器的光照度。
       - readLuxs(int no) ：讀取第 no 個感測器的光照度。
  4. 宣告全域變數 LValue，用來存放最新的光照度值，供主程式或雲端上傳使用。
  5. 實作三個自訂函式：
       - initLux()   呼叫 ALS.begin() 完成感測器初始化。
       - readLux()   呼叫 ALS.readLux(1) 並回傳第 1 個感測器光照度。
       - readLuxs(no)呼叫 ALS.readLux(no) 並回傳第 no 個感測器光照度。

【使用方式（在主程式 .ino 中）】
  #include "LuxLib.h"     // 引入本標頭檔
  void setup() {
      Serial.begin(115200);
      initLux();          // 初始化環境光感測器
  }
  void loop() {
      LValue = readLux(); // 讀取第 1 個感測器光照度並存入 LValue
      Serial.println(LValue);
      delay(1000);
  }

【注意事項】
  - 若使用 Wire1，請確認開發板對應的 SDA/SCL 腳位正確。
  - 若使用 Wire2，請將 &Wire1 改為 &Wire2（並使用對應的建構子行）。
  - 若不使用 Wire（改用 Wire1 或 Wire2），請將使用 Wire 的那一行註解掉。
  - 本檔為 .h 標頭檔，若在多個 .cpp/.ino 檔案中引入，建議將函式實作
    改為 inline 或移至 .cpp，以避免多重定義（multiple definition）問題。

================================================================================
*/
//--------引入官方感測模組------------
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

//----------自定義函式區宣告--------------
void initLux();         // 初始化 BME82M131 環境光感測模組
float readLux();        // 讀取第 1 個感測器的環境光照度並回傳
float readLuxs(int no); // 讀取第 no 個感測器的環境光照度並回傳

//------全域變數區--------------------------
float LValue;    // 環境光照度全域變數（供主程式與雲端上傳使用）

//----------自定義函式實體區--------------

/**
 * initLux() ── 初始化 BME82M131 環境光感測模組
 *
 * 說明：
 *   呼叫 ALS.begin() 啟動感測器，完成 I2C 通訊與內部暫存器設定。
 *   必須在 setup() 中呼叫一次，之後才能正確讀取光照度。
 *
 * 參數：無
 * 回傳：無
 */
void initLux()
{
    ALS.begin();     // 初始化 BME82M131 環境光感測模組（VEML7700）
}

/**
 * readLux() ── 讀取第 1 個感測器的環境光照度
 *
 * 說明：
 *   呼叫 ALS.readLux(1) 讀取編號 1 的感測器光照度（單位：Lux），
 *   並將結果回傳給呼叫者。通常用於只有一個感測器的場合。
 *
 * 參數：無
 * 回傳：float ── 第 1 個感測器的光照度值（Lux）
 */
float readLux()
{
    return ALS.readLux(1); // 讀取第 1 個感測器的環境光照度並回傳
}

/**
 * readLuxs(int no) ── 讀取第 no 個感測器的環境光照度
 *
 * 說明：
 *   呼叫 ALS.readLux(no) 讀取指定編號感測器的光照度（單位：Lux），
 *   並將結果回傳給呼叫者。可用於多感測器切換讀取的場合。
 *
 * 參數：
 *   no ── int，感測器編號（例如 1、2、3…）
 * 回傳：float ── 第 no 個感測器的光照度值（Lux）
 */
float readLuxs(int no)
{
    return ALS.readLux(no); // 讀取第 no 個感測器的環境光照度並回傳
}