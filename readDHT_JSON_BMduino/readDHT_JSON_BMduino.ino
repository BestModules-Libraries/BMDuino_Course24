// 匯入所需函式庫 ------------------------------------
// 引用 ArduinoJson 函式庫，用於建立與解析 JSON 字串
#include <ArduinoJson.h>  
// 引用 DHTLib 函式庫，用來操作 DHT 溫濕度感測器（例如 DHT11、DHT22 等）
#include <DHTLib.h>  
// 假設 JSONLib 是自訂函式庫，用來封裝與 JSON 處理相關的函式（如 setjsondata, initjson 等）
#include <JSONLib.h>  
// 引用字串操作函式庫（Arduino 中通常自動包含 String.h，可省略）
#include <String.h>  

// 定義與變數設定 ------------------------------------
// 定義主迴圈的延遲時間（單位為毫秒），即每 6 秒執行一次資料讀取與傳輸
#define loopdelay 6000
// 建立裝置的 MAC Address（通常為唯一識別碼，這裡是假資料）
String MacData = "112233AABBCC";  
// 宣告兩個全域變數，分別儲存讀取到的溫度與濕度值
float TValue;  // 溫度值
float HValue;  // 濕度值



// 主初始化函式 ------------------------------------
void setup() 
{
    initAll();   // 呼叫系統初始化函式，包括序列埠與感測器初始化
}



// 主執行迴圈 ------------------------------------
void loop() 
{
    // 讀取濕度值 ----------------------------------
    HValue = readHumidity();        // 透過感測器讀取濕度數值
    Serial.print("Humidity : ");    
    Serial.print(HValue);           // 在序列埠監控視窗顯示濕度數值
    Serial.print(" %    ");         // 顯示單位百分比符號
    

    // 讀取溫度值 ----------------------------------
    TValue = readTemperature();     // 透過感測器讀取溫度數值
    Serial.print("Temperature : ");
    Serial.print(BMht.readTemperature());  // 顯示讀取到的溫度（注意：此處 `BMht` 未宣告，可能是外部函式庫物件）
    Serial.println(" °C ");         // 顯示單位 °C
    

    // 設定 JSON 格式資料並顯示 -------------------
    setjsondata(MacData, TValue, HValue);  // 將 MAC、溫度、濕度封裝成 JSON 字串
    Serial.print("The Temperature & Humidity Sensor:");
    Serial.print("JSON DOC:(");
    Serial.print(json_data);      // 顯示轉換後的 JSON 資料（假設 `json_data` 是全域字串變數）
    Serial.print(")\n");   

    // 延遲 ---------------------------------------
    delay(loopdelay);  // 等待一段時間再重複下一輪讀取與傳送
}



// 感測器初始化函式 ----------------------------------
void initSensor() // 初始化所有感測器
{
    initjson(MacData);   // 初始化 JSON 架構（可能設定主結構與 MAC 資訊）
    initDHT();           // 初始化 DHT 溫溼度感測器，啟用 I2C 或數位腳位連接
}



// 系統總初始化函式 ----------------------------------
void initAll()  // 初始化整體系統設定
{
    Serial.begin(9600);         // 啟動序列埠通訊，設定傳輸速率為 9600 bps
    Serial.println("System Start");  // 顯示系統啟動訊息
    initSensor();               // 初始化所有感測器（如 DHT）
}
