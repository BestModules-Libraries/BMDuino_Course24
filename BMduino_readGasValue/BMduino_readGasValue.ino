/*************************************************
File:         readGasValue.ino
Description:  Receive the information automatically output by the module every second,
              and print part of the information to the serial port monitor
Note:
**************************************************/

#include <BM22S3031-1.h>  
// 引入 BM22S3031-1 氣體感測模組的函式庫，
// 用來處理模組的初始化、資料接收與解析等功能。

// 宣告一個大小為 34 的無符號 8 位元陣列，用來存放模組傳回的資訊封包
uint8_t moduleInfo[34] = {0};  

// 宣告變數，用來存放感測器數值
uint16_t ADValue, gasValue, gasAlarmThreshold;  

// === 建立感測器物件，依照不同的連接方式來初始化 ===
// BM22S3031_1 gas(8, 2, 3);       // 軟體序列埠 (Software Serial)：STATUS 腳位接 D8, RX 接 D2, TX 接 D3
// BM22S3031_1 gas(8, &Serial);    // 硬體序列埠 (HW Serial)，使用 Arduino 內建 Serial
BM22S3031_1 gas(STATUS1, &Serial1); // 使用 BMduino 上的 Serial1 作為通訊介面
// BM22S3031_1 gas(STATUS2, &Serial2); // 使用 BMduino 上的 Serial2 作為通訊介面


// ------------------ 初始化函式 ------------------
void setup()
{
  gas.begin(); // 初始化模組，設定通訊速率為 9600bps，並將 STATUS 腳位 (此例為 8) 設定為輸入模式

  Serial.begin(9600); // 啟動 Arduino 的序列埠監控器，鮑率設定為 9600bps

  // 提示使用者模組正在進行預熱
  Serial.println("Module preheating...(about 3 mins)");
  gas.preheatCountdown(); // 等待模組完成預熱，大約需要 3 分鐘
  Serial.println("End of module preheating.");
  Serial.println();

  delay(1200); // 延遲 1.2 秒，確保模組已經穩定
}


// ------------------ 主迴圈 ------------------
void loop()
{
  // 檢查模組是否有傳回資訊封包
  if (gas.isInfoAvailable() == true)
  {
    // 讀取感測模組的完整封包資訊，存放到 moduleInfo 陣列中
    gas.readInfoPackage(moduleInfo);

    // 將部份資訊印出到序列埠監控器
    printInfo();
  }
}


// ------------------ 輔助函式：輸出資訊 ------------------
void printInfo()
{
  /* 輸出氣體濃度警報門檻值 (PPM) */
  Serial.print("Gas alarm threshold: ");
  gasAlarmThreshold = (moduleInfo[23] << 8) + moduleInfo[24];  
  // 將第 23 與 24 位元組組合成 16 位元數值
  Serial.print(gasAlarmThreshold);
  Serial.println(" PPM");

  /* 輸出目前檢測到的氣體濃度 (PPM) */
  Serial.print("Gas concentration: ");
  gasValue = (moduleInfo[9] << 8) + moduleInfo[10];  
  // 將第 9 與 10 位元組組合成氣體濃度數值
  Serial.print(gasValue);
  Serial.println(" PPM");

  /* 輸出氣體 AD 原始值 (類比數位轉換值) */
  Serial.print("Gas AD Value: ");
  ADValue = (moduleInfo[5] << 8) + moduleInfo[6];  
  // 將第 5 與 6 位元組組合成 AD 數值
  Serial.println(ADValue);

  Serial.println(); // 換行，讓輸出結果更整齊
}
