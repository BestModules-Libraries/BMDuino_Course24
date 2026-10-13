/*******************************************************
 * 範例主題：BMH08002-4 血氧/心率感測器讀取（查詢/連續模式）
 * 板子環境：BMduino / 一般 Arduino 皆可（請依實際 UART 腳位調整）
 * 連線說明：
 *   - 若使用「軟體序列埠建構子」：BMH08002_4(enPin, rxPin, txPin)
 *       enPin：感測器電源/使能腳（拉高啟用）
 *       rxPin：MCU 接收腳（接感測器 TX）
 *       txPin：MCU 傳送腳（接感測器 RX）
 *   - 若使用「硬體序列埠建構子」：BMH08002_4(enPin, &SerialX)
 *       enPin：同上
 *       SerialX：例如 &Serial1 / &Serial2...
 *******************************************************/

#include <BMH08002-4.h>  // 引入血氧感測器驅動程式庫

/********************* 感測器物件宣告 ************************/
// ❶ 預設以「軟體序列埠」方式建立物件（請依實機腳位修改）
BMH08002_4 mySpo2(8, 7, 6);      // enPin=8, rxPin=7, txPin=6

// ❷ 若要改用「硬體序列埠」，請改用下列其中一行（解除註解並註解掉上面那行）
// BMH08002_4 mySpo2(22, &Serial1);  // enPin=22，綁定 Serial1
// BMH08002_4 mySpo2(25, &Serial2);  // enPin=25，綁定 Serial2
// BMH08002_4 mySpo2(2,  &Serial3);  // enPin=2， 綁定 Serial3
// BMH08002_4 mySpo2(2,  &Serial4);  // enPin=2， 綁定 Serial4

/********************* 全域變數 ************************/
uint8_t Mode = 0;           // 讀取模式：0=連續模式(Streaming)，1=查詢/回應模式(Query)
uint8_t rBuf[15] = {0};     // 感測器資料封包緩衝區（長度 15 bytes）
uint8_t Status = 0;         // 狀態碼（由感測器回報）
uint8_t flag = 0;           // 提示旗標（避免重複顯示相同提醒）

// 讀值快取（便於其他模組使用）
int oxyvalue = 0;           // SpO2 血氧飽和度 (%)
int hbvalue  = 0;           // 心率 (BPM)
int pivalue  = 0;           // 灌注指數 PI (原值/10 = %)
int readok   = 0;           // 資料是否成功更新（1=成功；0=未就緒/失敗）

/********************* 前置宣告 ************************/
void initOxygen();                 // 初始化感測器
void readOxygen();                 // 讀取/切換運作模式（會呼叫 beginMeasure）
void Mode_ask();                   // 查詢模式：主動請求一次資料封包
void Mode_continuous_timing();     // 連續模式：若有資料可讀就取出印出

/********************* 初始化：設定量測參數 ************************/
void initOxygen()
{
  mySpo2.begin();                // 初始化感測器（含 enPin 使能）
  mySpo2.setModeConfig(0x02);    // 設定為「查詢回應模式」(0x02)；0x00/0x01 通常為連續
  mySpo2.setTimeInterval(300);   // 量測間隔 300ms（查詢模式的回應時間間隔）
  Serial.println("請放置您的手指，保持穩定以獲得較佳量測結果。");
}

/********************* 進入量測並判定模式 ************************/
void readOxygen()
{
  mySpo2.beginMeasure();           // 送出開始量測指令（紅燈亮表示感測進行中）

  // 讀回目前感測器模式設定
  Mode = mySpo2.getModeConfig();

  // 0x02 / 0x03：屬於「查詢回應模式」族群 → 以查詢流程處理
  if (Mode == 0x02 || Mode == 0x03) {
    Mode = 1;  // 查詢模式
  } else {
    Mode = 0;  // 其他視為連續模式（裝置會主動持續送資料）
  }
}

/********************* 查詢一次資料封包 ************************/
void Mode_ask()
{
  // 主動請求資料，成功時 rBuf 會被填入 15bytes 封包；回傳 Status 狀態碼
  Status = mySpo2.requestInfoPackage(rBuf);
  readok = 0;  // 先預設此次尚未成功

  if (Status == 0x02) { // 0x02：一次有效量測完成
    Serial.println("測量完成，可以移開手指。");

    // rBuf[0]：SpO2（血氧%）
    Serial.print("SpO2: ");
    Serial.print(rBuf[0], DEC);
    Serial.println("%");
    oxyvalue = rBuf[0];

    // rBuf[1]：心率 BPM
    Serial.print("心率: ");
    Serial.print(rBuf[1], DEC);
    Serial.println(" BPM");
    hbvalue = rBuf[1];

    // rBuf[2]：灌注指數 PI（需/10 轉百分比）
    Serial.print("PI: ");
    Serial.print((float)rBuf[2] / 10.0f);
    Serial.println("%");
    pivalue = rBuf[2];

    readok = 1;            // 本輪讀值成功
   // mySpo2.endMeasure();   // 結束量測（避免持續耗電）
   // mySpo2.sleep();        // 進入低功耗
  }

  // 0x01：偵測到手指但量測中，且疑似有移動 → 提示使用者保持穩定
  if (Status == 0x01 && flag != 1) {
    Serial.println("請勿移動您的手指（保持靜止以利量測）。");
    flag = 1;
    readok = 0;
  }

  // 0x00：未檢測到手指 → 請使用者重新放置
  if (Status == 0x00 && flag != 0) {
    Serial.println("未偵測到手指，請重新放置並覆蓋感測區。");
    flag = 0;
    readok = 0;
  }
}

/********************* 連續模式取樣：有資料就讀出 ************************/
void Mode_continuous_timing()
{
  // 連續模式下，裝置會把資料推到 UART；此處輪詢接收緩衝區
  if (mySpo2.isInfoAvailable()) {
    mySpo2.readInfoPackage(rBuf); // 把一包 15bytes 讀進 rBuf

    // 示範：逐位輸出（除錯/驗證封包結構用）
    for (uint8_t i = 0; i < 15; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(rBuf[i]);
    }

    // 亦可在此解析關鍵欄位，與查詢模式相同
    oxyvalue = rBuf[0];
    hbvalue  = rBuf[1];
    pivalue  = rBuf[2];
    readok   = 1;
  }
}