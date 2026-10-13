/*
 * ==================================================
 * 程式名稱：clouding.h - 雲端資料傳送模組
 * 適用硬體：BMduino + ESP-12F WiFi 模組 + CO2 感測器
 * 通訊協定：HTTP GET (RESTful API)
 * 
 * 程式功能說明：
 *   本程式為雲端資料傳送模組，負責將 BMduino 開發板讀取到的感測器數值，
 *   透過 HTTP GET 方法上傳至指定的雲端伺服器。主要應用場景為環境監測，
 *   將二氧化碳 (CO2) 濃度數據與裝置 MAC 位址組合成 RESTful API 請求，
 *   傳送至雲端平台進行資料儲存與分析。
 * 
 * 資料流向與運作流程：
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 1. 感測器讀取 CO2 數值 (CO2Value)                       │
 *   │ 2. WiFi 模組取得裝置 MAC 位址 (MacData)                 │
 *   └─────────────────────┬───────────────────────────────────┘
 *                         ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 3. 使用 sprintf 組合 URL 參數                           │
 *   │    格式：/bmduino/co2/dataadd.php?MAC=xxxx&co2=xxxx     │
 *   └─────────────────────┬───────────────────────────────────┘
 *                         ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 4. 檢查 WiFi 連線狀態 (Wifi.getStatus())                │
 *   │    ├── 已連線 → 直接執行 HTTP GET                       │
 *   │    └── 未連線 → 執行 initWiFi() 重新連線                │
 *   │                → 重新取得 MAC 位址                      │
 *   │                → 執行 HTTP GET                          │
 *   └─────────────────────┬───────────────────────────────────┘
 *                         ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 5. 發送 HTTP GET 請求至雲端伺服器                       │
 *   │    伺服器位址：http://iot.arduino.org.tw:8888           │
 *   └─────────────────────┬───────────────────────────────────┘
 *                         ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 6. 接收伺服器回傳資料 (webresponse)                     │
 *   │    判斷是否包含 "Successful" 字串                       │
 *   │    ├── 包含 → 上傳成功                                  │
 *   │    └── 不包含 → 上傳失敗                                │
 *   └─────────────────────────────────────────────────────────┘
 * 
 * 主要功能：
 *   1. URL 參數組合：將 MAC 位址與 CO2 數值組合成完整的 API 查詢字串
 *   2. WiFi 狀態檢測：自動檢查 WiFi 連線狀態，未連線時自動重新連線
 *   3. HTTP GET 請求：透過 WiFi 模組發送 HTTP GET 請求至雲端伺服器
 *   4. 回應解析：解析伺服器回傳內容，判斷資料上傳是否成功
 *   5. 除錯輸出：將關鍵步驟輸出至序列埠，方便開發者監控與除錯
 * 
 * 應用場景：
 *   - 室內空氣品質監測系統
 *   - 智慧農業環境監測
 *   - 工業 CO2 濃度監控
 *   - IoT 感測器資料收集平台
 * 
 * 作者/版本：Best Module / 雲端資料傳送模組
 * 建立日期：2025.03.27
 * 修改紀錄：
 *   - 2025.03.27：加入詳細繁體中文註解與整體說明
 * ==================================================
 */

/*
 * 資料流向示意圖（原始註解保留）：
 * 
 * CO2Value/HTValue/TValue ....... ← 感測器讀值
 * MacData  ← WiFi 模組取得
 * 
 *         ▼ 組 URL 參數
 * sprintf("/bmduino/...MAC=xxxx&co2=xxxx")
 * 
 *         ▼
 * 檢查 Wifi.getStatus()
 *     ├── 已連線 → 執行 HTTP GET
 *     │          → 取得 webresponse
 *     │          → 判斷 Successful?
 *     │
 *     └── 未連線 → initWiFi()
 *                → GetMAC()
 *                → 再執行 HTTP GET
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

// 雲端伺服器主機 URL（不包含通訊埠與路徑）
String ServerURL = "http://iot.arduino.org.tw";

// RESTful API 的 URL 格式模板（使用 %s 讓 sprintf 動態組合 MAC 與 CO2 值）
// %s 為格式化佔位符，會被後續的 MAC 位址與 CO2 數值取代
#define dbagent "/bmduino/co2/dataadd.php?MAC=%s&co2=%s"

//================== 全域變數宣告區 (Global Variables) ==================

// sprintf 暫存區，用來組合完整 RESTful 查詢字串（C 字串格式）
// 大小設定為 300 字元，足夠容納 MAC 位址（12 字元）與 CO2 數值（約 5-6 字元）
char dbagentstr[300];

// 組合後最終使用的字串（String 物件格式），用於 http_begin() 函式
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
//   
//   呼叫時機：主程式中讀取到新的 CO2 數值後，呼叫此函式上傳資料
//   建議頻率：可設定定時上傳（如每 30 秒）或根據需求觸發上傳
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
    // 注意：CO2Value 應在主程式中定義，代表二氧化碳感測器的讀值
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
       
       完整 URL 範例：
       http://iot.arduino.org.tw:8888/bmduino/co2/dataadd.php?MAC=112233445566&co2=15600
    */

    // 印出組好的 URL 路徑與參數，方便除錯檢視
    Serial.println("組合的 URL 路徑與參數:");
    Serial.println(connectstr);


    //===============================================================
    //   步驟 1：確認 WiFi 是否已連線
    //   使用 Wifi.getStatus() 檢查連線狀態
    //   getStatus() 回傳 true 表示已連線，false 表示未連線
    //===============================================================
    if (Wifi.getStatus())
    {
        // =========================================================
        // 情況 1：WiFi 已連線
        // 直接發送 HTTP GET 請求，無需重新連線
        // =========================================================
        
        // 初始化 HTTP GET 連線
        // 參數說明：
        //   - 第一個參數：伺服器網址 (String)
        //   - 第二個參數：通訊埠號 (int)
        //   - 第三個參數：URL 路徑與參數 (String)
        Wifi.http_begin(ServerURL, ServerPort, connectstr);
        
        // 執行 HTTP GET 請求，向伺服器發送資料
        Wifi.http_get();
        
        // 取得伺服器回傳的內容（通常為 "Successful" 或其他狀態訊息）
        webresponse = Wifi.http_getString();
        
        // 印出伺服器回傳結果，方便監控上傳狀態
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

        // 結束 HTTP 連線，釋放網路資源
        Wifi.http_end();
    }
    //===============================================================
    //   WiFi 尚未連線 → 進行 WiFi 重新初始化與重新連線
    //===============================================================
    else
    {
        // =========================================================
        // 情況 2：WiFi 未連線
        // 先嘗試重新連線，再發送 HTTP GET 請求
        // =========================================================
        
        // 重新初始化 WiFi 模組並嘗試重新連線
        // 注意：initWiFi() 應在 TCP.h 或其他檔案中定義
        // 此函式會嘗試連線到預先設定的 WiFi 熱點
        initWiFi();
        
        // 重新取得 MAC 位址
        // 注意：GetMAC() 應在 TCP.h 或其他檔案中定義
        // MAC 位址是裝置的唯一識別碼，用於雲端識別資料來源
        MacData = GetMAC();
        
        // 顯示重新取得的 MAC 位址到序列埠，方便確認
        Serial.println("--- MAC 位址 ----");
        Serial.println(MacData);

        // 再次執行 HTTP GET 流程（此時 WiFi 應已重新連線）
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

        // 結束 HTTP 連線，釋放資源
        Wifi.http_end();
    }
    
    // 函式結束，返回呼叫處
    // 注意：此函式不處理重試機制，若上傳失敗需由主程式決定是否重試
}

//===============================================================
//   使用注意事項與開發者指南：
//===============================================================
/*
1. 外部依賴變數（需在主程式中定義）：
   - MacData: String 類型，儲存裝置 MAC 位址
     取得方式：呼叫 GetMAC() 函式
   - CO2Value: 整數類型，儲存 CO2 感測器讀值
     取得方式：從 CO2 感測器模組讀取
   - Wifi: BMC81M001 類別的物件實例
     用於 WiFi 連線與 HTTP 請求

2. 外部依賴函式（需在主程式中定義或引入）：
   - initWiFi(): 初始化 WiFi 連線
     功能：連線到預先設定的 WiFi 熱點
   - GetMAC(): 取得裝置 MAC 位址
     功能：讀取 WiFi 模組的 MAC 位址並回傳為 String

3. 網路連線流程說明：
   - 第一步：檢查 WiFi 狀態（Wifi.getStatus()）
   - 第二步：若未連線則呼叫 initWiFi() 重新連線
   - 第三步：組合 URL 參數（使用 sprintf 格式化）
   - 第四步：發送 HTTP GET 請求
   - 第五步：檢查伺服器回傳結果

4. 錯誤處理機制：
   - 函式會嘗試重新連線一次（若初始狀態為未連線）
   - 若重新連線後仍失敗，會顯示失敗訊息
   - 建議在主程式中加入以下機制：
     * 重試次數限制（避免無限迴圈）
     * 失敗後的延遲等待（避免頻繁請求）
     * 錯誤記錄功能（便於後續問題排查）

5. 雲端伺服器規格：
   - 主機位址：iot.arduino.org.tw
   - 通訊埠：8888
   - API 路徑：/bmduino/co2/dataadd.php
   - 請求方式：HTTP GET
   - 必要參數：MAC（裝置 MAC 位址）、co2（CO2 濃度數值）
   - 成功回傳：包含 "Successful" 字串的響應

6. 除錯建議：
   - 開啟序列埠監控視窗（速率 9600 bps）
   - 觀察 "組合的 URL 路徑與參數" 輸出，確認參數正確
   - 觀察 "伺服器回傳" 輸出，確認雲端回應狀態
   - 若上傳失敗，可嘗試手動在瀏覽器輸入完整 URL 測試

7. 效能考量：
   - HTTP GET 請求會佔用 WiFi 模組資源
   - 建議控制上傳頻率（如每 30 秒一次），避免過度負載
   - 可考慮在資料變化時才上傳，減少不必要的網路請求
*/