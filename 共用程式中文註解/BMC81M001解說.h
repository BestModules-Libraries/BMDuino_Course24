/*
 * ==================================================
 * 檔案名稱：BMC81M001.h - BMC81M001 Wi-Fi 模組控制函式庫
 * 適用硬體：BMC81M001 Wi-Fi 模組（基於 ESP-12F）
 * 通訊介面：UART（支援硬體串口與軟體串口）
 * 
 * 程式功能說明：
 *   本標頭檔定義了 BMC81M001 Wi-Fi 模組的控制類別與相關常數，
 *   提供完整的 Wi-Fi 連線、TCP/IP 通訊、MQTT 協定、HTTP GET 請求等功能。
 *   開發者可透過此函式庫輕鬆實現物聯網應用的網路連線與資料傳輸。
 * 
 * 主要功能模組：
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 1. Wi-Fi 連線管理                                       │
 *   │    - 連線至無線基地台 (connectToAP)                      │
 *   │    - 取得連線狀態 (getStatus)                            │
 *   │    - 取得網路參數 (SSID、IP、MAC、Gateway、Mask)        │
 *   └─────────────────────────────────────────────────────────┘
 *                              ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 2. TCP 通訊                                            │
 *   │    - 建立 TCP 連線 (connectTCP)                         │
 *   │    - 傳送資料 (writeDataTcp)                            │
 *   │    - 接收資料 (readDataTcp)                             │
 *   └─────────────────────────────────────────────────────────┘
 *                              ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 3. MQTT 協定支援                                        │
 *   │    - 設定 MQTT 連線參數 (configMqtt)                     │
 *   │    - 設定發布/訂閱主題 (setPublishTopic/setSubscribetopic)│
 *   │    - 發布訊息 (writeString/writeBytes)                   │
 *   │    - 接收訊息 (readIotData)                              │
 *   └─────────────────────────────────────────────────────────┘
 *                              ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 4. HTTP GET 請求                                        │
 *   │    - 初始化 HTTP 連線 (http_begin)                       │
 *   │    - 執行 GET 請求 (http_get)                            │
 *   │    - 取得回應內容 (http_getString)                       │
 *   │    - 結束連線 (http_end)                                 │
 *   └─────────────────────────────────────────────────────────┘
 *                              ▼
 *   ┌─────────────────────────────────────────────────────────┐
 *   │ 5. AT 指令介面                                          │
 *   │    - 傳送 AT 指令並等待回應 (sendATCommand)              │
 *   │    - 取得指令回應字串 (sendATCmd)                        │
 *   │    - 模組重設 (reset)                                    │
 *   └─────────────────────────────────────────────────────────┘
 * 
 * 應用場景：
 *   - 物聯網裝置資料上傳（雲端平台）
 *   - MQTT 物聯網訊息傳輸
 *   - Web API 資料獲取（HTTP GET）
 *   - 遠端設備控制
 *   - 感測器數據收集與傳輸
 * 
 * 作者/版本：BEST MODULES CORP. / V1.0.4 (2024-8-22)
 * 修改紀錄：
 *   - 2025.03.27：加入詳細繁體中文註解與整體說明
 * ==================================================
 */

/*************************************************
File:             BMC81M001.h
Author:           BEST MODULES CORP.
Description:      定義類別與所需變數
version:          V1.0.4-2024-8-22
**************************************************/

#ifndef _BMC81M001_H_
#define _BMC81M001_H_

#include <Arduino.h>          // Arduino 核心函式庫
#include <SoftwareSerial.h>   // 軟體序列埠函式庫（用於非硬體串口通訊）

// ==================== 模組通訊設定 ====================
// 模組預設鮑率（通訊速率）115200 bps
#define BMC81M001_baudRate 115200

// ==================== 傳送結果定義 ====================
#define SEND_SUCCESS 1        // 傳送成功
#define SEND_FAIL 0           // 傳送失敗
#define COMMUNICAT_ERROR -1   // 通訊錯誤（無法與模組通訊）
#define AT_ACK_ERROR -2       // AT 指令應答錯誤（模組回傳非預期回應）

// ==================== Wi-Fi 狀態定義 ====================
#define WIFI_STATUS_GOT_IP  2        // 已取得 IP 位址（DHCP 完成）
#define WIFI_STATUS_CONNETED  3      // 已連線至無線基地台
#define WIFI_STATUS_DISCONNETED  4   // 已斷線（曾經連線但現在斷開）
#define WIFI_STATUS_NO_CONNET  5     // 未連線（從未連線或連線失敗）

// ==================== HTTP GET 操作狀態定義 ====================
#define HTTP_GET_BEGIN_SUCCESS 0   // HTTP GET 開始成功（連線建立）
#define HTTP_GET_OP_SUCCESS 0      // HTTP GET 操作成功（請求發送成功）
#define HTTP_GET_URL_ERROR -1      // HTTP GET URL 錯誤（格式不正確）
#define HTTP_GET_OP_TIMEOUT -2     // HTTP GET 操作逾時（模組未回應）

// ==================== 緩衝區大小定義 ====================
#define RES_MAX_LENGTH 2000   // 回應資料最大長度（位元組）

/*!
 * BMC81M001 類別，用於控制 BMC81M001 Wi-Fi 模組
 * 繼承自 Print 類別，方便除錯時可同時輸出至其他串口
 * 
 * 使用方式：
 *   1. 建立 BMC81M001 物件（可選擇硬體串口或軟體串口）
 *   2. 呼叫 begin() 初始化模組
 *   3. 呼叫 connectToAP() 連線至 Wi-Fi 基地台
 *   4. 根據需求呼叫對應功能（MQTT、TCP、HTTP 等）
 */
class BMC81M001 
{
  public:
      // ==================== 建構函式 (Constructor) ====================
      
      // 建構函式：使用硬體串口（預設為 Serial）
      // 參數 theSerial：硬體串口物件指標，可傳入 &Serial、&Serial1、&Serial2 等
      BMC81M001(HardwareSerial *theSerial = &Serial);
      
      // 建構函式：使用軟體串口（指定 RX, TX 腳位）
      // 參數 rxPin：接收腳位（RX）
      // 參數 txPin：傳送腳位（TX）
      BMC81M001(uint16_t rxPin, uint16_t txPin);
      
      // ==================== 初始化與基本控制 ====================
      
      // 初始化模組，設定鮑率
      // 參數 baud：通訊速率，預設為 115200 bps
      void begin(uint32_t baud = BMC81M001_baudRate);  
      
      // 重設模組
      // 回傳值：true = 重設成功，false = 重設失敗
      bool reset(void);
      
      // ==================== Wi-Fi 連線管理 ====================
      
      // 連線至指定無線基地台 (Access Point)
      // 參數 ssid：無線網路名稱
      // 參數 pass：無線網路密碼
      // 回傳值：true = 連線成功，false = 連線失敗
      bool connectToAP(String ssid, String pass);
      
      // 取得目前連線的 SSID（無線網路名稱）
      String SSID();
      
      // 取得 Wi-Fi 狀態
      // 回傳值：WIFI_STATUS_GOT_IP / WIFI_STATUS_CONNETED / 
      //        WIFI_STATUS_DISCONNETED / WIFI_STATUS_NO_CONNET
      int getStatus();
      
      // 取得連線的 SSID（同 SSID() 函式）
      String getSSID();
      
      // 取得模組 IP 位址
      // 回傳值：IP 位址字串，如 "192.168.1.100"
      String getIP();
      
      // 取得閘道器位址
      // 回傳值：閘道器 IP 位址字串
      String getGateway();
      
      // 取得子網路遮罩
      // 回傳值：子網路遮罩字串，如 "255.255.255.0"
      String getMask();
      
      // 取得模組 MAC 位址
      // 回傳值：MAC 位址字串，格式為 "XX:XX:XX:XX:XX:XX"
      String getMacAddress();
      
      // 取得 AT 指令韌體版本
      // 回傳值：韌體版本字串
      String getATVersion();
      
      // ==================== TCP 通訊 ====================
      
      // 建立 TCP 連線至指定 IP 與埠號
      // 參數 ip：目標 IP 位址或網域名稱
      // 參數 port：目標埠號
      // 回傳值：true = 連線成功，false = 連線失敗
      bool connectTCP(String ip, int port);
      
      // 透過 TCP 傳送資料
      // 參數 Dlength：資料長度（位元組）
      // 參數 Dbuffer：要傳送的資料緩衝區
      // 回傳值：true = 傳送成功，false = 傳送失敗
      bool writeDataTcp(int Dlength, char Dbuffer[]);
      
      // 讀取 TCP 接收的資料
      // 回傳值：接收到的資料字串
      String readDataTcp();
      
      // ==================== MQTT 協定支援 ====================
      
      // 設定 MQTT 連線參數
      // 參數 clientid：MQTT 客戶端 ID（唯一識別碼）
      // 參數 username：MQTT 使用者名稱（可選，無需認證可留空）
      // 參數 password：MQTT 密碼（可選，無需認證可留空）
      // 參數 mqtt_host：MQTT Broker 主機位址
      // 參數 server_port：MQTT Broker 埠號（通常為 1883）
      // 回傳值：true = 設定成功，false = 設定失敗
      bool configMqtt(String clientid, String username, String password, 
                      String mqtt_host, int server_port);
      
      // 設定 MQTT 發布主題（用於發送訊息）
      // 參數 publishtopic：發布主題名稱
      // 回傳值：true = 設定成功，false = 設定失敗
      bool setPublishTopic(String publishtopic);
      
      // 設定 MQTT 訂閱主題（用於接收訊息）
      // 參數 subscribetopic：訂閱主題名稱
      // 回傳值：true = 設定成功，false = 設定失敗
      bool setSubscribetopic(String subscribetopic);
      
      // 設定 MQTT 主題（通用）
      // 參數 topic：主題名稱
      // 回傳值：true = 設定成功，false = 設定失敗
      bool setTopic(String topic);
      
      // 傳送字串資料至指定 MQTT 主題
      // 參數 Dbuffer：要傳送的字串資料
      // 參數 topic：目標主題名稱
      // 回傳值：true = 傳送成功，false = 傳送失敗
      bool writeString(String Dbuffer, String topic);
      
      // 傳送位元組陣列資料至指定 MQTT 主題
      // 參數 Dbuffer：要傳送的資料緩衝區
      // 參數 Dlength：資料長度（位元組）
      // 參數 topic：目標主題名稱
      // 回傳值：true = 傳送成功，false = 傳送失敗
      bool writeBytes(char Dbuffer[], int Dlength, String topic);
      
      // 讀取物聯網資料（MQTT 接收）
      // 參數 ReciveBuff：接收資料緩衝區（輸出參數）
      // 參數 ReciveBufflen：接收資料長度（輸出參數）
      // 參數 topic：接收資料的主題（輸出參數）
      // 說明：此函式會檢查是否有新的 MQTT 訊息，若有則填入輸出參數
      void readIotData(String *ReciveBuff, int *ReciveBufflen, String *topic);
      
      // ==================== HTTP GET 請求 ====================
      
      // 開始 HTTP GET 連線設定
      // 參數 serverURL：伺服器主機位址（不含 http:// 前綴）
      // 參數 port：伺服器埠號
      // 參數 subURL：URL 子路徑與參數（可選，如 "/api/data?id=1"）
      // 回傳值：HTTP_GET_BEGIN_SUCCESS 或錯誤代碼
      int http_begin(String serverURL, int port, String subURL = "");
      
      // 執行 HTTP GET 請求
      // 回傳值：HTTP_GET_OP_SUCCESS 或錯誤代碼
      int http_get(void);
      
      // 取得 HTTP GET 回應字串
      // 回傳值：伺服器回應的內容字串
      String http_getString(void);
      
      // 結束 HTTP 連線，釋放資源
      void http_end(void);
      
      // ==================== AT 指令介面 ====================
      
      // 傳送 AT 指令，並等待回應
      // 參數 StringstrCmd：要傳送的 AT 指令字串
      // 參數 timeout：等待回應的逾時時間（毫秒）
      // 參數 reTry：失敗時的重試次數
      // 回傳值：SEND_SUCCESS / SEND_FAIL / COMMUNICAT_ERROR / AT_ACK_ERROR
      int sendATCommand(String StringstrCmd, int timeout, uint8_t reTry);
      
      // 傳送 AT 指令並回傳字串回應
      // 參數 StringstrCmd：要傳送的 AT 指令字串
      // 參數 timeout：等待回應的逾時時間（毫秒）
      // 參數 reTry：失敗時的重試次數
      // 回傳值：模組回應的字串內容
      String sendATCmd(String StringstrCmd, int timeout, uint8_t reTry);
      
      // ==================== 公用變數 ====================
      
      // 儲存模組回應資料的緩衝區（C 字串格式）
      char BMC81M001Response[RES_MAX_LENGTH];
      
      // 回應資料長度（位元組數）
      int resLength = 0;
      
      // 用於 OneNet 平台資料接收緩衝區（中國移動物聯網平台）
      String OneNetReciveBuff;
      
  private:
      // ==================== 私有成員變數 ====================
      
      // 軟體串口腳位定義
      uint16_t _rxPin;   // 接收腳位（RX）
      uint16_t _txPin;   // 傳送腳位（TX）
      
      // 串口物件指標（用於與模組通訊）
      HardwareSerial *_serial = NULL;       // 硬體串口物件指標
      SoftwareSerial *_softSerial = NULL;   // 軟體串口物件指標
      
      // HTTP GET 相關參數
      String _host = "Host: ";  // HTTP Host 標頭
      int _port;                // 目標埠號
      String _type;             // 連線類型（如 "TCP"）
      String _url;              // 完整 URL
      String _suburl;           // URL 子路徑與參數
      int _len;                 // 內容長度
      
      // ==================== 私有成員函式 ====================
      
      // 讀取模組回應（內部使用）
      void readResponse(void);
      
      // 清除回應緩衝區（內部使用）
      void clearResponse(char Debugbuffer[]);
};

// ==================== 錯誤代碼列舉定義 ====================
// 這些錯誤碼用於標示模組操作失敗的具體原因
enum
{
  RST_Fail = 1,              // 重設失敗（模組無法回應重設指令）
  Init_Fail,                 // 初始化失敗（無法與模組建立通訊）
  Station_Fail,              // 站台模式設定失敗（無法設定為 Wi-Fi 客戶端模式）
  WIFI_CONNECTED_Fail,       // Wi-Fi 連線失敗（無法連線至指定基地台）
  TCP_CONNECTED_Fail,        // TCP 連線失敗（無法建立 TCP 連線）
  TCP_Senddata_Fail,         // TCP 傳送資料失敗

  MQTTSNTP_FAIL = 10,        // MQTT SNTP 設定失敗（時間同步設定失敗）
  MQTTUSERCFG_Fail,          // MQTT 使用者設定失敗
  MQTTCLIENTID_Fail,         // MQTT 客戶端 ID 設定失敗
  MQTTUSERNAME_Fail,         // MQTT 使用者名稱設定失敗
  MQTTPASSWORD_Fail,         // MQTT 密碼設定失敗
  MQTTConnect_Fail,          // MQTT 連線失敗（無法連線至 MQTT Broker）
  MQTTTopic_Fail             // MQTT 主題設定失敗
};

#endif // _BMC81M001_H_