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
      2. 使用 APP BLEDemo 搜尋裝置，名稱為 BMC77M001 / 您設定的 BDName
      3. 點選 Key1 / Key2 / Key3 測試「按下 / 放開」事件
**************************************************/

#include <BM7701-00-1.h>  // 引入 BLE 模組 BM7701-00-1 的函式庫，內含控制與封包處理 API

// 若使用軟體序列埠 (SoftwareSerial)，用以下形式建立：
// BM7701_00_1 BC7701(2, 3);  // rxPin = 2, txPin = 3

// 使用 BMduino 的硬體序列埠 Serial1
//BM7701_00_1 BC7701(&Serial1);

// 目前改用 BMduino 的硬體序列埠 Serial2 與 BLE 模組連線
BM7701_00_1 BC7701(&Serial2);  
// 也可以依實際接線改用 Serial3 / Serial4：
// BM7701_00_1 BC7701(&Serial3);  
// BM7701_00_1 BC7701(&Serial4);  


//------------------------------------------------------
// BLE 相關設定參數區（通訊參數 & 廣播行為設定）
//------------------------------------------------------
#define TX_POWER     0x0F  // BLE 模組無線發射功率設定，0x0F = 最大功率（發射最強、耗電相對較高）
#define XTAL_CLOAD   0x04  // 16MHz 晶振負載電容設定，需與硬體設計相符
#define ADV_MIN      100   // 廣播間隔最小值（毫秒），代表最短多久廣播一次
#define ADV_MAX      100   // 廣播間隔最大值（毫秒），此處與最小值相同，代表固定 100ms 廣播一次
#define CON_MIN      30    // 連線間隔最小值（毫秒），Central 與 Peripheral 傳輸的最短間隔
#define CON_MAX      30    // 連線間隔最大值（毫秒）
#define CON_LATENCY  00    // 連線延遲參數（slave latency），0 代表每個 connection interval 都要應答
#define CON_TIMEOUT  300   // 連線超時時間（毫秒），超過此時間未收到封包會視為斷線

// BLE MAC Address（設備位址）→ 將會設定到模組的 Bluetooth Address
uint8_t BDAddress[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};

// BLE 廣播名稱（最多 31 bytes），手機掃描時顯示的名字
uint8_t BDName[] = {'B','M','d','u','i','n','o','B','L','E','0','1'};

// BLE 廣播封包（Advertise Data）
// 0x02：長度 2 bytes
// 0x01：標籤為「Flag」類型
// 0x06：一般可連線裝置 + BR/EDR 不支援
uint8_t Adata[] = {0x02, 0x01, 0x06};

// BLE 掃描響應封包（Scan Response Data）
// 手機對裝置送出 Scan Request 時，裝置可以回應這些額外資料
uint8_t Sdata[] = {0x03, 0x02, 0x0F, 0x18};


//------------------------------------------------------
// 內部狀態常數與旗標變數
//------------------------------------------------------
// 下列 define 原本多用於按鍵去彈 / 長按計數等節奏判斷（本範例中未明顯使用）
#define BUTTON_CONSISTENCY_DURATION    6
#define BUTTON_REPEAT1_DURATION       (600 / BUTTON_CONSISTENCY_DURATION)
#define BUTTON_REPEAT2_DURATION       (150 / BUTTON_CONSISTENCY_DURATION)
#define INVERT_TIME                   500   // 交替閃爍時間（ms）

bool board_connect = false;  // BLE 是否已與手機成功建立連線（true = 已連線）
bool board_receive = false;  // 是否有收到來自 APP 的資料（true = 有新資料待處理）
bool board_conIntv = false;  // BLE 連線參數（connection interval 等）是否已設定完成

uint8_t Status;         // BLE 狀態碼（由 bleProcess() 回傳）
uint8_t flag = 0;       // 用於區分第一次 / 第二次回傳 Keymessage 的旗標
uint8_t count = 0;      // 記錄按鍵是「按下」或「放開」的次數狀態
uint8_t sel = 1;        // BLE 初始化的流程控制變數（1~9 依序執行各設定）
uint8_t receiveBuf[256] = {0};  // 接收 BLE 模組回傳資料用的緩衝區

// KEY_MESSAGE 為模組 API 定義的結構，內含：key、serial（序號）、checksum 等欄位
KEY_MESSAGE Keymessage; // 用來回傳給 APP 的鍵值資料封包



/*************************************************
  setup()：系統初始化
**************************************************/
void setup() {

  delay(60);  // BLE 上電後，晶片內部要先完成 Reset，約需 60ms 才能正常接受指令
  Serial.begin(9600);     // 啟動 USB Serial 監控，用於印出除錯與狀態訊息
  BC7701.begin(BAUD_115200); // 設定與 BLE 模組的 UART 通訊速率為 115200 bps

  //-------------------------------
  // BLE 模組初始化流程（狀態機設計）
  //-------------------------------
  // sel 會從 1 執行到 9，每一階段設定 BLE 的一個項目：
  // 位址 → 名稱 → 廣播間隔 → 廣播資料 → 掃描響應 → 發射功率 → 晶振校正 → 自動回傳狀態 → 啟用廣播
  // 若任何一步失敗，sel 會變成 0xFF，並亮起 LED 13 提示錯誤。
  while (sel != 10)   // 必須讓 BLE 模組初始化流程全部執行完畢，sel=10 才表示完成
  {
    switch (sel) 
    {

      case 1:
        // 設置 BLE MAC Address（藍牙位址）
        Serial.println("Stage01:設置 BLE MAC Address") ;
        if (BC7701.setAddress(BDAddress))
          sel++;      // 設定成功 → 進入下一階段
        else
          sel = 0xFF; // 設定失敗 → 跳到錯誤處理階段
        break;

      case 2:
        Serial.println("Stage02:設置 BLE 廣播名稱") ;
        // 設置 BLE 廣播名稱（手機掃描時顯示）
        if (BC7701.setName(sizeof(BDName), BDName))
          sel++;
        else
          sel = 0xFF;
        break;

      case 3:
        Serial.println("Stage03:設置廣播間隔（單位 = 0.625ms）") ;
        // 設置廣播間隔（API 接受的單位為 0.625ms，因此要除以 0.625）
        // ADV_MIN / 0.625 = 廣播間隔最小值的 unit 數
        if (BC7701.setAdvIntv(ADV_MIN / 0.625, ADV_MAX / 0.625, 7))
          sel++;
        else
          sel = 0xFF;
        break;

      case 4:
        Serial.println("Stage04:設置廣播資料（advertise data）") ;
        // 設置廣播資料（Advertise Data）
        // 可包含 flags / 部分 UUID / 廣告資訊等
        if (BC7701.setAdvData(APPEND_NAME, sizeof(Adata), Adata))
          sel++;
        else
          sel = 0xFF;
        break;

      case 5:
        Serial.println("Stage05:設置掃描響應資料（scan response）") ;
        // 設置掃描響應資料（Scan Response Data）
        // 手機發出 Scan Request 後，模組可回傳這份資料
        if (BC7701.setScanData(sizeof(Sdata), Sdata))
          sel++;
        else
          sel = 0xFF;
        break;

      case 6:
        Serial.println("Stage06:設定發射功率") ;
        // 設定無線發射功率（影響有效距離與功耗）
        if (BC7701.setTXpower(TX_POWER))
          sel++;
        else
          sel = 0xFF;
        break;

      case 7:
        Serial.println("Stage07:設置晶振偏移設定") ;
        // 設置晶振負載/偏移，確保 16MHz 時脈準確
        if (BC7701.setCrystalOffset(XTAL_CLOAD))
          sel++;
        else
          sel = 0xFF;
        break;

      case 8:
        Serial.println("Stage08:自動回傳狀態設定") ;
        // 設定模組的自動狀態回傳功能（例如連線 / 斷線自動通知）
        if (BC7701.setFeature(FEATURE_DIR, AUTO_SEND_SATUS))
          sel++;
        else
          sel = 0xFF;
        break;

      case 9:
        // 開啟 BLE 廣播，開始對外宣告這個裝置可被連線
        Serial.println("Stage09:開啟 BLE 廣播") ;
        if (BC7701.setAdvCtrl(ENABLE))
          sel++;
        else
          sel = 0xFF;
        break;

      case 0xFF:
        Serial.println("Stage99:初始化失敗 → 亮起 LED 13 通知使用者") ;
        // 初始化流程中任一步失敗 → 亮起 LED 13 當作錯誤警示
        pinMode(13, OUTPUT);
        digitalWrite(13, HIGH);
        break;
    }
  }

  delay(650);  // BLE 廣播啟動後，建議再延遲約 650ms，讓模組內部狀態穩定再開始進行後續操作
}



/*************************************************
  loop()：主要程式邏輯
**************************************************/
void loop() {

  // 呼叫 bleProcess() 判斷 BLE 模組目前狀態
  // 可能的狀態：已連線 / 已斷線 / 收到資料 / 讀取錯誤
  Status = bleProcess();

  if (Status) {   // 若有狀態變化（Status != 0）
    switch (Status) {

      case API_CONNECTED:  // BLE 成功與手機或 Central 建立連線
        if (!board_connect) {
          board_connect = true;   // 更新內部狀態：已連線
          board_receive = false;  // 尚未收到資料
        }
        break;

      case API_DISCONNECTED:  // BLE 連線中斷
        // 清除內部連線狀態
        board_connect = false;
        board_receive = false;
        board_conIntv = false;   // 連線參數需在下次連線時重新設定
        break;

      case DATA_RECEIVED:  // 收到手機 APP 傳來的資料（包含按鍵事件）
        if (board_connect) {
          // 若裝置處於已連線狀態，收到資料時可觸發某些動作
          digitalWrite(13, LOW);  // 例如：熄滅 LED 13 表示「有資料進來」
          board_receive = true;   // 通知主程式有一筆新的資料待處理
        }
        break;

      case API_ERROR:  // BLE 讀取發生錯誤
        digitalWrite(13, HIGH);  // 亮起 LED 當作錯誤警示
        break;
    }
  }


  //----------------------------------------------------
  // 若 BLE 已連線，則進一步設定連線參數 / 處理 APP 按鍵資料
  //----------------------------------------------------
  if (board_connect == true) {

    // 第一次連線後，只需設定一次連線參數（Connection Interval / Latency / Timeout）
    if (!board_conIntv) {
      BC7701.wakeUp();   // 喚醒模組（若有省電睡眠機制）
      delay(30);

      // 設置連線參數（Interval / Latency / Timeout）
      // CON_MIN / 1.25、CON_MAX / 1.25：轉成 BLE 規範內的「1.25ms 單位」
      if (BC7701.setConnIntv(CON_MIN / 1.25, CON_MAX / 1.25, CON_LATENCY, CON_TIMEOUT))
        board_conIntv = true;  // 代表連線參數設定完成
    }

    //----------------------------------------------------
    // 若 APP 有按鍵事件傳入（Press / Release）
    //----------------------------------------------------
    if (board_receive == true) {

      board_receive = false;   // 清除「尚待處理」旗標

      // BLE Key Event 封包格式判斷：
      // receiveBuf[3] == 0xB0 → 表示這是一個「按鍵控制事件」封包
      if (receiveBuf[3] == 0xB0) {

        // 第 4 Byte（receiveBuf[4]）表示「哪顆按鍵 / 按下 或 放開」
        switch (receiveBuf[4]) {
          case 0x11:
            count = 1;  // 標記為按下階段
            Serial.println("KEY1 icon Pushed");   // Key1 被按下
            break;
          case 0x10:
            count = 2;  // 標記為釋放階段
            Serial.println("KEY1 icon Released"); // Key1 放開
            break;

          case 0x22:
            count = 1;
            Serial.println("KEY2 icon Pushed");   // Key2 被按下
            break;
          case 0x20:
            count = 2;
            Serial.println("KEY2 icon Released"); // Key2 放開
            break;

          case 0x44:
            count = 1;
            Serial.println("KEY3 icon Pushed");   // Key3 被按下
            break;
          case 0x40:
            count = 2;
            Serial.println("KEY3 icon Released"); // Key3 放開
            break;
        }


        //----------------------------------------------------
        // 將按鍵事件回傳給手機 APP（APP 上的 LED 會亮起）
        // 這裡分成兩種 flag 狀態，代表兩種不同的封包組合方式
        //----------------------------------------------------
        // 情境 1：flag == 0 → 先以「高位 + 原值」方式計算 key 欄位
        if (receiveBuf[4] != 0 && count == 2 && flag == 0) {
          // 將按鍵資訊轉成 Keymessage.key 欄位
          Keymessage.key = receiveBuf[4] >> 4;  // 取高位 nibble
          Keymessage.key += receiveBuf[4];      // 再加上原始值（形成特定編碼）

          // 每送一次封包就讓 serial 編號 +1
          Keymessage.serial++;

          // 計算 Checksum：以 0xB1 為基底 XOR key XOR serial
          Keymessage.checksum = 0xB1 ^ Keymessage.key ^ Keymessage.serial;

          // 將 Keymessage 的前 3 個 byte 當作資料送回 APP
          BC7701.writeData((uint8_t*)&Keymessage, 3);

          flag = 1;           // 設定旗標，下一次依序改用另一種方式（見下方）
          receiveBuf[4] = 0;  // 清除該次按鍵資訊，避免重複處理
        }

        // 情境 2：flag == 1 → 直接將按鍵值放入 key 欄位
        if (receiveBuf[4] != 0 && count == 2 && flag == 1) {

          Keymessage.key = receiveBuf[4];  // 直接使用原始鍵值作為 key
          Keymessage.serial++;             // 序號累加
          Keymessage.checksum = 0xB1 ^ Keymessage.key ^ Keymessage.serial;

          BC7701.writeData((uint8_t*)&Keymessage, 3);

          flag = 0;           // 重置旗標，下一次又回到上一種編碼方式
          receiveBuf[4] = 0;  // 清除按鍵值
        }
      }
    }
  }
}



/**********************************************************
  函式：bleProcess()
  功能：讀取並解析 BLE 模組回傳的狀態封包，將其轉換為簡單狀態碼
  回傳值：
      API_CONNECTED     → BLE 已連線
      API_DISCONNECTED  → BLE 已斷線
      DATA_RECEIVED     → 收到 APP 傳來的資料
      API_ERROR         → 無資料或讀取錯誤
**********************************************************/
uint8_t bleProcess() {

  uint8_t st = 0x00;   // 預設狀態為 0（無變化）
  uint8_t lenth = 0;   // 接收資料長度（由 API 填入）

  // 嘗試從 BLE 模組讀取一筆資料到 receiveBuf
  if (BC7701.readData(receiveBuf, lenth)) {

    // 根據 receiveBuf[1] 判斷這筆資料的類型
    switch (receiveBuf[1]) {

      case 0x00:  // 系統狀態封包（包含連線 / 斷線旗標）
        if (receiveBuf[0] == 0x00) {
          // receiveBuf[3] bit0 = 1 → 已連線；=0 → 已斷線
          if (receiveBuf[3] & 0x01)
            st = API_CONNECTED;
          else
            st = API_DISCONNECTED;
        }
        break;

      case 0xF2:  // 0xF2：代表這是一筆「來自 APP 的資料封包」
        if (receiveBuf[0] == 0x00 && receiveBuf[2] == 0xFF)
          st = DATA_RECEIVED;
        break;

      default:
        // 其他類型封包暫不處理
        break;
    }
  }
  else {
    // 若 BC7701.readData() 回傳 false → 表示目前無資料或讀取失敗
    st = API_ERROR;
  }

  return st;  // 回傳目前 BLE 狀態給主程式 loop()
}
