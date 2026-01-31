/*************************************************
File:             BMC81M001.h
Author:           BEST MODULES CORP.
Description:      定義類別與所需變數
version:          V1.0.4-2024-8-22
**************************************************/

#ifndef _BMC81M001_H_
#define _BMC81M001_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

// 模組預設鮑率
#define BMC81M001_baudRate 115200

// 傳送結果定義
#define SEND_SUCCESS 1        // 傳送成功
#define SEND_FAIL 0           // 傳送失敗
#define COMMUNICAT_ERROR -1   // 通訊錯誤
#define AT_ACK_ERROR -2       // AT指令應答錯誤

//----------------------Wi-Fi 狀態定義---------------------------
#define WIFI_STATUS_GOT_IP  2        // 已取得IP
#define WIFI_STATUS_CONNETED  3      // 已連線
#define WIFI_STATUS_DISCONNETED  4   // 已斷線
#define WIFI_STATUS_NO_CONNET  5     // 未連線
//--------------------------------------------------------------

// HTTP GET 操作狀態定義
#define HTTP_GET_BEGIN_SUCCESS 0   // HTTP GET 開始成功
#define HTTP_GET_OP_SUCCESS 0      // HTTP GET 操作成功
#define HTTP_GET_URL_ERROR -1      // HTTP GET URL錯誤
#define HTTP_GET_OP_TIMEOUT -2     // HTTP GET 操作逾時

// 回應資料最大長度
#define RES_MAX_LENGTH 2000

/*!
 * BMC81M001 類別，用於控制 BMC81M001 Wi-Fi 模組
 * 繼承自 Print 類別，方便除錯時可同時輸出至其他串口
 */
class BMC81M001 
{
  public:
      // 建構函式：使用硬體串口 (預設為 Serial)
      BMC81M001(HardwareSerial *theSerial = &Serial);
      
      // 建構函式：使用軟體串口 (指定RX, TX腳位)
      BMC81M001(uint16_t rxPin, uint16_t txPin);
      
      // 初始化模組，設定鮑率
      void begin(uint32_t baud = BMC81M001_baudRate);  
      
      // 連線至指定無線基地台 (AP)
      bool connectToAP(String ssid, String pass);
      
      // 建立 TCP 連線至指定 IP 與埠號
      bool connectTCP(String ip, int port);
      
      // 透過 TCP 傳送資料
      bool writeDataTcp(int Dlength, char Dbuffer[]);
      
      // 讀取 TCP 接收的資料
      String readDataTcp();
      
      // 設定 MQTT 連線參數
      bool configMqtt(String clientid, String username, String password, 
                      String mqtt_host, int server_port);
      
      // 設定 MQTT 發布主題
      bool setPublishTopic(String publishtopic);
      
      // 設定 MQTT 訂閱主題
      bool setSubscribetopic(String subscribetopic);
      
      // 設定 MQTT 主題 (通用)
      bool setTopic(String topic);
      
      // 傳送字串資料至指定 MQTT 主題
      bool writeString(String Dbuffer, String topic);
      
      // 傳送位元組陣列資料至指定 MQTT 主題
      bool writeBytes(char Dbuffer[], int Dlength, String topic);
      
      // 讀取物聯網資料 (MQTT 接收)
      void readIotData(String *ReciveBuff, int *ReciveBufflen, String *topic);
      
      // 重設模組
      bool reset(void);
      
      // 傳送 AT 指令，並等待回應
      int sendATCommand(String StringstrCmd, int timeout, uint8_t reTry);
      
      //-------------------------------------------------------------------
      // 傳送 AT 指令並回傳字串回應
      String sendATCmd(String StringstrCmd, int timeout, uint8_t reTry);
      
      // 取得目前連線的 SSID
      String SSID();
      
      // 取得 Wi-Fi 狀態
      int getStatus();
      
      // 取得連線的 SSID
      String getSSID();
      
      // 取得模組 IP 位址
      String getIP();
      
      // 取得閘道器位址
      String getGateway();
      
      // 取得子網路遮罩
      String getMask();
      
      // 取得模組 MAC 位址
      String getMacAddress();
      
      // 取得 AT 指令韌體版本
      String getATVersion();
      //-------------------------------------------------------------------
      
      // 開始 HTTP GET 連線設定
      int http_begin(String serverURL, int port, String subURL = "");
      
      // 執行 HTTP GET 請求
      int http_get(void);
      
      // 取得 HTTP GET 回應字串
      String http_getString(void);
      
      // 結束 HTTP 連線
      void http_end(void);

      // 公用變數：儲存模組回應資料
      char BMC81M001Response[RES_MAX_LENGTH];
      int resLength = 0;  // 回應資料長度

      // 用於 OneNet 平台資料接收緩衝區
      String OneNetReciveBuff;
      
  private:
      // 軟體串口腳位
      uint16_t _rxPin;
      uint16_t _txPin;
      
      // 讀取模組回應
      void readResponse(void);
      
      // 清除回應緩衝區
      void clearResponse(char Debugbuffer[]);
      
      // 串口物件指標
      HardwareSerial *_serial = NULL;       // 硬體串口
      SoftwareSerial *_softSerial = NULL ;  // 軟體串口
      
      // HTTP GET 相關參數
      String _host = "Host: ";  // 主機標頭
      int _port;                // 埠號
      String _type;             // 連線類型
      String _url;              // 完整URL
      String _suburl;           // 子路徑
      int _len;                 // 內容長度
};

// 錯誤代碼列舉定義
enum
{
  RST_Fail = 1,              // 重設失敗
  Init_Fail,                 // 初始化失敗
  Station_Fail,              // 站台模式設定失敗
  WIFI_CONNECTED_Fail,       // Wi-Fi 連線失敗
  TCP_CONNECTED_Fail,        // TCP 連線失敗
  TCP_Senddata_Fail,         // TCP 傳送資料失敗

  MQTTSNTP_FAIL = 10,        // MQTT SNTP 設定失敗
  MQTTUSERCFG_Fail,          // MQTT 使用者設定失敗
  MQTTCLIENTID_Fail,         // MQTT 客戶端ID設定失敗
  MQTTUSERNAME_Fail,         // MQTT 使用者名稱設定失敗
  MQTTPASSWORD_Fail,         // MQTT 密碼設定失敗
  MQTTConnect_Fail,          // MQTT 連線失敗
  MQTTTopic_Fail             // MQTT 主題設定失敗
};

#endif // _BMC81M001_H_