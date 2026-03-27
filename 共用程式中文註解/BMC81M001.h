/*-------------------------------------------------------------
  程式名稱：BMC81M001 WiFi 通訊模組驅動程式
  程式用途：本程式為 BMC81M001 WiFi 模組的驅動程式庫，提供
           透過 AT 指令控制 WiFi 連線、TCP 通訊、MQTT 協定、
           HTTP GET 請求等功能，適用於物聯網應用開發。
  
  硬體架構：支援 BMduino 或一般 Arduino 開發板，可選擇使用
           硬體序列埠（HardwareSerial）或軟體序列埠（SoftwareSerial）
           與 BMC81M001 模組進行通訊。
  
  作者說明：本程式為 BEST MODULES CORP. 開發之 BMC81M001 WiFi 模組
           驅動程式，版本 V1.0.4 (2024-8-22)，採用 Arduino C 語言
           撰寫，提供完整的 WiFi 連線與網路通訊功能。
  
  主要功能：
    1. WiFi 連線管理：連接/斷開 AP、取得網路狀態、IP 位址等
    2. TCP 通訊：建立 TCP 連線、發送/接收資料
    3. MQTT 協定：設定 MQTT Broker、發布/訂閱主題
    4. HTTP 客戶端：發送 HTTP GET 請求、取得回應內容
    5. AT 指令控制：發送自訂 AT 指令、解析回應
  
  使用流程：
    1. 建立 BMC81M001 物件（可選硬體序列埠或軟體序列埠）
    2. 呼叫 begin() 初始化模組
    3. 呼叫 connectToAP() 連線至 WiFi 基地台
    4. 使用 connectTCP() 或 configMqtt() 進行網路通訊
    5. 透過 writeDataTcp()、writeString() 發送資料
    6. 使用 readDataTcp()、readIotData() 接收資料
  
  最後修改：2024年8月22日
  版本：V1.0.4
-------------------------------------------------------------*/

/*************************************************
File:             BMC81M001.h
Author:           BEST MODULES CORP.
Description:      Define classes and required variables 
version:          V1.0.4-2024-8-22
**************************************************/
#ifndef _BMC81M001_H_
#define _BMC81M001_H_

 
#include <Arduino.h>          // 引入 Arduino 核心函式庫，提供基本資料型態與函式
#include <SoftwareSerial.h>   // 引入軟體序列埠函式庫，用於軟體模擬 UART 通訊

//---------------------- 通訊參數定義 --------------------------
#define BMC81M001_baudRate 115200  // 定義 BMC81M001 模組的通訊速率（115200 bps）

//---------------------- AT 指令回傳狀態定義 --------------------
#define SEND_SUCCESS 1             // AT 指令發送成功
#define SEND_FAIL 0                // AT 指令發送失敗
#define COMMUNICAT_ERROR -1        // 通訊錯誤（無回應或回應異常）
#define AT_ACK_ERROR -2            // AT 指令回應錯誤（收到 ERROR 回應）

//---------------------- WiFi 狀態定義 --------------------------
#define WIFI_STATUS_GOT_IP 2       // WiFi 已取得 IP 位址（連線成功）
#define WIFI_STATUS_CONNETED 3     // WiFi 已連線至基地台
#define WIFI_STATUS_DISCONNETED 4  // WiFi 已斷線
#define WIFI_STATUS_NO_CONNET 5    // 未連線至任何基地台

//---------------------- HTTP GET 操作狀態定義 ------------------
#define HTTP_GET_BEGIN_SUCCESS 0   // HTTP GET 請求初始化成功
#define HTTP_GET_OP_SUCCESS 0      // HTTP GET 操作成功
#define HTTP_GET_URL_ERROR -1      // URL 格式錯誤
#define HTTP_GET_OP_TIMEOUT -2     // HTTP GET 操作超時

//---------------------- 緩衝區大小定義 --------------------------
#define RES_MAX_LENGTH 2000        // 定義回應緩衝區最大長度（2000 bytes）


/*! 
 * 類別名稱：BMC81M001
 * 功能說明：BMC81M001 WiFi 模組的驅動類別，封裝了所有與模組通訊的函式。
 *           繼承自 Print 類別，便於除錯時將輸出同時導向第二個串流（如 Serial）。
 */
class BMC81M001 
{
  public:
      //---------------------- 建構子（Constructor）----------------------
      /* 函式名稱：BMC81M001（硬體序列埠版本）
       * 功能說明：使用硬體序列埠（HardwareSerial）初始化 BMC81M001 物件
       * 輸入參數：theSerial - 指向硬體序列埠的指標，預設為 &Serial
       * 回傳值：無
       */
      BMC81M001( HardwareSerial *theSerial = &Serial);
      
      /* 函式名稱：BMC81M001（軟體序列埠版本）
       * 功能說明：使用軟體序列埠（SoftwareSerial）初始化 BMC81M001 物件
       * 輸入參數：rxPin - 接收腳位（RX），txPin - 傳送腳位（TX）
       * 回傳值：無
       */
      BMC81M001(uint16_t rxPin, uint16_t txPin);
      
      //---------------------- 初始化函式 ----------------------
      /* 函式名稱：begin
       * 功能說明：初始化 BMC81M001 模組，設定通訊速率
       * 輸入參數：baud - 通訊速率，預設為 BMC81M001_baudRate (115200)
       * 回傳值：無
       */
      void begin(uint32_t baud = BMC81M001_baudRate);  
      
      //---------------------- WiFi 連線函式 ----------------------
      /* 函式名稱：connectToAP
       * 功能說明：連線至指定的 WiFi 基地台（Access Point）
       * 輸入參數：ssid - 基地台名稱，pass - 基地台密碼
       * 回傳值：布林值，true 表示連線成功，false 表示連線失敗
       */
      bool connectToAP(String ssid, String pass);
      
      //---------------------- TCP 通訊函式 ----------------------
      /* 函式名稱：connectTCP
       * 功能說明：建立 TCP 連線至指定的伺服器
       * 輸入參數：ip - 伺服器 IP 位址（字串格式），port - 伺服器埠號
       * 回傳值：布林值，true 表示連線成功，false 表示連線失敗
       */
      bool connectTCP(String ip, int port);
      
      /* 函式名稱：writeDataTcp
       * 功能說明：透過 TCP 連線發送資料（指定長度與緩衝區）
       * 輸入參數：Dlength - 資料長度，Dbuffer - 資料緩衝區
       * 回傳值：布林值，true 表示發送成功，false 表示發送失敗
       */
      bool writeDataTcp(int Dlength, char Dbuffer[]);
      
      /* 函式名稱：readDataTcp
       * 功能說明：從 TCP 連線讀取資料
       * 輸入參數：無
       * 回傳值：String 型態，讀取到的資料內容
       */
      String readDataTcp();
      
      //---------------------- MQTT 通訊函式 ----------------------
      /* 函式名稱：configMqtt
       * 功能說明：設定 MQTT Broker 連線參數
       * 輸入參數：
       *   clientid - 客戶端識別碼
       *   username - 使用者名稱（若無需驗證可留空）
       *   password - 密碼（若無需驗證可留空）
       *   mqtt_host - MQTT Broker 主機位址
       *   server_port - MQTT Broker 埠號
       * 回傳值：布林值，true 表示設定成功，false 表示設定失敗
       */
      bool configMqtt(String clientid, String username, String password, String mqtt_host, int server_port);
      
      /* 函式名稱：setPublishTopic
       * 功能說明：設定 MQTT 發布主題（Publish Topic）
       * 輸入參數：publishtopic - 要發布的主題名稱
       * 回傳值：布林值，true 表示設定成功，false 表示設定失敗
       */
      bool setPublishTopic(String publishtopic);
      
      /* 函式名稱：setSubscribetopic
       * 功能說明：設定 MQTT 訂閱主題（Subscribe Topic）
       * 輸入參數：subscribetopic - 要訂閱的主題名稱
       * 回傳值：布林值，true 表示設定成功，false 表示設定失敗
       */
      bool setSubscribetopic(String subscribetopic);
      
      /* 函式名稱：setTopic
       * 功能說明：設定 MQTT 主題（通用版本）
       * 輸入參數：topic - 主題名稱
       * 回傳值：布林值，true 表示設定成功，false 表示設定失敗
       */
      bool setTopic(String topic);
      
      /* 函式名稱：writeString
       * 功能說明：透過 MQTT 發送字串資料至指定主題
       * 輸入參數：Dbuffer - 要發送的字串內容，topic - 主題名稱
       * 回傳值：布林值，true 表示發送成功，false 表示發送失敗
       */
      bool writeString(String Dbuffer, String topic);
      
      /* 函式名稱：writeBytes
       * 功能說明：透過 MQTT 發送二進位資料至指定主題
       * 輸入參數：Dbuffer - 資料緩衝區，Dlength - 資料長度，topic - 主題名稱
       * 回傳值：布林值，true 表示發送成功，false 表示發送失敗
       */
      bool writeBytes(char Dbuffer[], int Dlength, String topic);
      
      /* 函式名稱：readIotData
       * 功能說明：讀取 MQTT 訂閱主題接收到的物聯網資料
       * 輸入參數：
       *   ReciveBuff - 接收到的資料內容（透過指標回傳）
       *   ReciveBufflen - 接收資料的長度（透過指標回傳）
       *   topic - 接收資料的主題名稱（透過指標回傳）
       * 回傳值：無
       */
      void readIotData(String *ReciveBuff, int *ReciveBufflen, String *topic);
      
      //---------------------- 系統控制函式 ----------------------
      /* 函式名稱：reset
       * 功能說明：重置 BMC81M001 模組
       * 輸入參數：無
       * 回傳值：布林值，true 表示重置成功，false 表示重置失敗
       */
      bool reset(void);
      
      /* 函式名稱：sendATCommand
       * 功能說明：發送 AT 指令並等待回應（基礎版本）
       * 輸入參數：
       *   StringstrCmd - 要發送的 AT 指令字串
       *   timeout - 等待回應的超時時間（毫秒）
       *   reTry - 失敗時的重試次數
       * 回傳值：整數，SEND_SUCCESS 表示成功，SEND_FAIL 表示失敗
       */
      int sendATCommand(String StringstrCmd, int timeout, uint8_t reTry);
      
      //---------------------- AT 指令輔助函式 ----------------------
      /* 函式名稱：sendATCmd
       * 功能說明：發送 AT 指令並取得回應內容（進階版本）
       * 輸入參數：
       *   StringstrCmd - 要發送的 AT 指令字串
       *   timeout - 等待回應的超時時間（毫秒）
       *   reTry - 失敗時的重試次數
       * 回傳值：String 型態，回應內容
       */
      String sendATCmd(String StringstrCmd, int timeout, uint8_t reTry);
      
      /* 函式名稱：SSID
       * 功能說明：取得目前連線的基地台名稱（SSID）
       * 輸入參數：無
       * 回傳值：String 型態，基地台名稱
       */
      String SSID();
      
      /* 函式名稱：getStatus
       * 功能說明：取得 WiFi 連線狀態
       * 輸入參數：無
       * 回傳值：整數，狀態碼（WIFI_STATUS_xxx 系列定義）
       */
      int getStatus();
      
      /* 函式名稱：getSSID
       * 功能說明：取得目前連線的基地台名稱（與 SSID() 功能相同）
       * 輸入參數：無
       * 回傳值：String 型態，基地台名稱
       */
      String getSSID();
      
      /* 函式名稱：getIP
       * 功能說明：取得模組的 IP 位址
       * 輸入參數：無
       * 回傳值：String 型態，IP 位址（如 "192.168.1.100"）
       */
      String getIP();
      
      /* 函式名稱：getGateway
       * 功能說明：取得網關（Gateway）位址
       * 輸入參數：無
       * 回傳值：String 型態，網關位址
       */
      String getGateway();
      
      /* 函式名稱：getMask
       * 功能說明：取得子網路遮罩（Subnet Mask）
       * 輸入參數：無
       * 回傳值：String 型態，子網路遮罩
       */
      String getMask();
      
      /* 函式名稱：getMacAddress
       * 功能說明：取得模組的 MAC 位址
       * 輸入參數：無
       * 回傳值：String 型態，MAC 位址
       */
      String getMacAddress();
      
      /* 函式名稱：getATVersion
       * 功能說明：取得 AT 指令韌體版本
       * 輸入參數：無
       * 回傳值：String 型態，版本資訊
       */
      String getATVersion();
      
      //---------------------- HTTP 客戶端函式 ----------------------
      /* 函式名稱：http_begin
       * 功能說明：初始化 HTTP GET 請求
       * 輸入參數：
       *   serverURL - 伺服器位址（如 "http://example.com"）
       *   port - 伺服器埠號（通常為 80）
       *   subURL - 子路徑（如 "/api/data"），可選參數
       * 回傳值：整數，HTTP_GET_BEGIN_SUCCESS 表示成功，其他表示失敗
       */
      int http_begin(String serverURL, int port, String subURL = "");
      
      /* 函式名稱：http_get
       * 功能說明：執行 HTTP GET 請求
       * 輸入參數：無
       * 回傳值：整數，HTTP_GET_OP_SUCCESS 表示成功，其他表示失敗
       */
      int http_get(void);
      
      /* 函式名稱：http_getString
       * 功能說明：取得 HTTP GET 請求的回應內容（字串格式）
       * 輸入參數：無
       * 回傳值：String 型態，回應內容
       */
      String http_getString(void);
      
      /* 函式名稱：http_end
       * 功能說明：結束 HTTP 連線，釋放資源
       * 輸入參數：無
       * 回傳值：無
       */
      void http_end(void);

      //---------------------- 公開成員變數 ----------------------
      char BMC81M001Response[RES_MAX_LENGTH];  // 儲存模組回應內容的字元陣列
      int resLength = 0;                       // 回應內容的實際長度
      String OneNetReciveBuff;                 // OneNet 平台接收緩衝區

  private:
      //---------------------- 私有成員變數 ----------------------
      uint16_t _rxPin;                         // 軟體序列埠 RX 腳位
      uint16_t _txPin;                         // 軟體序列埠 TX 腳位
      HardwareSerial *_serial = NULL;          // 硬體序列埠物件指標
      SoftwareSerial *_softSerial = NULL ;     // 軟體序列埠物件指標
      
      //---------------------- HTTP GET 相關私有變數 --------------
      String _host = "Host: ";                 // HTTP Host 標頭
      int _port;                               // 伺服器埠號
      String _type;                            // 請求類型
      String _url;                             // 完整 URL
      String _suburl;                          // 子路徑
      int _len;                                // 回應長度
      
      //---------------------- 私有方法 --------------------------
      /* 函式名稱：readResponse
       * 功能說明：讀取模組回應內容
       * 輸入參數：無
       * 回傳值：無
       */
      void readResponse(void);
      
      /* 函式名稱：clearResponse
       * 功能說明：清除回應緩衝區
       * 輸入參數：Debugbuffer - 要清除的緩衝區
       * 回傳值：無
       */
      void clearResponse(char Debugbuffer[]);
};

/*---------------------- 錯誤碼列舉定義 --------------------------
 * 功能說明：定義各種操作失敗時的錯誤碼，便於除錯與狀態判斷
 */
enum
{
  RST_Fail = 1,              // 重置失敗
  Init_Fail,                 // 初始化失敗
  Station_Fail,              // 工作站模式設定失敗
  WIFI_CONNECTED_Fail,       // WiFi 連線失敗
  TCP_CONNECTED_Fail,        // TCP 連線失敗
  TCP_Senddata_Fail,         // TCP 資料發送失敗

  MQTTSNTP_FAIL = 10,        // MQTT SNTP 時間同步失敗
  MQTTUSERCFG_Fail,          // MQTT 使用者設定失敗
  MQTTCLIENTID_Fail,         // MQTT Client ID 設定失敗
  MQTTUSERNAME_Fail,         // MQTT 使用者名稱設定失敗
  MQTTPASSWORD_Fail,         // MQTT 密碼設定失敗
  MQTTConnect_Fail,          // MQTT Broker 連線失敗
  MQTTTopic_Fail             // MQTT 主題設定失敗
};

#endif // _BMC81M001_H_