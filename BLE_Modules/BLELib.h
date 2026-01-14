/*************************************************
  File:             write.ino
  Description:
      本範例示範 BM7701-00-1 BLE 模組
      搭配手機 APP「BLEDemo」進行按鍵收發測試。

      APP 上按下按鍵 → 模組接收按鍵事件 (KEY Event)
      → 將收到的資料印到序列埠
      → 再回傳對應資料給 APP → APP 上的 LED 亮起

  使用方式：
      1. 模組插上 BMduino 並上電
      2. 使用 APP BLEDemo 搜尋裝置，名稱為 BMC77M001
      3. 點選 Key1 / Key2 / Key3 測試按下與放開事件
**************************************************/


//-------全域變數宣告-----------------
//------------------------------------------------------
// BLE 相關設定參數區
//------------------------------------------------------
#define TX_POWER     0x0F  // BLE 模組無線發射功率，0x0F = 最大功率
#define XTAL_CLOAD   0x04  // 16MHz 晶振負載電容設定
#define ADV_MIN      100   // 廣播間隔最小值（ms）
#define ADV_MAX      100   // 廣播間隔最大值（ms）
#define CON_MIN      30    // 連線間隔最小值（ms）
#define CON_MAX      30    // 連線間隔最大值（ms）
#define CON_LATENCY  00    // 連線延遲參數（用於省電）
#define CON_TIMEOUT  300   // 連線超時時間（ms）

// BLE MAC Address（設備位址）
uint8_t BDAddress[6] = {0x22, 0x22, 0x33, 0x44, 0x55, 0x99};

// BLE 廣播名稱（最多 31 bytes）
uint8_t BDName[] = {'B','M','d','u','i','n','o','B','L','E','0','1'};


// BLE 廣播封包（Advertise Data）
uint8_t Adata[] = {0x02, 0x01, 0x06};

// BLE 掃描響應封包（Scan Response Data）
uint8_t Sdata[] = {0x03, 0x02, 0x0F, 0x18};


//------------------------------------------------------
// 內部狀態常數與旗標變數
//------------------------------------------------------
#define BUTTON_CONSISTENCY_DURATION    6
#define BUTTON_REPEAT1_DURATION       (600 / BUTTON_CONSISTENCY_DURATION)
#define BUTTON_REPEAT2_DURATION       (150 / BUTTON_CONSISTENCY_DURATION)
#define INVERT_TIME                   500

bool board_connect = false;  // 是否已連線
bool board_receive = false;  // 是否收到資料
bool board_conIntv = false;  // 連線參數是否已設定

uint8_t Status;         // BLE 狀態碼
uint8_t flag = 0;
uint8_t count = 0;
uint8_t sel = 1;        // BLE 初始化時的流程控制變數
uint8_t receiveBuf[256] = {0};  // BLE 接收資料緩衝區

KEY_MESSAGE Keymessage; // 用來回傳給 APP 的鍵值資料格式

//-------全域變數宣告------end------



//----------引用外部感測器函式庫------  ----
#include <BM7701-00-1.h>  // 引入 BLE 模組 BM7701-00-1 的函式庫


//----------產生感測器物件程式區------  ----
// 若使用軟體序列埠 (SoftwareSerial)，用以下形式建立：
// BM7701_00_1 BC7701(2, 3);  // rxPin = 2, txPin = 3

// 使用 BMduino 的硬體序列埠 Serial1
//BM7701_00_1 BC7701(&Serial1);  
BM7701_00_1 BC7701(&Serial2);  
// BM7701_00_1 BC7701(&Serial3);  
// BM7701_00_1 BC7701(&Serial4);  
// 也可改成 Serial2 / Serial3 / Serial4 依需求切換


//------ 自訂函式宣告區-----------------
void initBLE();//初始化BLE藍芽模組
uint8_t bleProcess();  //判斷 BLE 模組回傳的狀態
boolean BLEisConnected();  // 讀取 BLE 狀態（是否連線)
boolean BLEisunConnected();  // 讀取 BLE 狀態（是否斷線）
boolean BLEDataAvilable();  // 讀取 BLE 狀態（是否收到資料？）
boolean BLEError();  // 讀取錯誤

//------ 自訂函式本體程式區-----------------

void initBLE()//初始化BLE藍芽模組
{
  BC7701.begin(BAUD_115200); // 設定 BLE 模組 UART 通訊速率為 115200 bps

  //-------------------------------
  // BLE 模組初始化流程
  //-------------------------------
  while (sel != 10)   //必須從1~10  BLE 模組初始化流程全部執行完畢
  {
    switch (sel) 
    {

      case 1:
        // 設置 BLE MAC Address
        Serial.println("Stage01:設置 BLE MAC Address") ;
        if (BC7701.setAddress(BDAddress))
          sel++;
        else
          sel = 0xFF;
        break;

      case 2:
        Serial.println("Stage02:設置 BLE 廣播名稱") ;
        // 設置 BLE 廣播名稱
        if (BC7701.setName(sizeof(BDName), BDName))
          sel++;
        else
          sel = 0xFF;
        break;

      case 3:
        Serial.println("Stage03:置廣播間隔（單位 = 0.625ms）") ;
        // 設置廣播間隔（單位 = 0.625ms）
        if (BC7701.setAdvIntv(ADV_MIN / 0.625, ADV_MAX / 0.625, 7))
          sel++;
        else
          sel = 0xFF;
        break;

      case 4:
        Serial.println("Stage04:設置廣播資料（advertise data）") ;
        // 設置廣播資料（advertise data）
        if (BC7701.setAdvData(APPEND_NAME, sizeof(Adata), Adata))
          sel++;
        else
          sel = 0xFF;
        break;

      case 5:
        Serial.println("Stage05:設置掃描響應資料（scan response）") ;
        // 設置掃描響應資料（scan response）
        if (BC7701.setScanData(sizeof(Sdata), Sdata))
          sel++;
        else
          sel = 0xFF;
        break;

      case 6:
        Serial.println("Stage06:Stage06設定發射功率") ;
        // 設定發射功率
        if (BC7701.setTXpower(TX_POWER))
          sel++;
        else
          sel = 0xFF;
        break;

      case 7:
        Serial.println("Stage07:設置晶振偏移設定") ;
        // 設置晶振偏移設定
        if (BC7701.setCrystalOffset(XTAL_CLOAD))
          sel++;
        else
          sel = 0xFF;
        break;

      case 8:
        Serial.println("Stage08:自動回傳狀態設定") ;
        // 自動回傳狀態設定
        if (BC7701.setFeature(FEATURE_DIR, AUTO_SEND_SATUS))
          sel++;
        else
          sel = 0xFF;
        break;

      case 9:
        // 開啟 BLE 廣播
        Serial.println("Stage09:開啟 BLE 廣播") ;
        if (BC7701.setAdvCtrl(ENABLE))
          sel++;
        else
          sel = 0xFF;
        break;

      case 0xFF:
        Serial.println("Stage99:初始化失敗 → 亮起 LED 13 通知使用者") ;
        // 初始化失敗 → 亮起 LED 13 通知使用者
        digitalWrite(13, HIGH);
        break;
    }
  }

  delay(650);  // 廣播啟動後需等待 650ms 才能進行後續指令
}

/**********************************************************
  函式：bleProcess()
  功能：判斷 BLE 模組回傳的狀態
  回傳值：
      API_CONNECTED         → BLE 已連線
      API_DISCONNECTED      → BLE 已斷線
      DATA_RECEIVED         → 收到 APP 資料
      API_ERROR             → 讀取錯誤
**********************************************************/
uint8_t bleProcess()  //判斷 BLE 模組回傳的狀態
{

  uint8_t st = 0x00;
  uint8_t lenth = 0;

  // 嘗試讀取 BLE 回傳資料
  if (BC7701.readData(receiveBuf, lenth)) {

    switch (receiveBuf[1]) {

      case 0x00:  // 連線 or 斷線事件
        if (receiveBuf[0] == 0x00) {
          if (receiveBuf[3] & 0x01)
            st = API_CONNECTED;
          else
            st = API_DISCONNECTED;
        }
        break;

      case 0xF2:  // 收到 APP 的資料封包
        if (receiveBuf[0] == 0x00 && receiveBuf[2] == 0xFF)
          st = DATA_RECEIVED;
        break;

      default:
        break;
    }
  }
  else {
    st = API_ERROR;  // 無法讀取模組 → 錯誤
  }

  return st;
}

boolean BLEisConnected()  // 讀取 BLE 狀態（是否連線)
{
    // 讀取 BLE 狀態（是否連線？是否收到資料？）
  Status = bleProcess();

  if (Status == API_CONNECTED)
   {
      if (!board_connect) 
      {
        board_connect = true;
        board_receive = false;
        if (!board_conIntv) 
          {
            BC7701.wakeUp();
            delay(30);

            // 設置連線參數（Interval / Latency / Timeout）
            if (BC7701.setConnIntv(CON_MIN / 1.25, CON_MAX / 1.25, CON_LATENCY, CON_TIMEOUT))
              board_conIntv = true;
          }
        }
    return true ;
   }
    else
    {
      if (board_connect) 
      {
        board_connect = false ;
        board_receive = true;
      }
    return false ;

    }   
}

boolean BLEisunConnected()  // 讀取 BLE 狀態（是否斷線）
{
    // 讀取 BLE 狀態（是否斷線）
  Status = bleProcess();

  if (Status == API_DISCONNECTED)
   {
      if (!board_connect) 
      {
        board_connect = false;
        board_receive = false;
      }
    return true ;
   }
    else
    {
      if (board_connect) 
      {
        board_connect = false ;
        board_receive = false;
      }
    return false ;

    }   
}

boolean BLEDataAvilable()  // 讀取 BLE 狀態（是否收到資料？）
{
    // 讀取 BLE 狀態（是否連線？是否收到資料？）
  Status = bleProcess();
  if (Status == API_DISCONNECTED)
   {
      if (board_connect) 
      {
      digitalWrite(13, LOW);
      board_receive = true;
     }
        return true ;
    } 
    else
    {
      return false ;    
    }
}

boolean BLEError()  // 讀取錯誤
{
  if (Status == API_ERROR) // 讀取錯誤
   {
      if (board_connect) 
      {
        digitalWrite(13, HIGH);
     }
        return true ;
    } 
    else
    {
      return false ;    
    }
}

