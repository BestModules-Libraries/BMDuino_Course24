/*-------------------------------------------------------------
  程式名稱：BMC81M001 WiFi 通訊模組驅動程式實作
  程式用途：本檔案為 BMC81M001.h 標頭檔的實作部分，實現了
           透過 AT 指令控制 BMC81M001 WiFi 模組的所有功能，
           包括 WiFi 連線、TCP 通訊、MQTT 協定、HTTP GET 請求等。
  
  硬體架構：支援 BMduino 或一般 Arduino 開發板，可選擇使用
           硬體序列埠（HardwareSerial）或軟體序列埠（SoftwareSerial）
           與模組通訊。
  
  作者說明：本程式為 BEST MODULES CORP. 開發之 BMC81M001 WiFi 模組
           驅動程式，版本 V1.0.4 (2024-8-22)。
  
  主要函式實作：
    1. 建構子：支援硬體序列埠與軟體序列埠兩種初始化方式
    2. WiFi 連線：connectToAP()、getStatus()、getIP() 等
    3. TCP 通訊：connectTCP()、writeDataTcp()、readDataTcp()
    4. MQTT 協定：configMqtt()、writeString()、readIotData()
    5. HTTP 客戶端：http_begin()、http_get()、http_getString()
    6. AT 指令控制：sendATCommand()、sendATCmd()
  
  最後修改：2024年8月22日
  版本：V1.0.4
-------------------------------------------------------------*/

#include "variant.h"           // 引入開發板變體定義檔（BMduino 專用）
/*************************************************
File:             BMC81M001.cpp
Author:           BEST MODULES CORP.
Description:      UART communication with the BMC81M001 
version:          V1.0.4-2024-8-22
**************************************************/
#include "BMC81M001.h"         // 引入類別宣告頭檔

/**********************************************************
 * 函式名稱：BMC81M001（硬體序列埠建構子）
 * 功能說明：使用硬體序列埠（HardwareSerial）初始化 BMC81M001 物件
 * 輸入參數：*theSerial - 指向硬體序列埠的指標，預設為 Serial
 *           BMduino 可選：Serial（預設）/Serial1/Serial2/Serial3/Serial4
 *           Arduino UNO 可選：Serial（預設）
 * 回傳值：無
 * 說明：將軟體序列埠指標設為 NULL，並儲存硬體序列埠指標
 **********************************************************/
BMC81M001::BMC81M001(HardwareSerial *theSerial)
{
  _softSerial = NULL;          // 軟體序列埠指標設為空（表示未使用）
  _serial = theSerial;         // 儲存硬體序列埠指標
}

/**********************************************************
 * 函式名稱：BMC81M001（軟體序列埠建構子）
 * 功能說明：使用軟體序列埠（SoftwareSerial）初始化 BMC81M001 物件
 * 輸入參數：rxPin - UART 接收腳位（RX）
 *           txPin - UART 傳送腳位（TX）
 * 回傳值：無
 * 說明：將硬體序列埠指標設為 NULL，動態建立 SoftwareSerial 物件
 **********************************************************/
BMC81M001::BMC81M001(uint16_t rxPin, uint16_t txPin)
{
  _serial = NULL;              // 硬體序列埠指標設為空（表示未使用）
  _rxPin = rxPin;              // 儲存 RX 腳位
  _txPin = txPin;              // 儲存 TX 腳位
  _softSerial = new SoftwareSerial(_rxPin, _txPin);  // 動態建立軟體序列埠物件
}

/**********************************************************
 * 函式名稱：begin
 * 功能說明：初始化模組序列通訊，設定通訊速率
 * 輸入參數：baud - 通訊速率，預設為 BMC81M001_baudRate (115200)
 * 回傳值：無
 * 說明：根據 _serial 是否為 NULL 決定使用硬體或軟體序列埠
 **********************************************************/
void BMC81M001::begin(uint32_t baud)
{
  if(_serial != NULL)          // 如果使用硬體序列埠
  {
    _serial->begin(baud);      // 初始化硬體序列埠
  }
  else                         // 如果使用軟體序列埠
  {
    _softSerial->begin(baud);  // 初始化軟體序列埠
  }
}

/**********************************************************
 * 函式名稱：connectToAP
 * 功能說明：連線至指定的 WiFi 基地台（Access Point）
 * 輸入參數：ssid - 基地台名稱（WiFi SSID）
 *           pass - 基地台密碼（WiFi Password）
 * 回傳值：布林值，SEND_SUCCESS(1) 表示連線成功，SEND_FAIL(0) 表示失敗
 * 說明：先設定工作站模式（AT+CWMODE=1），再執行連線指令（AT+CWJAP）
 **********************************************************/
bool BMC81M001::connectToAP(String ssid, String pass)
{  
  /* 發送 AT+CWMODE=1 指令，設定為工作站模式（Station Mode） */
  if(sendATCommand("AT+CWMODE=1", 1000, 3) == SEND_SUCCESS)
  {
    // 設定成功，繼續執行
  }
  else 
  {
    return SEND_FAIL;          // 設定失敗，回傳失敗
  }
  
  /* 發送 AT+CWJAP="ssid","password" 指令，連線至指定基地台 */
  String cmd = "AT+CWJAP=\"";
  cmd += ssid;                 // 加入基地台名稱
  cmd += "\",\"";
  cmd += pass;                 // 加入密碼
  cmd += "\"";
  
  if(sendATCommand(cmd, 1000, 3) == SEND_SUCCESS) 
  {
    return SEND_SUCCESS;       // 連線成功
  }
  else 
  {
    return SEND_FAIL;          // 連線失敗
  }
}

/**********************************************************
 * 函式名稱：connectTCP
 * 功能說明：建立 TCP 連線至指定的伺服器
 * 輸入參數：ip - 伺服器 IP 位址（字串格式，如 "192.168.1.100"）
 *           port - 伺服器埠號（如 8080）
 * 回傳值：布林值，SEND_SUCCESS(1) 表示連線成功，SEND_FAIL(0) 表示失敗
 * 說明：發送 AT+CIPSTART 指令建立 TCP 連線
 **********************************************************/
bool BMC81M001::connectTCP(String ip, int port)
{  
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += ip;                   // 加入 IP 位址
  cmd += "\",";
  cmd += port;                 // 加入埠號
  
  if(sendATCommand(cmd, 1000, 3) == SEND_SUCCESS)
  {
    return SEND_SUCCESS;       // 連線成功
  }
  else 
  {
    return SEND_FAIL;          // 連線失敗
  }
}

/**********************************************************
 * 函式名稱：writeDataTcp
 * 功能說明：透過已建立的 TCP 連線發送資料
 * 輸入參數：Dlength - 要發送的資料長度（bytes）
 *           Dbuffer[] - 儲存要發送資料的緩衝區
 * 回傳值：布林值，SEND_SUCCESS(1) 表示發送成功，SEND_FAIL(0) 表示失敗
 * 說明：先發送 AT+CIPSEND 指令告知資料長度，再發送實際資料內容
 **********************************************************/
bool BMC81M001::writeDataTcp(int Dlength, char Dbuffer[])
{
  boolean found = SEND_SUCCESS;
  
  /* 發送 AT+CIPSEND=<length> 指令，告知模組準備接收資料 */
  String cmd = "AT+CIPSEND=";
  cmd += Dlength;
  
  if(sendATCommand(cmd, 1000, 3) == SEND_SUCCESS)
  {
    // 指令發送成功，繼續執行
  }
  else 
  {
    found = SEND_FAIL;         // 指令發送失敗
  }
  
  delay(200);                  // 等待模組準備就緒
  
  /* 發送實際資料內容 */
  String S_Dbuffer = Dbuffer;
  if(sendATCommand(S_Dbuffer, 1000, 1) == SEND_SUCCESS)  
  {
    found = SEND_SUCCESS;      // 資料發送成功
  }
  else 
  {
    found = SEND_FAIL;         // 資料發送失敗
  }
  
  return found;
}

/**********************************************************
 * 函式名稱：readDataTcp
 * 功能說明：從 TCP 連線讀取接收到的資料
 * 輸入參數：無
 * 回傳值：String 型態，讀取到的資料內容
 * 說明：解析模組回應中的 +IPD 指令，提取實際資料內容
 **********************************************************/
String BMC81M001::readDataTcp()
{
  int commaPosition;
  String tcpBuf;
  
  clearResponse(BMC81M001Response);  // 清空回應緩衝區
  
  /* 從對應的序列埠讀取資料 */
  if(_serial != NULL)
  {
    if(_serial->available())
    {
      delay(10);
      while(_serial->available())
      {
        BMC81M001Response[resLength++] = _serial->read();
        if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
      }
    }
  }
  else
  {
    if(_softSerial->available())
    {
      delay(10);
      while(_softSerial->available())
      {
        BMC81M001Response[resLength++] = _softSerial->read();
        if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
      }
    }
  }
  
  /* 解析回應內容，提取 +IPD 後的資料 */
  if(resLength > 0)    
  {
    if(strstr(BMC81M001Response, "+IPD") != NULL)
    {
      String S_Dbuffer = BMC81M001Response;
      do
      {
        commaPosition = S_Dbuffer.indexOf(':');
        if(commaPosition != -1)
        {
          // 跳過 ":" 前的部分，只保留實際資料
          S_Dbuffer = S_Dbuffer.substring(commaPosition + 1, S_Dbuffer.length());
        }
        else
        {
          if(S_Dbuffer.length() > 0)
            tcpBuf = S_Dbuffer;
        }
      }
      while(commaPosition >= 0);
    }
  }
  
  return tcpBuf;
}

/**********************************************************
 * 函式名稱：configMqtt
 * 功能說明：設定 MQTT 連線參數並連接至 MQTT Broker
 * 輸入參數：
 *   clientlid - 客戶端識別碼（Client ID）
 *   username - 使用者名稱（若無驗證可留空）
 *   password - 密碼（若無驗證可留空）
 *   mqtt_host - MQTT Broker 主機位址
 *   server_port - MQTT Broker 埠號
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：依序設定 MQTT 使用者配置、Client ID、使用者名稱、密碼，
 *       最後發送連線指令至 Broker
 **********************************************************/
bool BMC81M001::configMqtt(String clientlid, String username, String password, String mqtt_host, int server_port)
{
  /* 步驟 1：設定 MQTT 使用者配置
   * 指令格式：AT+MQTTUSERCFG=<linkid>, <scheme>, <"client_id">, <"username">, <"password">, <cert_key_id>, <ca_id>, <"path">
   */
  String cmd = "AT+MQTTUSERCFG=0,1,\"NULL\",\"NULL\",\"NULL\",0,0,\"\"";
  if(sendATCommand(cmd, 1000, 3) != SEND_SUCCESS)
  {
    return SEND_FAIL;
  }
  
  /* 步驟 2：設定 MQTT Client ID
   * 指令格式：AT+MQTTCLIENTID=<LinkID>,<"client_id">
   */
  cmd = "AT+MQTTCLIENTID=0,\"";
  cmd += clientlid;
  cmd += "\"";
  if(sendATCommand(cmd, 1000, 3) != SEND_SUCCESS)
  {
    return SEND_FAIL;
  }
  
  /* 步驟 3：設定 MQTT 使用者名稱
   * 指令格式：AT+MQTTUSERNAME=<LinkID>,<"username">
   */
  cmd = "AT+MQTTUSERNAME=0,\"";
  cmd += username;
  cmd += "\"";
  if(sendATCommand(cmd, 1000, 3) != SEND_SUCCESS)
  {
    return SEND_FAIL;
  }
  
  /* 步驟 4：設定 MQTT 密碼
   * 指令格式：AT+MQTTPASSWORD=<LinkID>,<"password">
   */
  cmd = "AT+MQTTPASSWORD=0,\"";
  cmd += password;
  cmd += "\"";
  if(sendATCommand(cmd, 1000, 3) != SEND_SUCCESS)
  {
    return SEND_FAIL;
  }
  
  /* 步驟 5：連接 MQTT Broker
   * 指令格式：AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
   */
  cmd = "AT+MQTTCONN=0,\"";
  cmd += mqtt_host;
  cmd += "\",";
  cmd += server_port;
  cmd += ",0";
  
  if(sendATCommand(cmd, 5000, 3) == SEND_SUCCESS)
  {
    return SEND_SUCCESS;       // 連線成功
  }
  else 
  {
    return SEND_FAIL;          // 連線失敗
  }
}

/**********************************************************
 * 函式名稱：setPublishTopic
 * 功能說明：設定 MQTT 發布主題（Publish Topic）
 * 輸入參數：publishtopic - 要發布的主題名稱
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：發送 AT+MQTTSUB 指令設定訂閱主題
 **********************************************************/
bool BMC81M001::setPublishTopic(String publishtopic)
{
  String cmd;
  cmd = "AT+MQTTSUB=0,\"";
  cmd += publishtopic;
  cmd += "\",0";
  
  if(sendATCommand(cmd, 1000, 3) == SEND_SUCCESS)
  {
    return SEND_SUCCESS;
  }
  else 
  {
    return SEND_FAIL;
  }
}

/**********************************************************
 * 函式名稱：setSubscribetopic
 * 功能說明：設定 MQTT 訂閱主題（Subscribe Topic）
 * 輸入參數：subscribetopic - 要訂閱的主題名稱
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：發送 AT+MQTTSUB 指令設定訂閱主題
 **********************************************************/
bool BMC81M001::setSubscribetopic(String subscribetopic)
{
  String cmd;
  cmd = "AT+MQTTSUB=0,\"";
  cmd += subscribetopic;
  cmd += "\",0";
  
  if(sendATCommand(cmd, 1000, 3) == SEND_SUCCESS)
  {
    return SEND_SUCCESS;
  }
  else 
  {
    return SEND_FAIL;
  }
}

/**********************************************************
 * 函式名稱：setTopic
 * 功能說明：設定自訂 MQTT 主題（通用版本）
 * 輸入參數：topic - 主題名稱
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 **********************************************************/
bool BMC81M001::setTopic(String topic)
{
  String cmd;
  cmd = "AT+MQTTSUB=0,\"";
  cmd += topic;
  cmd += "\",0";
  
  if(sendATCommand(cmd, 1000, 3) == SEND_SUCCESS)
  {
    return SEND_SUCCESS;
  }
  else 
  {
    return SEND_FAIL;
  }
}

/**********************************************************
 * 函式名稱：writeString
 * 功能說明：透過 MQTT 發送字串資料至指定主題
 * 輸入參數：Dbuffer - 要發送的字串內容
 *           topic - 主題名稱
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：發送 AT+MQTTPUB 指令，將資料發布至指定主題
 **********************************************************/
bool BMC81M001::writeString(String Dbuffer, String topic)
{
  String cmd = "AT+MQTTPUB=0,\"";
  cmd += topic;
  cmd += "\",\"";
  cmd += Dbuffer;
  cmd += "\",0,0";
  
  if(sendATCommand(cmd, 1000, 3) != SEND_SUCCESS)
  {
    return SEND_FAIL;
  }
  
  return SEND_SUCCESS;
}

/**********************************************************
 * 函式名稱：writeBytes
 * 功能說明：透過 MQTT 發送二進位資料至指定主題
 * 輸入參數：Dbuffer[] - 儲存要發送資料的緩衝區
 *           Dlength - 資料長度（bytes）
 *           topic - 主題名稱
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：先發送 AT+MQTTPUBRAW 指令告知資料長度，再發送實際二進位資料
 **********************************************************/
bool BMC81M001::writeBytes(char Dbuffer[], int Dlength, String topic)
{
  /* 發送 AT+MQTTPUBRAW 指令，準備發送二進位資料 */
  String cmd = "AT+MQTTPUBRAW=0,\"";
  cmd += topic;
  cmd += "\",";
  cmd += Dlength;
  cmd += ",0,0";
  
  if(sendATCommand(cmd, 1000, 3) != SEND_SUCCESS)
  {
    return SEND_FAIL;
  }
  
  /* 發送實際的二進位資料內容 */
  for(uint8_t i = 0; i < Dlength; i++)
  {
    if(_softSerial != NULL)
    {
      _softSerial->print(Dbuffer[i]);   // 使用軟體序列埠發送
    }
    else
    {
      _serial->print(Dbuffer[i]);       // 使用硬體序列埠發送
    }
  }
  
  return SEND_SUCCESS;
}

/**********************************************************
 * 函式名稱：readIotData
 * 功能說明：讀取 MQTT 訂閱主題接收到的物聯網資料
 * 輸入參數：
 *   IotReciveBuff - 接收到的資料內容（透過指標回傳）
 *   IotReciveBufflen - 接收資料的長度（透過指標回傳）
 *   topic - 接收資料的主題名稱（透過指標回傳）
 * 回傳值：無
 * 說明：解析模組回應中的 +MQTTSUBRECV 指令，提取主題名稱、資料長度和實際內容
 **********************************************************/
void BMC81M001::readIotData(String *IotReciveBuff, int *IotReciveBufflen, String *topic)
{
  String ReciveTopic;
  int commaPosition;
  char data_div = 0;
  String S_Dbuffer, S_Dbuffer1;
  
  clearResponse(BMC81M001Response);      // 清空回應緩衝區
  *IotReciveBufflen = 0;                 // 初始化資料長度
  
  /* 從對應的序列埠讀取資料 */
  if(_serial != NULL)
  {
    if(_serial->available())
    {
      delay(10);
      while(_serial->available())
      {
        uint8_t temp;
        temp = _serial->read();
        BMC81M001Response[resLength++] = temp;
        if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
      }
    }
  }
  else
  {
    if(_softSerial->available())
    {
      delay(10);
      while(_softSerial->available())
      {
        BMC81M001Response[resLength++] = _softSerial->read();
        if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
      }
    }
  }
  
  /* 解析回應內容，提取 +MQTTSUBRECV 後的資料 */
  if(resLength > 0)    
  {
    char *newstr;
    if(strstr(BMC81M001Response, "+MQTTSUBRECV") != NULL)
    {
      // 使用 strtok 分割字串，提取各欄位資料
      newstr = strtok(BMC81M001Response, ",");      // 跳過 "+MQTTSUBRECV"
      if(newstr == NULL) return;
      
      newstr = strtok(NULL, ",");                   // 提取 LinkID（可忽略）
      if(newstr == NULL) return;
      
      newstr = strtok(NULL, ",");                   // 提取主題名稱
      *topic = String(newstr);
      
      newstr = strtok(NULL, ",");                   // 提取資料長度
      *IotReciveBufflen = atoi(newstr);
      
      newstr = strtok(NULL, "\r\n");                // 提取實際資料內容
      *IotReciveBuff = String(newstr);
    }
  }
}

/**********************************************************
 * 函式名稱：reset
 * 功能說明：重置 BMC81M001 模組（軟體重置）
 * 輸入參數：無
 * 回傳值：布林值，SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：發送 AT+RST 指令重置模組，並等待模組重新啟動
 **********************************************************/
bool BMC81M001::reset(void)
{
  boolean found = SEND_SUCCESS;
  
  if(sendATCommand("AT+RST", 1000, 3) == SEND_SUCCESS)
  {
    clearResponse(BMC81M001Response);   // 清空回應緩衝區
    delay(2000);                        // 等待模組重置完成（約 2 秒）
  }
  else 
  {
    found = SEND_FAIL;
  }
  
  return found;
}

/**********************************************************
 * 函式名稱：sendATCommand
 * 功能說明：發送 AT 指令並等待回應，檢查是否包含 "OK"
 * 輸入參數：
 *   StringstrCmd - 要發送的 AT 指令字串
 *   timeout - 等待回應的超時時間（毫秒）
 *   reTry - 失敗時的重試次數
 * 回傳值：SEND_SUCCESS(1) 表示成功，SEND_FAIL(0) 表示失敗
 * 說明：重試機制確保通訊可靠性，收到 "OK" 即表示指令執行成功
 **********************************************************/
int BMC81M001::sendATCommand(String StringstrCmd, int timeout, uint8_t reTry)
{
  unsigned long t;
  char response[] = "OK";
  
  /* 在指定重試次數內重複發送指令 */
  for(unsigned char tryTime = 0; tryTime < reTry; tryTime++)
  {
    readResponse();                      // 讀取可能的殘留資料
    clearResponse(BMC81M001Response);    // 清空回應緩衝區
    
    /* 發送 AT 指令 */
    if(_softSerial != NULL)
    {
      _softSerial->println(StringstrCmd);
    }
    else
    {
      _serial->println(StringstrCmd);
    }
    
    t = millis();                        // 記錄發送時間
    
    /* 在超時時間內檢查回應內容 */
    for(uint8_t i = 0; ; i++)
    {
      readResponse();                    // 讀取模組回應
      
      /* 檢查回應中是否包含 "OK" */
      if(strstr(BMC81M001Response, response) != NULL)
      {
        return SEND_SUCCESS;             // 指令執行成功
      }
      
      /* 超時檢查 */
      if((t + timeout) < millis())
      {
        break;                           // 超時，跳出內層迴圈
      }
    }
  }
  
  return SEND_FAIL;                      // 所有重試都失敗
}

/**********************************************************
 * 函式名稱：sendATCmd
 * 功能說明：發送 AT 指令並回傳完整的回應內容
 * 輸入參數：
 *   StringstrCmd - 要發送的 AT 指令字串
 *   timeout - 等待回應的超時時間（毫秒）
 *   reTry - 失敗時的重試次數
 * 回傳值：String 型態，包含完整回應內容，超時則回傳 "TimeOut"
 * 說明：與 sendATCommand 類似，但會回傳完整的回應字串
 **********************************************************/
String BMC81M001::sendATCmd(String StringstrCmd, int timeout, uint8_t reTry)
{
  unsigned long t;
  char response[] = "OK";
  String AckString;
  
  /* 在指定重試次數內重複發送指令 */
  for(unsigned char tryTime = 0; tryTime < reTry; tryTime++)
  {
    readResponse();                      // 讀取可能的殘留資料
    clearResponse(BMC81M001Response);    // 清空回應緩衝區
    
    /* 發送 AT 指令 */
    if(_softSerial != NULL)
    {
      _softSerial->println(StringstrCmd);
    }
    else
    {
      _serial->println(StringstrCmd);
    }
    
    t = millis();                        // 記錄發送時間
    
    /* 在超時時間內檢查回應內容 */
    for(uint8_t i = 0; ; i++)
    {
      readResponse();                    // 讀取模組回應
      
      /* 檢查回應中是否包含 "OK" */
      if(strstr(BMC81M001Response, response) != NULL)
      {
        AckString = String(BMC81M001Response);  // 回傳完整回應
        return AckString;
      }
      
      /* 超時檢查 */
      if((t + timeout) < millis())
      {
        AckString = "TimeOut";           // 超時
        break;
      }
    }
  }
  
  return AckString;
}

/**********************************************************
 * 函式名稱：SSID
 * 功能說明：掃描並取得周圍 WiFi 基地台資訊
 * 輸入參數：無
 * 回傳值：String 型態，包含周圍 WiFi 資訊
 * 說明：發送 AT+CWLAP 指令掃描周圍基地台
 **********************************************************/
String BMC81M001::SSID()
{
  String result = "AT error";
  if(sendATCommand("AT+CWLAP", 5000, 3) == SEND_SUCCESS)
  {
    result = String(BMC81M001Response);  // 回傳掃描結果
  }
  return result;
}

/**********************************************************
 * 函式名稱：getSSID
 * 功能說明：取得目前連線的 WiFi 基地台名稱
 * 輸入參數：無
 * 回傳值：String 型態，目前連線的基地台名稱，未連線則回傳 "No wifi connected"
 * 說明：發送 AT+CWJAP? 指令查詢目前連線的基地台
 **********************************************************/
String BMC81M001::getSSID()
{
  char *token;
  String result = "AT error";
  
  if(sendATCommand("AT+CWJAP?", 1000, 3) == SEND_SUCCESS)
  {
    token = strtok(BMC81M001Response, "\"");  // 尋找第一個雙引號
    if(token != NULL)
    {
      token = strtok(NULL, "\"");             // 取得雙引號內的 SSID
      if(token != NULL)
      {
        result = String(token);                // 回傳 SSID
      }
      else
      {
        result = "No wifi connected";          // 未連線
      }
    }
  }
  return result;
}

/**********************************************************
 * 函式名稱：getStatus
 * 功能說明：取得 WiFi 連線狀態
 * 輸入參數：無
 * 回傳值：狀態碼
 *   WIFI_STATUS_GOT_IP (2) - 已取得 IP 位址
 *   WIFI_STATUS_CONNETED (3) - 已連線
 *   WIFI_STATUS_DISCONNETED (4) - 已斷線
 *   WIFI_STATUS_NO_CONNET (5) - 未連線
 *   COMMUNICAT_ERROR (-1) - 通訊錯誤
 *   AT_ACK_ERROR (-2) - AT 指令回應錯誤
 * 說明：發送 AT+CIPSTATUS 指令查詢連線狀態
 **********************************************************/
int BMC81M001::getStatus()
{
  int result = COMMUNICAT_ERROR;
  
  if(sendATCommand("AT+CIPSTATUS", 1000, 3) == SEND_SUCCESS)
  {
    char *pos = strchr(BMC81M001Response, ':');  // 尋找 ":" 位置
    if(pos != NULL)
    {
      result = atoi(&pos[1]);                    // 轉換為整數狀態碼
    }
    else
    {
      result = AT_ACK_ERROR;                     // 回應格式錯誤
    }
  }
  else
  {
    result = COMMUNICAT_ERROR;                   // 通訊失敗
  }
  
  return result;
}

/**********************************************************
 * 函式名稱：getIP
 * 功能說明：取得模組的 IP 位址
 * 輸入參數：無
 * 回傳值：String 型態，IP 位址（如 "192.168.1.100"）
 * 說明：發送 AT+CIPSTA? 指令查詢 IP 位址
 **********************************************************/
String BMC81M001::getIP()
{
  String AckString = "AT error";
  char *token;
  
  if(sendATCommand("AT+CIPSTA?", 1000, 3) == SEND_SUCCESS)
  {
    token = strtok(BMC81M001Response, "\"");     // 尋找第一個雙引號
    if(token != NULL)
    {
      token = strtok(NULL, "\"");                // 取得雙引號內的 IP
      if(token != NULL)
      {
        AckString = String(token);               // 回傳 IP 位址
      }
    }
  }
  
  return AckString;
}

/**********************************************************
 * 函式名稱：getGateway
 * 功能說明：取得目前連線的網關位址（Gateway）
 * 輸入參數：無
 * 回傳值：String 型態，網關位址
 * 說明：發送 AT+CIPSTA? 指令，提取第三個雙引號內的網關位址
 **********************************************************/
String BMC81M001::getGateway()
{
  String AckString = "AT error";
  char *token;
  uint8_t i;
  
  if(sendATCommand("AT+CIPSTA?", 1000, 3) == SEND_SUCCESS)
  {
    token = strtok(BMC81M001Response, "\"");
    if(token != NULL)
    {
      /* 跳過前兩個欄位（IP 和 gateway 前的文字），取得第三個雙引號內容 */
      for(i = 0; i < 3; i++)
      {
        token = strtok(NULL, "\"");
      }
      if(token != NULL)
      {
        AckString = String(token);               // 回傳網關位址
      }
    }
  }
  
  return AckString;
}

/**********************************************************
 * 函式名稱：getMask
 * 功能說明：取得目前連線的子網路遮罩（Subnet Mask）
 * 輸入參數：無
 * 回傳值：String 型態，子網路遮罩
 * 說明：發送 AT+CIPSTA? 指令，提取第五個雙引號內的遮罩位址
 **********************************************************/
String BMC81M001::getMask()
{
  String AckString = "AT error";
  char *token;
  uint8_t i;
  
  if(sendATCommand("AT+CIPSTA?", 1000, 3) == SEND_SUCCESS)
  {
    token = strtok(BMC81M001Response, "\"");
    if(token != NULL)
    {
      /* 跳過前五個欄位，取得第六個雙引號內容（netmask） */
      for(i = 0; i < 5; i++)
      {
        token = strtok(NULL, "\"");
      }
      if(token != NULL)
      {
        AckString = String(token);               // 回傳子網路遮罩
      }
    }
  }
  
  return AckString;
}

/**********************************************************
 * 函式名稱：getMacAddress
 * 功能說明：取得模組的 MAC 位址
 * 輸入參數：無
 * 回傳值：String 型態，MAC 位址（12 位十六進制字串）
 * 說明：發送 AT+CIPSTAMAC? 指令，將 MAC 位址轉為大寫並去除分隔符號
 **********************************************************/
String BMC81M001::getMacAddress()
{
  String AckString = "AT error";
  char *token;
  char sourceStr[20] = "\0";
  uint8_t i;
  
  if(sendATCommand("AT+CIPSTAMAC?", 1000, 3) == SEND_SUCCESS)
  {
    token = strtok(BMC81M001Response, "\"");
    if(token != NULL)
    {
      /* 提取 MAC 位址的各個部分（以 ":" 分隔） */
      for(i = 0; i < 6; i++)
      {
        token = strtok(NULL, ":");
        if(token != NULL)
        {
          strcat(sourceStr, token);              // 合併各段
          /* 轉換為大寫字母 */
          for(uint8_t i = 0; i < 12; i++)
          {
            sourceStr[i] = toupper(sourceStr[i]);
          }
        }
      }
      sourceStr[12] = '\0';                      // 字串結束符
      AckString = String(sourceStr);             // 回傳 MAC 位址
    }
  }
  
  return AckString;
}

/**********************************************************
 * 函式名稱：getATVersion
 * 功能說明：取得 AT 指令韌體版本
 * 輸入參數：無
 * 回傳值：String 型態，版本資訊
 * 說明：發送 AT+GMR 指令查詢版本號
 **********************************************************/
String BMC81M001::getATVersion()
{
  String AckString = "AT error";
  char sourceStr[20];
  char *pos1, *pos2;
  
  if(sendATCommand("AT+GMR", 1000, 3) == SEND_SUCCESS)
  {
    pos1 = strchr(BMC81M001Response, ':');       // 尋找 ":" 位置
    pos2 = strchr(BMC81M001Response, '-');       // 尋找 "-" 位置
    memcpy(sourceStr, pos1 + 1, pos2 - pos1 - 1); // 複製版本號
    AckString = String(sourceStr);               // 回傳版本資訊
  }
  
  return AckString;
}

/**********************************************************
 * 函式名稱：http_begin
 * 功能說明：初始化 HTTP GET 請求，解析 URL 並儲存相關參數
 * 輸入參數：
 *   serverURL - 伺服器完整 URL（如 "http://example.com/api"）
 *   port - 伺服器埠號（通常為 80 或 443）
 *   subURL - 子路徑（可選，預設為 "/"）
 * 回傳值：HTTP_GET_BEGIN_SUCCESS (0) 表示成功，HTTP_GET_URL_ERROR (-1) 表示 URL 錯誤
 * 說明：解析 URL 中的協議、主機名稱和路徑，為後續 HTTP GET 請求做準備
 **********************************************************/
int BMC81M001::http_begin(String serverURL, int port, String subURL)
{
  serverURL.toCharArray(BMC81M001Response, RES_MAX_LENGTH);
  
  /* 解析 URL，尋找 "//" 分隔符 */
  char *token = strtok(BMC81M001Response, "//");
  if(token == NULL) return HTTP_GET_URL_ERROR;
  
  /* 判斷協議類型（http 或 https） */
  if(strstr(token, "http:") != NULL)
  {
    _port = 80;
    _type = "TCP";
  }
  else if(strstr(token, "https:") != NULL)
  {
    _port = 443;
    _type = "TCP";
  }
  else
  {
    return HTTP_GET_URL_ERROR;      // 不支援的協議
  }
  
  /* 取得主機名稱 */
  token = strtok(NULL, "//");
  _url = String(token);
  
  /* 設定子路徑 */
  if(subURL == "")
    _suburl = "/";
  else
    _suburl = subURL;
  
  /* 計算 HTTP 請求長度（用於 CIPSEND） */
  _len = subURL.length() + 4 + 9 + 2;  // GET / HTTP/1.1 的固定長度
  
  /* 建構 Host 標頭 */
  _host = "Host: ";
  _host += _url;
  if(port != 80 && port != 443)
  {
    _port = port;
    _host += ":";
    _host += _port;
    _host += "\r\n\r\n";
  }
  else
  {
    _host += "\r\n\r\n";
  }
  _len += _host.length();
  
  return HTTP_GET_BEGIN_SUCCESS;
}

/**********************************************************
 * 函式名稱：is_blank_line
 * 功能說明：檢查字串是否為空行（僅包含 \r 和 \n）
 * 輸入參數：line - 要檢查的字串
 * 回傳值：布林值，true 表示為空行，false 表示不是空行
 * 說明：用於 HTTP 回應解析，判斷標頭結束位置
 **********************************************************/
bool is_blank_line(const char *line)
{
  while(*line != '\0')
  {
    if(*line != '\r' && *line != '\n')
    {
      return false;                // 包含非換行字元，非空行
    }
    line++;
  }
  return true;                     // 僅包含換行字元，為空行
}

/**********************************************************
 * 函式名稱：http_get
 * 功能說明：執行 HTTP GET 請求，取得伺服器回應
 * 輸入參數：無
 * 回傳值：HTTP_GET_OP_SUCCESS (0) 表示成功，COMMUNICAT_ERROR (-1) 表示通訊錯誤，
 *         HTTP_GET_OP_TIMEOUT (-2) 表示超時
 * 說明：建立 TCP 連線，發送 HTTP GET 請求，並解析回應內容
 **********************************************************/
int BMC81M001::http_get(void)
{
  String cmd;
  int result = HTTP_GET_OP_SUCCESS;
  
  /* 建立 TCP 連線 */
  cmd = "AT+CIPSTART=\"";
  cmd += _type;
  cmd += "\",\"";
  cmd += _url;
  cmd += "\",";
  cmd += _port;
  
  if(sendATCommand(cmd, 10000, 3) == SEND_SUCCESS)
  {
    /* 設定發送資料長度 */
    cmd = "AT+CIPSEND=";
    cmd += _len;
    sendATCommand(cmd, 1000, 3);
    delay(100);                    // 等待 ">" 提示符號
    readResponse();
    
    /* 發送 HTTP GET 請求 */
    cmd = "GET ";
    cmd += _suburl;
    cmd += " HTTP/1.1\r\n";
    
    if(_softSerial != NULL)
    {
      _softSerial->print(cmd);
    }
    else
    {
      _serial->print(cmd);
    }
    
    delay(500);
    
    /* 發送 Host 標頭 */
    if(_softSerial != NULL)
    {
      _softSerial->print(_host);
    }
    else
    {
      _serial->print(_host);
    }
    
    /* 接收伺服器回應 */
    resLength = 0;
    int delay_count = 0;
    int blank_line_count = 0;
    
    while(1)
    {
      if(_serial->available())
      {
        uint8_t temp;
        delay_count = 0;
        temp = _serial->read();
        
        if(blank_line_count < 3)   // 尚未找到標頭結束位置
        {
          BMC81M001Response[resLength++] = temp;
          if(temp == 0x0A)          // 換行字元，表示一行結束
          {
            if(is_blank_line(BMC81M001Response) == true)
            {
              blank_line_count++;    // 計數連續的空行
            }
            clearResponse(BMC81M001Response);
            resLength = 0;
          }
        }
        else                       // 標頭已結束，開始讀取實際資料
        {
          if(resLength < RES_MAX_LENGTH)
          {
            BMC81M001Response[resLength++] = temp;
          }
        }
      }
      else
      {
        delay(1);
        delay_count++;
        if(delay_count > 3000)      // 等待超時（3 秒）
        {
          if(blank_line_count == 3)
          {
            result = HTTP_GET_OP_SUCCESS;      // 成功接收
          }
          else
          {
            result = HTTP_GET_OP_TIMEOUT;      // 超時
          }
          break;
        }
      }
    }
  }
  else
  {
    result = COMMUNICAT_ERROR;      // TCP 連線失敗
  }
  
  return HTTP_GET_OP_SUCCESS;
}

/**********************************************************
 * 函式名稱：http_getString
 * 功能說明：取得 HTTP GET 請求的回應內容（字串格式）
 * 輸入參數：無
 * 回傳值：String 型態，回應內容
 * 說明：在執行 http_get() 後呼叫，取得已儲存的回應資料
 **********************************************************/
String BMC81M001::http_getString(void)
{
  if(resLength < RES_MAX_LENGTH)
  {
    BMC81M001Response[resLength] = '\0';  // 加入字串結束符
  }
  return String(BMC81M001Response);       // 回傳回應內容
}

/**********************************************************
 * 函式名稱：http_end
 * 功能說明：結束 HTTP 操作，關閉 TCP 連線
 * 輸入參數：無
 * 回傳值：無
 * 說明：必須在每次 HTTP 操作後呼叫，否則無法執行下一次 HTTP 操作
 **********************************************************/
void BMC81M001::http_end(void)
{
  sendATCommand("AT+CIPCLOSE", 1000, 3);   // 關閉 TCP 連線
}

/**********************************************************
 * 函式名稱：readResponse
 * 功能說明：從模組讀取資料並存入回應緩衝區
 * 輸入參數：無
 * 回傳值：無
 * 說明：持續讀取序列埠可用的資料，直到無資料可讀
 **********************************************************/
void BMC81M001::readResponse()
{
  if(_serial != NULL)
  {
    while(_serial->available())
    {
      uint8_t temp;
      temp = _serial->read();
      BMC81M001Response[resLength++] = temp;
      if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
    }
  }
  else
  {
    while(_softSerial->available())
    {
      BMC81M001Response[resLength++] = _softSerial->read();
      if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
    }
  }
}

/**********************************************************
 * 函式名稱：clearResponse
 * 功能說明：清空回應緩衝區
 * 輸入參數：Dbuffer[] - 要清空的緩衝區
 * 回傳值：無
 * 說明：將緩衝區全部設為 '\0'，並重設長度計數器
 **********************************************************/
void BMC81M001::clearResponse(char Dbuffer[])
{
  memset(Dbuffer, '\0', RES_MAX_LENGTH);  // 將緩衝區填滿 '\0'
  resLength = 0;                          // 重設長度計數器
}