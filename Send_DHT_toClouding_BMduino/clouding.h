//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dhDatatadd.php?MAC=112233445566&T=65.1&H=76

#define  HTTPGET_PORT_HTTP 80
#define  HTTPGET_PORT_HTTPS 443


#define ServerPort 8888
String ServerURL = "http://iot.arduino.org.tw";
#define dbagent "/bigdata/dhtdata/dhDatatadd.php?MAC=%s&T=%4.1f&H=%4.1f"
char dbagentstr[300] ; //sprint 使用之暫存區
 String connectstr ;    //一個空的字串變數，後續用來動態組成完整的 RESTful 請求參數。
String MacData ;//網路卡編號
float Tvalue=55, Hvalue=66; //用溫溼度

//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dhDatatadd.php?MAC=112233445566&T=65.1&H=76
// host is  ==>nuk.arduino.org.tw:8888
//  app program is ==> bigdata/dhtdata/dhDatatadd.php
//  App parameters ==> ?MAC=AABBCCDDEEFF&T=34&H=34
//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dhDatatadd.php?MAC=112233445566&T=65.1&H=76
/*
   完整的 HTTP 請求範例，分解如下：
  主機位址：iot.arduino.org.tw:8888（域名與通訊埠）。
  應用程式路徑：/bigdata/dhtdata/dhDatatadd.php（伺服器上的 PHP 程式，用來接收並處理資料）。
  參數：?MAC=112233445566&T=65.1&H=76，表示傳送的資料：
  MAC：設備的唯一識別碼（例如 112233445566）。
  T：溫度值（例如 65.1）。
  H：濕度值（例如 76）。
 
 */

void SendtoClouding() ;    //傳送感測資料到雲端

void SendtoClouding()     //傳送感測資料到雲端
{
   Tvalue = random(190, 281) / 10.0;  // random(190,281) 產生 190~280 的整數，除以10.0得到小數
  // 產生濕度（60 ~ 99）的隨機值
  Hvalue = random(600, 991) / 10.0;
  // 自訂函數，用來將感測器的資料（例如溫度和濕度）透過 HTTP GET 請求傳送到雲端伺服器。
//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dhDatatadd.php?MAC=AABBCCDDEEFF&T=34&H=34
// host is  ==>iot.arduino.org.tw:8888
//  app program is ==> bigdata/dhtdata/dhDatatadd.php
//  App parameters ==> ?MAC=AABBCCDDEEFF&T=34&H=34
          sprintf(dbagentstr,dbagent,MacData.c_str(),Tvalue,Hvalue) ;
          connectstr = String(dbagentstr) ;
          /*
          組成GET Format 的Resetful  的 Parameters 字串
          connectstr：動態組成 RESTful 請求的參數部分：
          MacData：設備的 MAC 位址（假設已在程式其他地方定義）。
          Tvalue：溫度值，轉換成字串格式。
          Hvalue：濕度值，轉換成字串格式。
          */
          
 Serial.println(connectstr) ;//將組合好的參數字串輸出到序列監控視窗，用於除錯
 
 if  (Wifi.getStatus())
 {
    Wifi.http_begin(ServerURL,ServerPort,connectstr);//begin http get 

    Wifi.http_get();//http get opration

    Serial.println(Wifi.http_getString());//get http result

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
    Serial.println(Wifi.http_getString());//get http result
    Wifi.http_end(); //end of http get
 }
}
 
