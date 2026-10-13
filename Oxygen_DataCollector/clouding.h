// 範例網址格式 (實際上就是 Arduino 要送出的 HTTP GET 請求)
// http://iot.arduino.org.tw:8888/bigdata/bloodoxygen/dataadd.php?MAC=AABBCCDDEEFF&bd=97.45&hb=83

// 定義 HTTP/HTTPS 的預設通訊埠
#define  HTTPGET_PORT_HTTP 80     // HTTP 預設連接埠
#define  HTTPGET_PORT_HTTPS 443   // HTTPS 預設連接埠

// 定義伺服器的專用通訊埠
#define ServerPort 8888           // 伺服器的實際服務埠號
String ServerURL = "http://iot.arduino.org.tw";   // 伺服器位址

// 設定 RESTful API 的資料上傳格式（使用 %s、%f、%d 作為格式化字元）
// MAC = 裝置的唯一識別碼
// bd = 血氧值 (float，小數點一位)
// hb = 心跳值 (整數)
#define dbagent "/bigdata/bloodoxygen/dataadd.php?MAC=%s&bd=%d&hb=%d"

// 宣告暫存變數，用來存放 sprintf 格式化後的 URL 字串
char dbagentstr[300];   // C 字元陣列，作為 sprintf() 格式化字串的暫存區
String connectstr;      // 字串物件，用來儲存完整的 RESTful 請求字串
String MacData;         // 紀錄本機網卡 MAC 位址
String webresponse;     // 儲存伺服器回傳的 HTTP GET 結果

/*
  HTTP 完整請求拆解：
  Host：iot.arduino.org.tw:8888
  Path：/bigdata/bloodoxygen/dataadd.php
  Parameters：?MAC=AABBCCDDEEFF&bd=97.45&hb=83
    - MAC：裝置的唯一識別碼
    - bd：血氧值
    - hb：心跳值
*/

// 函式宣告：傳送感測資料到雲端
void SendtoClouding();

// 實作：傳送感測資料到雲端伺服器
void SendtoClouding()
{
  // 使用 sprintf() 將感測數據格式化成完整的 URL 參數字串
  // MacData：裝置的 MAC 位址
  // oxyvalue：血氧值 (轉成浮點數並縮放，這裡假設 oxyvalue 已經是百分比 *100)
  // hbvalue：心跳值
  sprintf(dbagentstr, dbagent, MacData.c_str(), oxyvalue, hbvalue);

  // 將 C 字元陣列轉為 String，方便後續使用 WiFi 函式
  connectstr = String(dbagentstr);

  // 除錯用途：將組好的 URL 字串印到序列監控器
  Serial.println(connectstr) ;//將組合好的參數字串輸出到序列監控視窗，用於除錯
 
 if  (Wifi.getStatus())
 {
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
 
