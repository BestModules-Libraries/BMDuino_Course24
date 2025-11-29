#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，支援 JSON 格式的處理與建立

StaticJsonDocument<3000> json_doc;      // 宣告一個容量 3000 bytes 的 JSON 文件，主結構暫未使用
StaticJsonDocument<100> json_rowdata;   // 單筆資料用的暫存 JSON 物件（儲存單筆 Device, Temperature, Humidity）

#define arrayamount 10                 // 最大可儲存 10 筆資料
const int capacity = JSON_ARRAY_SIZE(arrayamount) + arrayamount * JSON_OBJECT_SIZE(2);
// 定義一個陣列的容量：最多可放 10 筆，每筆為 2 個欄位（Temperature, Humidity）
// JSON_ARRAY_SIZE(N)：估算 N 筆陣列所需空間
// JSON_OBJECT_SIZE(M)：估算 M 個欄位所需空間

StaticJsonDocument<capacity> json_row;  // 建立一個 JSON 陣列物件（內含多筆 rowdata）
int arraycount = 0;                     // 紀錄目前已加入的筆數
char json_data[5000];                 // 儲存最後 JSON 字串的緩衝區
DeserializationError json_error;     // 用於 JSON 反序列化的錯誤處理（目前未使用）


/*

[
  {
    "Device": "AABBCCDDEEFF",
    "Temperature": 29,
    "Humidity": 45
  },
  {
    "Device": "AABBCCDDEEFF",
    "Temperature": 30,
    "Humidity": 47
  }
 ]

 */

void initjson()
{
  // 初始化函式（目前未實作任何動作）
  // 可視需求加入 json_row.clear(); 或 json_doc.clear(); 做初始化處理
}


void setjsondata()
{
  // 將 json_row 陣列中的資料序列化成 JSON 字串，存入 json_data 字元陣列中
  serializeJson(json_row, json_data);
}


void appendjsondata(String mm, double d1, double d2)
{
  JsonObject row = json_row.createNestedObject();
  row["Device"] = mm;
  row["Temperature"] = d1;
  row["Humidity"] = d2;
}

