/*
================================================================================
【程式名稱】clouding1.h
【功能概述】本檔案為雲端資料上傳自訂函式庫，主要負責將 LUX 亮度感測器
            所讀取的亮度（L）數值，連同裝置的 MAC 位址，透過
            HTTP GET 請求傳送至遠端雲端伺服器（iot.arduino.org.tw），
            並解析伺服器回傳結果以判斷上傳是否成功。

【整體運作流程解說】
--------------------------------------------------------------------------------
1. 常數與變數定義區：
   (1) HTTPGET_PORT_HTTP / HTTPGET_PORT_HTTPS：定義 HTTP（80）與
       HTTPS（443）的標準通訊埠編號。
   (2) ServerPort：本系統實際使用的雲端伺服器通訊埠（8888）。
   (3) ServerURL：雲端伺服器網址（http://iot.arduino.org.tw）。
   (4) dbagent：RESTful 請求參數的格式化字串模板，包含 MAC、L 二個
       佔位符（%s、%6.1f）。
   (5) dbagentstr[300]：用於 sprintf 格式化輸出的字元暫存區。
   (6) connectstr：動態組成的完整 RESTful 請求參數字串。
   (7) MacData：網路卡編號（MAC 位址）字串，於主程式或 TCP.h 中取得。
   (8) webresponse：儲存 HTTP GET 請求後伺服器回傳的內容。

2. SendtoClouding() 函式運作流程：
   (1) 使用 sprintf 將 MacData、LValue 套入 dbagent 模板，
       組合成 RESTful 參數字串，存入 dbagentstr。
   (2) 將 dbagentstr 轉為 String 型別存入 connectstr，並印出至序列埠
       以供除錯。
   (3) 檢查 WiFi 連線狀態（Wifi.getStatus()）：
       - 若已連線：直接執行 HTTP GET 上傳流程。
       - 若未連線：先呼叫 initWiFi() 重新連線，並透過 GetMAC() 重新
         取得 MAC 位址後，再執行 HTTP GET 上傳流程。
   (4) HTTP GET 上傳流程：
       - Wifi.http_begin(ServerURL, ServerPort, connectstr)：
         初始化 HTTP 連線並帶入請求參數。
       - Wifi.http_get()：執行 HTTP GET 請求。
       - Wifi.http_getString()：取得伺服器回傳字串。
       - 判斷回傳字串中是否包含 "Successful"：
         * 包含 → 印出 "Http GET Successful"（上傳成功）。
         * 不包含 → 印出 "Http GET Fail"（上傳失敗）。
       - Wifi.http_end()：結束 HTTP 連線。

【HTTP 請求範例解析】
--------------------------------------------------------------------------------
請求網址：
  http://iot.arduino.org.tw:8888/bmduino/lux/dataadd.php?MAC=112233445566&L=300

分解說明：
- 主機位址（Host）    ：iot.arduino.org.tw:8888
- 應用程式路徑（Path）：/bmduino/lux/dataadd.php
- 請求參數（Query）   ：?MAC=112233445566&L=300
    * MAC：設備唯一識別碼（12 碼十六進位字串）
    * L  ：亮度值（攝氏，格式 %6.1f）
    

【硬體與函式庫對應】
- TCP.h：提供 Wifi 物件（getStatus、http_begin、http_get、http_getString、
         http_end）、initWiFi()、GetMAC() 等函式。
- LuxLib.h：提供 LValue 全域變數（亮度數值）。

【作者/版本】課程範例 clouding1.h
================================================================================
*/

// 雲端資料上傳範例網址（僅供參考）：
// http://iot.arduino.org.tw:8888/bmduino/lux/dataadd.php?MAC=112233445566&L=300

// #define  HTTPGET_PORT_HTTP 80    // HTTP 標準通訊埠編號
// #define  HTTPGET_PORT_HTTPS 443  // HTTPS 標準通訊埠編號


// #define ServerPort 8888          // 本系統使用的雲端伺服器通訊埠編號
// String ServerURL = "http://iot.arduino.org.tw"; // 雲端伺服器網址


// RESTful 請求參數格式化模板：
// %s    → MAC 位址字串
// %6.1f → 亮度值（欄寬 6、小數 1 位）

#define dbagent1 "/bmduino/lux/dataadd.php?MAC=%s&L=%5.1f"

// char dbagentstr[300]; // sprintf 使用之字元暫存區（存放格式化後的請求參數字串）
// String connectstr;    // 動態組成的完整 RESTful 請求參數字串
// String MacData;       // 網路卡編號（MAC 位址）字串
// // float Tvalue=55, Hvalue=66; // 測試用的溫濕度範例值（已註解）
// String webresponse;   // 儲存 HTTP GET 請求後伺服器回傳的內容



/*
   完整的 HTTP 請求範例，分解如下：
   主機位址：iot.arduino.org.tw:8888（域名與通訊埠）。
   應用程式路徑：/bmduino/lux/dataadd.php（伺服器上的 PHP 程式，
                 用來接收並處理資料）。
   參數：?MAC=112233445566&L=300，表示傳送的資料：
     MAC：設備的唯一識別碼（例如 112233445566）。
     L  ：亮值（例如 300 lux）。

 */

void SendtoClouding1(); // 函式原型宣告：傳送感測資料到雲端


//================================================================================
// SendtoClouding()：將亮度資料透過 HTTP GET 傳送至雲端伺服器
// 參數：無（使用全域變數 MacData、LValue）
// 回傳：無
//================================================================================
void SendtoClouding1()
{

    // 自訂函式，用來將感測器的資料（例如亮度）透過 HTTP GET 請求
    // 傳送到雲端伺服器。
    // http://iot.arduino.org.tw:8888/bmduino/lux/dataadd.php?MAC=AABBCCDDEEFF&L=300
    // 主機位址（Host）    ：iot.arduino.org.tw:8888
    // 應用程式路徑（Path）：/bmduino/lux/dataadd.php
    // 請求參數（Query）   ：?MAC=AABBCCDDEEFF&L=300

    // 使用 sprintf 將 MAC、溫度、濕度套入 dbagent 模板，組合成請求參數字串
    sprintf(dbagentstr, dbagent1, MacData.c_str(), LValue);
    connectstr = String(dbagentstr); // 將 char 陣列轉為 String 型別

    /*
      組成 GET 格式的 RESTful 請求參數字串：
      connectstr：動態組成 RESTful 請求的參數部分：
        MacData：設備的 MAC 位址（假設已在程式其他地方定義）。
        LValue ：亮度值，轉換成字串格式。
 
    */

    Serial.println(connectstr); // 將組合好的參數字串輸出到序列監控視窗，用於除錯

    // 檢查 WiFi 連線狀態
    if (Wifi.getStatus())
    {
        // ---------- WiFi 已連線：直接執行 HTTP GET 上傳 ----------
        Wifi.http_begin(ServerURL, ServerPort, connectstr); // 初始化 HTTP GET 連線
        Wifi.http_get();                                    // 執行 HTTP GET 請求
        webresponse = Wifi.http_getString();                // 取得伺服器回傳字串
        Serial.println(webresponse);                        // 印出回傳內容（除錯用）

        // 判斷回傳內容是否包含 "Successful" 字串
        if (webresponse.indexOf("Successful") != -1)
        {
            Serial.println("Http GET Successful"); // 印出 HTTP GET 上傳成功
        }
        else
        {
            Serial.println("Http GET Fail");       // 印出 HTTP GET 上傳失敗
        }
        Wifi.http_end(); // 結束 HTTP GET 連線
    }
    else
    {
        // ---------- WiFi 未連線：先重新連線再上傳 ----------
        initWiFi();             // 執行 WiFi 模組初始化與連線（定義於 TCP.h）
        MacData = GetMAC();     // 重新取得 MAC 位址字串
        Serial.println("---MAC Address----"); // 分隔線，美觀用途
        Serial.println(MacData); // 印出取得的 MAC 位址

        Wifi.http_begin(ServerURL, ServerPort, connectstr); // 初始化 HTTP GET 連線
        Wifi.http_get();                                    // 執行 HTTP GET 請求
        webresponse = Wifi.http_getString();                // 取得伺服器回傳字串
        Serial.println(webresponse);                        // 印出回傳內容（除錯用）

        // 判斷回傳內容是否包含 "Successful" 字串
        if (webresponse.indexOf("Successful") != -1)
        {
            Serial.println("Http GET Successful"); // 印出 HTTP GET 上傳成功
        }
        else
        {
            Serial.println("Http GET Fail");       // 印出 HTTP GET 上傳失敗
        }
        Wifi.http_end(); // 結束 HTTP GET 連線
    }
}