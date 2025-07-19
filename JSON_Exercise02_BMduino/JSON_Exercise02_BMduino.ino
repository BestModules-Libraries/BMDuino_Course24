#include "initPins.h"     // 引入自訂的腳位與系統初始化函式庫（假設你定義了腳位、LED、感測器）
#include "JSONLIB.h"      // 引入自訂的 JSON 模組（可能包含 setjsondate()、json_data 等功能）


void setup() 
{
    initALL();  // 執行硬體與軟體初始化（例如：啟動序列埠、設定腳位、初始化網路等）
}


void loop() 
{
    // 產生模擬的溫度值：
    // 基礎值為 25°C，加上 ±6°C 亂數（random(-300, 300)/50 約為 -6 ~ +6）
    double Temp_Value = 25 + (double)(random(-300, 300) / 50);

    // 產生模擬的濕度值：
    // 基礎值為 70%，加上 ±30% 亂數（random(-300, 300)/10 約為 -30 ~ +30）
    double Humid_Value = 70 + (double)(random(-300, 300) / 10);

    // 將溫度與濕度轉為字串並組合訊息
    String msg = "目前溫度:" + String(Temp_Value) + "°C, 目前濕度：" + String(Humid_Value);

    // 印出目前溫濕度到序列監控視窗
    Serial.print("Temp: "); 
    Serial.print(Temp_Value); 
    Serial.print(" C\t\t");

    Serial.print("Humidity: "); 
    Serial.print(Humid_Value); 
    Serial.println(" %");

    // 將 MAC 位址、溫度、濕度資料寫入 JSON 物件並序列化為 json_data
    // ※ setjsondate() 與 json_data 應來自 JSONLIB.h 中的函式與全域變數
    setjsondate(MacData, Temp_Value, Humid_Value);

    // 印出序列化後的 JSON 字串
    Serial.println(json_data);

    // 延遲指定的毫秒數（loopdelay 可能定義在某個 .h 中，例如 5000ms = 5 秒）
    delay(loopdelay);
}


void initALL()  // 系統初始化函式
{
    Serial.begin(9600);             // 啟動序列通訊，設定鮑率為 9600
    Serial.println("System Start"); // 通知使用者系統啟動成功
}

