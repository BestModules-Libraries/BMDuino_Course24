/***********************************************************
 File:                ISO14443A_UID.ino
 Description:         1. HardwareSerial4 介面 (BAUDRATE 115200) 用來與 BMC11T001 通訊。
                      2. Hardware serial (BAUDRATE 9600) 用來與序列埠監視視窗 (Serial Monitor) 通訊。
                      功能：取得 ISO14443A 卡片的 UID，並可讀寫其 EEPROM（範例為顯示 UID 到序列埠）。
                      並將結果顯示於序列埠監視器。
 connection method:   直接插上 BMduino UNO
***********************************************************/

#include "BMC11T001.h" 
// 包含 BMC11T001 的驅動函式庫，提供與 BMC11T001 感應器模組（讀卡器）溝通的 API。

// 使用 HardwareSerial4（Serial4）作為 BMC11T001 的通訊埠，建立一個物件 BMC11
BMC11T001     BMC11(&Serial4); // create an object, select Serial4 Interface Communication

int nlens;                              // 用來儲存讀取到 UID 的長度 (byte count)
uint8_t uid_buf[50] = {0};              // UID 讀取暫存緩衝區，最大預留 50 bytes（依 library 規格或可調整）

/* ========================= setup() ========================= */
void setup() 
{ 
  delay(1000);            // 等待 1 秒，讓 BMC11T001 模組完成上電或初始化程序
                          // （有些裝置開機需要短暫的啟動時間，delay 可避免太早通訊導致錯誤）

  BMC11.begin(115200);    // 初始化 BMC11 物件：設定與讀卡器之通訊速率為 115200 bps
                          // 注意：這個 begin() 是教 BMC11 物件使用的通訊速率（對 Serial4）
                          // 實際與 Serial Monitor 的速率是獨立的（由下方 Serial.begin() 決定）

  Serial.begin(9600);     // 開啟 Arduino 與電腦序列埠監視器的通訊（9600 bps）
                          // 提示：若你發現序列監視器的輸出亂碼或看不到正確的 UID，
                          // 請把 Serial.begin(...) 的參數改為 115200，以配合 BMC11.begin(115200) 的設定，
                          // 或確認 library 實際把哪個 Serial 用於輸出。通常建議讓 Serial Monitor baud 與模組輸出相同。

  BMC11.begin_ISO14443A();   // 初始化 BMC11 的 ISO14443A 功能
                            // 這步驟通常會設定讀卡器進入 ISO14443A（Type A，如 MIFARE、NTAG 等）感應模式
}

/* ========================= loop() ========================= */
void loop()
{  
  // 呼叫 library 的函式來取得 ISO14443A 卡片的 UID，
  // 將 UID 資料寫入 uid_buf，並回傳讀到的長度（bytes），存入 nlens
  nlens = BMC11.getUID_ISO14443A(uid_buf); // Gets the UID

  // 若 nlens > 0，代表成功讀到 UID，這裡直接將 uid_buf 的前 nlens bytes 寫到序列埠
  // Serial.write() 會以 raw binary（位元組）方式輸出，若要看到可讀的十六進位字串，可改用自訂輸出格式
  Serial.write(uid_buf, nlens); // The UID is displayed on the serial monitor.             

  // 寫一個換行（以便序列監視器上每次讀到的 UID 分行顯示）
  Serial.println(" ");
  
  delay(2000); // 等待 2 秒後再重試讀取（避免一直高速輪詢）
}
