/********************* 程式整體運作解說 *************************/
/*
 * ============================================================================
 * 【檔案名稱】clouding.h
 * ============================================================================
 *
 * 【程式用途】
 * ----------------------------------------------------------------------------
 * 本檔案負責將血氧／心率感測器量測到的資料，透過 Wi-Fi 以
 * HTTP GET 方式上傳到遠端雲端伺服器（RESTful API），並檢查
 * 伺服器回傳內容以判斷上傳是否成功。
 *
 * 【雲端伺服器資訊】
 * ----------------------------------------------------------------------------
 *   - 伺服器位址：http://iot.arduino.org.tw
 *   - 服務埠號  ：8888
 *   - API 路徑  ：/bmduino/bloodoxygen/dataadd.php
 *   - 請求方法  ：HTTP GET
 *
 * 【上傳資料格式（URL 參數）】
 * ----------------------------------------------------------------------------
 *   ?MAC=%s&bd=%s&hb=%d
 *     - MAC：裝置唯一識別碼（字串，如 112233445566）
 *     - bd ：血氧值（浮點數字串，如 97.45）
 *     - hb ：心跳值（整數，如 83）
 *
 *   完整範例：
 *   http://iot.arduino.org.tw:8888/bmduino/bloodoxygen/dataadd.php
 *       ?MAC=112233445566&bd=97.45&hb=83
 *
 * 【整體運作流程】
 * ----------------------------------------------------------------------------
 * 呼叫 SendtoClouding() 時，會依序執行下列步驟：
 *
 *   1. 使用 sprintf() 將 MAC、血氧值、心跳值格式化為 URL 參數字串，
 *      存入 C 字元陣列 dbagentstr。
 *   2. 將 C 字元陣列轉為 String 物件 connectstr，方便後續 WiFi 函式使用。
 *   3. 印出 connectstr 到序列埠，方便除錯確認 URL 是否正確。
 *   4. 檢查 Wi-Fi 是否已連線（Wifi.getStatus()）：
 *
 *      情況 A：Wi-Fi 已連線
 *        a. Wifi.http_begin(ServerURL, ServerPort, connectstr)
 *           開始 HTTP GET 請求。
 *        b. Wifi.http_get() 執行 GET 動作。
 *        c. Wifi.http_getString() 取得伺服器回傳字串 webresponse。
 *        d. 印出 webresponse 到序列埠。
 *        e. 判斷回傳內容是否包含 "Successful"：
 *             - 有：印出 "Http GET Successful"。
 *             - 無：印出 "Http GET Fail"。
 *        f. Wifi.http_end() 結束 HTTP 連線。
 *
 *      情況 B：Wi-Fi 尚未連線
 *        a. initWiFi() 重新初始化並連線 Wi-Fi（定義於 TCP.h / BMC81M001.h）。
 *        b. GetMAC() 重新取得 MAC 位址，存入 MacData。
 *        c. 印出 MAC 位址。
 *        d. 重複情況 A 的 a~f 步驟，完成 HTTP GET 上傳。
 *
 * 【與其他模組的關係】
 * ----------------------------------------------------------------------------
 *   - TCP.h / BMC81M001.h：提供 Wifi 物件、initWiFi()、GetMAC() 等函式，
 *                          以及 Wifi.http_begin()、Wifi.http_get()、
 *                          Wifi.http_getString()、Wifi.http_end() 等方法。
 *   - OxygenLib.h        ：提供 oxyvalue（血氧值）與 hbvalue（心跳值）
 *                          這兩個全域變數，供本檔格式化 URL 時使用。
 *   - course1017V1.ino   ：主程式在 readok != 0 時呼叫 SendtoClouding()。
 *
 * 【注意事項】
 * ----------------------------------------------------------------------------
 *   1. dbagentstr 大小為 300 字元，若未來 URL 參數變多，需加大此緩衝區。
 *   2. 血氧值傳入時使用 String((float)((float)oxyvalue/100))，
 *      假設 oxyvalue 為「百分比 *100」的整數，需先除以 100 還原為小數。
 *   3. 判斷上傳成功與否，係以伺服器回傳字串是否含 "Successful" 為依據；
 *      若伺服器改為回傳 JSON 或其它格式，需同步調整判斷邏輯。
 *   4. 若 Wi-Fi 尚未連線，本函式會自動呼叫 initWiFi() 重新連線後再上傳，
 *      但連線失敗時不會無限重試，僅嘗試一次。
 * ============================================================================
 */

// 範例網址格式 (實際上就是 Arduino 要送出的 HTTP GET 請求)
// http://iot.arduino.org.tw:8888/bmduino/bloodoxygen/dataadd.php?MAC=112233445566&bd=97.45&hb=83

// 定義 HTTP/HTTPS 的預設通訊埠
#define  HTTPGET_PORT_HTTP 80     // HTTP 預設連接埠
#define  HTTPGET_PORT_HTTPS 443   // HTTPS 預設連接埠

// 定義伺服器的專用通訊埠
#define ServerPort 8888           // 伺服器的實際服務埠號
String ServerURL = "http://iot.arduino.org.tw";   // 伺服器位址

// 設定 RESTful API 的資料上傳格式（使用 %s、%f、%d 作為格式化字元）
// MAC = 裝置的唯一識別碼
// bd = 血氧值 (整數)
// hb = 心跳值 (整數)
#define dbagent "/bmduino/bloodoxygen/dataadd.php?MAC=%s&bd=%d&hb=%d"
//http://iot.arduino.org.tw/bmduino/bloodoxygen/dataadd.php?MAC=AABBCCDDEEFF&bd=%5.3f&hb=%d

// 宣告暫存變數，用來存放 sprintf 格式化後的 URL 字串
char dbagentstr[300];   // C 字元陣列，作為 sprintf() 格式化字串的暫存區
String connectstr;      // 字串物件，用來儲存完整的 RESTful 請求字串
String MacData;         // 紀錄本機網卡 MAC 位址
String webresponse;     // 儲存伺服器回傳的 HTTP GET 結果

/*
  HTTP 完整請求拆解：
  Host：iot.arduino.org.tw:8888
  Path：/bmduino/bloodoxygen/dataadd.php
  Parameters：?MAC=AABBCCDDEEFF&bd=97.45&hb=83
    - MAC：裝置的唯一識別碼
    - bd：血氧值(整數)
    - hb：心跳值(整數)
*/

// 函式宣告：傳送感測資料到雲端
void SendtoClouding();

// 實作：傳送感測資料到雲端伺服器
void SendtoClouding()
{
  // 使用 sprintf() 將感測數據格式化成完整的 URL 參數字串
  // MacData：裝置的 MAC 位址
  // oxyvalue：血氧值 (整數)
  // hbvalue：心跳值(整數)
  sprintf(dbagentstr, dbagent, MacData.c_str(), oxyvalue, hbvalue);

  // 將 C 字元陣列轉為 String，方便後續使用 WiFi 函式
  connectstr = String(dbagentstr);

  // 除錯用途：將組好的 URL 字串印到序列監控器
  Serial.println(connectstr) ;//將組合好的參數字串輸出到序列監控視窗，用於除錯
 
 if  (Wifi.getStatus())
 {
    // 情況 A：Wi-Fi 已連線，直接進行 HTTP GET 上傳
    Wifi.http_begin(ServerURL,ServerPort,connectstr);//begin http get 
    Wifi.http_get();//http get opration
   webresponse = Wifi.http_getString();//get http result
    Serial.println(webresponse);//get http result
   if (webresponse.indexOf("Successful") != -1) //判斷回傳內容是否有"Successful" 字串
   {Serial.println("Http GET Successful"); /*印出http get上傳成功*/} 
   else
    {Serial.println("Http GET Fail"); /*印出http get上傳失敗*/ }
    Wifi.http_end(); //end of http get
   }
 else
 {
    // 情況 B：Wi-Fi 尚未連線，先重新初始化並取得 MAC，再進行上傳
    initWiFi();              // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）
      MacData = GetMAC() ; //取得 MAC 位址字串
    Serial.println("---MAC Address----"); // 分隔線，美觀用途
    Serial.println(MacData); // 印出取得 連接上的SSID熱點之後閘道器IP位址
   Wifi.http_begin(ServerURL,ServerPort,connectstr);//begin http get 
    Wifi.http_get();//http get opration
   webresponse = Wifi.http_getString();//get http result
    Serial.println(webresponse);//get http result
   if (webresponse.indexOf("Successful") != -1) //判斷回傳內容是否有"Successful" 字串
   {Serial.println("Http GET Successful"); /*印出http get上傳成功*/} 
   else
    {Serial.println("Http GET Fail"); /*印出http get上傳失敗*/ }
    Wifi.http_end(); //end of http get
 }
}