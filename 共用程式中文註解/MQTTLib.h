/*-------------------------------------------------------------
  程式名稱：MQTT 通訊模組 (MQTT Communication Module)
  程式用途：本程式提供 MQTT（Message Queuing Telemetry Transport）
           通訊協定的相關功能，用於物聯網設備與 MQTT Broker 伺服器
           之間的訊息發布（Publish）與訂閱（Subscribe）。
  
  硬體架構：適用於支援 WiFi 連線的 BMduino 或 Arduino 開發板，
           需搭配 WiFi 模組進行網路連線。
  
  作者說明：本程式為 Arduino C 語言撰寫，提供動態 Client ID 生成、
           主題格式化、JSON 資料封裝、OLED 顯示等功能，適用於
           物聯網資料上傳與遠端控制應用。
  
  主要功能：
    1. 動態產生 MQTT Client ID（基於設備 MAC 位址）
    2. 自動格式化發布與訂閱的主題名稱
    3. 連接 MQTT Broker 伺服器（預設為 broker.emqx.io）
    4. 提供 OLED 顯示功能（顯示標題、IP、設備ID、指令等）
    5. 支援 JSON 格式的資料封裝與傳輸
  
  MQTT 設定參數：
    - Broker: broker.emqx.io（公共測試伺服器）
    - Port: 1883（非加密連線）
    - Client ID: 動態生成，格式為 "tw" + MAC 位址
    - 訂閱主題: /arduino/SW/#
    - 發布主題: /arduino/SW/[MAC 位址]
  
  最後修改：2026年
-------------------------------------------------------------*/

#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用於處理 JSON 格式資料
StaticJsonDocument<200> doc; // 建立靜態 JSON 文件物件，容量為 200 bytes
                             // 用於儲存與序列化 JSON 資料結構

//-----宣告MQTT Broker伺服器登錄資料----- 
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   // 客戶端識別碼（預留，未使用）
#define USERNAME  ""                              // MQTT Broker 登入使用者名稱（若有需要可填入）
#define PASSWORD  ""                              // MQTT Broker 登入密碼（若有需要可填入）
#define MQTT_HOST "broker.emqx.io"                // MQTT Broker 伺服器位址（EMQX 公共測試伺服器）
#define SERVER_PORT 1883                          // MQTT Broker 伺服器埠號（1883 為非加密連線）

//------格式化字串變數區--------------------------
// Publish & Subscribe use（發布與訂閱主題相關變數）
// String topic ;                        // MQTT_Topic（預留變數）
String topic="/arduino/SW/%s" ;          // 主題格式字串（%s 會被 MAC 位址取代）
const char* PubTop = "/arduino/SW/%s" ;  // 發布主題的格式化字串（常數型態）
const char* SubTop = "/arduino/SW/#" ;   // 訂閱主題的格式化字串（# 為萬用字元，訂閱所有子主題）

String TopicT;                           // 儲存完整主題字串的變數
char SubTopicbuffer[200];                // 訂閱主題的字元緩衝區（用於儲存格式化後的主題）
char PubTopicbuffer[200];                // 發布主題的字元緩衝區（用於儲存格式化後的主題）
String PayloadT;                         // 儲存訊息內容（Payload）的字串變數
char Payloadbuffer[250];                 // 訊息內容的字元緩衝區（用於儲存格式化後的 payload）
char clintid[20];                        // 客戶端 ID（Client ID）的字元陣列，用於識別 MQTT 連線

//---------------------自定義函式區宣告--------------
void fillCID(String mm);                 // 產生動態 MQTT Broker Client ID（基於 MAC 位址）
void fillTopic(String mm);               // 填入對應 MAC 位址的發布與訂閱主題
void insertBeforeChar(char *str, char target, char toInsert); // 在字串中指定字元前插入新字元
//void fillPayload(String dev, float d1, float d2); // 根據 MAC 位址、溫度、濕度填入 payload（已註解）
void initMQTT();                         // 初始化 MQTT 連線設定，連接 Broker
void showTitleonOled(String ss);         // 在 OLED 螢幕上顯示標題文字
void showIPonOled(String ss);            // 在 OLED 螢幕上顯示 IP 位址
void showRelayNOonOled(int ss);          // 在 OLED 螢幕上顯示繼電器編號
void showCommandonOled(String ss);       // 在 OLED 螢幕上顯示控制指令
void showDeviceonOled(String ss);        // 在 OLED 螢幕上顯示設備 ID

//----------自定義函式主體區--------------
#define DEB_CNT     80                   // 定義延遲計數值（50ms * 80 = 4000ms，用於除錯延遲）
#define RES_MAX_LENGTH 200               // 定義接收緩衝區最大長度

String ReciveBuff;                       // 儲存接收到的資料（用於 MQTT 回傳訊息）
int ReciveBufflen;                       // 儲存接收緩衝區的資料長度

//-------------------------------------------------------------
// 函式名稱：fillCID
// 功能說明：產生動態的 MQTT Client ID，格式為 "tw" + 設備 MAC 位址
// 輸入參數：mm - 設備的 MAC 位址字串
// 回傳值：無
//-------------------------------------------------------------
void fillCID(String mm) //產生動態MQTT Broker ClientID
{
    // 根據 MAC 位址產生隨機的 client ID
    // 組合 client ID 為 "tw" + MAC 位址
    clintid[0]= 't' ;    // Client ID 第一個字元設為 't'
    clintid[1]= 'w' ;    // Client ID 第二個字元設為 'w'
    mm.toCharArray(&clintid[2], mm.length()+1) ; // 將 MAC 位址從索引 2 開始填入
    clintid[2+mm.length()+1] = '\n' ; // 在結尾加入換行字元
    Serial.print("Client ID:(") ;     // 透過序列埠輸出 Client ID 資訊
    Serial.print(clintid) ;
    Serial.print(") \n") ;
}

//-------------------------------------------------------------
// 函式名稱：fillTopic
// 功能說明：根據設備 MAC 位址，格式化發布與訂閱的主題名稱
// 輸入參數：mm - 設備的 MAC 位址字串
// 回傳值：無
//-------------------------------------------------------------
void fillTopic(String mm) //填入對應MAC Address的tpoic
{
  // 使用 sprintf 將 MAC 位址填入發布主題的格式字串中
  sprintf(PubTopicbuffer, PubTop, mm.c_str()) ;
  Serial.print("Publish Topic Name:(") ;  // 輸出發布主題資訊
  Serial.print(PubTopicbuffer) ;
  Serial.print("/") ;
  Serial.print(sizeof(PubTopicbuffer)) ;   
  Serial.print("^") ;
  Serial.print(String(PubTopicbuffer)) ;
  Serial.print("/") ;
  Serial.print(sizeof(String(PubTopicbuffer))) ;
  Serial.print(") \n") ;
  
  // 使用 sprintf 將 MAC 位址填入訂閱主題的格式字串中
  sprintf(SubTopicbuffer, SubTop, mm.c_str()) ;
  Serial.print("Subscribe Topic Name:(") ; // 輸出訂閱主題資訊
  Serial.print(SubTopicbuffer) ;
  Serial.print("/")  ;
  Serial.print(SubTop) ;
  Serial.print(") \n")  ;
}

//-------------------------------------------------------------
// 函式名稱：insertBeforeChar
// 功能說明：在字串中指定的字元前插入新的字元（用於處理 AT Command 的轉義字元）
// 輸入參數：
//   str - 要處理的字元陣列（字串）
//   target - 目標字元（在此字元前插入）
//   toInsert - 要插入的新字元
// 回傳值：無
//-------------------------------------------------------------
void insertBeforeChar(char *str, char target, char toInsert) 
{
  int len = strlen(str);      // 取得原始字串長度
  uint8_t count = 0;          // 計數器，記錄插入次數
  for (int j = 0; j < len + count; j++) {
    if(str[j] == target)      // 找到目標字元
    {
      count++;                // 增加插入計數
      for(int i = len + count; i >= j; i--) { // 向後移動字元，騰出空間
        str[i+1] = str[i];
      }
      str[j] = toInsert;      // 插入新字元
      j++;                    // 跳過剛插入的字元，繼續處理
    }
  }
}

//-------------------------------------------------------------
// 函式名稱：initMQTT
// 功能說明：初始化 MQTT 連線，產生 Client ID 與主題，並連接至 Broker
// 輸入參數：無
// 回傳值：無
// 注意：此函式假設已定義 MacData 全域變數（設備 MAC 位址）
//-------------------------------------------------------------
void initMQTT() //初始化init MQTT Server
{
    fillCID(MacData) ;      // 根據 MAC 位址產生 Client ID
    fillTopic(MacData) ;    // 根據 MAC 位址產生發布與訂閱主題
    
    // 輸出所有 MQTT 連線參數到序列埠監控視窗
    Serial.print("clintid:(") ;
    Serial.print(clintid) ;
    Serial.print(")\n") ;
    Serial.print("USERNAME:(") ;
    Serial.print(USERNAME) ;
    Serial.print(")\n") ;
    Serial.print("PASSWORD:(") ;
    Serial.print(PASSWORD) ;
    Serial.print(")\n") ;
    Serial.print("MQTT_HOST:(") ;
    Serial.print(MQTT_HOST) ;
    Serial.print(")\n") ;
    Serial.print("SERVER_PORT:(") ;
    Serial.print(SERVER_PORT) ;
    Serial.print(")\n") ;
    Serial.println("Now Connect MQTT Broker.....") ;

    //----判斷是否連上MQTT Broker伺服器-----------
    if (Wifi.configMqtt(String(clintid), USERNAME, PASSWORD, MQTT_HOST, SERVER_PORT) != 0)
    {
        //----成功連上 MQTT Broker 伺服器-----------
        Serial.println("Connect to MQTT Server Successful") ;
    }
    else 
    {
        //----無法連上 MQTT Broker 伺服器-----------
        Serial.println("Connect to MQTT Server failed") ;
        while(1);  // 進入永久迴圈，終止程式執行
    }
    
    // -----處理訂閱主題問題-----
    Wifi.setSubscribetopic(SubTop);  // 設定要訂閱的主題
}

//-------------------------------------------------------------
// 函式名稱：showTitleonOled
// 功能說明：在 OLED 螢幕的第一行顯示標題文字
// 輸入參數：ss - 要顯示的標題字串
// 回傳值：無
//-------------------------------------------------------------
void showTitleonOled(String ss) //列印抬頭於OLED上
{
    printText(0, 0, "              ") ;  // 先清空該行內容（填入空格）
    printText(0, 0, ss) ;               // 將標題字串顯示在 OLED 上
    Serial.print("Title on OLED:(") ;   // 同時輸出到序列埠供除錯
    Serial.print(ss) ;
    Serial.print(")\n") ;     
}

//-------------------------------------------------------------
// 函式名稱：showDeviceonOled
// 功能說明：在 OLED 螢幕的指定位置顯示設備 ID
// 輸入參數：ss - 要顯示的設備 ID 字串
// 回傳值：無
//-------------------------------------------------------------
void showDeviceonOled(String ss) //列印Device ID於OLED上
{
    printText(0, 2, "              ") ;  // 先清空該行內容
    printText(0, 2, ss) ;               // 將設備 ID 顯示在 OLED 上
    Serial.print("Title on OLED:(") ;   // 同時輸出到序列埠
    Serial.print(ss) ;
    Serial.print(")\n") ;     
}

//-------------------------------------------------------------
// 函式名稱：showIPonOled
// 功能說明：在 OLED 螢幕上顯示 IP 位址
// 輸入參數：ss - 要顯示的 IP 位址字串（如 "192.168.1.100"）
// 回傳值：無
//-------------------------------------------------------------
void showIPonOled(String ss) //列印IP Address於OLED上
{
    printText(0, 0, "              ") ;  // 先清空該行內容
    printText(0, 0, ss) ;               // 將 IP 位址顯示在 OLED 上
    Serial.print("IP Address on OLED:(") ; // 輸出到序列埠
    Serial.print(ss) ;
    Serial.print(")\n") ;     
}

//-------------------------------------------------------------
// 函式名稱：showRelayNOonOled
// 功能說明：在 OLED 螢幕上顯示繼電器編號
// 輸入參數：ss - 要顯示的繼電器編號（整數）
// 回傳值：無
//-------------------------------------------------------------
void showRelayNOonOled(int ss) //列印繼電器號碼於OLED上
{
    printText(0, 4, "              ") ;       // 先清空該行內容
    printText(0, 4, String(ss)) ;            // 將繼電器編號轉為字串並顯示
    Serial.print("Relay Number on OLED:(") ; // 輸出到序列埠
    Serial.print(ss) ;
    Serial.print(")\n") ;     
}

//-------------------------------------------------------------
// 函式名稱：showCommandonOled
// 功能說明：在 OLED 螢幕上顯示接收到的控制指令
// 輸入參數：ss - 要顯示的指令字串（如 "ON" 或 "OFF"）
// 回傳值：無
//-------------------------------------------------------------
void showCommandonOled(String ss)  //列印命令於OLED上-
{
    printText(30, 4, "         ") ;  // 先清空該行內容（從第 30 像素開始）
    printText(30, 4, ss) ;          // 將指令顯示在 OLED 上
    Serial.print("Command on OLED:(") ; // 輸出到序列埠
    Serial.print(ss) ;
    Serial.print(")\n") ;     
}