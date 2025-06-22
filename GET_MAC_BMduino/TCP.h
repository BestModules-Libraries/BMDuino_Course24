//BMC81M001 Wifi(6,7);//rxPin 6 , txPin 7, Please comment out this line of code if you don't use software Serial
//BMC81M001 Wifi(&Serial1);//Please uncomment out this line of code if you use HW Serial1 on BMduino




#ifndef _BMC81M001_H__
#define _BMC81M001_H__

//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "TCP.h"
#include "BMC81M001.h"

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

/*** 链接的wifi的账户密码*/
#define WIFI_SSID "NUKIOT"//热点名称
#define WIFI_PASS "iot12345"//热点密码

/*** 链接的TCP IP及端口信息*/
#define IP "www.hinet.net"  //TCP服务器 IP地址
#define IP_Port 80  //TCP服务器 端口

//*********************************************//
//************* IO_Port Define ***************//
//*********************************************//
int LED = 13;                         // LED port 
//*********************************************//
//************* Variable Define ***************//
//*********************************************//
#define DEB_CNT     50                //50ms
#define RES_MAX_LENGTH 200            //max buffer length

char  SerialBuff[RES_MAX_LENGTH];   //serial buffer
char  data[30];                       //key data buffer
int   resLen;                         //serial buffer use length
int   nKeyBuf;                        // KEY data buffer
String DATA_BUF;                      //
String tcpBuff;
#endif


//BMC81M001 Wifi(&Serial2);//Please uncomment out this line of code if you use HW Serial1 on BMduino



// void sendCommand(String command, const int timeout) 
// {
//   response = "";
//   wifi.sendATCommand(command,1000, 2); // 傳送 AT 指令
//   long int time = millis();

//   while ((time + timeout) > millis()) 
//   {
//     while (wifi.available()) 
//     {
//       char c = wifi.read();
//       response += c;
//     }
//   }

//   Serial.println("指令: " + command);
//   Serial.println("回應: ");
//   Serial.println(response);
// }

