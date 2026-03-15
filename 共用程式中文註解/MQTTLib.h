#include <ArduinoJson.h>  // Json 使用元件
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
String topic="/arduino/SW/%s" ;
const char* PubTop = "/arduino/SW/%s" ;
const char* SubTop = "/arduino/SW/#" ;

String TopicT;
char SubTopicbuffer[200];
char PubTopicbuffer[200];
String PayloadT;
char Payloadbuffer[250];
char clintid[20];
//---------------------
//----------自定義函式區宣告--------------
void fillCID(String mm); //產生動態MQTT Broker ClientID
void fillTopic(String mm); //填入對應MAC Address的tpoic
void insertBeforeChar(char *str, char target, char toInsert) ;
//void fillPayload(String dev, float d1, float d2) ; //根據MAC Address & Temperature & Humidity 填入payload
void initMQTT(); //init MQTT Server
void showTitleonOled(String ss); //列印抬頭於OLED上
void showIPonOled(String ss); //列印IP Address於OLED上
void showRelayNOonOled(int ss); //列印繼電器號碼於OLED上
void showCommandonOled(String ss) ; //列印命令於OLED上-
void showDeviceonOled(String ss); //列印Device ID於OLED上
//----------自定義函式主體區--------------
#define DEB_CNT     80                //50ms
#define RES_MAX_LENGTH 200            //max buffer length


//int   resLen;                         //serial buffer use length
String ReciveBuff;
int ReciveBufflen;

//String DATA_BUF ;                     //

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
// void fillPayload(String dev, float d1, float d2)  //根據MAC Address & Temperature & Humidity 填入payload
// {//根據MAC Address & Temperature & Humidity 填入payload
//     Serial.println("Fill Pay LOAD is Processing") ;
//     // sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; 
//     doc["Device"]=dev ;
//     doc["Temperature"]=d1 ;
//     doc["Humidity"]=d2 ;
//     serializeJson(doc,PayloadT) ;

//     // insertBeforeChar(Payloadbuffer,'\"','\\');
//     // insertBeforeChar(Payloadbuffer,',','\\');
//     PayloadT.toCharArray(Payloadbuffer, PayloadT.length()+1);
//     insertBeforeChar(Payloadbuffer,'\"','\\');
//     insertBeforeChar(Payloadbuffer,',','\\');
//     PayloadT = String(Payloadbuffer);
//     Serial.print("Sending:(") ;
//     Serial.print(PayloadT) ;
//    Serial.print(")\n") ;
    
//     // Serial.print("Payload Content:(") ;
//     // Serial.print(PayloadT) ;
//     // Serial.print(") \n") ;
// }

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

//-------列印抬頭於OLED上---------------
void showTitleonOled(String ss) //列印抬頭於OLED上
{
      printText(0,0,"              ")  ;  //清空
    printText(0,0,ss)  ;  //轉換string字串到char array
    Serial.print("Title on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印Device ID於OLED上---------------
void showDeviceonOled(String ss) //列印Device ID於OLED上
{
      printText(0,2,"              ")  ;  //清空
    printText(0,2,ss)  ;  //轉換string字串到char array
    Serial.print("Title on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印IP Address於OLED上---------------

void showIPonOled(String ss) //列印IP Address於OLED上
{
      printText(0,0,"              ")  ;  //清空
    printText(0,0,ss)  ;  //轉換string字串到char array
    Serial.print("IP Address on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印溫度於OLED上---------------
void showRelayNOonOled(int ss) //列印繼電器號碼於OLED上
{
      printText(0,4,"              ")  ;  //清空
    printText(0,4,String(ss))  ;  //轉換string字串到char array
    Serial.print("Relay Number on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印濕度於OLED上---------------
void showCommandonOled(String ss)  //列印命令於OLED上-
{
   printText(30,4,"         ")  ;  //清空
    printText(30,4,ss)  ;  //轉換string字串到char array
    Serial.print("Command on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}


