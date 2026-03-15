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






/*************************************************
  setup()：系統初始化
**************************************************/
void setup() {

  delay(60);  // BLE 上電後，晶片需等待 60ms 才能開始接受 AT / API 指令
  Serial.begin(9600);     // 用於印出除錯訊息
  BC7701.begin(BAUD_115200); // 設定 BLE 模組 UART 通訊速率為 115200 bps

  //-------------------------------
  // BLE 模組初始化流程
  //-------------------------------
  while (sel != 10) {
    switch (sel) {

      case 1:
        // 設置 BLE MAC Address
        if (BC7701.setAddress(BDAddress))
          sel++;
        else
          sel = 0xFF;
        break;

      case 2:
        // 設置 BLE 廣播名稱
        if (BC7701.setName(sizeof(BDName), BDName))
          sel++;
        else
          sel = 0xFF;
        break;

      case 3:
        // 設置廣播間隔（單位 = 0.625ms）
        if (BC7701.setAdvIntv(ADV_MIN / 0.625, ADV_MAX / 0.625, 7))
          sel++;
        else
          sel = 0xFF;
        break;

      case 4:
        // 設置廣播資料（advertise data）
        if (BC7701.setAdvData(APPEND_NAME, sizeof(Adata), Adata))
          sel++;
        else
          sel = 0xFF;
        break;

      case 5:
        // 設置掃描響應資料（scan response）
        if (BC7701.setScanData(sizeof(Sdata), Sdata))
          sel++;
        else
          sel = 0xFF;
        break;

      case 6:
        // 設定發射功率
        if (BC7701.setTXpower(TX_POWER))
          sel++;
        else
          sel = 0xFF;
        break;

      case 7:
        // 設置晶振偏移設定
        if (BC7701.setCrystalOffset(XTAL_CLOAD))
          sel++;
        else
          sel = 0xFF;
        break;

      case 8:
        // 自動回傳狀態設定
        if (BC7701.setFeature(FEATURE_DIR, AUTO_SEND_SATUS))
          sel++;
        else
          sel = 0xFF;
        break;

      case 9:
        // 開啟 BLE 廣播
        if (BC7701.setAdvCtrl(ENABLE))
          sel++;
        else
          sel = 0xFF;
        break;

      case 0xFF:
        // 初始化失敗 → 亮起 LED 13 通知使用者
        digitalWrite(13, HIGH);
        break;
    }
  }

  delay(650);  // 廣播啟動後需等待 650ms 才能進行後續指令
}



/*************************************************
  loop()：主要程式邏輯
**************************************************/
void loop() {

  // 讀取 BLE 狀態（是否連線？是否收到資料？）
  Status = bleProcess();

  if (Status) {
    switch (Status) {

      case API_CONNECTED:  // BLE 成功連線
        if (!board_connect) {
          board_connect = true;
          board_receive = false;
        }
        break;

      case API_DISCONNECTED:  // BLE 斷線
        board_connect = false;
        board_receive = false;
        board_conIntv = false;
        break;

      case DATA_RECEIVED:  // 收到手機 APP 傳來的 Key 資料
        if (board_connect) {
          digitalWrite(13, LOW);
          board_receive = true;
        }
        break;

      case API_ERROR:  // 讀取錯誤
        digitalWrite(13, HIGH);
        break;
    }
  }


  //----------------------------------------------------
  // 若 BLE 已連線，則設定連線參數（連線間隔、延遲、timeout）
  //----------------------------------------------------
  if (board_connect == true) 
  {

    if (!board_conIntv) 
    {
      BC7701.wakeUp();
      delay(30);

      // 設置連線參數（Interval / Latency / Timeout）
      if (BC7701.setConnIntv(CON_MIN / 1.25, CON_MAX / 1.25, CON_LATENCY, CON_TIMEOUT))
        board_conIntv = true;
    }

    //----------------------------------------------------
    // 若 APP 有按鍵事件傳入（Press / Release）
    //----------------------------------------------------
    if (board_receive == true) {

      board_receive = false;

      // BLE Key Event 的格式：(receiveBuf[3] == 0xB0)
      if (receiveBuf[3] == 0xB0) {

        switch (receiveBuf[4]) {
          case 0x11:
            count = 1;
            Serial.println("KEY1 icon Pushed");
            break;
          case 0x10:
            count = 2;
            Serial.println("KEY1 icon Released");
            break;

          case 0x22:
            count = 1;
            Serial.println("KEY2 icon Pushed");
            break;
          case 0x20:
            count = 2;
            Serial.println("KEY2 icon Released");
            break;

          case 0x44:
            count = 1;
            Serial.println("KEY3 icon Pushed");
            break;
          case 0x40:
            count = 2;
            Serial.println("KEY3 icon Released");
            break;
        }


        //----------------------------------------------------
        // 將按鍵事件回傳給手機 APP（APP 上的 LED 會亮起）
        //----------------------------------------------------
        if (receiveBuf[4] != 0 && count == 2 && flag == 0) {
          Keymessage.key = receiveBuf[4] >> 4;
          Keymessage.key += receiveBuf[4];
          Keymessage.serial++;
          Keymessage.checksum = 0xB1 ^ Keymessage.key ^ Keymessage.serial;
          BC7701.writeData((uint8_t*)&Keymessage, 3);
          flag = 1;
          receiveBuf[4] = 0;
        }

        if (receiveBuf[4] != 0 && count == 2 && flag == 1) {
          Keymessage.key = receiveBuf[4];
          Keymessage.serial++;
          Keymessage.checksum = 0xB1 ^ Keymessage.key ^ Keymessage.serial;
          BC7701.writeData((uint8_t*)&Keymessage, 3);
          flag = 0;
          receiveBuf[4] = 0;
        }
      }
    }
  }
}



