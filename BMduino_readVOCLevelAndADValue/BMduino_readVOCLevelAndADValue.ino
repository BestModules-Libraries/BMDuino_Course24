/*************************************************
File:         readVOCLevelAndADValue
Description:  Receive the information automatically output by the module every second,
               and prints the VOC level and VOC A/D value to the serial port monitor
Note:
**************************************************/

#include <BM25S3421-1.h>  
// 引入 BM25S3421-1 VOC 感測器模組的函式庫，
// 用於與該感測器進行資料讀取與控制。

// 儲存從模組接收到的完整資料封包，大小為 14 個位元組
uint8_t moduleInfo[14] = {0};  

// VOC A/D 轉換值與 VOC 等級的變數
uint16_t ADValue, VOCLevel;

// 不同初始化方式（只會啟用其中一種）
// BM25S3421_1 VOC(8, 2, 3); // 軟體序列埠模式，8: STATUS 腳位, 2: RX 腳位, 3: TX 腳位
// BM25S3421_1 VOC(8, &Serial); // 硬體序列埠模式，8: STATUS 腳位，使用 Serial（UART0）

BM25S3421_1 VOC(STATUS1, &Serial1); 
// 使用硬體序列埠 Serial1 與感測器溝通
// STATUS1 腳位（22 號）接到模組的狀態訊號輸出腳，
// Serial1 負責資料通訊（UART1）
// 備註：在 Arduino Mega / ESP32 等支援多 UART 的開發板上可用

// BM25S3421_1 VOC(STATUS2, &Serial2); // 同理，使用 STATUS2 與 Serial2

void setup()
{
  VOC.begin();        
  // 初始化感測器的 SW/HW 序列埠設定
  // 預設鮑率 9600 bps，STATUS 腳位設為輸入模式
  
  Serial.begin(9600); 
  // 初始化主序列埠（Serial），用於與電腦上的序列監控視窗溝通

  Serial.println("Module power on  preheating...(about 3 mins)");
  // 提示使用者模組正在加熱（預熱時間約 3 分鐘）

  preheatCountdown(); 
  // 執行模組的預熱倒數程序（避免剛開機數據不準確）

  Serial.println("End of preheating.");
  Serial.println();

  // VOC.writeCommand(0xe0, 0x1e, AUTO_MODE);
  // 這行可選，設定模組為自動輸出模式（AUTO_MODE），
  // 預設已經是自動模式，所以註解掉
}

void loop()
{
  // 檢查是否有來自感測器的資料封包可讀取
  if (VOC.isInfoAvailable() == true) 
  {
    // 讀取模組傳來的完整資料封包，存到 moduleInfo 陣列
    VOC.readInfoPackage(moduleInfo);

    // 將資料封包中關於 VOC 等級與 A/D 值的資訊輸出到序列監控視窗
    printInfo();
  }
}

void printInfo()
{
  /* 印出 VOC 等級 */
  Serial.print("VOC level: ");
  VOCLevel = moduleInfo[7];  
  // moduleInfo[7] 儲存的是 VOC 等級（Level）
  Serial.println(VOCLevel);

  /* 印出 VOC A/D 轉換值 */
  Serial.print("VOC A/D Value: ");
  ADValue = ((uint16_t)moduleInfo[5] << 8) + moduleInfo[6];  
  // moduleInfo[5] 與 moduleInfo[6] 是 VOC A/D 值的高位元組與低位元組
  // 先將高位左移 8 bits，再加上低位組合成完整的 16 位元數值
  Serial.println(ADValue);

  Serial.println(); // 空行，讓輸出更易讀
}

void preheatCountdown()
{
  int16_t time = 180; // 預設倒數時間 180 秒（3 分鐘）
  delay(1200);        // 延遲約 1.2 秒，確保模組已開始傳送資訊

  // 如果模組在自動輸出模式（AUTO_MODE）下
  if (VOC.isInfoAvailable() == true)
  {
    while (time > 0) 
    {
      if (VOC.isInfoAvailable() == true)
      {
        VOC.readInfoPackage(moduleInfo);
        time = moduleInfo[10];  
        // moduleInfo[10] 儲存剩餘預熱時間
        Serial.print("time:");
        Serial.println(time);
      }
    }
  }
  else
  {
    // 如果是命令模式（CMD_MODE），就自己遞減倒數
    while (time > 0) 
    {
      time--;
      delay(1030); // 約 1 秒一次
    }
  }
}
