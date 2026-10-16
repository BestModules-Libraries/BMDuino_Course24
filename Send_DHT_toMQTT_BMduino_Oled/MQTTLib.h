#include <ArduinoJson.h>  // Json 使用元件
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

 
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   
#define USERNAME  ""                                        
#define PASSWORD  ""                         
#define MQTT_HOST "broker.emqx.io"               
#define SERVER_PORT 1883

String MacData ;//網路卡編號
// float TValue, HValue; //用溫溼度
float TValue ;  //溫度值全域變數
float HValue ;  //濕度值全域變數

//Publish & Subscribe use
const char* PubTop = "/arduino/dht/%s" ;
const char* SubTop = "/arduino/dht/%s" ;
const char* PrePayload = "{\"Device\":\"%s\",\"Temperature\":%4.3f,\"Humidity\":%4.1f}" ;
String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];
String PayloadT;
char Payloadbuffer[250];
char clintid[20];


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

void fillTopic(String mm)
{
  sprintf(PubTopicbuffer,PubTop,mm.c_str()) ;
      Serial.print("Publish Topic Name:(") ;
    Serial.print(PubTopicbuffer) ;
    Serial.print(") \n") ;
  sprintf(SubTopicbuffer,SubTop,mm.c_str()) ;
      Serial.print("Subscribe Topic Name:(") ;
    Serial.print(SubTopicbuffer) ;
    Serial.print(") \n")  ;
}

void insertBeforeChar(char *str, char target, char toInsert) {
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
void fillPayload(String dev, float d1, float d2)
{
    Serial.println("Fill Pay LOAD is Processing") ;
     // sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; 
    doc["Device"]=dev ;
    doc["Temperature"]=d1 ;
    doc["Humidity"]=d2 ;
 
    insertBeforeChar(Payloadbuffer,'\"','\\');
    insertBeforeChar(Payloadbuffer,',','\\');
   Serial.print("Sending:(") ;
    Serial.print(PrePayload) ;
    Serial.print("/") ;
    Serial.print(Payloadbuffer) ;
   Serial.print(")\n") ;
    
    Serial.print("Payload Content:(") ;
    Serial.print(Payloadbuffer) ;
    Serial.print(") \n") ;
}


void initMQTT() //init MQTT Server
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

    if (Wifi.configMqtt(CLIENTLID,USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT) != 0)
      {

            Serial.println("Connect to MQTT Server Successful") ;

      }
      else {

          Serial.println("Connect to MQTT Server failed") ;

           while(1);
      }
     // Wifi.setPublishTopic(String(PubTopicbuffer));
    Wifi.setSubscribetopic(String(SubTopicbuffer));

  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //mq.tongxinmao.com:18832
  // mqttclient.setCallback(callback);
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    

}

void MQTTPublish()
{
  
  fillPayload(MacData.c_str(),TValue,HValue) ;
    Serial.print("Now payload:(") ;
    Serial.print(PubTopicbuffer) ;
    Serial.print("==>") ;
    Serial.print(Payloadbuffer) ;
    Serial.print(")\n") ;

    if(Wifi.writeString(String(Payloadbuffer),String(PubTopicbuffer)))
      {
        Serial.println("Send String data sucess");
        delay(1000);
      }

}