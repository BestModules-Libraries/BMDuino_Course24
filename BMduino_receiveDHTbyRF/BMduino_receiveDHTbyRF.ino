#include "RFLib.h"
#include "OledLib.h"
#include "LedButtonLib.h"


// === 建立 BMC36M0x1 無線模組物件 ===
//BMC36M0x1     BMC36(5,4);        // 軟體序列埠：RX=D5, TX=D4
//BMC36M0x1     BMC36(&Serial);    // 使用 BMduino 硬體序列埠 Serial
//BMC36M0x1     BMC36(&Serial1);   // 使用 BMduino 硬體序列埠 Serial1
//BMC36M0x1     BMC36(&Serial2);    // 使用 BMduino 硬體序列埠 Serial2
//BMC36M0x1     BMC36(&Serial3);   // 使用 BMduino 硬體序列埠 Serial3
//BMC36M0x1     BMC36(&Serial4);   // 使用 BMduino 硬體序列埠 Serial4

//-------------------
void Handle_RFPacket_Process() ;
void Handle_RFPacket_Process();
void initSensor();//初始化所有模組
void initAll() ;   // init system all configuration

// ------------------ 初始化 ------------------
void setup()
{
    initAll() ;   // init system all configuration
      
}


// ------------------ 主迴圈 ------------------
void loop() 
{              
  RFMessage_Process();           // 處理按鍵，決定 RF 行為
  Handle_RFPacket_Process();     // 處理 RF 接收的資料封包
}


/******************************************************
函式名稱: RFMessage_Process
功能: 根據按鍵執行 RF 模組對應操作
*******************************************************/
void RFMessage_Process()
{
if (getButtonStatus() == 2) 
  {
  // KEY1 → 進入配對模式
  Serial.println("now 進入配對模式....") ;
  BMC36.setMode(Pairing_Mode);    
  Flag_Pairing = TRUE; 
  Flag_PairSuccess = FALSE;            
  } // end of if (getButtonStatus() == 2) 

if (getButtonStatus() == 1) 
  {    
        Serial.println("配對模式下，發送配對請求封包....") ;
      if(Flag_Pairing)
      {
        // 配對模式下，發送配對請求封包
        BMC36.writePairPackage(SA_Peer_Pair | SA_Pairing); 
      }   // end of if(Flag_Pairing)
      if(Flag_PairSuccess)     
      {
        // 配對成功後，發送資料封包
        for(uint8_t temp=0; temp<16; temp++)
        {
          TXDATA[temp] = DATA++;
        }   // end of for(uint8_t temp=0; temp<16; temp++)
        BMC36.writeRFData(SA_Peer, 16, TXDATA);  // 傳送 16 bytes 資料
      } // end of if(Flag_PairSuccess)  
  } // end of if (getButtonStatus() == 1) 
 
}   // end of void RFMessage_Process()


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
      Serial.println("Start Pairing....") ;
      Flag_Pairing = FALSE;
      Flag_PairSuccess = TRUE;  
      Serial.println("Pair Success");                                                          
    }  
    if(STATUS == 2) // 配對失敗
    {
       Serial.println("配對失敗....") ;
      Flag_Pairing = FALSE;
      Flag_PairSuccess = FALSE;
    }   
    if(STATUS == 3) // 配對逾時
    {
      Serial.println("配對逾時....") ;
      Flag_Pairing = FALSE;
      Flag_PairSuccess = FALSE;                                                            
    }                                              
  }
   
  // --- 接收資料 ---
  if(Flag_PairSuccess)
  {
      Serial.println("接收資料....") ;
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
      } // end of if(STATUS == 1)  // 成功接收到資料
    }   // end of if(BMC36.isInfoAvailable())  
  }   // end of if(Flag_PairSuccess)
} // end of void Handle_RFPacket_Process()

void initSensor()//初始化所有模組
{
  initButton() ;  //init LedButton模組
  initOled() ;//初始化OLED12864，0.96吋OLED顯示模組 BMD31M090s

  initRF() ; //init 433MHz GFSK收發透傳模組
}

void initAll()    // init system all configuration
{
    // 啟動序列埠 (USB Debug 用)
  Serial.begin(9600); 
  initSensor() ;//初始化所有模組
}