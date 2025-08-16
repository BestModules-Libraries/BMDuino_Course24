#include <ArduinoJson.h>  // Json 使用元件
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

//-----宣告MQTT Broker伺服器登錄資料----- 
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   
#define USERNAME  ""                                        
#define PASSWORD  ""                         
#define MQTT_HOST "broker.emqx.io"               
#define SERVER_PORT 1883

//------全域變數區--------------------------
String MacData ;//網路卡編號
// float TValue, HValue; //用溫溼度
float TValue ;  //溫度值全域變數
float HValue ;  //濕度值全域變數

//------格式化字串變數區--------------------------
//Publish & Subscribe use
const char* PubTop = "/arduino/dht/%s" ;
const char* SubTop = "/arduino/dht/%s" ;
//const char* PrePayload = "{\"Device\":\"%s\",\"Temperature\":%4.3f,\"Humidity\":%4.1f}" ;
String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];
String PayloadT;  // 用來儲存轉換doc json物件內容的字串內容，
char Payloadbuffer[250];  //用來發送mqtt broker內容payload真實內容
char clintid[20]; //用來儲存產生的MQTT Broker Client ID內容的字元陣列
//---------------------
//----------自定義函式區宣告--------------
void fillCID(String mm); //產生動態MQTT Broker ClientID
void fillTopic(String mm); //填入對應MAC Address的tpoic
void insertBeforeChar(char *str, char target, char toInsert) ;
void fillPayload(String dev, float d1, float d2) ; //根據MAC Address & Temperature & Humidity 填入payload
void initMQTT(); //init MQTT Server
void MQTTPublish();  //將payload內容PayloadT，發佈到topic:PubTopicbuffer變數

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


//--------產生動態MQTT Broker ClientID
void fillCID(String mm) //產生動態MQTT Broker ClientID
{
    // generate a random clientid based MAC
  //compose clientid with "tw"+MAC 
  clintid[0]= 't' ;  
  clintid[1]= 'w' ;  
      mm.toCharArray(&clintid[2],mm.length()+1) ;
    clintid[2+mm.length()+1] = '\n' ;
    Serial.print("Client ID:(") ;
    Serial.print(clintid) ;
    Serial.print(") \n") ;
}

//------填入對應MAC Address的tpoic
void fillTopic(String mm) //填入對應MAC Address的tpoic
{//填入對應MAC Address的tpoic
  sprintf(PubTopicbuffer,PubTop,mm.c_str()) ;
      Serial.print("Publish Topic Name:(") ;
    Serial.print(PubTopicbuffer) ;
    Serial.print("/") ;
     Serial.print(sizeof(PubTopicbuffer)) ;   
    Serial.print("^") ;
    Serial.print(String(PubTopicbuffer)) ;
    Serial.print("/") ;
    Serial.print(sizeof(String(PubTopicbuffer))) ;
    Serial.print(") \n") ;
  sprintf(SubTopicbuffer,SubTop,mm.c_str()) ;
      Serial.print("Subscribe Topic Name:(") ;
    Serial.print(SubTopicbuffer) ;
    Serial.print(") \n")  ;
}

//-----處理字串"雙引號中在AT Command需轉成/"斜線雙引號需求
void insertBeforeChar(char *str, char target, char toInsert) 
{
  int len=strlen(str);
  uint8_t count=0;
    for (int j=0; j<len+count; j++) {

      if(str[j]==target)
      {
        count++;
        for(int i = len+count;i>=j; i--) {// 向后移动字符腾出空间
          str[i+1] = str[i];
        }
        str[j] = toInsert;// 插入新字符
        j++;
      }
  }
}
void fillPayload(String dev, float d1, float d2)  //根據MAC Address & Temperature & Humidity 填入payload
{//根據MAC Address & Temperature & Humidity 填入payload
    Serial.println("Fill Pay LOAD is Processing") ;
    // sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; 
    doc["Device"]=dev ;
    doc["Temperature"]=d1 ;
    doc["Humidity"]=d2 ;
    serializeJson(doc,PayloadT) ; //將doc的json物件，轉成字串PayloadT 的內容

    // insertBeforeChar(Payloadbuffer,'\"','\\');
    // insertBeforeChar(Payloadbuffer,',','\\');
    PayloadT.toCharArray(Payloadbuffer, PayloadT.length()+1);
    insertBeforeChar(Payloadbuffer,'\"','\\');
    insertBeforeChar(Payloadbuffer,',','\\');
    PayloadT = String(Payloadbuffer);
    Serial.print("Sending:(") ;
    Serial.print(PayloadT) ;
   Serial.print(")\n") ;
    
    // Serial.print("Payload Content:(") ;
    // Serial.print(PayloadT) ;
    // Serial.print(") \n") ;
}

//----初始化init MQTT Server
void initMQTT() //初始化init MQTT Server
{
    fillCID(MacData) ;
    fillTopic(MacData) ;
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
    if (Wifi.configMqtt(String(clintid),USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT) != 0)
      {//----判斷是否連上MQTT Broker伺服器
            Serial.println("Connect to MQTT Server Successful") ;
     }
      else 
      {
        //----沒辦法連上MQTT Broker伺服器-----------
          Serial.println("Connect to MQTT Server failed") ;
           while(1);  //永久迴圈，無法離開，代表終止程式
      }
      // -----處理訂閱主題問題-----
      Wifi.setSubscribetopic(String(SubTopicbuffer));
    //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
    //mq.tongxinmao.com:18832
    // mqttclient.setCallback(callback);
    // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
  }

void MQTTPublish()  //將payload內容PayloadT，發佈到topic:PubTopicbuffer變數
{//將payload內容PayloadT，發佈到topic:PubTopicbuffer變數
  fillPayload(MacData.c_str(),TValue,HValue) ;//轉換資料為payload資料
    Serial.print("Now payload:(") ; //印出資訊
    Serial.print(PubTopicbuffer) ; //印出payload資料資訊
    Serial.print("==>") ; //印出資訊
    Serial.print(PayloadT) ;   //印出payload資料資訊
    Serial.print(")\n") ; //印出資訊

    if(Wifi.writeString(PayloadT,String(PubTopicbuffer)))
      {//將payload內容publish到PubTopicbuffer主題上
        Serial.println("Send String data sucess");//印出成功傳送
        //showStatusonOled("MQTT OK") ; //印出MQTT OK
        delay(1000);  //延遲一秒鐘
      }
      else
      {
       //  showStatusonOled("MQTT Fail") ;  //印出傳送師失敗
      }
}

