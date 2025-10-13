#include "Commlib.h"     // 共用函式模組
#include "RFIDLib.h"

void setup() 
{

  Serial.begin(9600);    // 啟動電腦序列埠通訊，設定波特率為 9600（用於顯示資料）
  initRFID() ;
}

void loop()
{
  // 取得目前卡片的 UID 並存入 uid_buf，回傳值為 UID 的長度（bytes）
   if (checkReadRFIDSuccess())  //讀取卡號，如果成功，傳回true，反之false
    {
        uidStr = readRFIDUIDString() ; //建立讀取RFID UUID卡號函式
        //uidByteStr = readRFIDUUIDString() ; //建立讀取RFID UUID卡號函式(Byte內容字串回傳)
      // 輸出換行符號，讓每筆資料換行顯示
       Serial.print("read RFID Card Number is :(");
       Serial.print(uidStr);
       Serial.print(")\n");
      //  Serial.print("read RFID Card Number2 is :(");
      //  Serial.print(uidByteStr);
      //  Serial.print(")\n");
       //Serial.write(uid_buf, nlens);
    }
  // 使用 Serial.write 將 UID 的二進位原始值輸出到序列埠監控工具

  // 每 2 秒讀取一次卡片 UID
  delay(2000);
}
