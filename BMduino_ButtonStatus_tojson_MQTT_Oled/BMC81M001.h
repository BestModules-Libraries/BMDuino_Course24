/*************************************************
File:             BMC81M001.h
Author:           BEST MODULES CORP.
Description:      Define classes and required variables 
version:          V1.0.4-2024-8-22
**************************************************/
#ifndef _BMC81M001_H_
#define _BMC81M001_H_

 
#include <Arduino.h>
#include <SoftwareSerial.h>

#define BMC81M001_baudRate 115200
#define SEND_SUCCESS 1
#define SEND_FAIL 0
#define  COMMUNICAT_ERROR -1
#define  AT_ACK_ERROR -2
//----------------------wifi status---------------------------
#define  WIFI_STATUS_GOT_IP  2
#define  WIFI_STATUS_CONNETED  3
#define  WIFI_STATUS_DISCONNETED  4
#define  WIFI_STATUS_NO_CONNET  5
//----------------------wifi status---------------------------
#define HTTP_GET_BEGIN_SUCCESS 0
#define HTTP_GET_OP_SUCCESS 0
#define HTTP_GET_URL_ERROR -1
#define HTTP_GET_OP_TIMEOUT -2


#define RES_MAX_LENGTH 2000


/*! Subclassing Print makes debugging easier -- output en route to ESP8266 module
 * can be duplicated on a second stream (e.g. Serial). !*/
class BMC81M001 
{
  public:
      BMC81M001( HardwareSerial *theSerial = &Serial);
      BMC81M001(uint16_t rxPin,uint16_t txPin);
      void begin(uint32_t baud = BMC81M001_baudRate);  
      bool connectToAP(String ssid,String pass);
      bool connectTCP(String ip,  int port);
      bool writeDataTcp(int Dlength,char Dbuffer[]);
      String readDataTcp();
      bool configMqtt(String clientid,String username,String password,String mqtt_host,int server_port);
      bool setPublishTopic(String publishtopic);
      bool setSubscribetopic(String subscribetopic);
      bool setTopic(String topic);
      bool writeString(String Dbuffer,String topic);
      bool writeBytes(char Dbuffer[],int Dlength,String topic);
      void readIotData(String *ReciveBuff,int *ReciveBufflen,String *topic);
      bool reset(void);
      int  sendATCommand(String StringstrCmd,int timeout,uint8_t reTry);
      //-------------------------------------------------------------------
      String sendATCmd(String StringstrCmd,int timeout,uint8_t reTry);
      String SSID();
      int getStatus();
      String getSSID();
      String getIP();
      String getGateway();
      String getMask();
      String getMacAddress();
      String getATVersion();
      //-------------------------------------------------------------------
      int http_begin(String serverURL,int port,String subURL="");
      int http_get(void);
      String http_getString(void);
      void http_end(void);


      char BMC81M001Response[RES_MAX_LENGTH];
      int resLength = 0;

      String  OneNetReciveBuff;
  private:
      uint16_t _rxPin;
      uint16_t _txPin;
      void readResponse(void);
      void clearResponse(char Debugbuffer[]);
      HardwareSerial *_serial = NULL;
      SoftwareSerial *_softSerial = NULL ;
      //http get------------------------
      String _host="Host: ";
      int _port;
      String _type;
      String _url;
      String _suburl;
      int _len;
      //--------------------------------
};

enum
{
  
  RST_Fail=1,
  Init_Fail,
  Station_Fail,
  WIFI_CONNECTED_Fail,
  TCP_CONNECTED_Fail,
  TCP_Senddata_Fail,

  MQTTSNTP_FAIL= 10,
  MQTTUSERCFG_Fail,
  MQTTCLIENTID_Fail,
  MQTTUSERNAME_Fail,
  MQTTPASSWORD_Fail,
  MQTTConnect_Fail,
  MQTTTopic_Fail
};

#endif // _BMC81M001_H_
