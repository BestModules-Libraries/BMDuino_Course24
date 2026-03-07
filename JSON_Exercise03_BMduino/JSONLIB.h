#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來建立與處理 JSON 資料格式
StaticJsonDocument<3000> json_doc;        // 主 JSON 文件，存放整體結構（包含裝置名稱與資料陣列）
StaticJsonDocument<100> json_rowdata;     // 用來暫存單筆資料（含溫度與濕度）

#define arrayamount 10  
const int capacity = JSON_ARRAY_SIZE(arrayamount) + arrayamount * JSON_OBJECT_SIZE(2);
// 定義一個陣列的容量：最多可放 10 筆，每筆為 2 個欄位（Temperature, Humidity）
// JSON_ARRAY_SIZE(N)：估算 N 筆陣列所需空間
// JSON_OBJECT_SIZE(M)：估算 M 個欄位所需空間

StaticJsonDocument<capacity> json_row;  // 建立一個 JSON 陣列物件（內含多筆 rowdata）
int arraycount = 0;                     // 記錄目前陣列內資料筆數（尚未使用）
               // 陣列最大筆數限制（常數定義）


char json_data[5000];       // 用來儲存序列化後的 JSON 字串（可傳輸）
DeserializationError json_error;  // 反序列化時的錯誤結果變數（尚未使用）




/*

{
  "Device": "E89F6DE8F3BC",
  "Data": [
    { "Temperature": 24, "Humidity": 77 },
    { "Temperature": 25, "Humidity": 70 },
    ...
  ]
}


 */

void initjson()
{
  // 可視需要初始化 json_doc / json_row
  // 例如：json_row.clear(); json_doc.clear();
}

void appendjsondata(double d1, double d2) //累加溫度與濕度
{
  // 設定單筆資料內容：溫度與濕度
  json_rowdata["Temperature"] = d1;
  json_rowdata["Humidity"] = d2;

  // 將這筆資料新增至 json_row 陣列中
  json_row.add(json_rowdata);
}

void setjsondata(String mm)//產生整個 json文件//產生整個 json文件
{
  // 設定主 JSON 物件中的 "Device" 欄位
  json_doc["Device"] = mm;

  // 將資料陣列 json_row 加入到主結構中的 "Data" 欄位
  json_doc["Data"] = json_row;

  // 將整個 json_doc 結構序列化為 JSON 字串，儲存到 json_data
  serializeJson(json_doc, json_data);
}


