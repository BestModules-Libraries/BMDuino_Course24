/*****************************************************************
File:         Peer.ino
Description: 
    功能說明：
    1. 短按 KEY1 (D8)，MRX 指示燈開始閃爍 → 進入配對模式 (持續 8 秒)。
    2. 當兩塊板子都進入配對模式後，任意一塊板子短按 KEY2 (D9)，會發送配對請求封包。
       - 配對成功 → MRX 燈熄滅，序列埠顯示 "Pair Success"
       - 8 秒內未成功 → MRX 燈熄滅，需要重新長按 KEY1 進入配對模式
    3. 配對成功後，進入接收模式。
       - 按 KEY2 (D9) → 發送一筆資料，MTX 指示燈閃爍一次
       - 接收成功 → MRX 指示燈閃爍一次，序列埠顯示接收到的資料
Note:  rxPin:D5 txPin:D4  
******************************************************************/

#include "BMC36M0x1.h"  

// === 建立 BMC36M0x1 無線模組物件 ===
//BMC36M0x1     BMC36(5,4);        // 軟體序列埠：RX=D5, TX=D4
//BMC36M0x1     BMC36(&Serial);    // 使用 BMduino 硬體序列埠 Serial
//BMC36M0x1     BMC36(&Serial1);   // 使用 BMduino 硬體序列埠 Serial1
BMC36M0x1     BMC36(&Serial2);    // 使用 BMduino 硬體序列埠 Serial2
//BMC36M0x1     BMC36(&Serial3);   // 使用 BMduino 硬體序列埠 Serial3
//BMC36M0x1     BMC36(&Serial4);   // 使用 BMduino 硬體序列埠 Serial4

// === 定義按鍵腳位 ===
#define   KEY1_Pin (8)   // KEY1 → D8
#define   KEY2_Pin (9)   // KEY2 → D9
#define   KEY3_Pin (10)  // KEY3 → D10

// === 狀態旗標 ===
bool Flag_Pairing, Flag_PairSuccess;  // 紀錄是否在配對中、是否配對成功
uint8_t TXDATA[16] = {0}, RXDATA[32] = {0}; // 傳送與接收資料緩衝區
uint8_t DATA, STATUS, len;  
uint8_t Flag_RX = 1;          // 用來切換接收/休眠模式
uint8_t keyFlag = 0;          // 按鍵狀態記錄
uint8_t keyFlagTemp = 0;      // 暫存按鍵狀態

// === 函式前置宣告 ===
uint8_t Sys_KEY(void);             // 偵測按鍵輸入
void RFMessage_Process();          // 按鍵對應的 RF 操作
void Handle_RFPacket_Process();    // 處理 RF 封包


// ------------------ 初始化 ------------------
void setup()
{
  // 設定按鍵腳位為內部上拉
  pinMode(KEY1_Pin, INPUT_PULLUP);  
  pinMode(KEY2_Pin, INPUT_PULLUP); 
  pinMode(KEY3_Pin, INPUT_PULLUP); 

  // 啟動序列埠 (USB Debug 用)
  Serial.begin(115200); 

  // 設定 BMC36 模組的 UART 通訊鮑率
  BMC36.begin(BDR_38400);   

  // 設定裝置角色為 Peer (點對點裝置)
  BMC36.setDeviceRole(Peer);        
}


// ------------------ 主迴圈 ------------------
void loop() 
{              
  RFMessage_Process();           // 處理按鍵，決定 RF 行為
  Handle_RFPacket_Process();     // 處理 RF 接收的資料封包
}


/******************************************************
函式名稱: Sys_KEY
功能: 掃描按鍵輸入
輸入: 無
輸出: 回傳按鍵值 (0x01=KEY1, 0x02=KEY2, 0x03=KEY3)
說明: 放在 loop 內循環執行，具去抖動功能
*******************************************************/
uint8_t Sys_KEY(void)
{
  uint8_t result=0;

  // 讀取按鍵狀態 (低電位表示按下)
  if(!digitalRead(KEY1_Pin))
    keyFlagTemp |= 0x01;
  else if(!digitalRead(KEY2_Pin))
    keyFlagTemp |= 0x02;
  else if(!digitalRead(KEY3_Pin))
    keyFlagTemp |= 0x04;
  else
    keyFlagTemp = 0x00;
    
  // 偵測到新按鍵事件
  if((keyFlagTemp != keyFlag) && (keyFlag == 0))
  {
    delay(10); // 去抖動
    if(!digitalRead(KEY1_Pin))
      result = 0x01;
    else if(!digitalRead(KEY2_Pin))
      result = 0x02;
    else if(!digitalRead(KEY3_Pin))
      result = 0x03;
  }

  keyFlag = keyFlagTemp; // 更新按鍵狀態
  return result;
}


/******************************************************
函式名稱: RFMessage_Process
功能: 根據按鍵執行 RF 模組對應操作
*******************************************************/
void RFMessage_Process()
{
  switch(Sys_KEY())
  { 
   case 0x01: 
      // KEY1 → 進入配對模式
      BMC36.setMode(Pairing_Mode);    
      Flag_Pairing = TRUE; 
      Flag_PairSuccess = FALSE;            
    break;
          
   case 0x02:   
      // KEY2 → 切換接收模式/休眠模式
      if(Flag_RX == 1)
      {
        BMC36.setMode(Rx_Mode);   // 切換為接收模式
        Flag_RX = 0;
      } 
      else if(Flag_RX == 0)
      {
        BMC36.setMode(Sleep_Mode); // 切換為休眠模式
        Flag_RX = 1;
      }     
    break;  
      
   case 0x03:    
      // KEY3 → 根據狀態做不同動作
      if(Flag_Pairing)
      {
        // 配對模式下，發送配對請求封包
        BMC36.writePairPackage(SA_Peer_Pair | SA_Pairing); 
      } 
      if(Flag_PairSuccess)     
      {
        // 配對成功後，發送資料封包
        for(uint8_t temp=0; temp<16; temp++)
        {
          TXDATA[temp] = DATA++;
        }
        BMC36.writeRFData(SA_Peer, 16, TXDATA);  // 傳送 16 bytes 資料
      }
    break;
  }       
}


/******************************************************
函式名稱: Handle_RFPacket_Process
功能: 處理 RF 模組接收到的資料封包
*******************************************************/
void Handle_RFPacket_Process()
{
  // --- 檢查配對狀態 ---
  if(Flag_Pairing)
  {
    STATUS = BMC36.getPairStatus(); 

    if(STATUS == 1) // 配對成功
    {
      Flag_Pairing = FALSE;
      Flag_PairSuccess = TRUE;  
      Serial.println("Pair Success");                                                          
    }  
    if(STATUS == 2) // 配對失敗
    {
      Flag_Pairing = FALSE;
      Flag_PairSuccess = FALSE;
    }   
    if(STATUS == 3) // 配對逾時
    {
      Flag_Pairing = FALSE;
      Flag_PairSuccess = FALSE;                                                            
    }                                              
  }
   
  // --- 接收資料 ---
  if(Flag_PairSuccess)
  {
    if(BMC36.isInfoAvailable())  
    {
      STATUS = BMC36.readRFData(RXDATA, len);  
      if(STATUS == 1)  // 成功接收到資料
      { 
        Serial.print("RXDATA[]:");
        for(uint8_t temp=0; temp<len; temp++)
        {
          Serial.print(RXDATA[temp], HEX);
          Serial.print(" ");
        }
        Serial.println(" ");
      }
    }
  }
}
