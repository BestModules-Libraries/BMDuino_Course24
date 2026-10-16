/*************************************************
  檔案:             BMduino_simpleWriteAndRead.ino
  描述:      將模組連接到手機應用程式（任何BLE偵錯助手）；
             APP可以透過「寫入UUID」向模組發送數據，模組接收相應數據並印出到序列監視器；
             APP可以透過「讀取UUID」監聽模組發送的數據，模組發送的數據可透過序列監視器輸入。
  注意:
  操作:
**************************************************/

#include "BM7701-00-1.h"  //BMduino Bluetooth BLE Modules Library

// 使用軟體序列埠（RX腳位=2, TX腳位=3），若使用硬體序列埠請註解此行
// BM7701_00_1       BC7701(2, 3); // rxPin, txPin

// 若使用硬體序列埠，請取消對應行的註解
//BM7701_00_1     BC7701(&Serial1); // 使用 BMduino 上的 HW Serial1
BM7701_00_1     BC7701(&Serial2); // 使用 BMduino 上的 HW Serial2
//BM7701_00_1     BC7701(&Serial3); // 使用 BMduino 上的 HW Serial3
//BM7701_00_1     BC7701(&Serial4); // 使用 BMduino 上的 HW Serial4

// 定義 BLE 參數
#define TX_POWER     0x0F                   // 發射功率
#define XTAL_CLOAD   0x04                   // 16MHz 晶體負載電容
#define ADV_MIN      100                    // 廣播間隔最小值 = 100ms
#define ADV_MAX      100                    // 廣播間隔最大值 = 100ms
#define CON_MIN      30                     // 連線間隔最小值 = 30ms
#define CON_MAX      30                     // 連線間隔最大值 = 30ms
#define CON_LATENCY  00                     // 連線延遲 = 0
#define CON_TIMEOUT  300                    // 連線逾時 = 300ms

uint8_t BDAddress[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};  // 裝置藍牙位址
uint8_t BDName[] = {'B', 'M', 'C', '7', '7', 'M', '0', '0', '1'}; // 裝置名稱（最多 31 個字元）
uint8_t Adata[] = {0x02, 0x01, 0x06};      // 廣播數據（BLE 標準廣播格式）
uint8_t Sdata[] = {0x03, 0x02, 0x0f, 0x18}; // 掃描回應數據（包含完整服務 UUID）

/////////////////////////////////////////////////////////////////////////////////////////

// 按鍵相關定義（可能為預留功能或範例程式碼片段）
#define BUTTON_CONSISTENCY_DURATION    6
#define BUTTON_REPEAT1_DURATION       (600 / BUTTON_CONSISTENCY_DURATION)
#define BUTTON_REPEAT2_DURATION       (150 / BUTTON_CONSISTENCY_DURATION)
#define INVERT_TIME                   500

// 狀態旗標變數
bool board_connect = false;   // 是否已連線
bool board_receive = false;   // 是否接收到數據
bool board_conIntv = false;   // 連線間隔是否已設定
uint8_t Status;               // BLE 狀態碼
uint8_t flag=0;               // 通用旗標
uint8_t count=0;              // 計數器
uint8_t sel = 1;              // 初始化步驟選擇器

// 數據緩衝區
uint8_t receiveBuf[256] = {0}; // 接收緩衝區
uint8_t receiveLen = 0 ;       // 接收數據長度
uint8_t sendBuf[256] = {0};    // 發送緩衝區
uint8_t sendLen = 0;           // 發送數據長度
uint32_t timeLast = 0;         // 用於時間記錄

// 初始化設定
void setup() 
{
  delay(60); // 上電後延遲 60ms，期間不能發送命令
  Serial.begin(9600);          // 初始化序列埠，用於與電腦通訊
  BC7701.begin(BAUD_115200);   // 初始化 BLE 模組通訊序列埠

  // BLE 模組初始化流程（依序設定各項參數）
  while (sel != 10) // 直到所有步驟完成（sel=10 表示完成）
  {
    switch (sel)
    {
      case 1: if (BC7701.setAddress(BDAddress) == true) sel++; // 設定藍牙位址
        else sel = 0xFF; break; // 若失敗則跳至錯誤處理
      case 2: if (BC7701.setName(sizeof(BDName), BDName) == true) sel++; // 設定裝置名稱
        else sel = 0xFF; break;
      case 3: if (BC7701.setAdvIntv(ADV_MIN / 0.625, ADV_MAX / 0.625, 7) == true) sel++; // 設定廣播間隔
        else sel = 0xFF; break;
      case 4: if (BC7701.setAdvData(APPEND_NAME, sizeof(Adata), Adata) == true) sel++; // 設定廣播數據
        else sel = 0xFF; break;
      case 5: if (BC7701.setScanData(sizeof(Sdata), Sdata) == true) sel++; // 設定掃描回應數據
        else sel = 0xFF; break;
      case 6: if (BC7701.setTXpower(TX_POWER) == true) sel++; // 設定發射功率
        else sel = 0xFF; break;
      case 7: if (BC7701.setCrystalOffset(XTAL_CLOAD) == true) sel++; // 設定晶體負載
        else sel = 0xFF; break;
      case 8: if (BC7701.setFeature(FEATURE_DIR, AUTO_SEND_SATUS) == true) sel++; // 啟用自動發送狀態
        else sel = 0xFF; break;
      case 9: if (BC7701.setAdvCtrl(ENABLE) == true) sel++; // 開啟廣播
        else sel = 0xFF; break;
      case 0xFF: digitalWrite(13,HIGH); // 錯誤處理：點亮板載 LED（腳位 13）
        break;
    }
  }
  delay(650); // 開啟廣播後延遲 650ms，期間不能發送命令
  Serial.println("BLE init completed"); // 印出初始化完成訊息
}

// 主迴圈
void loop()
{
  // 讀取 BLE 狀態與接收數據(透傳模式)
  Status = BC7701.receiveData(receiveBuf,receiveLen);

  if (Status) // 若有狀態事件發生
  {
    switch (Status)
    {
      case API_CONNECTED: // BLE 已連線
        if (board_connect == false)
        {
          board_connect = true;
          board_receive = false;
          if (board_conIntv == false) // 連線成功後需設定一次連線間隔
          {
            BC7701.wakeUp(); // 喚醒模組
            delay(30);
            // 設定連線間隔參數
            if (BC7701.setConnIntv(CON_MIN / 1.25, CON_MAX / 1.25, CON_LATENCY, CON_TIMEOUT) == true)
            {
              board_conIntv = true;
              Serial.println("Set ConnIntv completed");
            }
          }
        }
        break;
      case API_DISCONNECTED: // BLE 斷線
        board_connect = false;
        board_receive = false;
        board_conIntv = false;
        break;
      case DATA_RECEIVED: // 收到數據
        if (board_connect == true)
        {
          digitalWrite(13,LOW); // 收到數據時關閉 LED（可視為指示）
          board_receive = true; // 標記有待處理的接收數據
        }
        break;
      case API_ERROR: // BLE 錯誤
         digitalWrite(13,HIGH); // 點亮 LED 表示錯誤
        break;
    }
  }

  // 處理接收到的數據
  if (board_receive == true)
  {
    board_receive = false; // 重置接收旗標

    /* 使用者數據處理區塊 */
    Serial.print("APP to BLE : "); // 印出提示訊息
    for(uint8_t i=0;i<receiveLen;i++) // 逐位元組印出接收到的數據（十六進制格式）
    {
      Serial.print(receiveBuf[i],HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }

  // 檢查序列監視器是否有輸入數據需發送至 BLE
  if(readSerialMonitor(sendBuf,sendLen) == true) // 若序列監視器有數據
  {
    if(BC7701.sendData(sendBuf,sendLen) == true) // 發送數據至 BLE
    {
      Serial.print("BLE to APP : "); // 印出發送提示
      for(uint8_t i=0;i<sendLen;i++) // 逐位元組印出發送的數據（十六進制格式）
      {
        Serial.print(sendBuf[i],HEX);
        Serial.print(" ");
      }
      Serial.println("");
    }
    else
    {
      Serial.println("BLE to APP : Fail"); // 發送失敗訊息
    }
  }
}

// 讀取序列監視器輸入的數據
bool readSerialMonitor(uint8_t buff[],uint8_t &len)
{
  if(Serial.available() > 0) // 檢查是否有可讀數據
  {
    sendLen = 0; // 重置發送長度
    while(Serial.available() != 0) // 持續讀取直到緩衝區為空
    {
      timeLast = millis(); // 記錄當前時間
      sendBuf[sendLen] = Serial.read(); // 讀取一個位元組
      sendLen++; // 增加長度計數
      while(Serial.available() == 0) // 等待下一個位元組
      {
        if(millis() - timeLast >= 50) // 若超過 50ms 未收到下一個位元組，視為數據包結束
        {
          break;
        }
      }
    }
    return true; // 回傳 true 表示有讀取到數據
  }
  return false; // 回傳 false 表示無數據
}