/*************************************************
File:             BM7701-00-1.h
Author:           BEST MODULES CORP.
Description:      Header file
Version:          V1.0.3   -- 2025-01-21
**************************************************/
#ifndef _BM7701_00_1_H__
#define _BM7701_00_1_H__

#include "Arduino.h"
#include <SoftwareSerial.h>

extern uint8_t receive_data[256];

typedef struct 
{  
  u8    key;
  u8    serial;
  u8    checksum;
} KEY_MESSAGE;

#define EVT_RETURN_TIME 150 

#define ENABLE           true
#define DISABLE          false

#define COMMAND          0x77      //Command
#define EVENT            0x78      //Event

#define API_CONNECTED      0x81
#define API_DISCONNECTED   0x82
#define DATA_RECEIVED      0x83
#define API_ERROR          0x84


#define BAUD_9600          9600
#define BAUD_14400         14400
#define BAUD_19200         19200
#define BAUD_38400         38400
#define BAUD_57600         57600
#define BAUD_115200        115200
#define BAUD_125000        125000


#define FEATURE_DIR        0x00
#define FEATURE_OR         0x10
#define FEATURE_AND        0x20

#define APPEND_NAME        0x00
#define NO_APPEND_NAME     0x10


#define ADV_DATA_NOT_APPEND_NAME     0x00000004
#define SW_PARA_UPDATA               0x00000010
#define SW_PARA_ERASE                0x00000020
#define AUTO_SEND_SATUS              0x00000080
#define EXTERNAL32K                  0x00001000
#define RF_CALIB_FORCE               0x02000000
#define CLOCK_32K_OUT                0x20000000


#define SW_BOOT              0x00000000
#define SW_STACK             0x00002000
#define SW_APP1              0x0001A000
#define SW_APP2              0x00020E00           //BlockAddr


#define NORMAL              0x00
#define DEEPSLEEP           0x01
#define POWERDOWN           0x15


#define API_UnknowService_OP     0xFFF0
#define API_NotifyFFF1_OP        0xFFF1
#define API_WithoutResponse_OP   0xFFF2
#define API_STATUS_OP            0x0000
#define API_DISCONN_OP           0x0002
#define API_CONN_INTV_OP         0x0003
#define API_CONN_INTV1_OP        0x0004
#define API_BD_NAME_OP           0x0005
#define API_BD_ADDR_OP           0x0006
#define API_ADV_EN_OP            0x0007
#define API_ADV_INTV_OP          0x0008
#define API_ADV_DATA_OP          0x0009
#define API_SCAN_DATA_OP         0x000A
#define API_TX_POWER_OP          0x000B
#define API_XTAL_OFFSET_OP       0x000E
#define API_PEER_BD_ADDR_OP      0x000F
#define API_FEATURE_OP           0x0010
#define API_VERSION_OP           0x0020
#define API_POWER_SAVING_OP      0x0025
#define API_BAUDRATE_OP          0x0026
#define API_BAUDRATE_MAX_OP      0x0027
#define API_RESET_OP             0x0028
#define API_WHILELIST_OP         0x002A




 
 class BM7701_00_1
 {
  public:  
  BM7701_00_1(HardwareSerial *theSerial=&Serial);                                                                   //Hardware Serial 
  BM7701_00_1(uint16_t rxPin,uint16_t txPin);                                                                          //Software Serial 
  void begin(uint32_t baud=115200);                                                                                           //Initial baud rate                                                                                    //Change baud rate 
  bool setAdvCtrl(bool ctrl);                                                                                //Turn on broadcast
  bool disconnect();                                                                                                 //Disconnect 
  bool writeData(uint8_t dataBuf[],uint8_t length);                                                                       //BC7701 Send Data
  bool readData(uint8_t receiveBuff[],uint8_t &length);  
  bool sendData(uint8_t dataBuf[],uint8_t length);                                                                       //BC7701 Send Data
  uint8_t receiveData(uint8_t receiveBuff[],uint8_t &length);  
  bool setAdvIntv(uint16_t min,uint16_t max,uint8_t advMap);                                                          //Set Advertise Interval
  bool setAdvData(uint8_t appendName,uint8_t length,uint8_t advData[]);                                                                  //Set Advertise Data
  bool setScanData(uint8_t length,uint8_t scanData[]);                                                                //Set Scan Data
  bool setConnIntv(uint16_t interval);                                                                               //Set Connected Interval
  bool setConnIntv(uint16_t minIntv,uint16_t maxIntv,uint16_t latency,uint16_t timeout);                            //Set Connected Interval1 
  bool setName(uint8_t length,uint8_t name[]);                                                                        //Set Device Name
  bool setAddress(uint8_t address[]);                                                                                 //Set MAC address                                
  bool setFeature(uint8_t cmdFlag,uint32_t codeFeature);                                                                           //Set Feature
  bool setPowerSaving(uint8_t mode);                                                                                   //Enter Sleep mode
  bool setWhiteList(bool erase,uint8_t whiteListAddr[],uint8_t mask[]);                                                //Set Whilelist addr
  bool setTXpower(uint8_t power);                                                                                    //Set TX power
  bool setCrystalOffset(uint8_t cload);                                                                                  //Set Xtal Cload
  bool restoreDefault();                                                                                              //Software Reset,no save configuration                                                                 
  bool reset();                                                                                          //Software Reset,save configuration                                                                                               //BC7701 Receive Data
  void wakeUp();                                                                                                  //Dummy wake up from deepsleep/powerdown mode                                                                                             //Clear Rx Buffer 
  bool sendCMD(uint16_t type,uint8_t cmd_flag,uint8_t cmd_length,uint8_t value[]);                           //Send BCI Command with waiting evt
  void sendCMD_NoResponse(uint16_t type,uint8_t cmd_flag,uint8_t cmd_length,uint8_t value[]);                     //Send BCI Command with no waiting evt
  bool readCMD(uint16_t type,uint8_t cmd_flag);                                                                 //Send Read Command with waiting evt
  void readCMD_NoResponse(uint16_t type,uint8_t cmd_flag);                                                           //Send Read Command with no waiting evt
  
  uint8_t cmdFlag = 0;  //MCU to BLE
  uint8_t evtStatus = 0;//BLE to MCU
  private:
  uint8_t receive_data[256];      //receive buffer
  uint8_t len;
  void writeBytes(uint8_t wbuf[], uint8_t wlen);
  uint8_t readBytes(uint8_t rbuf[],uint8_t rlen,uint16_t timeout);
  uint16_t _rxPin;
  uint16_t _txPin;
  HardwareSerial *_serial = NULL;
  SoftwareSerial *_softSerial = NULL ;
  bool setBaudRate(uint32_t baud);
  };


#endif
