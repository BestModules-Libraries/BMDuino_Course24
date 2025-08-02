// 引入 ArduinoJson 函式庫，用於建立、編輯與序列化（轉換為字串）JSON 資料
#include <ArduinoJson.h>  


// ========== JSON 全域物件與變數宣告區 ==========

// 建立一個靜態 JSON 文件（容量為 500 bytes）
// 這個變數會儲存完整的 JSON 結構，例如：{ "Device": "xx", "Temperature": xx, "Humidity": xx }
StaticJsonDocument<500> json_doc;  

// 宣告一個字元陣列，用來儲存序列化後的 JSON 字串結果
// 例如：{"Device":"E89F6DE8F3BC","Temperature":25.0,"Humidity":60.0}
char json_data[1000];  

// 用來儲存 JSON 反序列化（例如 parse JSON 字串時）的錯誤訊息狀態
DeserializationError json_error;  



// ========== 範例 JSON 資料格式說明 ==========
/*
JSON 結構示意：

{
  "Device": "E89F6DE8F3BC",   ← 裝置識別碼（可為 MAC address）
  "Temperature": 24,          ← 溫度資料（float 或 int）
  "Humidity": 77              ← 濕度資料（float 或 int）
}

📌 此結構常見於：
1. 上傳感測器資料到 MQTT / HTTP 伺服器
2. 儲存至本地檔案（如 SD 卡）
3. 提供網頁或 App 顯示用的 API 輸出
*/



// ========== JSON 初始化函式 ==========
// 用來設定或初始化 JSON 結構的基本內容（例如放入裝置 MAC 位址）
void initjson(String mm) 
{
  // 將參數 mm 指定為 JSON 物件中的 "Device" 欄位，例如 "Device": "112233AABBCC"
  json_doc["Device"] = mm;

  // 如果需要清空 json_doc，可取消下列註解使用
  // json_doc.clear();  // 清除所有先前內容（還原為空物件）
}



// ========== JSON 組裝與序列化函式 ==========
// 傳入裝置名稱、溫度與濕度資料，將其轉換為完整 JSON 字串
void setjsondata(String mm, float d1, float d2)
{
  // 將參數填入 JSON 結構中對應的欄位
  json_doc["Device"] = mm;         // 裝置代碼或 MAC 位址
  json_doc["Temperature"] = d1;    // 溫度資料
  json_doc["Humidity"] = d2;       // 濕度資料

   // 將 JSON 結構轉換為字串，儲存到 json_data 陣列中
  // 此結果可直接傳送至伺服器（如 POST、MQTT 發布等）
  serializeJson(json_doc, json_data);
}
