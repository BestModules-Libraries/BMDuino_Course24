/*
************************************************
File: readCO2Concentration.ino
Description: 讀取二氧化碳濃度（CO2 concentration），單位為 ppm
Note: 使用 BM25S3321-1 CO2 感測模組搭配 BMduino-UNO
*************************************************
程式功能摘要（加強說明版）
模組	功能說明
BM25S3321-1.h	取得 CO2 濃度，解析 UART 封包
CO2.begin()	初始化 UART / 模組設定
preheatCountdown()	執行約 60 秒的預熱程序（必要）
setRangeMax()	設定量測上限（常用 2000 或 5000 ppm）
readCO2Value()	回傳解析後的 CO2 濃度（ppm）

*/

#include <BM25S3321-1.h>  
// 匯入 BestModules 的 BM25S3321-1 感測器函式庫
// 此模組能透過 UART 方式取得 CO2 ppm 數值（類似 MH-Z19B 感測器）

#define STA_PIN 22 // 感測器狀態腳位（STA_PIN），模組會透過該腳位輸出狀態訊號
// #define RX_PIN 2  // 軟體序列埠 RX 腳（若使用軟體序列埠時會啟用）
// #define TX_PIN 3  // 軟體序列埠 TX 腳

uint16_t CO2Value = 0;  
// 宣告 CO2Value 變數，用來儲存讀取到的 CO2 濃度值（0~5000 ppm）

/*
 * 建立 CO2 感測物件（物件建構方式取決於你使用的序列埠）
 * 可選用軟體序列埠或硬體序列埠
 * 以下範例展示不同硬體序列埠的使用方式
 */

// （軟體序列埠範例，不使用時註解）
// BM25S3321_1 CO2(STA_PIN, RX_PIN, TX_PIN);

// （BMduino-UNO 使用硬體序列埠取得資料）
// BM25S3321_1 CO2(STA_PIN, &Serial);    // 若使用 Serial 作為 UART
BM25S3321_1 CO2(STA_PIN, &Serial1);      // 使用 Serial1 讀取 CO2 感測器數據
// BM25S3321_1 CO2(STA_PIN, &Serial2);
// BM25S3321_1 CO2(STA_PIN, &Serial3);
// BM25S3321_1 CO2(STA_PIN, &Serial4);







/*************************************************
 * setup()：系統初始化
 *************************************************/
void setup()
{
  CO2.begin();        // 初始化 CO2 模組，設定模組 UART 通訊速度為 9600bps
   Serial.println("Module preheating...(about 60 second)");
  /*
   * CO2 感測器需要進行預熱（Pre-heat）
   * 預熱大約 60 秒，用於確保內部紅外線測量系統達到穩定狀態
   * 未預熱的讀值會不準，因此必須等待
   */
  CO2.preheatCountdown(); // 執行倒數計時（會阻塞程式約 60 秒）
  
  Serial.println("End of module preheating.");
  Serial.println();
  Serial.println("Perform initial setup.");

  // CO2.calibrateZeroPoint();
  // 這行可啟用零點校正（Zero-calibration），建議在戶外純淨空氣中校正一次
  // 預設關閉，避免誤校正造成誤差

  CO2.setRangeMax(5000);
  /*
   * 設定 CO2 量測範圍上限（Max range）
   * 常見模式為 2000ppm 或 5000ppm
   * 若選擇 5000ppm，代表模組允許從 0 ~ 5000 ppm 間量測
   */
}

/*************************************************
 * loop()：主迴圈
 *************************************************/
void loop()
{
  CO2Value = CO2.readCO2Value();
  /*
   * 調用 readCO2Value()
   * → 函式會向 CO2 模組發送查詢指令
   * → 模組回傳含 CO2 ppm 的封包
   * → 函式解析封包後回傳 ppm 數值（整數）
   */
  
  Serial.print("CO2: ");
  Serial.print(CO2Value); // 印出 CO2 濃度（ppm）
  Serial.println(" ppm");

  delay(2000); // 延遲 2 秒後再次量測，避免過於頻繁占用 UART 資源
}
