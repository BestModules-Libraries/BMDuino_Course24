#include <ArduinoJson.h>  // Json 使用元件
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

String MacData ="AABBCC123456";//網路卡編號
String PayloadT;

//---------------------
void initMQTT(); //init MQTT Server

//--------------------------------


void fillPayload(String dev, int d1, String cc)  //根據MAC Address & Temperature & Humidity 填入payload
{//根據MAC Address & Temperature & Humidity 填入payload
    Serial.println("Fill Pay LOAD is Processing") ;
    // sprintf(Payloadbuffer,PrePayload,dev.c_str(),d1,d2) ; 
    doc["Device"]=dev ;
    doc["RelayNumber"]=d1 ;
    doc["Command"]=cc ;
    serializeJson(doc,PayloadT) ;
    Serial.print("JSON:(") ;
    Serial.print(PayloadT) ;
    // Serial.print("/") ;
    // Serial.print(Payloadbuffer) ;
   Serial.print(")\n") ;
    
    // Serial.print("Payload Content:(") ;
    // Serial.print(PayloadT) ;
    // Serial.print(") \n") ;
}


void initMQTT() //init MQTT Server
{
}

