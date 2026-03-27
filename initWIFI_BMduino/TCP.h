// BMC81M001 Wifi(6,7); // rxPin 6 , txPin 7，若使用軟體序列埠請取消註解此行
// BMC81M001 Wifi(&Serial1); // 若使用 BMduino 板上的硬體 Serial1，請取消註解此行
#ifndef _BMC81M001_H__  // 防止重複定義 Header
#define _BMC81M001_H__


//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "BMC81M001.h"  // 引入 WiFi 模組控制函式庫
#include <String.h>     // 引入 String 類別（注意 Arduino 已預設內建）

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

/*** Connect的wifi的Access Point SSID & SSIDPWD*/
#define WIFI_SSID "NUKIOT"     // 無線網路的 SSID (名稱)
#define WIFI_PASS "iot12345"   // 無線網路的密碼

#define IP "www.hinet.net"     // TCP 伺服器的網址或 IP 位址
#define IP_Port 80             // TCP 伺服器連接埠（80 是 HTTP 預設埠）


//*********************************************//
//************* IO_Port Define ***************//
//*********************************************//
int LED = 13;                         // LED port 
//*********************************************//
//************* Variable Define ***************//
//*********************************************//
int LED = 13;   // 內建 LED 腳位，通常可用於指示狀態

#define DEB_CNT     50                // 除錯延遲常數：50 毫秒
#define RES_MAX_LENGTH 200           // 串列緩衝區最大長度

char  SerialBuff[RES_MAX_LENGTH];   // 串列接收資料緩衝區
char  data[30];                     // 用來儲存關鍵資料的緩衝區
int   resLen;                       // 接收資料的實際長度
int   nKeyBuf;                      // Key data 的緩衝處理指標
String DATA_BUF;                   // 暫存資料用的字串變數
String tcpBuff;                    // TCP 傳輸用資料緩衝字串
#endif

BMC81M001 Wifi(&Serial2); // 使用 BMduino 板上的硬體 Serial2 腳位控制 WiFi 模組（例如 RX2, TX2）


void initWiFi() ; // 初始化 WiFi 自訂模組

void initWiFi()   // 初始化 WiFi 自訂模組
{
   Wifi.begin();  // 初始化 WiFi 模組

   Serial.print("TCP Connect Result："); 
   
   // 嘗試連線至指定的 WiFi 熱點
   if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS)) 
   {
      Serial.print("WIFI fail,");  // 連線失敗
   }  
   else
   {
      Serial.print("WIFI success,");  // 連線成功
   }
}





