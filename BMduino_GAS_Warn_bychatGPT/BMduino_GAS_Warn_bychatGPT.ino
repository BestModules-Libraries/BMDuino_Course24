/*******************************************************
 * 專案名稱：BMduino + BMA36M302（半導體式燃氣偵測模組）UART 讀值與安全告警
 * 硬體說明：
 *   - 感測器：BMA36M302（內建 BM22S3021-1）
 *   - 介面：UART（感測資料/狀態）、STA 輸出（硬體警報）
 *   - 供電：VDD 4.9V~5.1V，GND 共地，STA 為數位輸出腳位
 *   - BMA36M302 腳位（官方頁面）：STA, RX, TX, VDD, GND
 *     STA 觸發時（超過校準濃度）預設由 LOW 轉 HIGH（警報）  <-- 觸發條件以模組內建校準點為準
 *
 * 程式需求：
 *   1) 使用多個自訂函式拆分：初始化、讀值、判斷、安全提示、列印等
 *   2) loop() 每 3 分鐘（180,000 ms）讀取一次感測器資料
 *   3) 當濃度超過「模組校準之警報濃度」或 STA 觸發，顯示「注意安全」
 *
 * 參考資料：
 *   - BMA36M302 產品頁（含腳位與 STA 行為）：https://www.bestmodulescorp.com/tc/bma36m302.html
 *   - Arduino 函式庫（BM22S3021-1/BMA36M302）：https://github.com/BestModules-Libraries/BM22S3021-1
 *******************************************************/

#include <Arduino.h>

// === 官方函式庫 ===
// 函式庫名稱：BM22S3021-1（適用 BMA36M302/BMA36M302A）
// 安裝方式：Arduino IDE -> 管理程式庫 -> 搜尋 "BM22S3021-1"
// 注意：標頭檔名多為「BM22S3021_1.h」；若您的版本不同，請依實際檔名調整。
#include "BM22S3021_1.h"

// === 串列埠選擇（依您的 BMduino 型號與接線調整）===
//
// 方案 A：使用硬體序列埠（建議，有效避免軟序列的時序限制）
//   - 將 BMA36M302.TX -> BMduino.RX (例如 Serial1 RX)
//   - 將 BMA36M302.RX -> BMduino.TX (例如 Serial1 TX)
//
// 方案 B：使用軟體序列埠（若板上沒有多組硬體序列可用，或腳位佈局需求）
//   - 需安裝 <SoftwareSerial.h>，但在某些 MCU 或較高鮑率下可能不穩定。
//
// 請依需求取消/保留註解。預設示範使用硬體序列 Serial1。

// -------------------- 使用硬體序列 --------------------
#define USE_HARDWARE_SERIAL 1
#if USE_HARDWARE_SERIAL
  HardwareSerial &SENSOR_SERIAL = Serial1;  // 依實板調整（如有 Serial2/Serial3）
  const uint32_t SENSOR_BAUD = 9600;        // 多數 BestModules 安全/環境感測模組常見 9600；若官方範例不同請更改
#else
// -------------------- 使用軟體序列（備用） --------------------
  #include <SoftwareSerial.h>
  // 請依實際佈線修改：rxPin <- 連到感測器 TX，txPin -> 連到感測器 RX
  const uint8_t RX_PIN = 4; // 例：D4 接模組 TX
  const uint8_t TX_PIN = 5; // 例：D5 接模組 RX
  SoftwareSerial SENSOR_SERIAL(RX_PIN, TX_PIN);
  const uint32_t SENSOR_BAUD = 9600;
#endif

// === STA 硬體警報腳位（BMA36M302 的 pin1）===
// 官方說明：超過校準濃度時 STA 由低轉高；未觸發時為低（預設）
// 請接到 BMduino 任何可讀取數位輸入之腳位，並依實接腳位修改。
const uint8_t PIN_STA = 8;   // 例：D8 讀取模組 STA

// === 讀取間隔（3 分鐘）===
const unsigned long READ_INTERVAL_MS = 180000UL;  // 180,000 ms = 3 分鐘

// === 物件：感測器驅動 ===
// 大多數 BestModules 函式庫會接受 Stream 或 Serial 物件；若您的版本需要 STA 腳位一起傳入，請依實際函式庫調整建構式。
BM22S3021_1 gas(&SENSOR_SERIAL);  // 若您的庫需要 gas(PIN_STA, &SENSOR_SERIAL) 請改用該建構式

// === 狀態變數 ===
unsigned long g_lastReadMillis = 0;

// === 前置宣告：自訂函式 ===
void initSerialPorts();
void initPins();
bool sensorBegin();
bool readSensorOnce(uint16_t &adRaw, uint16_t &alarmPoint, uint8_t &workStatus);
bool isStaAlarm();
void printReading(uint16_t adRaw, uint16_t alarmPoint, uint8_t workStatus);
void checkSafetyAndAlert(uint16_t adRaw, uint16_t alarmPoint, bool staAlarm);
void requestInfoPacketSafeDelay();

// ------------------------------------------------------
// setup：初始化序列埠、腳位、感測器
// ------------------------------------------------------
void setup() {
  // 1) 初始化 USB 序列埠：用於監看列印（Serial Monitor）
  Serial.begin(115200);
  while (!Serial) { ; }  // 等待序列埠就緒（視板子而定）

  Serial.println();
  Serial.println(F("=== BMduino + BMA36M302 UART 讀值與安全告警 ==="));
  Serial.println(F("提示：請確認 BMA36M302 接腳 -> RX/TX/VDD/GND 與 BMduino 對應正確，STA 接至數位輸入。"));

  // 2) 初始化感測器序列與腳位
  initSerialPorts();
  initPins();

  // 3) 初始化感測器
  if (!sensorBegin()) {
    Serial.println(F("[錯誤] 感測器初始化失敗，請檢查接線、鮑率或函式庫版本！"));
  } else {
    Serial.println(F("[完成] 感測器初始化成功。"));
  }

  // 立即做第一次讀取（不必等 3 分鐘）
  uint16_t adRaw = 0, alarmPoint = 0;
  uint8_t workStatus = 0;
  if (readSensorOnce(adRaw, alarmPoint, workStatus)) {
    bool staAlarm = isStaAlarm();
    printReading(adRaw, alarmPoint, workStatus);
    checkSafetyAndAlert(adRaw, alarmPoint, staAlarm);
  } else {
    Serial.println(F("[警告] 初次讀取失敗，稍後將於排程再次嘗試。"));
  }

  g_lastReadMillis = millis();
}

// ------------------------------------------------------
// loop：每 3 分鐘讀取一次（非阻塞計時），其餘時間可執行其他任務
// ------------------------------------------------------
void loop() {
  unsigned long now = millis();

  // 到達讀取間隔就執行一次
  if (now - g_lastReadMillis >= READ_INTERVAL_MS) {
    g_lastReadMillis = now;

    uint16_t adRaw = 0, alarmPoint = 0;
    uint8_t workStatus = 0;

    if (readSensorOnce(adRaw, alarmPoint, workStatus)) {
      bool staAlarm = isStaAlarm();
      printReading(adRaw, alarmPoint, workStatus);
      checkSafetyAndAlert(adRaw, alarmPoint, staAlarm);
    } else {
      Serial.println(F("[警告] 讀取失敗（逾時或資料無效）。下次間隔再試。"));
    }
  }

  // ...此處可放置其他背景工作（例如：LED 閃爍、看門狗、網路維護等）
}

// ======================================================
// 以下為自訂函式區塊
// ======================================================

/** 初始化與感測器相連的序列埠 */
void initSerialPorts() {
#if USE_HARDWARE_SERIAL
  SENSOR_SERIAL.begin(SENSOR_BAUD);
#else
  SENSOR_SERIAL.begin(SENSOR_BAUD);
#endif
  Serial.print(F("[資訊] 感測器 UART 鮑率："));
  Serial.println(SENSOR_BAUD);
}

/** 初始化腳位（STA 輸入）*/
void initPins() {
  pinMode(PIN_STA, INPUT);  // STA 為感測器輸出->MCU 輸入
  Serial.print(F("[資訊] STA 讀取腳位：D"));
  Serial.println(PIN_STA);
}

/** 感測器初始化
 *  說明：多數 BestModules 函式庫提供 begin() 或 reset()/wake() 等初始流程。
 *       若您的函式庫版本不同，請參考其 README 調整。
 */
bool sensorBegin() {
  // 若函式庫提供 begin()，可於此呼叫；以下示意回傳 true 視為成功。
  // 亦可於此做一次基本通訊確認（如請求資訊包）。
  delay(100);
  requestInfoPacketSafeDelay();
  return true;
}

/** 主動要求感測器回傳資訊封包後，讀一次資料
 *  讀值包含：
 *   - adRaw       ：感測器 ADC/內部量測原始值（函式庫命名為 readADValue）
 *   - alarmPoint  ：模組內建之校準警報點（函式庫命名為 readAlarmPoint）
 *   - workStatus  ：工作狀態（函式庫命名為 getWorkStatus）
 */
bool readSensorOnce(uint16_t &adRaw, uint16_t &alarmPoint, uint8_t &workStatus) {
  bool ok = true;

  // 1) 要求感測器主動回覆資訊封包（避免讀到舊資料）
  requestInfoPacketSafeDelay();

  // 2) 讀取工作狀態
  if (!gas.getWorkStatus(workStatus)) {
    ok = false;
    Serial.println(F("[錯誤] 讀取工作狀態失敗（getWorkStatus）。"));
  }

  // 3) 讀取 AD 原始值（代表濃度相關的即時量測）
  uint16_t adTmp = 0;
  if (!gas.readADValue(adTmp)) {
    ok = false;
    Serial.println(F("[錯誤] 讀取 AD 值失敗（readADValue）。"));
  } else {
    adRaw = adTmp;
  }

  // 4) 讀取模組內建之警報門檻（校準濃度點）
  uint16_t ap = 0;
  if (!gas.readAlarmPoint(ap)) {
    ok = false;
    Serial.println(F("[錯誤] 讀取警報門檻失敗（readAlarmPoint）。"));
  } else {
    alarmPoint = ap;
  }

  return ok;
}

/** 讀取 STA 硬體警報腳位（HIGH 表示觸發，LOW 表示未觸發） */
bool isStaAlarm() {
  int v = digitalRead(PIN_STA);
  return (v == HIGH);  // 官方預設：觸發時 STA=HIGH，未觸發 STA=LOW
}

/** 印出一次讀值結果（提供偵錯與監看） */
void printReading(uint16_t adRaw, uint16_t alarmPoint, uint8_t workStatus) {
  Serial.println(F("--------------------------------------------------"));
  Serial.print(F("工作狀態 (workStatus)："));
  Serial.println(workStatus);

  Serial.print(F("AD 原始值 (readADValue)："));
  Serial.println(adRaw);

  Serial.print(F("警報門檻 (readAlarmPoint)："));
  Serial.println(alarmPoint);

  Serial.print(F("STA 腳位(硬體)狀態："));
  Serial.println(isStaAlarm() ? F("觸發(HIGH)") : F("未觸發(LOW)"));
  Serial.println(F("--------------------------------------------------"));
}

/** 安全判斷與告警：
 *  條件一：AD 原始值 >= 模組警報門檻（代表超過校準濃度）
 *  條件二：STA 腳位為 HIGH（模組硬體已判斷為警報）
 *  任一條件成立則印出「注意安全」
 */
void checkSafetyAndAlert(uint16_t adRaw, uint16_t alarmPoint, bool staAlarm) {
  bool overThreshold = (adRaw >= alarmPoint);
  if (overThreshold || staAlarm) {
    Serial.println(F("★★★ 注意安全 ★★★"));
    Serial.println(F("偵測到可燃氣體濃度已超過安全門檻，請立即檢查環境、加強通風並遠離火源！"));
  } else {
    Serial.println(F("目前濃度低於警戒門檻，狀態正常。"));
  }
}

/** 依函式庫建議：讀值前主動要求感測器回覆資訊封包，再留一點處理時間 */
void requestInfoPacketSafeDelay() {
  // 某些版本函式庫提供 requestInfoPackage() 以觸發主動回覆；
  // 本呼叫後稍等一小段時間，確保下一步 readXXX 可讀到新鮮資料。
  gas.requestInfoPackage();
  delay(50);  // 依實測可微調，避免過長阻塞
}
