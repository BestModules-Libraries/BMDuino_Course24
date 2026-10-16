//------外部引入函式區--------------------------

#ifndef _BMC81M001_H__  // 防止重複定義 Header
#define _BMC81M001_H__


//*********************************************//
#include "BMC81M001.h"  // 引入 WiFi 模組控制函式庫
#include <String.h>     // 引入 String 類別（注意 Arduino 已預設內建）


// =============== 全域變數宣告區 (Global Variables) ===============
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

// ------- 宣告感測物件程式區 -----------
// BMC81M001 Wifi(6,7); // rxPin 6 , txPin 7，若使用軟體序列埠請取消註解此行
// BMC81M001 Wifi(&Serial1); // 若使用 BMduino 板上的硬體 Serial1，請取消註解此行
BMC81M001 Wifi(&Serial2); // 使用 BMduino 板上的硬體 Serial2 腳位控制 WiFi 模組（例如 RX2, TX2）

// ------- 自定義函式宣告區 -----------
void initWiFi() ; // 初始化 WiFi 自訂模組
String GetMAC() ; //取得 MAC 位址字串
String GetSSID() ;  //取得 SSID熱點字串
String GetIP()  ; //取得 連接上的SSID熱點之後由DHCP取得的IP ADDRESS
String GetGateWay() ;  //取得 連接上的SSID熱點之後閘道器IP位址
String GetsubMask() ;  //取得 連接上的SSID熱點之後子遮罩位址
String ScanAP() ;  //取得附近可以連接到的所有熱點

// ------- 自定義函式內容程式區 -----------
void initWiFi()   // 初始化 WiFi 自訂模組
{
   Wifi.begin();   // 啟動 WiFi 模組的內部初始化程序
                   // 包含 UART 連線初始化、模組啟動、內部參數設定等

   Wifi.reset();   // 重新重置 WiFi 模組（清除目前所有連線狀態與設定）
                   // reset 功能通常會重新啟動模組，確保處於乾淨狀態

   delay(1000);    // 延遲 1 秒，確保模組 reset 完成後完全啟動好

   Serial.print("init WFIF："); 
                   // (註：WIFI 打成 WFIF，若需我可幫你修正顯示文字)
                   // 將初始化提示文字輸出到序列監控視窗

   // 嘗試連線到指定的 WiFi 熱點 (AP)
   // WIFI_SSID = 無線網路名稱
   // WIFI_PASS = 無線網路密碼
   if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS)) 
   {
      Serial.print("WIFI fail,");  // 若 connectToAP() 回傳 false → 連線失敗
                                   // 例如：密碼錯誤、找不到 AP、訊號弱、DHCP 失敗等
   }  
   else
   {
      Serial.print("WIFI success,"); // 若回傳 true → 成功連上 WiFi
                                     // 此時模組已取得 IP、DNS 等網路參數
   }

   delay(500);     // 再等待 0.5 秒，確保 WiFi 模組與路由器溝通已完全穩定
                   // 避免後續立即開始使用網路功能時出現錯誤
}



String GetMAC()   //取得 SSID熱點字串
{
   delay(500); // 等待模組穩定
   String tmp = "" ;  //產生暫存字串
   tmp = Wifi.getMacAddress(); // 從模組取得 MAC 位址
  tmp.toUpperCase(); // 將字串轉成大寫
  return tmp;        // 回傳 MAC 位址字串
}

String GetSSID()   // 取得目前所連接的 WiFi SSID（無線網路名稱）
{
   delay(500);    // 延遲 0.5 秒，確保 WiFi 模組已穩定回應
                  // （尤其在剛完成連線或執行指令後）

   String tmp = "";   // 宣告一個暫存字串變數 tmp
                      // 用來存放之後取得的 SSID 名稱

   // Wifi.getStatus() 用來確認 WiFi 模組是否處於已連線狀態
   // 回傳 true 表示：已成功連上 AP（有網路連線）
   if (Wifi.getStatus())
   {
      // 若已連線，則從 WiFi 模組讀取 SSID 名稱
      tmp = Wifi.getSSID();

      // 將 SSID 字串轉成大寫（不會更動原始 SSID，只是顯示時變成大寫）
      // 好處：統一格式，避免大小寫不同造成比對困難
      tmp.toUpperCase();
   }
   else
   {
      // 若尚未連上 WiFi，則 SSID 設為空字串
      tmp = "";
   }

   return tmp;   // 回傳目前 SSID 字串（可能為空字串）
}


String GetIP()   // 取得 WiFi 連線後由 DHCP 分配的 IP 位址
{
   delay(500);   // 延遲 0.5 秒，讓 WiFi 模組在前一個指令後有足夠時間回應
                 // 尤其在剛完成連線或 DHCP 尚未完全取得 IP 時很重要

   String tmp = "";   // 宣告暫存字串，用來存放 IP Address

   // Wifi.getStatus() 用來檢查 WiFi 是否處於「已連線」狀態
   // 回傳 true → 已成功連上無線基地台（AP）
   // 回傳 false → 尚未連線或連線中斷
   if (Wifi.getStatus())
   {
      // 若 WiFi 已連線，就從 WiFi 模組取得目前的 IP Address
      // 通常是 DHCP 分配，例如：192.168.1.123
      tmp = Wifi.getIP();

      // 將 IP 文字全部轉成大寫
      // 雖然 IP 不會有大小寫之分，但保留與 SSID 一致的格式處理方法
      tmp.toUpperCase();
   }
   else
   {
      // 若無法取得 WiFi 連線狀態，代表尚未分配 IP
      // 回傳空字串代表「沒有 IP」
      tmp = "";
   }

   // 最後將取得的 IP 位址（可能為空字串）回傳給呼叫者
   return tmp;
}


String GetGateWay()   // 取得 WiFi 熱點連線後，由路由器分配的「閘道器 Gateway IP 位址」
{
   delay(500); // 延遲 0.5 秒，讓 WiFi 模組狀態穩定再讀取資料
               // 避免在剛連線或尚未完整取得 DHCP 資訊時讀取錯誤

   String tmp = "";  // 建立暫存字串，用來存放閘道器 IP

   // 檢查 WiFi 模組目前是否處於「已連線」狀態
   // Wifi.getStatus() = true → 代表已連線成功
   // Wifi.getStatus() = false → 尚未連線，不需讀取資料
   if (Wifi.getStatus())
   {
      tmp = Wifi.getGateway();  // 從 WiFi 模組取得 Gateway（通常為 192.168.1.1）
      tmp.toUpperCase();        // 將字串轉成大寫（保持格式統一）
   }
   else
   {
      tmp = "";  // 若尚未連線，則無法取得 Gateway → 回傳空字串
   }

   return tmp;   // 回傳閘道器 Gateway IP
}


String GetsubMask()   // 取得 WiFi 連線後由 DHCP 分配的子網路遮罩（Subnet Mask）
{
   delay(500); // 延遲 0.5 秒，等待 WiFi 模組準備好

   String tmp = "";  // 建立暫存字串

   if (Wifi.getStatus())  // 若 WiFi 已成功連線
   {
      tmp = Wifi.getMask();  // 取得 Subnet Mask（例如 255.255.255.0）
      tmp.toUpperCase();     // 將字串轉大寫
   }
   else
   {
      tmp = "";  // 未連線 → 無法取得 → 回傳空字串
   }

   return tmp;   // 回傳子網路遮罩
}


String ScanAP()   // 掃描附近可連接的 WiFi 熱點 SSID 列表
{
   delay(500); // 等待 WiFi 模組準備，避免掃描時狀態不穩

   String tmp = "";  // 宣告暫存字串

   if (Wifi.getStatus())  // 若 WiFi 模組處於可運作狀態（即模組有開啟）
   {
      tmp = Wifi.SSID();  // 掃描周遭的 WiFi 熱點列表（通常會是以字串形式回傳）
      tmp.toUpperCase();  // 字串轉大寫，統一顯示格式
   }
   else
   {
      tmp = "";  // 若模組無法工作或未啟動 WiFi 功能，則沒有 SSID 資料
   }

   return tmp;   // 回傳掃描到的 SSID 列表（字串格式）
}

