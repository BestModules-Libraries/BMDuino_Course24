/***********************************************************
檔案名稱：ISO14443A_UID.ino
功能描述：
1. 使用 Serial4（硬體串列埠）與 BMC11T001 RFID 讀寫模組進行通訊，波特率為 115200。
2. 使用預設的 Serial（Serial0）與電腦的序列埠監控工具通訊，波特率為 9600。
3. 程式功能為：讀取 ISO14443A 類型卡片的 UID（唯一識別碼），並透過序列埠監控顯示。
4. 支援 EEPROM 資料讀寫功能（此範例僅示範讀 UID）。
硬體連接方式：直接插在 BMduino UNO 開發板上使用。

***********************************************************/
#include "BMC11T001.h"  // 引入控制 BMC11T001 RFID 模組的函式庫

// 建立一個 BMC11T001 的物件，並設定使用 Serial4 作為通訊埠
BMC11T001 BMC11(&Serial4);

// 宣告變數：儲存讀取到的 UID 長度
int nlens;

// 宣告陣列：用來儲存從 RFID 卡片讀取到的 UID 資料（最大 50 bytes）
uint8_t uid_buf[50] = {0};

void setup() 
{
  delay(1000);  // 延遲 1 秒，等待 BMC11T001 模組完成啟動初始化
  
  BMC11.begin(115200);   // 啟動 Serial4 串列通訊，設定波特率為 115200（與模組一致）
  
  Serial.begin(9600);    // 啟動電腦序列埠通訊，設定波特率為 9600（用於顯示資料）

  BMC11.begin_ISO14443A();  // 初始化 ISO14443A 協定（通常是 MIFARE 卡片）
}

void loop()
{
  // 取得目前卡片的 UID 並存入 uid_buf，回傳值為 UID 的長度（bytes）
  nlens = BMC11.getUID_ISO14443A(uid_buf);

  // 使用 Serial.write 將 UID 的二進位原始值輸出到序列埠監控工具
  Serial.write(uid_buf, nlens);

  // 輸出換行符號，讓每筆資料換行顯示
  Serial.println(" ");

  // 每 2 秒讀取一次卡片 UID
  delay(2000);
}
