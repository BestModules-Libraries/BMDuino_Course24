#ifndef _BMC81M001_H__
#define _BMC81M001_H__

//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "BMC81M001.h"

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

/*** 链接的wifi的账户密码*/
#define WIFI_SSID "NUKIOT"//热点名称
#define WIFI_PASS "iot12345"//热点密码

/*** 链接的TCP IP及端口信息*/
#define IP "203.66.34.45"  //TCP服务器 IP地址
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

//BMC81M001 Wifi(6,7);// 使用軟體序列（rxPin 6 , txPin 7），若不使用軟體序列請註解掉這行
BMC81M001 Wifi(&Serial); // 使用硬體序列0（HW Serial），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial1); // 使用硬體序列1（HW Serial1），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial2); // 使用硬體序列2（HW Serial2），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial3); // 使用硬體序列3（HW Serial3），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial4); // 使用硬體序列4（HW Serial4），若使用 BMduino 的硬體序列，請取消這行的註解
