#include "commlib.h"
#include "RGBLEDLib.h"  // RGBLED 自訂模組，用於控制 RGB LED 燈泡
#include "BT5Lib.h"     // 藍芽模組相關函式庫

/*
這段程式會使 LED 燈以流動的方式顯示不同的 RGB 顏色，
並根據 flow 變數改變色彩效果，達到持續變換的視覺效果。
*/

String btstr = ""; // 用來儲存從藍芽接收到的字串資料

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
String BTbuffer2String();          // 將藍芽接收緩衝區的資料轉換為字串

void setup() 
{      
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay 等）
  delay(200);      // 延遲 200ms，確保硬體模組穩定

  //CheckLed(); // 可選擇性呼叫，用來測試燈泡是否正常
  Serial.println("Enter Loop()"); // 顯示進入主迴圈的訊息
}

// 主迴圈
void loop()
{
  // 讀取 BLE 模組的狀態與接收資料（透傳模式）
  Status = BC7701.receiveData(receiveBuf, receiveLen);

  if (Status) // 若有狀態事件發生
  {
    switch (Status)
    {
      case API_CONNECTED: // BLE 已連線
        if (board_connect == false)
        {
          board_connect = true;   // 標記為已連線
          board_receive = false;  // 重置接收旗標
          if (board_conIntv == false) // 連線成功後需設定一次連線間隔
          {
            BC7701.wakeUp(); // 喚醒模組
            delay(30);
            // 設定連線間隔參數（單位為 1.25ms）
            if (BC7701.setConnIntv(CON_MIN / 1.25, CON_MAX / 1.25, CON_LATENCY, CON_TIMEOUT) == true)
            {
              board_conIntv = true; // 標記已設定連線間隔
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
          digitalWrite(13, LOW); // 收到數據時關閉 Pin 13 LED（可視為指示）
          board_receive = true;  // 標記有待處理的接收數據
        }
        break;

      case API_ERROR: // BLE 錯誤
        digitalWrite(13, HIGH); // 點亮 Pin 13 LED 表示錯誤
        break;
    }
  }

  // 處理接收到的數據
  if (board_receive == true)
  {
    board_receive = false; // 重置接收旗標

    /* 使用者數據處理區塊 */
    Serial.print("APP to BLE : "); // 印出提示訊息
    btstr = BTbuffer2String();     // 將藍芽接收緩衝區資料轉為字串
    Serial.print("(");
    Serial.print(btstr);
    Serial.print(")\n");

    // 解析字串中的 RGB 數值，格式如 @255255255#
    if (DecodeString(btstr, &RedValue, &GreenValue, &BlueValue))
    {
      DebugMsgln("Change RGB Led Color");
      ChangeBulbColor(RedValue, GreenValue, BlueValue); // 改變燈泡顏色
    }
  }

  // 檢查序列監視器是否有輸入數據需發送至 BLE
  if (readSerialMonitor(sendBuf, sendLen) == true) // 若序列監視器有數據
  {
    if (BC7701.sendData(sendBuf, sendLen) == true) // 發送數據至 BLE
    {
      Serial.print("BLE to APP : "); // 印出發送提示
      for (uint8_t i = 0; i < sendLen; i++) // 逐位元組印出發送的數據（十六進制格式）
      {
        Serial.print(sendBuf[i], HEX);
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

//---------------  主程式自訂函式區 ------------------

//-----初始化所有感測模組----------
void initSensor() // 初始化所有感測模組
{
  initRGBLED();   // 起始 RGB LED 模組
  initBT5();      // 起始化藍芽模組
  delay(2000);    // 延遲 2 秒，等待顯示模組穩定
}

// ------------------ 系統初始化區 ------------------
// 初始化整體系統
void initAll() // 初始化整體系統
{
  Serial.begin(9600);               // 啟動序列埠，速率 9600 bps
  Serial.println("System Start....."); // 顯示系統啟動訊息
  initSensor();                     // 初始化感測模組
}

// 將藍芽接收緩衝區的資料轉換為字串
String BTbuffer2String()
{
  String tmp = "";
  for (uint8_t i = 0; i < receiveLen; i++) // 逐位元組處理接收到的數據
  {
    tmp.concat((char)receiveBuf[i]);       // 轉為字元並附加到字串中
    Serial.print(receiveBuf[i], HEX);      // 以十六進制顯示
    Serial.print(" ");
  }
  return tmp; // 回傳轉換後的字串
}