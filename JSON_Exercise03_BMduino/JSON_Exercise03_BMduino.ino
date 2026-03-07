//#include "initPins.h"   // 引入自訂腳位設定與初始化模組，內含 LED 控制、感測器腳位定義等功能（需自建）
#include "JSONLIB.h"    // 引入自訂的 JSON 功能模組（含 setjsondata()、appendjsondata()、json_data、json_row 等定義）

#define loopdelay 6000
String MacData="112233AABBCC" ; //建立一個MAC Address
// int arraycount =0 ;// 記錄 JSON 陣列累積筆數，初始化為 0
// int arrayamount = 50 ; // 記錄 JSON 陣列累積總筆數，初始化為 50
void setup() 
{
    initALL();      // 初始化系統（如啟動序列埠、設定腳位、初始化變數）

    arraycount = 0; // 記錄 JSON 陣列累積筆數，初始化為 0
}


void loop() 
{
    // 若累積資料已達設定上限（例如 arrayamount = 10 筆）
    if (arraycount >= arrayamount)
    {
        setjsondata(MacData);           // 將目前累積的溫濕度資料，加上裝置 MAC，組成主 JSON 結構
        serializeJson(json_row, Serial); // 將資料陣列直接序列化並印出到序列埠（格式為 JSON 陣列）
        Serial.println("");              // 換行
        Serial.println(json_data);       // 印出完整 JSON 文件（內含 "Device" 與 "Data" 陣列）

        arraycount = 0;                  // 重設筆數計數器
        json_row.clear();                // 清空陣列內容，準備下一輪紀錄
    }

    // ---------- 模擬產生感測器資料 ---------- //
    double Temp_Value = 25 + (double)(random(-300, 300) / 50);   // 產生 ±6°C 的亂數波動
    double Humid_Value = 70 + (double)(random(-300, 300) / 10);  // 產生 ±30% 的亂數波動
    // 組合溫濕度提示訊息（可顯示在 OLED 或上傳用）
    String msg = "目前溫度:" + String(Temp_Value) + "°C, 目前濕度：" + String(Humid_Value);
    // 印出模擬溫濕度資料到序列埠
    Serial.print("Temp: "); Serial.print(Temp_Value); Serial.print(" C");
    Serial.print("\t\t");
    Serial.print("Humidity: "); Serial.print(Humid_Value); Serial.println(" %");

    // 若目前累積筆數尚未達上限，將這筆資料加入 JSON 陣列中
    if (arraycount < arrayamount)
    {
        appendjsondata(Temp_Value, Humid_Value);  // 呼叫自訂函式加入資料
    }

    arraycount++;   // 筆數加 1
    delay(loopdelay);  // 延遲一段時間（如 5 秒，定義於 JSONLIB.h）
}

void initALL()  // 系統初始化函式
{
    Serial.begin(9600);             // 啟動序列通訊，設定鮑率為 9600
    Serial.println("System Start"); // 通知使用者系統啟動成功
}
