#include <ArduinoJson.h>  // 引入 ArduinoJson 函式庫，用來建立與處理 JSON 格式資料

// 宣告一個容量為 500 bytes 的靜態 JSON 文件物件，用於存放結構化的 JSON 資料
StaticJsonDocument<500> json_doc;

// 宣告一個字元陣列，用來儲存轉換後的 JSON 字串資料（如傳給伺服器）
char json_data[1000];

// 宣告一個錯誤物件，用於接收 JSON 反序列化（deserialize）後的錯誤結果
DeserializationError json_error;

/*
JSON 資料結構範例：

{
  "Device": "E89F6DE8F3BC",   ← 裝置 MAC 位址（或裝置代號）
  "Temperature": 24,          ← 溫度數值（浮點數或整數）
  "Humidity": 77              ← 濕度數值（浮點數或整數）
}

這種格式可用於上傳至 IoT 伺服器、儲存到 SD 卡，或經由 HTTP 傳送
*/

// JSON 初始化函式（目前尚未實作，可作為清除或初始化用途）
void initjson()
{
  // 若需要初始化可使用 json_doc.clear();
  // json_doc.clear();  // 清空所有內容
}
void setjsondata(String mm, double d1, double d2)
{
  // 設定主 JSON 物件中的 "Device" 欄位
json_doc["Device"] = mm;
json_doc["Temperature"] = d1;
json_doc["Humidity"] = d2;

  // 將資料陣列 json_row 加入到主結構中的 "Data" 欄位
 
  // 將整個 json_doc 結構序列化為 JSON 字串，儲存到 json_data
  serializeJson(json_doc, json_data);
}