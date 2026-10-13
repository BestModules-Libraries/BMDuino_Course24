/*****************************************************************
File:         NetworkClock.ino
Description: 1. Hardware serial port 1 is used for communication with BMC81M001, with a communication rate of 115200bps
             2. The serial port is used as a monitoring serial port to print and output some status of BMC81M001
             3. Automatically complete BMC81M001 networking action when powered on
             4. When serial port sends data, its data is directly transmitted through to TCP server
******************************************************************/
#include "NetworkClock.h"
// BMC81M001 Wifi(6,7); // rxPin 6 , txPin 7，若使用軟體序列埠請取消註解此行
// BMC81M001 Wifi(&Serial1); // 若使用 BMduino 板上的硬體 Serial1，請取消註解此行
BMC81M001 Wifi(&Serial2); // 使用 BMduino 板上的硬體 Serial2 腳位控制 WiFi 模組（例如 RX2, TX2）


char SerialBuff[250];
uint8_t i;

void setup() 
{
  Serial.begin(9600);           
  Wifi.begin();
  Serial.print("TCP Connect Result："); 
  if(!Wifi.connectToAP(WIFI_SSID,WIFI_PASS)) //Connect Wifi
  {
    Serial.print("WIFI fail,");
  }  
  else
  {Serial.print("WIFI success,"); }

  Wifi.sendATCommand("AT+CIPMUX=0", 1000, 3);//Single connection
  Wifi.connectTCP("www.beijingtime.com.cn",80);   //TCP connection to web pages
  Wifi.sendATCommand("AT+CIPMODE=1", 1000, 3); //transparent mode
  Wifi.sendATCommand("AT+CIPSEND", 1000, 3); //Enter send
}





void loop() {
  i=0;
  memset(SerialBuff, '\0', 250); 
  WIFI_SERIAL.println("GET");//Apply to obtain webpage content
  delay(500);
  while (WIFI_SERIAL.available()) 
  {
    SerialBuff[i++] = WIFI_SERIAL.read();   //read data
  }
  if(strstr(SerialBuff,"HTTP") != NULL)
  {
    uint8_t index_position1,index_position2;
    String tcpBuff=String(SerialBuff);
    index_position1 =  tcpBuff.indexOf("Date");//serach for "Date"
    index_position2 =  tcpBuff.indexOf("GMT");//serach for "GMT"
    Serial.println("Current date is:");
    Serial.println(tcpBuff.substring(index_position1, index_position2));//Cut fixed position
    Serial.println();
    Serial.println();
  }
    delay(2500);
}
/*Recive data from HTTP.
>HTTP/1.1 400 Bad Request
Server: wts/1.7.0
Date: Wed, 25 Jun 2025 07:43:53 GMT
Content-Type: text/html
Content-Length: 154
Connection: close

<html>
<head><title>400 Bad Request</title></head>
<body>
<center><h1>400 Bad Request</h1></center>
<hr><center>wts/1.7.0</center>
</body>
</html>
*/
