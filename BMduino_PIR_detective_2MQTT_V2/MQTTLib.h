//----------外部引用函式區----------------

#include <ArduinoJson.h>  // Json 使用元件

//------------全域變數區----------------
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

//-----宣告MQTT Broker伺服器登錄資料----- 
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   
#define USERNAME  ""                                        
#define PASSWORD  ""                         
#define MQTT_HOST "broker.emqx.io"               
#define SERVER_PORT 1883

//------格式化字串變數區--------------------------
//Publish & Subscribe use
// String topic ;                        //MQTT_Topic
String topic="/arduino/PIR/%s" ;
const char* PubTop = "/arduino/PIR/%s" ;
const char* SubTop = "/arduino/PIR/#" ;

String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];
String PayloadT;
char Payloadbuffer[250];
char clintid[20];
//---------------------
//----------自定義函式主體區--------------
#define DEB_CNT     80                //50ms
#define RES_MAX_LENGTH 200            //max buffer length
String ReciveBuff;
int ReciveBufflen;

//String DATA_BUF ;  

//----------自定義函式區宣告--------------
void fillCID(String mm); //產生動態MQTT Broker ClientID
void fillTopic(String mm); //填入對應MAC Address的tpoic
void insertBeforeChar(char *str, char target, char toInsert) ;
void fillPayload(String dev, String s1, String sd2);  //根據MAC Address & s1 & s2 填入payloadvoid initMQTT(); //init MQTT Server
void MQTTPublish();  //將payload內容PayloadT，發佈到topic:PubTopicbuffer變數


// ------- 自定義函式內容程式區 -----------
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

//------填入對應MAC Address的Topic
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
    Serial.print("/")  ;
    Serial.print(SubTop) ;
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

//根據MAC Address & s1 & s2 填入payload
void fillPayload(String dev, String s1, String s2)  //根據MAC Address & s1 & s2 填入payload
{//根據MAC Address & Temperature & Humidity 填入payload
    Serial.println("Fill Pay LOAD is Processing") ;
    // sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; 
    doc["Device"]=dev ;
    doc["Location"]=s1 ;
    doc["Status"]=s2 ;
    serializeJson(doc,PayloadT) ;
    PayloadT.toCharArray(Payloadbuffer, PayloadT.length()+1);
    insertBeforeChar(Payloadbuffer,'\"','\\');
    insertBeforeChar(Payloadbuffer,',','\\');
    PayloadT = String(Payloadbuffer);
    Serial.print("Sending:(") ;
    Serial.print(PayloadT) ;
   Serial.print(")\n") ;
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
      Wifi.setSubscribetopic(SubTop);
    //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
    //mq.tongxinmao.com:18832
    // mqttclient.setCallback(callback);
    // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
  }

void MQTTPublish()  //將payload內容PayloadT，發佈到topic:PubTopicbuffer變數
{//將payload內容PayloadT，發佈到topic:PubTopicbuffer變數
  fillPayload(MacData.c_str(),PirLocation,"Active") ;//轉換資料為payload資料
    Serial.print("Now payload:(") ; //印出資訊
    Serial.print(PubTopicbuffer) ; //印出payload資料資訊
    Serial.print("==>") ; //印出資訊
    Serial.print(PayloadT) ;   //印出payload資料資訊
    Serial.print(")\n") ; //印出資訊

    if(Wifi.writeString(PayloadT,String(PubTopicbuffer)))
      {//將payload內容publish到PubTopicbuffer主題上
        Serial.println("Send String data sucess");//印出成功傳送
        // showStatusonOled("MQTT OK") ; //印出MQTT OK
        delay(1000);  //延遲一秒鐘
      }
      else
      {
         //showStatusonOled("MQTT Fail") ;  //印出傳送師失敗
      }
}
