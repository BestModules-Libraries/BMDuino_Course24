#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來處理 JSON 格式的資料
StaticJsonDocument<200> doc; // 建立一個 200 bytes 的靜態 JSON 文件物件，用來存放序列化與反序列化的 JSON 資料

//----- 宣告 MQTT Broker 伺服器連線設定 -----
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   // 預設 MQTT Client ID
#define USERNAME  ""                              // 預設帳號（此範例未使用）
#define PASSWORD  ""                              // 預設密碼（此範例未使用）
#define MQTT_HOST "broker.emqx.io"                // MQTT Broker 主機位置（使用 EMQX 公開伺服器）
#define SERVER_PORT 1883                          // MQTT Broker 通訊埠（1883 為非加密通訊的預設埠）

//------ 全域變數區 --------------------------
String MacData ;      // 儲存網路卡的 MAC Address
float TValue ;        // 儲存感測器溫度值
float HValue ;        // 儲存感測器濕度值

//------ 格式化字串變數區 ---------------------
// 訂閱與發布主題 (Topic) 格式，%s 代表替換為裝置 MAC Address
const char* PubTop = "/arduino/dht/%s" ;  
const char* SubTop = "/arduino/dht/%s" ;  
//const char* PrePayload = "{\"Device\":\"%s\",\"Temperature\":%4.3f,\"Humidity\":%4.1f}" ;

String TopicT;                 // 用來暫存組合後的 Topic
char SubTopicbuffer[200];      // 儲存訂閱主題字串的緩衝區
char PubTopicbuffer[200];      // 儲存發布主題字串的緩衝區
String PayloadT;               // 用來儲存 JSON 格式的 Payload 字串
char Payloadbuffer[250];       // 發佈到 MQTT Broker 的實際 Payload 資料
char clintid[20];              // 儲存 MQTT Client ID 的字元陣列

//---------- 自定義函式宣告區 ----------------
void fillCID(String mm); // 依據 MAC Address 產生 MQTT Broker ClientID
void fillTopic(String mm); // 依據 MAC Address 產生訂閱與發布的 Topic
void insertBeforeChar(char *str, char target, char toInsert); // 在指定字元前插入新字元
void fillPayload(String dev, float d1, float d2); // 根據裝置資訊、溫度與濕度建立 Payload
void initMQTT();   // 初始化並連線至 MQTT Server
void MQTTPublish();  // 將資料封裝成 Payload 並發佈到 MQTT Broker

//----------自定義函式主體區--------------
/*
{
"Device":"48E729732158",
"Temperature":26,
"Humidity":61
}
--必須轉成char與逃脫字串\"
{
\"Device\":\"48E729732158\",
\"Temperature\":26,
\"Humidity\":61
}
*/
//-------- 產生動態 MQTT Broker ClientID ----------
void fillCID(String mm) 
{
    // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  
  // Client ID 格式：「tw + MAC Address」
  clintid[0] = 't';  
  clintid[1] = 'w';  
  mm.toCharArray(&clintid[2], mm.length()+1); 
  clintid[2+mm.length()+1] = '\n'; // 字串結尾加換行符號

  Serial.print("Client ID:(");
  Serial.print(clintid);
  Serial.print(") \n");
}

//------ 產生訂閱與發佈的 Topic ----------
void fillTopic(String mm) 	//填入對應MAC Address的tpoic
{
  // 使用 sprintf() 將 MAC Address 填入 PubTop 與 SubTop
  sprintf(PubTopicbuffer, PubTop, mm.c_str());  
  Serial.print("Publish Topic Name:(");
  Serial.print(PubTopicbuffer);
  Serial.print(") \n");

  sprintf(SubTopicbuffer, SubTop, mm.c_str());
  Serial.print("Subscribe Topic Name:(");
  Serial.print(SubTopicbuffer);
  Serial.print(") \n");
}

//----- 在字串內特定字元前插入新字元（主要用來處理「\"」需求）-----
void insertBeforeChar(char *str, char target, char toInsert) 
{
//-----處理字串"雙引號中在AT Command需轉成/"斜線雙引號需求
  int len = strlen(str);
  uint8_t count = 0;

  for (int j = 0; j < len + count; j++) 
  {
    if (str[j] == target) {
      count++;
      // 將字元向後移動，空出位置
      for (int i = len+count; i >= j; i--)	// 向后移动字符腾出空间
	  {
        str[i+1] = str[i];
      }
      str[j] = toInsert; // 插入新字元
      j++; // 跳過已插入字元
    }
  }
}

//----- 建立 MQTT Payload 資料 ----------
void fillPayload(String dev, float d1, float d2)  
{
//根據MAC Address & Temperature & Humidity 填入payload
  Serial.println("Fill Pay LOAD is Processing");
// sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; 

  // 建立 JSON 內容
  doc["Device"] = dev;
  doc["Temperature"] = d1;
  doc["Humidity"] = d2;

  // 將 JSON 文件序列化成字串
  serializeJson(doc, PayloadT);
	//將doc的json物件，轉成字串PayloadT 的內容
	
  // 轉成字元陣列，方便進一步處理字串
  PayloadT.toCharArray(Payloadbuffer, PayloadT.length()+1);

  // 處理字串中的引號與逗號
  insertBeforeChar(Payloadbuffer,'\"','\\');
  insertBeforeChar(Payloadbuffer,',','\\');

  // 更新 PayloadT 為處理後的字串
  PayloadT = String(Payloadbuffer);

  Serial.print("Sending:(");
  Serial.print(PayloadT);
  Serial.print(")\n");
      // Serial.print("Payload Content:(") ;
    // Serial.print(PayloadT) ;
    // Serial.print(") \n") ;
}

//----- 初始化 MQTT Server 並嘗試連線 ----------
void initMQTT() 
{
  fillCID(MacData);   // 使用 MacData  變數產生 Client ID
  fillTopic(MacData); // 使用 MacData  變數 產生TPIC

  Serial.print("MQTT_HOST:(");	//印出資訊
  Serial.print(MQTT_HOST);	//印出 MQTT_HOST 變數
  Serial.print(")\n");	//印出資訊

  Serial.print("SERVER_PORT:(");	//印出資訊
  Serial.print(SERVER_PORT);	//印出 SERVER_PORT 變數
  Serial.print(")\n");	//印出資訊

  Serial.println("Now Connect MQTT Broker.....");//印出Now Connect MQTT Broker.....

  // 嘗試與 MQTT Broker 建立連線
  if (Wifi.configMqtt(String(clintid), USERNAME, PASSWORD, MQTT_HOST, SERVER_PORT) != 0) 
  {
  //----判斷是否連上MQTT Broker伺服器
    Serial.println("Connect to MQTT Server Successful"); 
  }
  else 
  {
  //----沒辦法連上MQTT Broker伺服器-----------
    Serial.println("Connect to MQTT Server failed");
    while(1);  // 無限迴圈：無法連線時停在此
  }

  // 設定訂閱主題
  Wifi.setSubscribetopic(String(SubTopicbuffer));
    //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
    //mq.tongxinmao.com:18832
    // mqttclient.setCallback(callback);
    // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
  
}

//----- 發佈資料到 MQTT Broker ----------
void MQTTPublish()  	//將payload內容PayloadT，發佈到topic:PubTopicbuffer變數
{
//將payload內容PayloadT，發佈到topic:PubTopicbuffer變數
  // 建立 Payload（包含裝置編號、溫度、濕度）
  fillPayload(MacData.c_str(), TValue, HValue);	//轉換資料為payload資料

  Serial.print("Now payload:(");	//印出資訊
  Serial.print(PubTopicbuffer); //印出payload資料資訊
  Serial.print("==>");		//印出資訊
  Serial.print(PayloadT);        //印出payload資料資訊
  Serial.print(")\n");	 //印出資訊
	
  // 發佈 Payload
  if (Wifi.writeString(PayloadT, String(PubTopicbuffer))) 
  {
  //將payload內容publish到PubTopicbuffer主題上
    Serial.println("Send String data success"); 
	//showStatusonOled("MQTT OK") ; //印出MQTT OK
    delay(1000);  // 延遲 1 秒，避免過快發送
  } else {
    Serial.println("Send String data failed"); 	//印出傳送失敗
  }
}
