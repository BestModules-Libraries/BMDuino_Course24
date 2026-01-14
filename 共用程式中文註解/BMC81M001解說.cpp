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

// 引入平台相關變體定義
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
說明：適用於使用硬體 UART 的情況
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
  rxPin：UART 接收腳位
  txPin：UART 發送腳位
傳回值：無
說明：適用於使用 SoftwareSerial 的情況
**********************************************************/
BMC81M001::BMC81M001(uint16_t rxPin, uint16_t txPin)
{
    _serial = NULL;                      // 硬體序列埠指標設為空
    _rxPin = rxPin;                      // 儲存接收腳位
    _txPin = txPin;                      // 儲存發送腳位
    _softSerial = new SoftwareSerial(_rxPin, _txPin);  // 建立軟體序列埠物件
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
  - 如果使用硬體序列埠，初始化硬體序列埠
  - 如果使用軟體序列埠，初始化軟體序列埠
  - 根據 _serial 指標是否為空來判斷使用哪種序列埠
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
  ssid：WiFi 網路名稱
  pass：WiFi 密碼
傳回值：
  SEND_SUCCESS（1）：連線成功
  SEND_FAIL（0）：連線失敗
流程：
  1. 設定模組為站點模式（Station Mode）
  2. 使用 SSID 和密碼連線到熱點
**********************************************************/
bool BMC81M001::connectToAP(String ssid, String pass)
{
    /* 步驟1：設定站點模式 */
    // 發送 AT+CWMODE=1 命令，將模組設為站點模式
    if (sendATCommand("AT+CWMODE=1", 1000, 3) == SEND_SUCCESS) {
        ;  // 成功，繼續執行
    } else {
        return SEND_FAIL;  // 失敗，直接回傳
    }

    /* 步驟2：連線到指定的 WiFi 熱點 */
    // 組裝 AT+CWJAP 命令字串
    String cmd = "AT+CWJAP=\"";
    cmd += ssid;      // 加入 SSID
    cmd += "\",\"";
    cmd += pass;      // 加入密碼
    cmd += "\"";

    // 發送連線命令
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
  "AT error"：AT 命令失敗
  "No wifi connected"：未連線到任何 WiFi
說明：發送 AT+CWJAP? 命令取得連線資訊
**********************************************************/
String BMC81M001::getSSID()
{
    char *token;
    String result = "AT error";
    
    // 發送 AT+CWJAP? 命令取得連線狀態
    if (sendATCommand("AT+CWJAP?", 1000, 3) == SEND_SUCCESS) {
        // 使用 strtok 解析回應字串（尋找雙引號內的 SSID）
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
  2：WIFI_STATUS_GOT_IP（已取得 IP）
  3：WIFI_STATUS_CONNECTED（已連線）
  4：WIFI_STATUS_DISCONNECTED（已斷線）
  5：WIFI_STATUS_NO_CONNECT（未連線）
  -1：COMMUNICAT_ERROR（通訊錯誤）
  -2：AT_ACK_ERROR（AT 回應錯誤）
說明：發送 AT+CIPSTATUS 命令取得狀態碼
**********************************************************/
int BMC81M001::getStatus()
{
    int result = COMMUNICAT_ERROR;
    
    // 發送 AT+CIPSTATUS 命令
    if (sendATCommand("AT+CIPSTATUS", 1000, 3) == SEND_SUCCESS) {
        // 在回應中尋找冒號後面的狀態碼
        char *pos = strchr(BMC81M001Response, ':');
        if (pos != NULL) {
            result = atoi(&pos[1]);  // 轉換為整數
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
  "AT error"：AT 命令失敗
說明：發送 AT+CIPSTA? 命令取得 IP 設定
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
  String：MAC 位址字串（例如："D8BFC0123456"）
  "AT error"：AT 命令失敗
說明：發送 AT+CIPSTAMAC? 命令取得 MAC 位址
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
                    strcat(sourceStr, token);  // 串接 MAC 位址片段
                }
            }
            // 將所有字元轉為大寫
            for (uint8_t i = 0; i < 12; i++) {
                sourceStr[i] = toupper(sourceStr[i]);
            }
            sourceStr[12] = '\0';  // 確保字串終止
            AckString = String(sourceStr);  // 轉換為 String
        }
    }
    return AckString;
}

// ================================================================
// =============== MQTT 相關函式區 ===============
// ================================================================

/**********************************************************
函式名稱：BMC81M001::configMqtt
功能：設定 MQTT 連線參數
參數：
  clientlid：MQTT 客戶端 ID
  username：MQTT 使用者名稱
  password：MQTT 密碼
  mqtt_host：MQTT 伺服器主機名稱
  server_port：MQTT 伺服器埠號
傳回值：
  SEND_SUCCESS：設定成功
  SEND_FAIL：設定失敗
流程：
  1. 設定 MQTT 使用者屬性
  2. 設定 MQTT 客戶端 ID
  3. 設定 MQTT 使用者名稱
  4. 設定 MQTT 密碼
  5. 連線到 MQTT Broker
**********************************************************/
bool BMC81M001::configMqtt(String clientlid, String username, String password, 
                           String mqtt_host, int server_port)
{
    String cmd;
    
    /*** 步驟1：設定 MQTT 使用者屬性 ***/
    // AT+MQTTUSERCFG 命令：設定連線屬性
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
說明：使用 AT+MQTTPUB 命令發佈訊息到指定主題
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
  subscribetopic：要訂閱的主題名稱
傳回值：
  SEND_SUCCESS：訂閱成功
  SEND_FAIL：訂閱失敗
說明：使用 AT+MQTTSUB 命令訂閱主題
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
說明：解析 +MQTTSUBRECV 回應格式：
      +MQTTSUBRECV:<linkID>,<topic>,<data_len>,<data>
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
            delay(10);
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
            newstr = strtok(BMC81M001Response, ",");
            if (newstr == NULL) return;
            
            newstr = strtok(NULL, ",");  // 取得主題名稱
            *topic = String(newstr);
            
            newstr = strtok(NULL, ",");  // 取得資料長度
            *IotReciveBufflen = atoi(newstr);
            
            newstr = strtok(NULL, "\r\n");  // 取得資料內容
            *IotReciveBuff = String(newstr);
            
            // 偵錯輸出（已註解）
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
  ip：TCP 伺服器 IP 位址
  port：TCP 伺服器埠號
傳回值：
  SEND_SUCCESS：連線成功
  SEND_FAIL：連線失敗
說明：使用 AT+CIPSTART 命令建立 TCP 連線
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
  Dlength：資料長度
  Dbuffer[]：資料緩衝區
傳回值：
  SEND_SUCCESS：發送成功
  SEND_FAIL：發送失敗
流程：
  1. 發送 AT+CIPSEND 命令指定發送長度
  2. 發送實際資料內容
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
    delay(200);  // 等待模組準備接收資料
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
  String：接收到的資料字串
說明：解析 +IPD 回應格式：+IPD,<len>:<data>
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
  1. 解析 URL 取得通訊協定類型（HTTP/HTTPS）
  2. 解析主機名稱
  3. 建構 HTTP 請求標頭
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
        _type = "SSL";   // 使用 SSL 通訊
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
  2. 發送 HTTP GET 請求
  3. 接收伺服器回應
  4. 解析 HTTP 回應
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
        readResponse();
        
        cmd = "GET ";
        cmd += _suburl;
        cmd += " HTTP/1.1\r\n";
        
        // 發送請求行
        if (_softSerial != NULL) {
            _softSerial->print(cmd);
        } else {
            _serial->print(cmd);
        }
        
        delay(500);
        
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
                if (blank_line_count < 3) {
                    BMC81M001Response[resLength++] = temp;
                    if (temp == 0x0A) {  // 換行字元
                        // 檢查是否為空行
                        if (is_blank_line(BMC81M001Response) == true) {
                            blank_line_count++;  // 空行計數增加
                        }
                        clearResponse(BMC81M001Response);
                        resLength = 0;
                    }
                } else {
                    // 儲存回應內容
                    if (resLength < RES_MAX_LENGTH) {
                        BMC81M001Response[resLength++] = temp;
                    }
                }
            } else {
                delay(1);
                delay_count++;
                
                // 檢查超時
                if (delay_count > 3000) {
                    if (blank_line_count == 3) {
                        result = HTTP_GET_OP_SUCCESS;  // 成功接收
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
功能：檢查是否為空白行（僅包含換行字元）
參數：
  line：要檢查的字串
傳回值：
  true：是空白行
  false：不是空白行
說明：HTTP 回應中空白行分隔標頭和內容
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
功能：發送 AT 命令到模組
參數：
  StringstrCmd：AT 命令字串
  timeout：回應超時時間（毫秒）
  reTry：重試次數
傳回值：
  SEND_SUCCESS：命令成功執行
  SEND_FAIL：命令執行失敗
流程：
  1. 多次嘗試發送命令（根據 reTry 參數）
  2. 在超時時間內等待回應
  3. 檢查回應中是否包含 "OK"
**********************************************************/
int BMC81M001::sendATCommand(String StringstrCmd, int timeout, uint8_t reTry)
{
    unsigned long t;
    char response[] = "OK";
    
    // 多次嘗試發送
    for (unsigned char tryTime = 0; tryTime < reTry; tryTime++) {
        readResponse();  // 清除之前的回應
        clearResponse(BMC81M001Response);  // 清空緩衝區
        
        // 發送 AT 命令
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
  - 如果緩衝區滿了，自動清除
**********************************************************/
void BMC81M001::readResponse()
{
    if (_serial != NULL) {
        // 從硬體序列埠讀取
        while (_serial->available()) {
            uint8_t temp = _serial->read();
            BMC81M001Response[resLength++] = temp;
            // 如果緩衝區滿了，清除它
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
  - 使用 memset 將緩衝區填充為空字元
  - 重設緩衝區長度計數器
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
   - 確保鮑率設定正確（通常為 115200）
   - 硬體序列埠效能較穩定，建議優先使用
   - 軟體序列埠可能會有時序問題，特別在高鮑率時

2. 記憶體管理：
   - BMC81M001Response 緩衝區大小為 RES_MAX_LENGTH（200）
   - 長時間運作需注意記憶體碎片問題
   - 大型資料傳輸可能需要更大的緩衝區

3. 錯誤處理：
   - 所有函式都有回傳值，應檢查是否成功
   - AT 命令超時時間需根據實際情況調整
   - 重要的操作應加入重試機制

4. MQTT 連線：
   - 確保 MQTT Broker 可達且埠號正確
   - 匿名連線可能被某些伺服器拒絕
   - 考慮實現遺言（Last Will）和保持連線（Keep Alive）

5. HTTP 功能：
   - 僅支援 GET 請求，如需 POST 需自行擴充
   - HTTPS 需要模組支援 SSL
   - 大型回應可能需要分次讀取

6. 多工處理：
   - 模組同時處理多種通訊時需注意狀態切換
   - HTTP 操作後需呼叫 http_end() 釋放資源
   - 避免同時進行多個網路操作

7. 電源管理：
   - WiFi 模組耗電較大，電池供電需注意
   - 可考慮實作睡眠模式節省電力
   - 確保電源供應穩定，避免重啟

8. 韌體版本：
   - 不同韌體版本的 AT 命令可能不同
   - 使用前確認模組韌體版本
   - 更新韌體可能增加新功能或修復問題

9. 網路穩定性：
   - 加入斷線偵測和自動重連機制
   - 監控訊號強度（RSSI）
   - 實現連線逾時和重試

10. 安全性：
    - 生產環境應使用加密連線（SSL/TLS）
    - 避免在程式碼中硬編碼敏感資訊
    - 定期更新模組韌體以修補安全漏洞
*/