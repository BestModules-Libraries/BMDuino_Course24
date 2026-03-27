/*************************************************
  描述:      將模組連接到手機應用程式（任何BLE偵錯助手）；
             APP可以透過「寫入UUID」向模組發送數據，模組接收相應數據並印出到序列監視器；
             APP可以透過「讀取UUID」監聽模組發送的數據，模組發送的數據可透過序列監視器輸入。
  注意:
  操作:
**************************************************/

// 匯入 BM7701 BLE 模組的函式庫
#include "BM7701-00-1.h"  // BMduino 藍牙 BLE 模組函式庫

// 使用軟體序列埠（RX腳位=2, TX腳位=3），若使用硬體序列埠請註解此行
// BM7701_00_1       BC7701(2, 3); // rxPin, txPin

// 若使用硬體序列埠，請取消對應行的註解
//BM7701_00_1     BC7701(&Serial1); // 使用 BMduino 上的硬體序列埠 Serial1
BM7701_00_1     BC7701(&Serial2); // 使用 BMduino 上的硬體序列埠 Serial2
//BM7701_00_1     BC7701(&Serial3); // 使用 BMduino 上的硬體序列埠 Serial3
//BM7701_00_1     BC7701(&Serial4); // 使用 BMduino 上的硬體序列埠 Serial4

// 定義 BLE 相關參數
#define TX_POWER     0x0F                   // 發射功率設定
#define XTAL_CLOAD   0x04                   // 16MHz 晶體負載電容值
#define ADV_MIN      100                    // 廣播間隔最小值 = 100ms
#define ADV_MAX      100                    // 廣播間隔最大值 = 100ms
#define CON_MIN      30                     // 連線間隔最小值 = 30ms
#define CON_MAX      30                     // 連線間隔最大值 = 30ms
#define CON_LATENCY  00                     // 連線延遲次數 = 0
#define CON_TIMEOUT  300                    // 連線逾時時間 = 300ms

// 設定裝置的藍牙位址（6 bytes）
uint8_t BDAddress[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
//BDAddress[0] = (int)random(255); // 可選：隨機產生第一個位元組

// 設定裝置名稱（最多 31 個字元）
uint8_t BDName[] = {'B', 'M', 'C', '7', '7', 'M', '0', '0', '1'};

// 廣播數據（使用 BLE 標準格式，此處代表 Flags 欄位）
uint8_t Adata[] = {0x02, 0x01, 0x06};

// 掃描回應數據（包含完整的服務 UUID）
uint8_t Sdata[] = {0x03, 0x02, 0x0f, 0x18};

/////////////////////////////////////////////////////////////////////////////////////////

// 按鍵相關定義（可能用於去彈跳或長按偵測，但目前尚未使用）
#define BUTTON_CONSISTENCY_DURATION    6
#define BUTTON_REPEAT1_DURATION       (600 / BUTTON_CONSISTENCY_DURATION)
#define BUTTON_REPEAT2_DURATION       (150 / BUTTON_CONSISTENCY_DURATION)
#define INVERT_TIME                   500

// 全域狀態旗標變數
bool board_connect = false;   // 記錄 BLE 是否已連線
bool board_receive = false;   // 記錄是否接收到數據
bool board_conIntv = false;   // 記錄連線間隔是否已設定完成
uint8_t Status;               // 儲存 BLE 模組回傳的狀態碼
uint8_t flag = 0;             // 通用旗標，可供其他功能使用
uint8_t count = 0;            // 通用計數器
uint8_t sel = 1;              // 初始化步驟選擇器（用於控制初始化流程）

// 數據緩衝區
uint8_t receiveBuf[256] = {0}; // 存放從 BLE 接收到的數據
uint8_t receiveLen = 0;        // 接收數據的實際長度
uint8_t sendBuf[256] = {0};    // 存放要透過 BLE 發送出去的數據
uint8_t sendLen = 0;           // 發送數據的實際長度
uint32_t timeLast = 0;         // 用於計時（例如判斷封包是否結束）

// 函式宣告
void initBT5();                 // 初始化藍牙模組
bool readSerialMonitor(uint8_t buff[], uint8_t &len); // 從序列監視器讀取輸入數據

// --------------
// 初始化藍牙模組
// --------------
void initBT5()
{
    // 上電後需延遲 60ms，這段期間不可發送任何命令
    delay(60);
    
    // 初始化與電腦通訊的序列埠（用於顯示除錯訊息）
    Serial.begin(9600);
    
    // 初始化與 BLE 模組通訊的序列埠，設定鮑率為 115200
    BC7701.begin(BAUD_115200);

    // BLE 模組初始化流程，依序設定各項參數
    while (sel != 10) // 持續執行直到所有步驟完成（sel = 10 表示完成）
    {
        switch (sel)
        {
            case 1:
                // 設定藍牙位址
                if (BC7701.setAddress(BDAddress) == true) sel++;
                else sel = 0xFF; // 失敗則跳至錯誤處理
                break;
                
            case 2:
                // 設定裝置名稱
                if (BC7701.setName(sizeof(BDName), BDName) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 3:
                // 設定廣播間隔（單位為 0.625ms）
                if (BC7701.setAdvIntv(ADV_MIN / 0.625, ADV_MAX / 0.625, 7) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 4:
                // 設定廣播數據（包含裝置名稱）
                if (BC7701.setAdvData(APPEND_NAME, sizeof(Adata), Adata) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 5:
                // 設定掃描回應數據
                if (BC7701.setScanData(sizeof(Sdata), Sdata) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 6:
                // 設定發射功率
                if (BC7701.setTXpower(TX_POWER) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 7:
                // 設定晶體負載電容
                if (BC7701.setCrystalOffset(XTAL_CLOAD) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 8:
                // 啟用自動發送狀態功能
                if (BC7701.setFeature(FEATURE_DIR, AUTO_SEND_SATUS) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 9:
                // 開啟廣播
                if (BC7701.setAdvCtrl(ENABLE) == true) sel++;
                else sel = 0xFF;
                break;
                
            case 0xFF:
                // 錯誤處理：點亮板載 LED（腳位 13）
                digitalWrite(13, HIGH);
                break;
        }
    }
    
    // 開啟廣播後需延遲 650ms，期間不可發送命令
    delay(650);
    
    // 初始化完成，顯示訊息
    Serial.println("BLE init completed");
}

// --------------------------------------------
// 從序列監視器讀取輸入的數據
// 參數 buff：存放數據的緩衝區（此範例直接使用全域變數 sendBuf）
// 參數 len：回傳數據長度
// 回傳值：true 表示有讀取到數據，false 表示無數據
// --------------------------------------------
bool readSerialMonitor(uint8_t buff[], uint8_t &len)
{
    // 檢查序列埠是否有數據可讀
    if (Serial.available() > 0)
    {
        sendLen = 0; // 重置發送長度
        
        // 持續讀取直到緩衝區為空
        while (Serial.available() != 0)
        {
            timeLast = millis();               // 記錄當前時間
            sendBuf[sendLen] = Serial.read();   // 讀取一個位元組
            sendLen++;                           // 增加長度計數
            
            // 等待下一個位元組，若超過 50ms 未收到則視為封包結束
            while (Serial.available() == 0)
            {
                if (millis() - timeLast >= 50)
                {
                    break; // 超過 50ms 無資料，跳出迴圈
                }
            }
        }
        return true; // 有讀取到數據
    }
    return false; // 無數據
}