/*
CO2Value/HTValue/TValue ....... ← 感測器讀值
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

// 通訊埠定義
#define HTTPGET_PORT_HTTP 80    // HTTP 協定的預設通訊埠（一般未加密網頁）
#define HTTPGET_PORT_HTTPS 443  // HTTPS 協定的預設通訊埠（加密連線）

#define ServerPort 8888  // 雲端平台後端 API 使用的 Port 8888

// 雲端伺服器主機 URL
String ServerURL = "http://iot.arduino.org.tw";

// RESTful API 的 URL 格式模板（使用 %s 讓 sprintf 動態組合 MAC 與 CO2 值）
#define dbagent "/bmduino/co2/dataadd.php?MAC=%s&co2=%s"

//================== 全域變數宣告區 (Global Variables) ==================

// sprintf 暫存區，用來組合完整 RESTful 查詢字串
char dbagentstr[300];

// 組合後最終使用的字串，用於 http_begin()
String connectstr;

// 儲存雲端回傳的 GET 回應文字（如 Successful、Fail 等）
String webresponse;

/*
 * 注意：以下變數應在主程式中已定義
 * 
 * MacData：裝置 MAC 位址，通常在 initWiFi() 後會取得
 * 例如：String MacData;
 * 
 * CO2Value：二氧化碳感測器讀值，應在其他部分取得
 * 例如：int CO2Value;
 * 
 * Wifi：BMC81M001 類別的實例物件
 * 例如：BMC81M001 Wifi;
 */

//===============================================================
//  函式原型宣告 (Function Prototypes)
//===============================================================
void SendtoClouding();  // 將感測資料傳送到雲端伺服器的主要函式


//===============================================================
//   函式名稱：SendtoClouding()
//   功能：將 CO2 感測數值與 MAC 組成 RESTful API URL
//        並透過 HTTP GET 傳送到雲端伺服器
//   參數：無
//   回傳值：無
//===============================================================
void SendtoClouding()
{
    // ------------------ 組成 RESTful GET URL 參數字串 ------------------
    //
    // 使用 sprintf 函式將 MAC 位址和 CO2 數值填入 URL 模板
    // 
    // 格式說明：
    // dbagent = "/bmduino/co2/dataadd.php?MAC=%s&co2=%s"
    // 
    // MacData.c_str()       → 將 String 類型的 MAC 位址轉換成 C 字串 (char*)
    // String(CO2Value).c_str() → 將整數 CO2Value 轉為 String，再轉為 C 字串
    //
    sprintf(dbagentstr, dbagent, MacData.c_str(), String(CO2Value).c_str());
    
    // 將 C 字串陣列轉換成 String 物件，以便後續 http_begin() 函式使用
    connectstr = String(dbagentstr);

    /*
       組合後的 connectstr 內容範例：
       
       "/bmduino/co2/dataadd.php?MAC=112233445566&co2=15600"
       
       注意：
       - 此部分為 URL 的「路徑 + 查詢參數」
       - 不包含主機名稱與通訊埠
       - 主機名稱和通訊埠會在 Wifi.http_begin() 函式中組合
       
       http_begin() 會自動組成完整 URL：
       ServerURL + ":" + ServerPort + connectstr
    */

    // 印出組好的 URL 路徑與參數，方便除錯檢視
    Serial.println("組合的 URL 路徑與參數:");
    Serial.println(connectstr);


    //===============================================================
    //   步驟 1：確認 WiFi 是否已連線
    //   使用 Wifi.getStatus() 檢查連線狀態
    //===============================================================
    if (Wifi.getStatus())
    {
        // 情況 1：WiFi 已連線
        // ------------------------------------------
        
        // 初始化 HTTP GET 連線
        // 參數：伺服器網址、通訊埠、URL 路徑與參數
        Wifi.http_begin(ServerURL, ServerPort, connectstr);
        
        // 執行 HTTP GET 請求
        Wifi.http_get();
        
        // 取得伺服器回傳的內容
        webresponse = Wifi.http_getString();
        
        // 印出伺服器回傳結果
        Serial.println("伺服器回傳:");
        Serial.println(webresponse);

        // 判斷回傳內容是否包含 "Successful" 字串
        // indexOf() 會回傳字串位置，若找不到則回傳 -1
        if (webresponse.indexOf("Successful") != -1)
        {
            Serial.println("Http GET 成功");  // 上傳成功
        }
        else
        {
            Serial.println("Http GET 失敗");  // 上傳失敗
        }

        // 結束 HTTP 連線，釋放資源
        Wifi.http_end();
    }
    //===============================================================
    //   WiFi 尚未連線 → 進行 WiFi 重新初始化與重新連線
    //===============================================================
    else
    {
        // 情況 2：WiFi 未連線
        // ------------------------------------------
        
        // 重新初始化 WiFi 模組並嘗試重新連線
        // 注意：initWiFi() 應在其他檔案中定義
        initWiFi();
        
        // 重新取得 MAC 位址
        // 注意：GetMAC() 應在其他檔案中定義
        MacData = GetMAC();
        
        // 顯示重新取得的 MAC 位址
        Serial.println("--- MAC 位址 ----");
        Serial.println(MacData);

        // 再次執行 HTTP GET 流程
        Wifi.http_begin(ServerURL, ServerPort, connectstr);
        Wifi.http_get();
        webresponse = Wifi.http_getString();
        Serial.println("伺服器回傳:");
        Serial.println(webresponse);

        // 判斷上傳結果
        if (webresponse.indexOf("Successful") != -1)
        {
            Serial.println("Http GET 成功");
        }
        else
        {
            Serial.println("Http GET 失敗");
        }

        // 結束 HTTP 連線
        Wifi.http_end();
    }
    
    // 函式結束，返回呼叫處
}

//===============================================================
//   使用注意事項：
//===============================================================
/*
1. 此函式依賴以下外部變數：
   - MacData: String 類型，儲存裝置 MAC 位址
   - CO2Value: 整數類型，儲存 CO2 感測器讀值
   - Wifi: BMC81M001 類別的物件實例

2. 此函式依賴以下外部函式：
   - initWiFi(): 初始化 WiFi 連線
   - GetMAC(): 取得裝置 MAC 位址

3. 網路連線流程：
   - 先檢查 WiFi 狀態
   - 若未連線則重新初始化
   - 組合 URL 參數
   - 發送 HTTP GET 請求
   - 檢查伺服器回傳結果

4. 錯誤處理：
   - 函式會嘗試重新連線一次
   - 若重新連線後仍失敗，會顯示失敗訊息
   - 建議在主程式中加入重試機制或錯誤記錄
*/