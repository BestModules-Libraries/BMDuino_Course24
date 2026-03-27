/*
CO2Value ← 感測器讀值
MacData  ← WiFi 模組取得

         ▼ 組 URL 參數
sprintf("/bmduino/...MAC=xxxx&co2=xxxx")

         ▼
檢查 Wifi.getStatus()
    ├── 已連線 → 執行 HTTP GET
    │          → 取得 webresponse
    │          → 判斷 Successful?
    │
    └── 未連線 → initWiFi()
               → GetMAC()
               → 再執行 HTTP GET

*/

//===============================================================
//  專案用途：
//  將 BMduino + ESP-12F 讀取到的 CO2 數值，以 HTTP GET 方法
//  傳送至雲端平台：http://iot.arduino.org.tw:8888
//
//  完整格式範例：
//  http://iot.arduino.org.tw:8888/bmduino/co2/dataadd.php?MAC=AABBCCDDEEFF&co2=15600
//
//  上傳參數說明：
//  MAC  → 裝置唯一識別碼（通常由 WiFi 模組讀取）
//  co2  → 量測到的二氧化碳數值
//===============================================================

//-------------- Include 全域變數與常數定義區 ---------------------

#define HTTPGET_PORT_HTTP 80    // HTTP 協定的預設通訊埠（一般未加密網頁）
#define HTTPGET_PORT_HTTPS 443  // HTTPS 協定的預設通訊埠（加密連線）

#define ServerPort 8888  // 雲端平台後端 API 使用的 Port 8888
String ServerURL = "http://iot.arduino.org.tw";   // 雲端伺服器主機 URL

// RESTful API 的 URL 格式模板（使用 %s 讓 sprintf 動態組合 MAC 與 CO2 值）
#define dbagent "/bmduino/co2/dataadd.php?MAC=%s&co2=%s"

//================== 全域變數宣告區 (Global Variables) ==================

char dbagentstr[300];   // sprintf 暫存區，用來組合完整 RESTful 查詢字串
String connectstr;      // 組合後最終使用的字串，用於 http_begin()
String webresponse;     // 儲存雲端回傳的 GET 回應文字（如 Successful、Fail 等）

// MacData 請注意：此變數在主程式其他地方已定義，例如 initWiFi() 後會取得
// CO2Value：也必須在程式其他地方取得感測器讀值

//-----------http get 內容----------------------
//http://iot.arduino.org.tw:8888/bmduino/co2/dataadd.php?MAC=AABBCCDDEEFF&co2=15600
// host is  ==> iot.arduino.org.tw:8888
//  app program is ==> /bmduino/co2/dataadd.php 
//  App parameters ==> ?MAC=AABBCCDDEEFF&co2=15600 
//http://iot.arduino.org.tw:8888/bmduino/co2/dataadd.php?MAC=AABBCCDDEEFF&co2=15600


//===============================================================
//  函式原型宣告
//===============================================================
void SendtoClouding();  // 將感測資料傳送到雲端伺服器的主要函式


//===============================================================
//   函式名稱：SendtoClouding()
//   功能：將 CO2 感測數值與 MAC 組成 RESTful API URL
//        並透過 HTTP GET 傳送到雲端伺服器
//===============================================================
void SendtoClouding()
{
    // ------------------ 組成 RESTful GET URL 參數字串 ------------------
    //
    // sprintf 的功能是將 dbagent 格式串接 MAC 與 CO2 值，
    // dbagent = "/bmduino/co2/dataadd.php?MAC=%s&co2=%s"
    //
    // MacData.c_str()  → 將 String 轉成 char*，以供 sprintf 使用
    // String(CO2Value).c_str() → 將 CO2 數字轉字串後再轉 char*
    //
    sprintf(dbagentstr, dbagent, MacData.c_str(), String(CO2Value).c_str());
    
    // 將 char[] 轉換成 String 以便後續 http_begin() 使用
    connectstr = String(dbagentstr);

   /*
      connectstr 內容範例：
      /bmduino/co2/dataadd.php?MAC=112233445566&co2=15600
      → 此部分是 URL 的「路徑 + 參數」，不包含主機名稱與 Port，
      因為 Wifi.http_begin(ServerURL, Port, connectstr)
      會自動組成完整 URL。
   */

    // 印出組好的 URL path + parameters，方便除錯
    Serial.println(connectstr);
    //===============================================================
    //   步驟 1：確認 WiFi 是否已連線
    //===============================================================
    if (Wifi.getStatus())
    {
        // 已連線情況 → 直接開始 HTTP GET
        Wifi.http_begin(ServerURL, ServerPort, connectstr);  // 初始化 HTTP GET
 
        Wifi.http_get();                                     // 執行 HTTP GET 動作

        webresponse = Wifi.http_getString();                 // 取得伺服器回傳內容
        Serial.println(webresponse);                         // 印出回傳結果

        // 偵測伺服器是否回傳 "Successful"
        if (webresponse.indexOf("Successful") != -1)
        {
            Serial.println("Http GET Successful");  // 上傳成功
        }
        else
        {
            Serial.println("Http GET Fail");        // 上傳失敗
        }

        Wifi.http_end();     // 結束 HTTP 連線
    }

    //===============================================================
    //   WiFi 尚未連線 → 進行 WiFi 重新初始化與重新連線
    //===============================================================
    else
    {
        initWiFi();              // 重新初始化 WiFi 並嘗試重新連線（外部函式）
        MacData = GetMAC();      // 再次取得 MAC 位址
        Serial.println("---MAC Address----");
        Serial.println(MacData);

        // 再次執行 HTTP GET 流程
        Wifi.http_begin(ServerURL, ServerPort, connectstr);
        Wifi.http_get();
        webresponse = Wifi.http_getString();
        Serial.println(webresponse);

        if (webresponse.indexOf("Successful") != -1)
        {
            Serial.println("Http GET Successful");
        }
        else
        {
            Serial.println("Http GET Fail");
        }

        Wifi.http_end();
    }
}

