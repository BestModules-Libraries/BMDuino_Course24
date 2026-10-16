/*************************************************
File:         readAlcLevelAndADValue
Description:  接收模組每秒自動輸出的資料，並將酒精濃度數值 (AlcLevel) 
              與酒精感測器的 A/D 轉換值 (ADValue) 輸出到序列埠監控視窗。
Note:         本程式需等待模組預熱約 3 分鐘，數據才會穩定。
**************************************************/

#include <BM22S3421-1.h>   // 引入酒精感測模組 BM22S3421-1 的函式庫

// 宣告一個 14 bytes 的陣列，用來儲存模組回傳的完整資訊封包
uint8_t moduleInfo[14] = {0};

// 宣告與酒精數據相關的變數
uint16_t ADValue, AlcLevel, ADValueH, ADValueL;

// 以下是模組初始化的方式（僅能選擇一種方式使用，其他的註解掉）
// BM22S3421_1 Alc(8, 2, 3);       // 使用軟體序列通訊，Pin8 = STATUS，Pin2 = RX，Pin3 = TX
// BM22S3421_1 Alc(8, &Serial);    // 使用硬體序列通訊，Pin8 = STATUS，UART = Serial (Arduino 內建序列)
// BM22S3421_1 Alc(STATUS1, &Serial1); // 使用 BMduino-UNO 的 Serial1，Pin22(STATUS1) = STATUS
// BM22S3421_1 Alc(STATUS2, &Serial2); // 使用 BMduino-UNO 的 Serial2，Pin25(STATUS2) = STATUS

BM22S3421_1 Alc(STATUS1, &Serial1); // 這裡使用 STATUS1 腳位，並透過 Serial1 與模組通訊

// 初始化函式
void setup()
{
  Alc.begin();        // 初始化軟體/硬體序列埠，波特率 = 9600bps，STATUS 腳位設為輸入模式
  Serial.begin(9600); // 啟動序列監控 (Arduino IDE Serial Monitor)，波特率 = 9600bps

  Serial.println("Module power on preheating...(about 3 mins)");
  preheatCountdown(); // 執行模組預熱倒數，等待感測器達到工作溫度
  Serial.println("End of preheating.");
  Serial.println();

  // 如果需要手動設定工作模式，可取消下行註解：
  // Alc.writeCommand(0xe0, 0x1e, AUTO_MODE); // 設定模組為自動輸出資料模式 (預設為 AUTO_MODE)
}

// 主迴圈
void loop()
{
  // 持續檢查模組是否有新資料可讀取
  if (Alc.isInfoAvailable() == true) // 掃描序列緩衝區是否有新的封包
  {
    Alc.readInfoPackage(moduleInfo); // 讀取模組回傳的資訊並存入 moduleInfo 陣列
    printInfo();                     // 呼叫函式，將數據輸出到序列監控視窗
  }
}

// 印出模組輸出的酒精濃度與 A/D 數值
void printInfo()
{
  /* 印出酒精濃度數值 (Level) */
  Serial.print("Alc level: ");
  AlcLevel = moduleInfo[7];  // 模組封包的第 7 byte 存放酒精濃度等級
  Serial.println(AlcLevel);

  /* 印出酒精感測器的 A/D 轉換值 */
  Serial.print("Alc A/D Value: ");
  ADValue = ((uint16_t)moduleInfo[5] << 8) + moduleInfo[6]; 
  // 第 5 與第 6 byte 分別為 A/D 數值的高位與低位，需組合成 16 位元整數
  Serial.println(ADValue);
  Serial.println();
}

// 模組預熱倒數函式 (約 180 秒)
void preheatCountdown()
{
  int16_t time = 180;   // 預熱時間 (秒數) = 180 秒 = 3 分鐘
  delay(1200);          // 延遲約 1.2 秒，避免剛啟動的瞬間干擾

  // 如果模組已經開始輸出自動模式的數據 (AUTO_MODE)
  if (Alc.isInfoAvailable() == true)
  {
    while (time > 0) 
    {
      if (Alc.isInfoAvailable() == true) // 持續檢查是否有新封包
      {
        Alc.readInfoPackage(moduleInfo); // 讀取封包
        time = moduleInfo[10];           // 模組封包的第 10 byte 會回傳剩餘預熱時間
        Serial.print("time:");
        Serial.println(time);
      }
    }
  }
  else
  {
    // 如果模組在命令模式 (CMD_MODE)，則只能手動遞減倒數
    while (time > 0) 
    {
      time--;          // 每秒減 1
      delay(1030);     // 約 1 秒延遲
    }
  }
}
