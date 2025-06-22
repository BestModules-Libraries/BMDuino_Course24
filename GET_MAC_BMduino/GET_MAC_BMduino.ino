/*****************************************************************
File:         TCP.ino
Description: 1. Hardware serial port 1 is used for communication with BMC81M001, with a communication rate of 115200bps
             2. The serial port is used as a monitoring serial port to print and output some status of BMC81M001
             3. Automatically complete BMC81M001 networking action when powered on
             4. When serial port sends data, its data is directly transmitted through to TCP server
******************************************************************/
#include "TCP.h"
BMC81M001 Wifi(&Serial1);//Please uncomment out this line of code if you use HW Serial1 on BMduino
String response = "";
void initWiFi() ;
String GetMAC() ;



void setup() 
{
  digitalWrite(LED, LOW);
  Serial.begin(9600);           
  initWiFi() ;
  Serial.println("") ;
  delay(2000) ;
  Serial.println(Wifi.sendATCommand("AT", 1000, 2));
   Serial.println( Wifi.sendATCommand("AT+CWMODE=1", 1000, 2));
    Serial.println(Wifi.sendATCommand("AT+CIPSTAMAC?", 1000, 2));

  // Wifi.sendCommand("AT", 1000);                    // 測試連線
  // Wifi.sendCommand("AT+CWMODE=1", 1000);           // 設定為 STA 模式
  // Wifi.sendCommand("AT+CIPSTAMAC?", 1000);         // 取得 MAC Address
  //Wifi.sendATCommand()
  // if(!Wifi.connectTCP(IP,IP_Port))
  // {
  //   Serial.print("IP fail");
  // } 
  // else
  // {Serial.print("IP success"); }
}

void loop() {
    tcpBuff=Wifi.readDataTcp();
    if(tcpBuff!=0) 
    {
      Serial.println(tcpBuff);
    }
    while (Serial.available() > 0)
    {
      SerialBuff[resLen++] = Serial.read();
      delay(10);
    }
    if(resLen>0)
    {
      digitalWrite(LED, HIGH);
      if(Wifi.writeDataTcp(resLen,SerialBuff))    
      {
        Serial.println("Send  data sucess");
        digitalWrite(LED,LOW);
      }
      clearBuff();
    }
}
void clearBuff(){
  memset(SerialBuff,'\0',RES_MAX_LENGTH);
  resLen = 0;
}
void initWiFi()
{
   Wifi.begin();
  Serial.print("TCP Connect Result："); 
  if(!Wifi.connectToAP(WIFI_SSID,WIFI_PASS)) 
  {
    Serial.print("WIFI fail,");
  }  
  else
  {
    Serial.print("WIFI success,"); 
    }

}

String GetMAC()
{


}