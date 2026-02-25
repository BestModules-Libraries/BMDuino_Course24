/*****************************************************************
File:         Peer.ino
Description: 
    功能說明：
    1. 短按 KEY1 (D8)，MRX 指示燈開始閃爍 → 進入配對模式 (持續 8 秒)。
    2. 當兩塊板子都進入配對模式後，任意一塊板子短按 KEY2 (D9)，會發送配對請求封包。
       - 配對成功 → MRX 燈熄滅，序列埠顯示 "Pair Success"
       - 8 秒內未成功 → MRX 燈熄滅，需要重新長按 KEY1 進入配對模式
    3. 配對成功後，
    進入接收模式。
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
void initRF(); //init 433MHz GFSK收發透傳模組


//-----------------------
void initRF() //init 433MHz GFSK收發透傳模組
{
  Serial.println("now init 433MHz GFSK收發透傳模組....") ;
    // 設定 BMC36 模組的 UART 通訊鮑率
  BMC36.begin(BDR_38400);   

  // 設定裝置角色為 Peer (點對點裝置)
  BMC36.setDeviceRole(Peer); 
  Serial.println("now init 433MHz GFSK收發透傳模組 OK....") ;
}

void setRFPair()
{
        // KEY1 → 進入配對模式
      BMC36.setMode(Pairing_Mode);    
      Flag_Pairing = TRUE; 
      Flag_PairSuccess = FALSE;   
}

void setRFSleep()
{
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
}

void RFPairing()
{
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
 
}