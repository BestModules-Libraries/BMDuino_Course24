
//----------外部引用函式區----------------
#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來處理 JSON 序列化與反序列化


/********************* 感測器物件宣告 ************************/
StaticJsonDocument<200> doc; // 建立一個靜態 JSON 文件物件，最大容量 200 bytes，用來暫存要送出或解析的 JSON 資料


/********************* 全域變數 ************************/
// ---------------------------------------------------------------------
// ------------- 宣告 MQTT Broker 伺服器登入與連線相關常數 ----------------
// ---------------------------------------------------------------------
#define CLIENTLID  "aaaLiwJDBccDxULGxUwDiANEjs"   // 預設的 MQTT Client ID（目前實際上未使用，改用動態產生 clintid）
#define USERNAME   ""                             // MQTT 登入使用者帳號（空字串代表匿名或不需帳密）
#define PASSWORD   ""                             // MQTT 登入密碼
#define MQTT_HOST  "broker.emqx.io"              // MQTT Broker 主機名稱或 IP
#define SERVER_PORT 1883                         // MQTT Broker 連線埠（1883 為未加密的標準 MQTT Port）

/********************* 全域變數 ************************/
// ---------------------------------------------------------------------
// --------------------- 格式化字串與 Topic 變數區 -----------------------
// ---------------------------------------------------------------------
// Publish & Subscribe 使用之主題格式
// String topic ;  // 舊寫法：單一 topic 字串，已註解不用

// topic 以 C 語言格式字串表示，%s 會在程式內用 MAC Address 取代
String topic = "/arduino/PIR/%s" ;            // 未實際使用的 String 版本（保留作參考）
const char* PubTop = "/arduino/PIR/%s" ;      // 發佈用 Topic 模板，例如 "/arduino/PIR/48E729732158"
const char* SubTop = "/arduino/PIR/#" ;       // 訂閱用 Topic 模板，# 表示通配符訂閱所有後綴主題

String TopicT;                // 暫存組好的 Topic（String 版本）
char SubTopicbuffer[200];     // 儲存實際訂閱主題的 C-style 字元陣列
char PubTopicbuffer[200];     // 儲存實際發佈主題的 C-style 字元陣列
String PayloadT;              // JSON Payload 的字串版本
char Payloadbuffer[250];      // JSON Payload 的 C-style 字元陣列版本（方便做字元插入處理）
char clintid[20];             // MQTT ClientID 用的字元陣列，長度 20（注意：MAC 太長可能會被截斷）

// ---------------------------------------------------------------------
// ---------------------------- 常數宣告區 --------------------------------
// ---------------------------------------------------------------------
#define DEB_CNT       80      // 去抖動計數上限（假設 50ms 為單位，此處註解僅供參考）
#define RES_MAX_LENGTH 200    // 最大接收緩衝區長度（用於接收資料的 buffer 上限）

// ---------------------------------------------------------------------
// --------------------------- 通訊暫存變數區 ------------------------------
// ---------------------------------------------------------------------
// int resLen;                // 若用 Serial 緩衝時，可記錄接收資料長度（目前註解不用）
String ReciveBuff;           // 用來接收 MQTT 或其他通訊資料的字串緩衝
int ReciveBufflen;           // 實際接收到的資料長度

// String DATA_BUF;          // 備用的資料緩衝區（目前未使用）


// ---------------------------------------------------------------------
// -------------------------- 自訂函式宣告區 -----------------------------
// ---------------------------------------------------------------------
void fillCID(String mm);                    // 依據 MAC 字串動態產生 MQTT Broker ClientID
void fillTopic(String mm);                  // 依據 MAC Address 填入 Publish/Subscribe 對應的 Topic
void insertBeforeChar(char *str, char target, char toInsert); // 在字串中每個指定字元前插入另一個字元（用於跳脫字元）
void fillPayload(String dev, float d1, float d2); // 依據 MAC Address & 溫度 & 溼度產生 JSON Payload（目前整段註解）
void initMQTT();                           // 初始化並連線到 MQTT Server



// =====================================================================
// =============== 產生動態 MQTT Broker ClientID 的函式 ==================
// =====================================================================
void fillCID(String mm) // mm 通常為 MAC Address 字串
{
  // 目標：產生類似 "tw48E729732158" 的 ClientID
  // clintid[0] 與 clintid[1] 固定為 't','w'，之後接上 MAC 字串
  clintid[0] = 't';   // ClientID 第一個字元
  clintid[1] = 'w';   // ClientID 第二個字元
  // 將 mm 這個 String 轉成 C-style 字串，從 clintid[2] 開始放
  // mm.length() + 1 是因為結尾還要放 '\0'
  mm.toCharArray(&clintid[2], mm.length() + 1);
  // 在字串最後一格再放 '\n'（換行），這裡實務上較少這樣使用 ClientID，
  // 但您原本程式如此撰寫，這行會在字串尾端新增一個換行符號
  clintid[2 + mm.length() + 1] = '\n';
  // 在序列埠顯示產生後的 Client ID
  Serial.print("Client ID:(");
  Serial.print(clintid);
  Serial.print(") \n");
}

// =====================================================================
// ====== 根據 MAC Address 建立 Publish 與 Subscribe 專屬 Topic ==========
// =====================================================================
void fillTopic(String mm) // mm 通常為 MAC Address 字串
{
  // 使用 sprintf 將 PubTop 內的 "%s" 以 mm.c_str() 取代
  // 例如：PubTop="/arduino/PIR/%s"，mm="48E729732158"
  // 產生結果 "/arduino/PIR/48E729732158"
  sprintf(PubTopicbuffer, PubTop, mm.c_str());
  Serial.print("Publish Topic Name:(");
  Serial.print(PubTopicbuffer);              // 顯示組好的發佈主題
  Serial.print("/");
  Serial.print(sizeof(PubTopicbuffer));      // 顯示 PubTopicbuffer 陣列大小（固定 200）
  Serial.print("^");
  Serial.print(String(PubTopicbuffer));      // 將 C-style 字串轉成 String 再印一次
  Serial.print("/");
  Serial.print(sizeof(String(PubTopicbuffer))); // String 物件大小（與字串長度不同）
  Serial.print(") \n");
  // 訂閱主題，SubTop 是 "/arduino/PIR/#"
  // 這裡使用 sprintf 雖然帶 mm.c_str()，但 SubTop 裡沒有 %s，
  // 所以實際填入的是 "/arduino/PIR/#"
  sprintf(SubTopicbuffer, SubTop, mm.c_str());
  Serial.print("Subscribe Topic Name:(");
  Serial.print(SubTopicbuffer);   // "/arduino/PIR/#"
  Serial.print("/");
  Serial.print(SubTop);           // 原始模板字串
  Serial.print(") \n");
}

// =====================================================================
// ====== 在字串中，每碰到指定 target 字元前插入 toInsert 字元的函式 =====
// =====================================================================
void insertBeforeChar(char *str, char target, char toInsert) 
{
  int len = strlen(str);   // 目前字串長度（不含 '\0'）
  uint8_t count = 0;       // 計數：已插入幾個字元（字串會變長）

  // 迴圈條件 len+count：因為每插入一個新字元，實際有效字串長度會增加 1
  for (int j = 0; j < len + count; j++) {

    if (str[j] == target)  // 當前位置字元等於目標 target
    {
      count++;             // 將插入數量加 1

      // 從字串尾巴（len+count）往後搬移一格，為插入新字元騰出空間
      for (int i = len + count; i >= j; i--) {
        str[i + 1] = str[i];
      }

      str[j] = toInsert;   // 在原來位置插入 toInsert 字元
      j++;                 // 跳過剛插入的字元，繼續往後檢查
    }
  }
}


// =====================================================================
// ========== 產生 MQTT Payload(JSON) 的函式（目前整段註解） ============
// =====================================================================
void fillPayload(String dev, float d1, float d2)  // 根據 MAC+溫度+濕度 產生 JSON 並跳脫字元
{
  Serial.println("Fill Pay LOAD is Processing");

  // 建立 JSON 內容
  doc["Device"]      = dev;  // 裝置 ID（通常為 MAC）
  doc["Temperature"] = d1;   // 溫度
  doc["Humidity"]    = d2;   // 濕度

  // 將 JSON 物件序列化為字串，存入 PayloadT
  serializeJson(doc, PayloadT);

  // 將 String 轉為 char[]，以利 insertBeforeChar 操作
  PayloadT.toCharArray(Payloadbuffer, PayloadT.length() + 1);

  // 在每個雙引號前插入 '\'，例如 " 變成 \" （適用 AT 指令中需要跳脫字元的場合）
  insertBeforeChar(Payloadbuffer, '\"', '\\');
  // 在每個逗號前插入 '\'，例如 , 變成 \,（視通訊協定而定）
  insertBeforeChar(Payloadbuffer, ',', '\\');

  // 將處理過的 char[] 再轉回 String 存回 PayloadT
  PayloadT = String(Payloadbuffer);

  Serial.print("Sending:(");
  Serial.print(PayloadT);
  Serial.print(")\n");
}

// =====================================================================
// ======================= 初始化 MQTT Server ==========================
// =====================================================================
void initMQTT() // 初始化並連線 MQTT Broker
{
  // 1. 產生 ClientID（以 MAC 為基礎）
  fillCID(MacData);

  // 2. 依照 MAC 填入要用的 Topic
  fillTopic(MacData);

  // 3. 將連線所需參數印出，方便除錯
  Serial.print("clintid:(");
  Serial.print(clintid);
  Serial.print(")\n");

  Serial.print("USERNAME:(");
  Serial.print(USERNAME);
  Serial.print(")\n");

  Serial.print("PASSWORD:(");
  Serial.print(PASSWORD);
  Serial.print(")\n");

  Serial.print("MQTT_HOST:(");
  Serial.print(MQTT_HOST);
  Serial.print(")\n");

  Serial.print("SERVER_PORT:(");
  Serial.print(SERVER_PORT);
  Serial.print(")\n");

  Serial.println("Now Connect MQTT Broker.....");

  // 4. 呼叫 Wifi.configMqtt() 設定並連線到 MQTT Broker
  //    傳入參數：ClientID, USERNAME, PASSWORD, HOST, PORT
  if (Wifi.configMqtt(String(clintid), USERNAME, PASSWORD, MQTT_HOST, SERVER_PORT) != 0)
  {
    // 回傳值非 0 表示成功（依您函式庫實作）
    Serial.println("Connect to MQTT Server Successful");
  }
  else 
  {
    // 若連線失敗，印出錯誤並停在 while(1) 無限迴圈
    Serial.println("Connect to MQTT Server failed");
    while (1);  // 永久停在這裡，代表系統無法繼續運作
  }

  // 5. 設定預設訂閱主題（範例直接用 SubTop="/arduino/PIR/#"）
  Wifi.setSubscribetopic(SubTop);

  // 備註：
  //   這裡可以再搭配 mqttclient.setCallback(callback);
  //   讓有資料進來時呼叫指定的 callback 函式處理。
}


