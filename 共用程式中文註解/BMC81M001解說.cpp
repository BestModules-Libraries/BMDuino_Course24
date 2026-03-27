/*
 * ==================================================
 * 檔案名稱：BMC81M001.cpp - BMC81M001 Wi-Fi 模組通訊驅動程式
 * 適用硬體：BMC81M001 Wi-Fi 模組（基於 ESP-12F）
 * 通訊介面：UART（支援硬體串口與軟體串口）
 * 
 * 程式功能說明：
 *   本程式為 BMC81M001 Wi-Fi 模組的通訊驅動程式，透過 UART 介面與模組進行 AT 指令通訊，
 *   提供完整的 Wi-Fi 連線管理、TCP/IP 通訊、MQTT 協定支援、HTTP GET 請求等功能。
 *   開發者可透過此驅動程式輕鬆實現物聯網應用的網路連線與資料傳輸。
 * 
 * 模組功能架構：
 *   ┌─────────────────────────────────────────────────────────────────┐
 *   │                    BMC81M001 Wi-Fi 模組驅動                      │
 *   ├─────────────────────────────────────────────────────────────────┤
 *   │ 1. 基礎通訊層                                                   │
 *   │    - 序列埠初始化 (begin)                                        │
 *   │    - 資料讀取與緩衝區管理 (readResponse, clearResponse)          │
 *   │    - AT 指令發送與回應處理 (sendATCommand, sendATCmd)            │
 *   ├─────────────────────────────────────────────────────────────────┤
 *   │ 2. Wi-Fi 連線管理層                                              │
 *   │    - 連線至無線基地台 (connectToAP)                              │
 *   │    - 取得連線狀態 (getStatus)                                    │
 *   │    - 取得網路參數 (getSSID, getIP, getMacAddress, etc.)         │
 *   ├─────────────────────────────────────────────────────────────────┤
 *   │ 3. TCP 通訊層                                                    │
 *   │    - TCP 連線建立 (connectTCP)                                   │
 *   │    - TCP 資料傳送 (writeDataTcp)                                 │
 *   │    - TCP 資料接收 (readDataTcp)                                  │
 *   ├─────────────────────────────────────────────────────────────────┤
 *   │ 4. MQTT 協定層                                                   │
 *   │    - MQTT 連線設定 (configMqtt)                                  │
 *   │    - 主題訂閱與發布 (setSubscribetopic, writeString)            │
 *   │    - MQTT 資料接收 (readIotData)                                 │
 *   ├─────────────────────────────────────────────────────────────────┤
 *   │ 5. HTTP 協定層                                                   │
 *   │    - HTTP GET 請求初始化 (http_begin)                            │
 *   │    - HTTP GET 請求執行 (http_get)                                │
 *   │    - HTTP 回應處理 (http_getString)                              │
 *   │    - 連線結束 (http_end)                                         │
 *   └─────────────────────────────────────────────────────────────────┘
 * 
 * AT 指令通訊流程：
 *   1. 發送指令：透過序列埠發送 AT 指令（結尾加 \r\n）
 *   2. 等待回應：讀取模組回應，等待 "OK" 或 "ERROR"
 *   3. 解析資料：從回應中提取需要的資訊（如 IP、SSID、MAC 等）
 *   4. 錯誤處理：逾時或錯誤時進行重試
 * 
 * 應用場景：
 *   - 物聯網感測器資料上傳（雲端平台）
 *   - MQTT 物聯網訊息傳輸（智慧家庭、工業控制）
 *   - Web API 資料獲取（天氣、股市、即時資訊）
 *   - 遠端設備控制與監控
 *   - 多裝置物聯網系統整合
 * 
 * 作者/版本：BEST MODULES CORP. / V1.0.4 (2024-8-22)
 * 修改紀錄：
 *   - 2025.03.27：加入詳細繁體中文註解與整體說明
 * ==================================================
 */

/*****************************************************************
檔案名稱：BMC81M001.cpp
公司：BEST MODULES CORP.
描述：BMC81M001 WiFi 模組通訊驅動程式
版本：V1.0.4-2024-8-22
功能：透過 UART 與 BMC81M001 WiFi 模組進行通訊
      包含 WiFi 連線、TCP/IP 通訊、MQTT 協議、HTTP 請求等功能
******************************************************************/

// ================================================================
// =============== 前置處理指令區 ===============
// ================================================================

// 引入平台相關變體定義（包含腳位定義、板型資訊等）
#include "variant.h"

// 引入模組標頭檔（包含類別定義和常數宣告）
#include "BMC81M001.h"

// ================================================================
// =============== 建構函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::BMC81M001 (建構函式1)
功能：使用硬體序列埠初始化 BMC81M001 物件
參數：
  *theSerial：硬體序列埠指標
    - BMduino 可選：serial（預設）、serial1、serial2、serial3、serial4
    - UNO 可選：serial（預設）
傳回值：無
說明：適用於使用硬體 UART 的情況，效能較穩定
**********************************************************/
BMC81M001::BMC81M001(HardwareSerial *theSerial)
{
    _softSerial = NULL;        // 軟體序列埠指標設為空（不使用軟體序列埠）
    _serial = theSerial;       // 設定硬體序列埠指標
}

/**********************************************************
函式名稱：BMC81M001::BMC81M001 (建構函式2)
功能：使用軟體序列埠初始化 BMC81M001 物件
參數：
  rxPin：UART 接收腳位（RX）
  txPin：UART 發送腳位（TX）
傳回值：無
說明：適用於使用 SoftwareSerial 的情況，適合硬體序列埠已被佔用的場景
      注意：軟體序列埠在高鮑率下可能不穩定
**********************************************************/
BMC81M001::BMC81M001(uint16_t rxPin, uint16_t txPin)
{
    _serial = NULL;                      // 硬體序列埠指標設為空
    _rxPin = rxPin;                      // 儲存接收腳位
    _txPin = txPin;                      // 儲存發送腳位
    _softSerial = new SoftwareSerial(_rxPin, _txPin);  // 動態建立軟體序列埠物件
}

// ================================================================
// =============== 基本通訊函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::begin
功能：初始化模組序列通訊
參數：
  baud：通訊鮑率（例如：9600、115200）
傳回值：無
說明：
  - 根據 _serial 指標是否為空來判斷使用硬體或軟體序列埠
  - 模組預設鮑率為 115200，建議使用此數值以獲得最佳效能
  - 此函式必須在呼叫任何其他功能前執行
**********************************************************/
void BMC81M001::begin(uint32_t baud)
{
    if (_serial != NULL) {
        // 使用硬體序列埠
        _serial->begin(baud);
    } else {
        // 使用軟體序列埠
        _softSerial->begin(baud);
    }
}

// ================================================================
// =============== WiFi 連線相關函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::connectToAP
功能：連線到指定的 WiFi 熱點（Access Point）
參數：
  ssid：WiFi 網路名稱（SSID）
  pass：WiFi 密碼
傳回值：
  SEND_SUCCESS（1）：連線成功
  SEND_FAIL（0）：連線失敗
流程：
  1. 設定模組為站點模式（Station Mode）- AT+CWMODE=1
  2. 使用 SSID 和密碼連線到熱點 - AT+CWJAP
  3. 等待連線完成並檢查回應
**********************************************************/
bool BMC81M001::connectToAP(String ssid, String pass)
{
    /* 步驟1：設定站點模式（Station Mode） */
    // 發送 AT+CWMODE=1 命令，將模組設為站點模式（客戶端模式）
    // 其他模式：0=休眠模式，1=站點模式，2=AP模式，3=站點+AP模式
    if (sendATCommand("AT+CWMODE=1", 1000, 3) == SEND_SUCCESS) {
        ;  // 成功，繼續執行
    } else {
        return SEND_FAIL;  // 失敗，直接回傳
    }

    /* 步驟2：連線到指定的 WiFi 熱點 */
    // 組裝 AT+CWJAP 命令字串
    // 格式：AT+CWJAP="SSID","PASSWORD"
    String cmd = "AT+CWJAP=\"";
    cmd += ssid;      // 加入 SSID
    cmd += "\",\"";
    cmd += pass;      // 加入密碼
    cmd += "\"";

    // 發送連線命令，等待連線完成
    // 連線可能需要較長時間（5-10秒），設定 1000ms 超時可能不足
    // 建議根據實際網路環境調整超時時間
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        return SEND_SUCCESS;  // 連線成功
    } else {
        return SEND_FAIL;     // 連線失敗
    }
}

/**********************************************************
函式名稱：BMC81M001::getSSID
功能：取得目前連線的 WiFi SSID 名稱
參數：無
傳回值：
  String：目前連線的 SSID 名稱
  "AT error"：AT 命令失敗或回應格式錯誤
  "No wifi connected"：未連線到任何 WiFi
說明：
  發送 AT+CWJAP? 命令取得連線資訊
  回應格式：+CWJAP:"SSID","BSSID","CHANNEL","RSSI"
  函式會解析回應並提取雙引號內的 SSID
**********************************************************/
String BMC81M001::getSSID()
{
    char *token;
    String result = "AT error";
    
    // 發送 AT+CWJAP? 命令取得連線狀態
    if (sendATCommand("AT+CWJAP?", 1000, 3) == SEND_SUCCESS) {
        // 使用 strtok 解析回應字串（尋找雙引號內的 SSID）
        // 回應格式範例：+CWJAP:"MyWiFi","xx:xx:xx:xx:xx:xx",1,-45
        token = strtok(BMC81M001Response, "\"");
        if (token != NULL) {
            token = strtok(NULL, "\"");
            if (token != NULL) {
                result = String(token);  // 取得 SSID
            } else {
                result = "No wifi connected";  // 未連線
            }
        }
    }
    return result;
}

/**********************************************************
函式名稱：BMC81M001::getStatus
功能：取得 WiFi 連線狀態
參數：無
傳回值：
  2：WIFI_STATUS_GOT_IP（已取得 IP 位址）
  3：WIFI_STATUS_CONNETED（已連線至 AP）
  4：WIFI_STATUS_DISCONNETED（已斷線）
  5：WIFI_STATUS_NO_CONNET（未連線）
  -1：COMMUNICAT_ERROR（通訊錯誤，無法與模組通訊）
  -2：AT_ACK_ERROR（AT 回應格式錯誤）
說明：
  發送 AT+CIPSTATUS 命令取得狀態碼
  狀態碼對應的連線階段：
    - 2：已取得 IP（DHCP 完成，可以進行網路通訊）
    - 3：已連線（已連接到 AP，但可能尚未取得 IP）
    - 4：已斷線（曾經連線但現在已斷開）
    - 5：未連線（從未連線或連線失敗）
**********************************************************/
int BMC81M001::getStatus()
{
    int result = COMMUNICAT_ERROR;
    
    // 發送 AT+CIPSTATUS 命令
    if (sendATCommand("AT+CIPSTATUS", 1000, 3) == SEND_SUCCESS) {
        // 在回應中尋找冒號後面的狀態碼
        // 回應格式範例：STATUS:3
        char *pos = strchr(BMC81M001Response, ':');
        if (pos != NULL) {
            result = atoi(&pos[1]);  // 將狀態碼轉換為整數
        } else {
            result = AT_ACK_ERROR;  // 回應格式錯誤
        }
    } else {
        result = COMMUNICAT_ERROR;  // 通訊失敗
    }
    return result;
}

/**********************************************************
函式名稱：BMC81M001::getIP
功能：取得目前連線的 IP 位址
參數：無
傳回值：
  String：IP 位址字串（例如："192.168.1.105"）
  "AT error"：AT 命令失敗或無法取得 IP
說明：
  發送 AT+CIPSTA? 命令取得 IP 設定
  回應格式：+CIPSTA:ip:"192.168.1.105"
  函式會解析回應並提取雙引號內的 IP 位址
**********************************************************/
String BMC81M001::getIP()
{
    String AckString = "AT error";
    char *token;
    
    // 發送 AT+CIPSTA? 命令取得 IP 設定
    if (sendATCommand("AT+CIPSTA?", 1000, 3) == SEND_SUCCESS) {
        // 解析回應字串取得 IP 位址
        token = strtok(BMC81M001Response, "\"");
        if (token != NULL) {
            token = strtok(NULL, "\"");
            if (token != NULL) {
                AckString = String(token);  // 取得 IP 位址
            }
        }
    }
    return AckString;
}

/**********************************************************
函式名稱：BMC81M001::getMacAddress
功能：取得 WiFi 模組的 MAC 位址
參數：無
傳回值：
  String：MAC 位址字串（例如："D8BFC0123456"，共 12 個字元）
  "AT error"：AT 命令失敗或無法取得 MAC
說明：
  發送 AT+CIPSTAMAC? 命令取得 MAC 位址
  回應格式：+CIPSTAMAC:"D8:BF:C0:12:34:56"
  函式會解析回應，移除冒號並轉為大寫後回傳
**********************************************************/
String BMC81M001::getMacAddress()
{
    String AckString = "AT error";
    char *token;
    char sourceStr[20] = "\0";
    
    // 發送 AT+CIPSTAMAC? 命令取得 MAC 位址
    if (sendATCommand("AT+CIPSTAMAC?", 1000, 3) == SEND_SUCCESS) {
        token = strtok(BMC81M001Response, "\"");
        if (token != NULL) {
            // 解析 MAC 位址（格式：XX:XX:XX:XX:XX:XX）
            for (uint8_t i = 0; i < 6; i++) {
                token = strtok(NULL, ":");
                if (token != NULL) {
                    strcat(sourceStr, token);  // 串接 MAC 位址片段（移除冒號）
                }
            }
            // 將所有字元轉為大寫，確保格式統一
            for (uint8_t i = 0; i < 12; i++) {
                sourceStr[i] = toupper(sourceStr[i]);
            }
            sourceStr[12] = '\0';  // 確保字串終止
            AckString = String(sourceStr);  // 轉換為 String 物件
        }
    }
    return AckString;
}

// ================================================================
// =============== MQTT 相關函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::configMqtt
功能：設定 MQTT 連線參數並連線到 MQTT Broker
參數：
  clientlid：MQTT 客戶端 ID（必須唯一，用於識別此客戶端）
  username：MQTT 使用者名稱（可選，無需認證可留空字串）
  password：MQTT 密碼（可選，無需認證可留空字串）
  mqtt_host：MQTT Broker 主機位址（例如："broker.emqx.io"）
  server_port：MQTT Broker 埠號（通常為 1883）
傳回值：
  SEND_SUCCESS：設定成功並連線
  SEND_FAIL：設定失敗或連線失敗
流程：
  1. 設定 MQTT 使用者屬性 (AT+MQTTUSERCFG)
  2. 設定 MQTT 客戶端 ID (AT+MQTTCLIENTID)
  3. 設定 MQTT 使用者名稱 (AT+MQTTUSERNAME)
  4. 設定 MQTT 密碼 (AT+MQTTPASSWORD)
  5. 連線到 MQTT Broker (AT+MQTTCONN)
**********************************************************/
bool BMC81M001::configMqtt(String clientlid, String username, String password, 
                           String mqtt_host, int server_port)
{
    String cmd;
    
    /*** 步驟1：設定 MQTT 使用者屬性 ***/
    // AT+MQTTUSERCFG 命令：設定連線屬性
    // 格式：AT+MQTTUSERCFG=<linkID>,<scheme>,<clientID>,<username>,<password>,<ssl_enable>,<ca_domain>,<certificate>
    // 這裡使用基本設定，不使用 SSL 加密
    cmd = "AT+MQTTUSERCFG=0,1,\"NULL\",\"NULL\",\"NULL\",0,0,\"\"";
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        ;  // 繼續執行
    } else {
        return SEND_FAIL;
    }
    
    /*** 步驟2：設定 MQTT 客戶端 ID ***/
    // AT+MQTTCLIENTID 命令：設定客戶端識別碼
    cmd = "AT+MQTTCLIENTID=0,\"";
    cmd += clientlid;
    cmd += "\"";
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        ;  // 繼續執行
    } else {
        return SEND_FAIL;
    }
    
    /*** 步驟3：設定 MQTT 使用者名稱 ***/
    // AT+MQTTUSERNAME 命令：設定認證使用者名稱
    cmd = "AT+MQTTUSERNAME=0,\"";
    cmd += username;
    cmd += "\"";
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        ;  // 繼續執行
    } else {
        return SEND_FAIL;
    }
    
    /*** 步驟4：設定 MQTT 密碼 ***/
    // AT+MQTTPASSWORD 命令：設定認證密碼
    cmd = "AT+MQTTPASSWORD=0,\"";
    cmd += password;
    cmd += "\"";
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        ;  // 繼續執行
    } else {
        return SEND_FAIL;
    }
    
    /*** 步驟5：連線到 MQTT Broker ***/
    // AT+MQTTCONN 命令：連線到 MQTT 伺服器
    // 格式：AT+MQTTCONN=<linkID>,<host>,<port>,<reconnect>
    cmd = "AT+MQTTCONN=0,\"";
    cmd += mqtt_host;
    cmd += "\",";
    cmd += server_port;
    cmd += ",0";  // 不自動重連（0：關閉，1：開啟）
    
    // 連線命令需要較長時間（設定 5 秒超時）
    if (sendATCommand(cmd, 5000, 3) == SEND_SUCCESS) {
        return SEND_SUCCESS;  // 連線成功
    } else {
        return SEND_FAIL;     // 連線失敗
    }
}

/**********************************************************
函式名稱：BMC81M001::writeString
功能：發送字串資料到 MQTT 主題
參數：
  Dbuffer：要發送的字串資料
  topic：MQTT 主題名稱
傳回值：
  SEND_SUCCESS：發送成功
  SEND_FAIL：發送失敗
說明：
  使用 AT+MQTTPUB 命令發佈訊息到指定主題
  格式：AT+MQTTPUB=<linkID>,<topic>,<data>,<qos>,<retain>
  - QoS=0：最多一次傳送（不確認）
  - Retain=0：不保留訊息
**********************************************************/
bool BMC81M001::writeString(String Dbuffer, String topic)
{
    // 組裝 AT+MQTTPUB 命令
    String cmd = "AT+MQTTPUB=0,\"";
    cmd += topic;
    cmd += "\",\"";
    cmd += Dbuffer;
    cmd += "\",0,0";  // QoS=0，Retain=0
    
    // 發送 MQTT 發佈命令
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        return SEND_SUCCESS;
    } else {
        return SEND_FAIL;
    }
}

/**********************************************************
函式名稱：BMC81M001::setSubscribetopic
功能：訂閱 MQTT 主題
參數：
  subscribetopic：要訂閱的主題名稱（可使用通配符 + 和 #）
傳回值：
  SEND_SUCCESS：訂閱成功
  SEND_FAIL：訂閱失敗
說明：
  使用 AT+MQTTSUB 命令訂閱主題
  格式：AT+MQTTSUB=<linkID>,<topic>,<qos>
  訂閱後，當該主題有訊息發布時，模組會自動收到 +MQTTSUBRECV 通知
**********************************************************/
bool BMC81M001::setSubscribetopic(String subscribetopic)
{
    String cmd;
    cmd = "AT+MQTTSUB=0,\"";
    cmd += subscribetopic;
    cmd += "\",0";  // QoS=0
    
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        return SEND_SUCCESS;
    } else {
        return SEND_FAIL;
    }
}

/**********************************************************
函式名稱：BMC81M001::readIotData
功能：讀取從 MQTT 訂閱收到的資料
參數：
  IotReciveBuff：接收資料的字串指標（輸出參數）
  IotReciveBufflen：接收資料長度的指標（輸出參數）
  topic：主題名稱的指標（輸出參數）
傳回值：無
說明：
  解析 +MQTTSUBRECV 回應格式：
  +MQTTSUBRECV:<linkID>,<topic>,<data_len>,<data>
  此函式會在序列埠緩衝區中尋找符合此格式的資料並解析
**********************************************************/
void BMC81M001::readIotData(String *IotReciveBuff, int *IotReciveBufflen, String *topic)
{
    char *newstr;
    
    // 清除回應緩衝區
    clearResponse(BMC81M001Response);
    *IotReciveBufflen = 0;
    
    // 讀取序列埠資料到緩衝區
    if (_serial != NULL) {
        if (_serial->available()) {
            delay(10);  // 等待資料完全到達
            while (_serial->available()) {
                uint8_t temp = _serial->read();
                BMC81M001Response[resLength++] = temp;
                if (resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
            }
        }
    } else {
        if (_softSerial->available()) {
            delay(10);
            while (_softSerial->available()) {
                BMC81M001Response[resLength++] = _softSerial->read();
                if (resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
            }
        }
    }
    
    // 檢查是否有 MQTT 訂閱資料
    if (resLength > 0) {
        if (strstr(BMC81M001Response, "+MQTTSUBRECV") != NULL) {
            // 使用 strtok 解析回應字串
            // 回應格式：+MQTTSUBRECV,<topic>,<data_len>,<data>
            newstr = strtok(BMC81M001Response, ",");
            if (newstr == NULL) return;
            
            newstr = strtok(NULL, ",");  // 取得主題名稱
            *topic = String(newstr);
            
            newstr = strtok(NULL, ",");  // 取得資料長度
            *IotReciveBufflen = atoi(newstr);
            
            newstr = strtok(NULL, "\r\n");  // 取得資料內容
            *IotReciveBuff = String(newstr);
            
            // 偵錯輸出（已註解，可視需要開啟）
            // Serial.print("topic:"); Serial.println(*topic);
            // Serial.print("IotReciveBufflen:"); Serial.println(*IotReciveBufflen);
            // Serial.print("IotReciveBuff:"); Serial.println(*IotReciveBuff);
        }
    }
}

// ================================================================
// =============== TCP 通訊相關函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::connectTCP
功能：連線到 TCP 伺服器
參數：
  ip：TCP 伺服器 IP 位址或網域名稱
  port：TCP 伺服器埠號
傳回值：
  SEND_SUCCESS：連線成功
  SEND_FAIL：連線失敗
說明：
  使用 AT+CIPSTART 命令建立 TCP 連線
  格式：AT+CIPSTART="TCP","<ip>",<port>
**********************************************************/
bool BMC81M001::connectTCP(String ip, int port)
{
    // 組裝 AT+CIPSTART 命令
    String cmd = "AT+CIPSTART=\"TCP\",\"";
    cmd += ip;
    cmd += "\",";
    cmd += port;
    
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        return SEND_SUCCESS;
    } else {
        return SEND_FAIL;
    }
}

/**********************************************************
函式名稱：BMC81M001::writeDataTcp
功能：發送資料到 TCP 伺服器
參數：
  Dlength：資料長度（位元組）
  Dbuffer[]：資料緩衝區（字元陣列）
傳回值：
  SEND_SUCCESS：發送成功
  SEND_FAIL：發送失敗
流程：
  1. 發送 AT+CIPSEND 命令指定發送長度
  2. 等待模組返回 ">" 提示符
  3. 發送實際資料內容
  4. 等待發送完成確認（"SEND OK"）
**********************************************************/
bool BMC81M001::writeDataTcp(int Dlength, char Dbuffer[])
{
    boolean found = SEND_SUCCESS;
    
    // 步驟1：設定發送資料長度
    String cmd = "AT+CIPSEND=";
    cmd += Dlength;
    if (sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) {
        ;  // 繼續執行
    } else {
        found = SEND_FAIL;
    }
    
    // 步驟2：發送實際資料
    delay(200);  // 等待模組準備接收資料（顯示 ">" 提示符）
    String S_Dbuffer = Dbuffer;
    if (sendATCommand(S_Dbuffer, 1000, 1) == SEND_SUCCESS) {
        found = SEND_SUCCESS;
    } else {
        found = SEND_FAIL;
    }
    
    return found;
}

/**********************************************************
函式名稱：BMC81M001::readDataTcp
功能：讀取從 TCP 伺服器接收的資料
參數：無
傳回值：
  String：接收到的資料字串（不含 +IPD 標頭）
說明：
  解析 +IPD 回應格式：+IPD,<len>:<data>
  此函式會在序列埠緩衝區中尋找 +IPD 開頭的資料並提取實際內容
**********************************************************/
String BMC81M001::readDataTcp()
{
    int commaPosition;
    String tcpBuf;
    
    // 清除回應緩衝區
    clearResponse(BMC81M001Response);
    
    // 從序列埠讀取資料
    if (_serial != NULL) {
        if (_serial->available()) {
            delay(10);
            while (_serial->available()) {
                BMC81M001Response[resLength++] = _serial->read();
                if (resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
            }
        }
    } else {
        if (_softSerial->available()) {
            delay(10);
            while (_softSerial->available()) {
                BMC81M001Response[resLength++] = _softSerial->read();
                if (resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
            }
        }
    }
    
    // 解析 TCP 資料
    if (resLength > 0) {
        if (strstr(BMC81M001Response, "+IPD") != NULL) {
            String S_Dbuffer = BMC81M001Response;
            
            // 尋找冒號位置（分隔長度和資料）
            // 格式：+IPD,<len>:<data>
            do {
                commaPosition = S_Dbuffer.indexOf(':');
                if (commaPosition != -1) {
                    // 取得冒號後面的資料部分
                    S_Dbuffer = S_Dbuffer.substring(commaPosition + 1, S_Dbuffer.length());
                } else {
                    // 沒有冒號表示已經是純資料
                    if (S_Dbuffer.length() > 0) {
                        tcpBuf = S_Dbuffer;
                    }
                }
            } while (commaPosition >= 0);
        }
    }
    
    return tcpBuf;
}

// ================================================================
// =============== HTTP 相關函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::http_begin
功能：開始 HTTP GET 請求準備工作
參數：
  serverURL：伺服器網域名稱（例如："http://example.com"）
  port：伺服器埠號（預設為 80 或 443）
  subURL：子路徑（例如："/api/data"，預設為 "/"）
傳回值：
  HTTP_GET_BEGIN_SUCCESS（0）：初始化成功
  HTTP_GET_URL_ERROR（-1）：URL 格式錯誤
說明：
  此函式會解析 URL，判斷使用 HTTP 或 HTTPS，
  並建構後續請求所需的 Host 標頭
**********************************************************/
int BMC81M001::http_begin(String serverURL, int port, String subURL)
{
    // 將 URL 轉換為字元陣列以便解析
    serverURL.toCharArray(BMC81M001Response, RES_MAX_LENGTH);
    
    // 步驟1：解析通訊協定類型
    char *token = strtok(BMC81M001Response, "//");
    if (token == NULL) return HTTP_GET_URL_ERROR;
    
    if (strstr(token, "http:") != NULL) {
        _port = 80;      // HTTP 預設埠
        _type = "TCP";   // 使用 TCP 通訊
    } else if (strstr(token, "https:") != NULL) {
        _port = 443;     // HTTPS 預設埠
        _type = "SSL";   // 使用 SSL 加密通訊
    } else {
        return HTTP_GET_URL_ERROR;
    }
    
    // 步驟2：解析主機名稱
    token = strtok(NULL, "//");
    _url = String(token);  // 儲存主機名稱
    
    // 步驟3：設定子路徑
    if (subURL == "") {
        _suburl = "/";  // 預設根路徑
    } else {
        _suburl = subURL;
    }
    
    // 步驟4：計算請求長度
    _len = subURL.length() + 4 + 9 + 2;  // "GET " + " HTTP/1.1\r\n" 的長度
    
    // 步驟5：建構 Host 標頭
    _host = "Host: ";
    _host += _url;
    
    // 如果埠號不是標準埠，需要加入埠號
    if (port != 80 && port != 443) {
        _port = port;
        _host += ":";
        _host += _port;
        _host += "\r\n\r\n";
    } else {
        _host += "\r\n\r\n";
    }
    
    // 計算總請求長度
    _len += _host.length();
    
    return HTTP_GET_BEGIN_SUCCESS;
}

/**********************************************************
函式名稱：BMC81M001::http_get
功能：執行 HTTP GET 請求
參數：無
傳回值：
  HTTP_GET_OP_SUCCESS（0）：請求成功
  HTTP_GET_OP_TIMEOUT（-2）：請求超時
  COMMUNICAT_ERROR：通訊錯誤
流程：
  1. 建立 TCP/SSL 連線
  2. 發送 HTTP GET 請求（GET 行 + Host 標頭）
  3. 接收伺服器回應
  4. 解析 HTTP 回應，提取內容部分
**********************************************************/
int BMC81M001::http_get(void)
{
    String cmd;
    int result = HTTP_GET_OP_SUCCESS;
    
    // 步驟1：建立連線
    cmd = "AT+CIPSTART=\"";
    cmd += _type;
    cmd += "\",\"";
    cmd += _url;
    cmd += "\",";
    cmd += _port;
    
    if (sendATCommand(cmd, 10000, 3) == SEND_SUCCESS) {
        // 步驟2：設定發送長度
        cmd = "AT+CIPSEND=";
        cmd += _len;
        sendATCommand(cmd, 1000, 3);
        
        // 步驟3：發送 HTTP GET 請求
        delay(100);  // 等待模組準備
        readResponse();  // 清除之前的回應
        
        // 發送 GET 請求行
        cmd = "GET ";
        cmd += _suburl;
        cmd += " HTTP/1.1\r\n";
        
        if (_softSerial != NULL) {
            _softSerial->print(cmd);
        } else {
            _serial->print(cmd);
        }
        
        delay(500);  // 等待模組發送資料
        
        // 發送 Host 標頭
        if (_softSerial != NULL) {
            _softSerial->print(_host);
        } else {
            _serial->print(_host);
        }
        
        // 步驟4：接收回應
        resLength = 0;
        int delay_count = 0;
        int blank_line_count = 0;
        
        while (1) {
            if (_serial->available()) {
                uint8_t temp;
                delay_count = 0;
                temp = _serial->read();
                
                // 解析 HTTP 標頭（尋找空行分隔標頭和內容）
                // HTTP 回應格式：狀態行\r\n標頭行\r\n...\r\n\r\n內容
                if (blank_line_count < 3) {
                    BMC81M001Response[resLength++] = temp;
                    if (temp == 0x0A) {  // 換行字元 \n
                        // 檢查是否為空行（僅包含 \r\n）
                        if (is_blank_line(BMC81M001Response) == true) {
                            blank_line_count++;  // 空行計數增加
                        }
                        clearResponse(BMC81M001Response);
                        resLength = 0;
                    }
                } else {
                    // 已跳過標頭，開始儲存回應內容
                    if (resLength < RES_MAX_LENGTH) {
                        BMC81M001Response[resLength++] = temp;
                    }
                }
            } else {
                delay(1);
                delay_count++;
                
                // 檢查超時（3 秒無資料）
                if (delay_count > 3000) {
                    if (blank_line_count == 3) {
                        result = HTTP_GET_OP_SUCCESS;  // 成功接收內容
                    } else {
                        result = HTTP_GET_OP_TIMEOUT;  // 接收超時
                    }
                    break;
                }
            }
        }
    } else {
        result = COMMUNICAT_ERROR;  // 連線失敗
    }
    
    return result;
}

/**********************************************************
函式名稱：is_blank_line
功能：檢查是否為空白行（僅包含換行/回車字元）
參數：
  line：要檢查的字串
傳回值：
  true：是空白行
  false：不是空白行
說明：
  HTTP 回應中空白行（\r\n）分隔標頭和內容
  此函式檢查一行內容是否只包含 \r 和 \n 字元
**********************************************************/
bool is_blank_line(const char *line)
{
    while (*line != '\0') {
        // 如果不是換行或回車字元，就不是空白行
        if (*line != '\r' && *line != '\n') {
            return false;
        }
        line++;
    }
    return true;
}

// ================================================================
// =============== AT 命令處理函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::sendATCommand
功能：發送 AT 命令到模組並等待回應
參數：
  StringstrCmd：AT 命令字串（會自動加上 \r\n）
  timeout：回應超時時間（毫秒）
  reTry：失敗時的重試次數
傳回值：
  SEND_SUCCESS：命令成功執行（收到 "OK"）
  SEND_FAIL：命令執行失敗（收到 "ERROR" 或逾時）
流程：
  1. 根據 reTry 參數進行多次嘗試
  2. 發送命令前先清除緩衝區
  3. 發送命令（自動加上 \r\n）
  4. 在超時時間內等待回應
  5. 檢查回應中是否包含 "OK"
  6. 若失敗則重試，直到達到重試次數上限
**********************************************************/
int BMC81M001::sendATCommand(String StringstrCmd, int timeout, uint8_t reTry)
{
    unsigned long t;
    char response[] = "OK";
    
    // 多次嘗試發送
    for (unsigned char tryTime = 0; tryTime < reTry; tryTime++) {
        readResponse();  // 清除之前的回應
        clearResponse(BMC81M001Response);  // 清空緩衝區
        
        // 發送 AT 命令（println 會自動加上 \r\n）
        if (_softSerial != NULL) {
            _softSerial->println(StringstrCmd);
        } else {
            _serial->println(StringstrCmd);
        }
        
        t = millis();  // 記錄開始時間
        
        // 等待回應
        for (uint8_t i = 0; ; i++) {
            readResponse();  // 讀取回應
            
            // 檢查是否收到 "OK"
            if (strstr(BMC81M001Response, response) != NULL) {
                return SEND_SUCCESS;  // 成功
            }
            
            // 檢查是否超時
            if ((t + timeout) < millis()) {
                break;  // 超時，跳出迴圈
            }
        }
    }
    
    return SEND_FAIL;  // 所有嘗試都失敗
}

// ================================================================
// =============== 資料讀取與緩衝區管理函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::readResponse
功能：從序列埠讀取資料到緩衝區
參數：無
傳回值：無
說明：
  - 檢查序列埠是否有可用資料
  - 將資料讀取到 BMC81M001Response 緩衝區
  - 如果緩衝區滿了，自動清除並從頭開始
  - 此函式為非阻塞式，僅讀取當前可用的資料
**********************************************************/
void BMC81M001::readResponse()
{
    if (_serial != NULL) {
        // 從硬體序列埠讀取
        while (_serial->available()) {
            uint8_t temp = _serial->read();
            BMC81M001Response[resLength++] = temp;
            // 如果緩衝區滿了，清除它（避免緩衝區溢出）
            if (resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
        }
    } else {
        // 從軟體序列埠讀取
        while (_softSerial->available()) {
            BMC81M001Response[resLength++] = _softSerial->read();
            // 如果緩衝區滿了，清除它
            if (resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
        }
    }
}

/**********************************************************
函式名稱：BMC81M001::clearResponse
功能：清除回應緩衝區
參數：
  Dbuffer[]：要清除的緩衝區
傳回值：無
說明：
  - 使用 memset 將緩衝區填充為空字元（\0）
  - 重設緩衝區長度計數器 resLength 為 0
  - 此函式應在每次發送新命令前呼叫，確保不會殘留舊資料
**********************************************************/
void BMC81M001::clearResponse(char Dbuffer[])
{
    memset(Dbuffer, '\0', RES_MAX_LENGTH);  // 清除緩衝區內容
    resLength = 0;  // 重設長度計數器
}

// ================================================================
// ===================== 使用注意事項 ============================
// ================================================================
/*
重要提醒：

1. 序列通訊設定：
   - 確保鮑率設定正確（模組預設為 115200）
   - 硬體序列埠效能較穩定，建議優先使用
   - 軟體序列埠可能會有時序問題，特別在高鮑率時
   - 使用前確認腳位配置與電壓準位（3.3V）

2. 記憶體管理：
   - BMC81M001Response 緩衝區大小為 RES_MAX_LENGTH（2000）
   - 長時間運作需注意記憶體碎片問題
   - 大型資料傳輸可能需要更大的緩衝區
   - 建議定期清除緩衝區避免資料混亂

3. 錯誤處理：
   - 所有函式都有回傳值，應檢查是否成功
   - AT 命令超時時間需根據實際情況調整
   - 重要的操作應加入重試機制（如 connectToAP）
   - 建議實作全域錯誤狀態記錄

4. MQTT 連線：
   - 確保 MQTT Broker 可達且埠號正確
   - 匿名連線可能被某些伺服器拒絕
   - 考慮實現遺言（Last Will）和保持連線（Keep Alive）
   - 定期發送心跳維持連線

5. HTTP 功能：
   - 僅支援 GET 請求，如需 POST 需自行擴充
   - HTTPS 需要模組韌體支援 SSL
   - 大型回應可能需要分次讀取
   - 注意 URL 編碼問題（空格、中文等）

6. 多工處理：
   - 模組同時處理多種通訊時需注意狀態切換
   - HTTP 操作後需呼叫 http_end() 釋放資源
   - 避免同時進行多個網路操作
   - 建議使用狀態機管理連線狀態

7. 電源管理：
   - WiFi 模組耗電較大（峰值可達 300mA）
   - 電池供電需注意電源穩定性
   - 可考慮實作睡眠模式節省電力（AT+GSLP）
   - 確保電源供應穩定，避免模組重啟

8. 韌體版本：
   - 不同韌體版本的 AT 命令可能不同
   - 使用前確認模組韌體版本（AT+GMR）
   - 更新韌體可能增加新功能或修復問題
   - 參考模組手冊確認指令相容性

9. 網路穩定性：
   - 加入斷線偵測和自動重連機制
   - 監控訊號強度（AT+CWJAP? 可取得 RSSI）
   - 實現連線逾時和重試策略
   - 考慮實作斷線緩衝機制

10. 安全性：
    - 生產環境應使用加密連線（SSL/TLS）
    - 避免在程式碼中硬編碼敏感資訊（密碼、金鑰）
    - 定期更新模組韌體以修補安全漏洞
    - 考慮使用隨機客戶端 ID 避免衝突
*/