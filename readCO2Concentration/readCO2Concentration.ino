/*************************************************
檔案名稱: readCO2Concentration.ino
說明: 讀取二氧化碳 (CO2) 濃度，單位為 ppm
注意事項: 無
**************************************************/

// 引入 BM25S3321-1 感測器對應的函式庫，BM25S3321_1 是一種紅外線原理的二氧化碳感測器模組，透過序列埠與主控板通訊。
#include <BM25S3321-1.h>

// 定義 CO2 模組的狀態輸出腳位為 GPIO 22
#define STA_PIN 22 // 輸入腳位，用來接收模組的狀態輸出（如預熱中、運作中等）

// 以下兩行是軟體序列埠（SoftSerial）的定義，但目前註解掉未使用
// #define RX_PIN 2  // 模擬序列埠的 RX 腳
// #define TX_PIN 3  // 模擬序列埠的 TX 腳

// 宣告一個變數用來儲存讀取到的 CO2 濃度值（16 位元無號整數）
uint16_t CO2Value = 0;

// 建立 CO2 感測器的物件，使用不同的序列埠方式（根據開發板不同選擇）
// 若使用軟體序列埠，可用如下方式（目前註解掉）：
// BM25S3321_1 CO2(STA_PIN, RX_PIN, TX_PIN);

// 若使用 BMduino-UNO 開發板，使用硬體序列埠的方式建立物件：
// 使用 Serial（USB 序列埠）：
// BM25S3321_1 CO2(STA_PIN, &Serial);

// 使用 Serial1：將 CO2 模組連接到 Serial1 所對應的 TX/RX 腳位
BM25S3321_1 CO2(STA_PIN, &Serial1); // 使用硬體序列埠 Serial1

// 若使用其他板子，也可改用 Serial2, Serial3, Serial4（視硬體支援而定）
// BM25S3321_1 CO2(STA_PIN, &Serial2);
// BM25S3321_1 CO2(STA_PIN, &Serial3);
// BM25S3321_1 CO2(STA_PIN, &Serial4);

void setup()
{
  // 啟動 CO2 模組，預設使用 9600 bps 傳輸速率
  CO2.begin();

  // 啟動與電腦通訊的序列埠，用於顯示資料
  Serial.begin(9600);

  // 通知使用者模組正在預熱中（通常需 60 秒）
  Serial.println("Module preheating...(about 60 second)");

  // 等待模組完成預熱（會根據 STA_PIN 狀態判斷是否完成）
  CO2.preheatCountdown();
  //preheatCountdown() 是該模組的專用功能，會監控模組狀態直到預熱結束

  // 預熱結束提示
  Serial.println("End of module preheating.");
  Serial.println();

  // 執行初始設定提示
  Serial.println("Perform initial setup.");

  // 如果需要歸零校正，可以啟用下列指令（目前註解掉）
  // CO2.calibrateZeroPoint();

  // 設定 CO2 感測器量測範圍的最大值（單位 ppm）
  // 根據模組型號，可設為 2000、5000 等，需與模組一致
  CO2.setRangeMax(5000);
}

void loop()
{
  // 從 CO2 模組讀取目前的濃度值
  CO2Value = CO2.readCO2Value();  //readCO2Value() 是從模組中讀取即時的 CO2 濃度值，單位為 ppm（每百萬分之一）。

  // 將讀到的 CO2 濃度值顯示在序列埠監控器
  Serial.print("CO2: ");
  Serial.print(CO2Value);
  Serial.println(" ppm");

  // 每 2 秒讀取一次資料
  delay(2000);
}
