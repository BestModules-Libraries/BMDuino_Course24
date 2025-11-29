#include <ArduinoJson.h>  // 引用 ArduinoJson 函式庫，用來處理 JSON 格式資料

void setup() 
{
  // 初始化序列埠，與監控視窗通訊，速度為 9600 位元每秒（bps）
  Serial.begin(9600);   

  // 若尚未開啟序列埠監控視窗，則停在這邊等待（常見於部分設備）
  while (!Serial) continue; 

  // 宣告靜態 JSON 文件物件，分配記憶體大小為 200 bytes
  // StaticJsonDocument 是在堆疊 (stack) 中配置記憶體，速度快但大小固定
  // 這個大小應根據 JSON 資料的複雜度與長度做調整
  StaticJsonDocument<200> doc;

  /*
    備註：
    若需要動態記憶體配置（例如資料量不固定），可改用：
    DynamicJsonDocument doc(200);
  */

  // 定義一段 JSON 格式的字元陣列（字串）
  // 使用 char[] 可啟用 "zero-copy" 模式：避免重複複製資料，節省記憶體空間
  char json[] = 
    "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  
  // 將 JSON 字串反序列化為 JSON 物件，結果儲存在 doc 變數
  DeserializationError error = deserializeJson(doc, json);

  // 檢查是否反序列化成功
  if (error)  
  {
    // 若失敗，印出錯誤訊息與錯誤代碼
    Serial.print(F("deserializeJson() failed: "));  
    Serial.println(error.f_str());   
    return; // 提前結束 setup()
  }

  // 從 JSON 物件中提取資料
  // 取得 "sensor" 欄位內容（字串型態），例如 "gps"
  const char* sensor = doc["sensor"];  
  // 取得 "time" 欄位內容（長整數型態），例如 1351824120
  long time = doc["time"];           
  // 取得 "data" 陣列的第一個元素（浮點數）：緯度
  double latitude = doc["data"][0];     
  // 取得 "data" 陣列的第二個元素（浮點數）：經度
  double longitude = doc["data"][1];    

  // 將上述取得的資料印出到序列埠監控視窗
  Serial.println(sensor);       // 印出 sensor 名稱
  Serial.println(time);         // 印出時間數值
  Serial.println(latitude, 6);  // 印出緯度，精確到小數點第六位
  Serial.println(longitude, 6); // 印出經度，精確到小數點第六位
}

void loop() {
  // 這個範例不使用 loop()，所以保持空白
}
