/********************* 程式整體運作解說 *************************/
/*
 * 【檔案名稱】TCP.h
 * 【程式用途】
 *   本檔案為 Wi-Fi／TCP 連線功能的封裝標頭檔，主要負責：
 *     1. 定義 Wi-Fi 連線所需的 SSID 與密碼。
 *     2. 建立 BMC81M001 Wi-Fi 模組物件（Wifi）。
 *     3. 提供初始化連線與查詢網路資訊的函式：
 *          - initWiFi() ：初始化模組並連線至指定 AP。
 *          - GetMAC()   ：取得本機 MAC 位址。
 *          - GetSSID()  ：取得目前連線的 AP 名稱（SSID）。
 *          - GetIP()    ：取得 DHCP 配發的本機 IP。
 *          - GetGateWay()：取得閘道器 IP。
 *          - GetsubMask()：取得子網路遮罩。
 *          - ScanAP()   ：掃描附近可連線的熱點。
 *
 * 【硬體與模組】
 *   - Wi-Fi 模組：BMC81M001（BMduino 板載或外接）。
 *   - 序列埠介面：本檔預設使用硬體 Serial2（&Serial2）。
 *     可依實際接線改用軟體序列埠或 Serial1，
 *     檔案中已保留對應的註解行供切換。
 *   - 內建 LED（腳位 13）：可用於指示狀態。
 *
 * 【連線設定】
 *   - WIFI_SSID："NUKIOT"     （無線網路名稱）
 *   - WIFI_PASS："iot12345"   （無線網路密碼）
 *   ※ 實際部署時請依現場 AP 設定修改這兩個巨集。
 *
 * 【整體運作流程】
 *   ┌────────────────────┐
 *   │    initWiFi()      │  ← 由主程式 setup() 的 INITtWIFI() 呼叫
 *   └─────────┬──────────┘
 *             │
 *             ├─(1) Wifi.begin()  ：初始化 Wi-Fi 模組。
 *             │
 *             ├─(2) Wifi.reset()  ：重置模組，清除先前狀態。
 *             │
 *             ├─(3) delay(1000)   ：等待模組穩定。
 *             │
 *             ├─(4) Wifi.connectToAP(WIFI_SSID, WIFI_PASS)：
 *             │       嘗試連線至指定 AP，並印出成功／失敗訊息。
 *             │
 *             └─(5) delay(500)    ：等待連線與 DHCP 完成。
 *
 *   連線完成後，主程式可呼叫以下查詢函式取得網路資訊：
 *   ┌──────────────┬──────────────────────────────────────┐
 *   │  GetMAC()    │ 取得本機 MAC（轉大寫），常作為裝置 ID │
 *   │  GetSSID()   │ 取得目前連線 AP 名稱（需已連線）      │
 *   │  GetIP()     │ 取得 DHCP 配發的本機 IP              │
 *   │  GetGateWay()│ 取得閘道器 IP                        │
 *   │  GetsubMask()│ 取得子網路遮罩                       │
 *   │  ScanAP()    │ 掃描附近可連線的熱點清單             │
 *   └──────────────┴──────────────────────────────────────┘
 *   ※ 除 GetMAC() 外，其餘函式皆會先以 Wifi.getStatus() 檢查連線狀態；
 *     若未連線則回傳空字串 ""，避免取得無效資料。
 *
 * 【重要全域變數】
 *   - Wifi        ：BMC81M001 模組物件（本檔預設綁定 &Serial2）。
 *   - LED         ：內建 LED 腳位（13）。
 *   - SerialBuff  ：串列接收資料緩衝區（長度 RES_MAX_LENGTH=200）。
 *   - data[30]    ：儲存關鍵資料的緩衝區。
 *   - resLen      ：接收資料的實際長度。
 *   - nKeyBuf     ：Key data 的緩衝處理指標。
 *   - DATA_BUF    ：暫存資料用的字串變數。
 *   - tcpBuff     ：TCP 傳輸用資料緩衝字串。
 *
 * 【巨集定義】
 *   - WIFI_SSID / WIFI_PASS ：連線 AP 的名稱與密碼。
 *   - DEB_CNT              ：除錯延遲常數（50 毫秒）。
 *   - RES_MAX_LENGTH       ：串列緩衝區最大長度（200）。
 *
 * 【與其他模組的關係】
 *   - 本檔被 clouding.h 引用，於 SendtoClouding() 中在 Wi-Fi 未連線時
 *     呼叫 initWiFi() 重新連線，並以 GetMAC() 取得裝置識別碼。
 *   - 本檔被主程式 Oxygen_DataCollectorV2.ino 引用，於 INITtWIFI() 中
 *     呼叫 initWiFi()、GetSSID()、GetIP()、GetMAC() 取得網路資訊。
 *   - 底層依賴 BMC81M001.h 提供的 Wifi 物件與其成員函式：
 *       begin()、reset()、connectToAP()、getStatus()、getMacAddress()、
 *       getSSID()、getIP()、getGateway()、getMask()、SSID() 等。
 *
 * 【注意事項】
 *   - 標頭檔使用 #ifndef / #define / #endif 防止重複引用。
 *   - 注意 #endif 出現在檔案中段（原本即如此），
 *     之後的 Wifi 物件宣告與函式實作位於 #endif 之外，
 *     實際使用時請留意此結構是否符合編譯需求。
 *   - 各查詢函式內部皆有 delay(500) 等待模組穩定，
 *     呼叫時會產生約 0.5 秒的延遲，需注意即時性需求。
 *   - Wifi.getStatus() 回傳真值表示已連線；
 *     未連線時除 GetMAC() 外皆回傳空字串。
 *   - 請依實際硬體接線選擇軟體序列埠、Serial1 或 Serial2。
 */
#ifndef _BMC81M001_H__  // 防止重複定義 Header
#define _BMC81M001_H__


//*********************************************//
#include "BMC81M001.h"  // 引入 WiFi 模組控制函式庫
#include <String.h>     // 引入 String 類別（注意 Arduino 已預設內建）


//*********** wifi information ****************//
/*** Connect的wifi的Access Point SSID & SSIDPWD*/
#define WIFI_SSID "NUKIOT"     // 無線網路的 SSID (名稱)
#define WIFI_PASS "iot12345"   // 無線網路的密碼



//************* IO_Port Define ***************//
//************* Variable Define ***************//

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

// BMC81M001 Wifi(6,7); // rxPin 6 , txPin 7，若使用軟體序列埠請取消註解此行
// BMC81M001 Wifi(&Serial1); // 若使用 BMduino 板上的硬體 Serial1，請取消註解此行
BMC81M001 Wifi(&Serial2); // 使用 BMduino 板上的硬體 Serial2 腳位控制 WiFi 模組（例如 RX2, TX2）


void initWiFi() ; // 初始化 WiFi 自訂模組
String GetMAC() ; //取得 MAC 位址字串
String GetSSID() ;  //取得 SSID熱點字串
String GetIP()  ; //取得 連接上的SSID熱點之後由DHCP取得的IP ADDRESS
String GetGateWay() ;  //取得 連接上的SSID熱點之後閘道器IP位址
String GetsubMask() ;  //取得 連接上的SSID熱點之後子遮罩位址
String ScanAP() ;  //取得附近可以連接到的所有熱點


void initWiFi()   // 初始化 WiFi 自訂模組
{
   Wifi.begin();  // 初始化 WiFi 模組
   Wifi.reset() ;;//reset wifi clear
   delay(1000) ;

   Serial.print("init WFIF："); 
   
   // 嘗試連線至指定的 WiFi 熱點
   if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS)) 
   {
      Serial.print("WIFI fail,");  // 連線失敗
   }  
   else
   {
      Serial.print("WIFI success,");  // 連線成功
   }
   delay(500) ;//wait for all wifi and is ok
}



String GetMAC()   //取得 SSID熱點字串
{
   delay(500); // 等待模組穩定
   String tmp = "" ;  //產生暫存字串
   tmp = Wifi.getMacAddress(); // 從模組取得 MAC 位址
  tmp.toUpperCase(); // 將字串轉成大寫
  return tmp;        // 回傳 MAC 位址字串
}

String GetSSID()   //取得 連接上的SSID熱點字串
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getSSID(); // 從模組取得SSID熱點字串
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳 SSID熱點字串
}

String GetIP()   //取得 連接上的SSID熱點之後由DHCP取得的IP ADDRESS
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getIP(); // 從模組取得SSID熱點字串
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳 SSID熱點字串
}

String GetGateWay()   //取得 連接上的SSID熱點之後閘道器IP位址
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getGateway(); // 從模組取得 連接上的SSID熱點之後閘道器IP位址
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳  連接上的SSID熱點之後閘道器IP位址
}

String GetsubMask()   //取得 連接上的SSID熱點之後子遮罩位址
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.getMask(); // 從模組取得 連接上的SSID熱點之後子遮罩位址
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳  連接上的SSID熱點之後子遮罩位址
}

String ScanAP()   //取得附近可以連接到的所有熱點
{
   delay(500); // 等待模組穩定
    String tmp = "" ;  //產生暫存字串
   if  (Wifi.getStatus())
   {
      tmp = Wifi.SSID(); // 取得附近可以連接到的所有熱點
      tmp.toUpperCase(); // 將字串轉成大寫
   }
   else
   {
      tmp = "" ;   
   }

   return tmp;        // 回傳  連接上的SSID熱點之後子遮罩位址
}