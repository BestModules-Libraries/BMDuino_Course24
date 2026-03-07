/***********************************************************
檔案名稱：ISO14443A_UID.ino
功能描述：
1. 使用 Serial4（硬體串列埠）與 BMC11T001 RFID 讀寫模組進行通訊，波特率為 115200。
2. 使用預設的 Serial（Serial0）與電腦的序列埠監控工具通訊，波特率為 9600。
3. 程式功能為：讀取 ISO14443A 類型卡片的 UID（唯一識別碼），並透過序列埠監控顯示。
4. 支援 EEPROM 資料讀寫功能（此範例僅示範讀 UID）。
硬體連接方式：直接插在 BMduino UNO 開發板上使用。

***********************************************************/
#include <String.h>
#include "BMC11T001.h"  // 引入控制 BMC11T001 RFID 模組的函式庫

// 建立一個 BMC11T001 的物件，並設定使用 Serial4 作為通訊埠
BMC11T001 BMC11(&Serial4);

// 宣告變數：儲存讀取到的 UID 長度
int nlens;

// 宣告陣列：用來儲存從 RFID 卡片讀取到的 UID 資料（最大 50 bytes）
uint8_t uid_buf[50] = {0};
String uidStr = "" ;// RFID卡號的String變數
String uidByteStr = "" ;// RFID卡號的ByteString變數


void initRFID() //初始化RFID Reader
{
    delay(1000);  // 延遲 1 秒，等待 BMC11T001 模組完成啟動初始化
  BMC11.begin(115200);   // 啟動 Serial4 串列通訊，設定波特率為 115200（與模組一致）
  BMC11.begin_ISO14443A();  // 初始化 ISO14443A 協定（通常是 MIFARE 卡片）
}

// 輸出RFID Tag 常見的10位數字以內的卡值(將 4 個 byte 數據轉換成一個 long 整數（32位元）)
unsigned long UUIDString(int d4,int d3, int d2, int d1) //// 輸出RFID Tag 常見的10位數字以內的卡值(將 4 個 byte 數據轉換成一個 long 整數（32位元）)
{
  //讀取 RFID 卡片的 UID，然後把 UID 傳入十六進位轉成之整數與十進位數值方式輸出
  unsigned long tmp = 0;
  tmp = d4 * (unsigned long)16777216 + d3 * (unsigned long)65536 + d2 * (unsigned long)256 + (unsigned long)d1; 
  // 依據每個 byte 的位元組位置，加權組合成一個長整數
  return tmp;
}


boolean checkReadRFIDSuccess()  //讀取卡號，如果成功，傳回true，反之false
{
   nlens = BMC11.getUID_ISO14443A(uid_buf);
    // Serial.print("Len is :(") ;
    // Serial.print(nlens) ;
    // Serial.print(")\n") ;
    
   if (nlens == 12)
    {
      return true ;
    }
    else
    {
      return false ;
    }
}
String readRFIDUUID() //建立讀取RFID UUID卡號函式
{
  String tmp = "" ;  //宣告回傳變數
  unsigned long ttmp ;

  // 使用自訂函式 UUIDString()，輸出RFID Tag 常見的10位數字以內的卡值
  ttmp = UUIDString(
          uid_buf[3],
          uid_buf[2],
          uid_buf[1],
          uid_buf[0]
        );
        tmp = String(ttmp) ;
        Serial.print("tmp is :(");
        Serial.print(tmp);
        Serial.print(")\n");
        
      int len = tmp.length() ;
        Serial.print("Len is :(");
        Serial.print(len);
        Serial.print(")\n");
    if (len <10)
    {
        return genstr('0',10-len)+tmp ;
    }
    else
    {
       return tmp; //回傳卡號到readRFIDUUIDString()函式
    }
  return tmp ;//回傳卡號到readRFIDUUID()函式
}

String readRFIDUUIDString() //建立讀取RFID UUID卡號函式(Byte內容字串回傳)
{
    // 將 UID 轉換成字串格式儲存
  String uidString = "";  //宣告回傳變數
  for (int i = 3; i >=0; i--)   //迴圈讀取RFID四個byte內容
  {
     uidString += print2HEX((int)uid_buf[i]);  // 若小於 16，前面補零，使每個 byte 保持兩位數
  }
    uidString.toUpperCase() ; //字串轉成大寫英文
    return uidString ;

}

String readRFIDUIDString() //建立讀取RFID UUID卡號函式(Byte內容字串回傳)
{
    // 將 UID 轉換成字串格式儲存
  String tmp = "" ;  //宣告回傳變數
  unsigned long ttmp ;
  String uidString = "";  //宣告回傳變數
  int d1,d2,d3,d4;
  String s1,s2,s3,s4 ;
  for (int i = 4; i <12; i++)   //迴圈讀取RFID四個byte內容
  {
     uidString += String(char(uid_buf[i]));  // 若小於 16，前面補零，使每個 byte 保持兩位數
  }
    uidString.toUpperCase() ; //字串轉成大寫英文
    // d1 = (int)unstrzero(,16) ;
    // d2 = (int)unstrzero(substr(uidString,2,4),16) ;
    // d3 = (int)unstrzero(substr(uidString,4,6),16) ;
    // d4 = (int)unstrzero(substr(uidString,6,8),16) ;
    s1 = uidString.substring(0,2) ;
    s2 = uidString.substring(2,4) ;
    s3 = uidString.substring(4,6) ;
    s4 = uidString.substring(6,8) ;

    
     d1 = (int)strtoul(s1.c_str(), NULL, 16);
     d2 = (int)strtoul(s2.c_str(), NULL, 16);
     d3 = (int)strtoul(s3.c_str(), NULL, 16);
     d4 = (int)strtoul(s4.c_str(), NULL, 16);
  // 使用自訂函式 UUIDString()，輸出RFID Tag 常見的10位數字以內的卡值
  ttmp = UUIDString(
          d4,
          d3,
          d2,
          d1
        );
    tmp = String(ttmp) ;
    return tmp ;

}



