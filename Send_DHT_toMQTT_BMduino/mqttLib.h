#include <ArduinoJson.h>  // Json 使用元件
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

#define CLIENTLID  "LiwJDBccDxULGxUwDiANEjs"   
#define USERNAME  ""                                        
#define PASSWORD  ""                         
#define MQTT_HOST "broker.emqx.io"               
#define SERVER_PORT 1883

String MacData ;//網路卡編號
float Tvalue=55, Hvalue=66; //用溫溼度


//Publish & Subscribe use
const char* PubTop = "/arduino/dht/%s" ;
const char* SubTop = "/arduino/dht/%s" ;
String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];
const char* PrePayload = "{\"Device\":\"%s\",\"Temperature\":%4.3f,\"Humidity\":%4.1f}" ;
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

void fillPayload(String dev, double d1, double d2)
{
  sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; ;
      Serial.print("Payload Content:(") ;
    Serial.print(Payloadbuffer) ;
    Serial.print(") \n") ;
}


void initMQTT() //init MQTT Server
{
    fillCID(MacData) ;
    fillTopic(MacData) ;
    Wifi.setPublishTopic(String(PubTopicbuffer));
    Wifi.setSubscribetopic(String(SubTopicbuffer));

    if (Wifi.configMqtt(String(clintid),USERNAME,PASSWORD,MQTT_HOST,SERVER_PORT) != 0)
      {

            Serial.println("Connect to MQTT Server Successful") ;

      }

  //連接MQTT Server ， Servar name :MQTTServer， Server Port :MQTTPort
  //mq.tongxinmao.com:18832
  // mqttclient.setCallback(callback);
  // 設定MQTT Server ， 有subscribed的topic有訊息時，通知的函數
      
//--------------------------    

}

void MQTTPublish()
{
     Tvalue = random(190, 281) / 10.0;  // random(190,281) 產生 190~280 的整數，除以10.0得到小數
  // 產生濕度（60 ~ 99）的隨機值
  Hvalue = random(600, 991) / 10.0;
  fillPayload(MacData.c_str(),Tvalue,Hvalue) ;
    if(Wifi.writeString(String(Payloadbuffer),String(PubTopicbuffer)))
      {
        Serial.println("Send String data sucess");
        delay(1000);
      }

}