#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來建立與處理 JSON 格式資料
StaticJsonDocument<200> doc; // 宣告一個容量為 200 bytes 的靜態 JSON 文件物件，用於存放結構化的 JSON 資料

//-------------------- MQTT Broker 相關參數定義 --------------------
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   // MQTT Client 的預設 ID（可被動態覆蓋）
#define USERNAME  ""                              // MQTT 登入帳號（此處為空字串，代表匿名）
#define PASSWORD  ""                              // MQTT 登入密碼（此處為空字串，代表匿名）
#define MQTT_HOST "broker.emqx.io"                // MQTT Broker 主機位址
#define SERVER_PORT 1883                          // MQTT Broker 通訊埠（預設 1883 為非加密連線）

//-------------------- MQTT 主題 (Topic) 定義 --------------------
// 使用者裝置會根據 MAC 位址產生對應的 topic
const char* PubTop = "/arduino/SW/%s" ;   // 發佈訊息用的主題格式字串
const char* SubTop = "/arduino/SW/%s" ;   // 訂閱訊息用的主題格式字串
//const char* PrePayload = "{\"Device\":\"%s\",\"Temperature\":%4.3f,\"Humidity\":%4.1f}" ; // 範例 Payload 格式（已註解掉）

//-------------------- 全域變數宣告 --------------------
String TopicT;                // 儲存 Topic 名稱的暫存字串
char SubTopicbuffer[200];     // 訂閱用主題緩衝區
char PubTopicbuffer[200];     // 發佈用主題緩衝區
String PayloadT;              // JSON Payload 的字串版本
char Payloadbuffer[250];      // JSON Payload 的字元陣列版本（方便傳輸）
char clintid[20];             // MQTT Client ID 暫存陣列
String MacData ;              // 裝置的 MAC Address（網卡編號，用於辨識裝置）

//-------------------- 函式前置宣告 --------------------
void printText(int x,int y, uint8_t *str) ;       // 在 OLED 指定位置印出文字
void fillCID(String mm);                          // 產生動態 MQTT Client ID（根據 MAC Address）
void fillTopic(String mm);                        // 根據 MAC Address 填入對應的 Topic
void insertBeforeChar(char *str, char target, char toInsert) ; // 在字串指定字元前插入新字元
void fillPayload(String dev, int d1, String cc) ; // 建立 JSON Payload（裝置 ID、繼電器編號、命令）
void initMQTT();                                  // 初始化並連線到 MQTT Broker
void MQTTPublish();                               // 發佈 Payload 到指定 Topic
void showStatusonOled(String ss);                 // 顯示系統狀態於 OLED 螢幕

//----------------------------------------------------
// 產生動態 MQTT Broker Client ID
//----------------------------------------------------
void fillCID(String mm)
{
    // Client ID 格式為 "tw"+MAC
    clintid[0]= 't' ;  
    clintid[1]= 'w' ;  
    mm.toCharArray(&clintid[2], mm.length()+1) ; // 將 MAC 位址字串轉為字元陣列，放到 Client ID 後面
    clintid[2+mm.length()+1] = '\n' ;            // 加上換行符號（結尾符號）
    
    // 序列埠輸出 Client ID 以供除錯
    Serial.print("Client ID:(") ;
    Serial.print(clintid) ;
    Serial.print(") \n") ;
}

//----------------------------------------------------
// 根據 MAC Address 填入對應的 MQTT Topic
//----------------------------------------------------
void fillTopic(String mm)
{
  // sprintf() 會根據 PubTop 格式字串，將 mm 代入後輸出到 PubTopicbuffer
  sprintf(PubTopicbuffer, PubTop, mm.c_str()) ;
  Serial.print("Publish Topic Name:(") ;
  Serial.print(PubTopicbuffer) ;
  Serial.print(") \n") ;

  // 同樣方法設定訂閱用 Topic
  sprintf(SubTopicbuffer, SubTop, mm.c_str()) ;
  Serial.print("Subscribe Topic Name:(") ;
  Serial.print(SubTopicbuffer) ;
  Serial.print(") \n")  ;
}

//----------------------------------------------------
// 在指定字元前插入一個新字元（處理 JSON 特殊符號轉義）
//----------------------------------------------------
void insertBeforeChar(char *str, char target, char toInsert) 
{
  int len = strlen(str);    // 計算字串長度
  uint8_t count = 0;        // 插入次數計數
  for (int j=0; j<len+count; j++) {
    if (str[j]==target) {   // 找到目標字元
      count++;
      // 從後往前移動字元，騰出空間插入新字元
      for(int i = len+count; i>=j; i--) {
        str[i+1] = str[i];
      }
      str[j] = toInsert; // 插入新字元
      j++;
    }
  }
}

//----------------------------------------------------
// 建立 JSON Payload（根據裝置 MAC、繼電器編號、命令字串）
//----------------------------------------------------
void fillPayload(String dev, int d1, String cc)  
{
    Serial.println("Fill Pay LOAD is Processing") ;

    // 建立 JSON 物件
    doc["Device"] = dev ;   // 裝置編號（MAC）
    doc["RelayNumber"] = d1 ; // 繼電器編號
    doc["Command"] = cc ;     // 指令（ON/OFF）
    
    // 將 JSON 轉為字串存放到 PayloadT
    serializeJson(doc, PayloadT) ;
    Serial.print("JSON:(") ;
    Serial.print(PayloadT) ;
    Serial.print(")\n") ;
    
    // 將字串轉成字元陣列（方便傳送）
    PayloadT.toCharArray(Payloadbuffer, PayloadT.length()+1);

    // 處理 JSON 特殊字元（加入轉義字元 \）
    insertBeforeChar(Payloadbuffer,'\"','\\');
    insertBeforeChar(Payloadbuffer,',','\\');

    // 更新 PayloadT 為處理後的版本
    PayloadT = String(Payloadbuffer);

    // 輸出最終要送出的 JSON Payload
    Serial.print("Sending:(") ;
    Serial.print(PayloadT) ;
    Serial.print(")\n") ;
}

//----------------------------------------------------
// 初始化並連線到 MQTT Broker
//----------------------------------------------------
void initMQTT()
{
    fillCID(MacData) ;   // 產生 Client ID
    fillTopic(MacData) ; // 填入對應的 Publish / Subscribe Topic
    
    // 輸出連線參數到序列埠監控
    Serial.print("clintid:(") ; Serial.print(clintid) ; Serial.print(")\n") ;
    Serial.print("USERNAME:(") ; Serial.print(USERNAME) ; Serial.print(")\n") ;
    Serial.print("PASSWORD:(") ; Serial.print(PASSWORD) ; Serial.print(")\n") ;
    Serial.print("MQTT_HOST:(") ; Serial.print(MQTT_HOST) ; Serial.print(")\n") ;
    Serial.print("SERVER_PORT:(") ; Serial.print(SERVER_PORT) ; Serial.print(")\n") ;
    Serial.println("Now Connect MQTT Broker.....") ;

    // 嘗試連線到 MQTT Broker
    if (Wifi.configMqtt(String(clintid),USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT) != 0) {
        Serial.println("Connect to MQTT Server Successful") ;
    }
    else {
        Serial.println("Connect to MQTT Server failed") ;
        while(1); // 無限迴圈卡住，表示無法繼續
    }

    // 設定要訂閱的主題
    Wifi.setSubscribetopic(String(SubTopicbuffer));
}

//----------------------------------------------------
// 發佈 Payload 到 MQTT Topic
//----------------------------------------------------
void MQTTPublish()  
{
    Serial.print("Now payload:(") ;
    Serial.print(PubTopicbuffer) ; // 發佈的 Topic
    Serial.print("==>") ;
    Serial.print(PayloadT) ;       // 發佈的內容
    Serial.print(")\n") ;

    // 嘗試發送資料
    if(Wifi.writeString(PayloadT, String(PubTopicbuffer))) {
        Serial.println("Send String data sucess");
        showStatusonOled("MQTT OK") ; // 顯示成功訊息
        delay(1000);
    }
    else {
        showStatusonOled("MQTT Fail") ; // 顯示失敗訊息
    }
}

//----------------------------------------------------
// 在 OLED 螢幕上顯示系統狀態
//----------------------------------------------------
void showStatusonOled(String ss)  
{
   printText(0,6,"         ")  ;  // 先清空原有文字
   printText(0,6,ss)  ;            // 再印出新的狀態文字

   // 同步輸出到序列埠監控
   Serial.print("Status:(") ;
   Serial.print(ss) ;
   Serial.print(")\n") ;     
}
