/*****************************************************************
File:         readWeight.ino
Description:  關閉模組自動輸出，改由主機傳送指令主動取得重量值
******************************************************************/
#include "BMH12M205.h"  // 引入 BMH12M205 感測器控制函式庫

// 若使用軟體序列通訊（SoftwareSerial）連接到腳位 D6 與 D7，請取消下列註解
// BMH12M205 weight(6,7);        

// 使用 BMduino 上的 Serial1（硬體 UART）連接重量感測器
BMH12M205 weight(&Serial1); 

// 若是使用 Serial2（另一路硬體序列埠），可使用下列定義
// BMH12M205 weight(&Serial2); 

// 宣告一個用來儲存讀取資料的結構體
BMH12M205_GET_t getData;

void setup()
{
   Serial.begin(9600);  // 初始化電腦與主控板之間的序列通訊，速率為 9600bps
   weight.begin();      // 啟動重量感測器模組的初始化通訊設定
   weight.setInfoNotifyStatus(0, false);  
   // 關閉模組的自動輸出功能 (參數 0：表示重量數據，false：表示關閉)
   // 若不關閉，模組會持續主動輸出數據，會造成主動讀取時干擾

   weight.calibrationZero();  
   // 執行歸零校正，將目前重量視為 0，這需要need 300ms完成
   delay(500);  // 等待 0.5 秒以確保歸零完成
}

void loop()
{
    // 讀取重量並印出（使用十進位表示法）
    Serial.print(weight.readWeight(), DEC);  
    Serial.println("g");  // 輸出單位為公克（gram）
    delay(200);           // 每 200 毫秒讀取一次重量
}
