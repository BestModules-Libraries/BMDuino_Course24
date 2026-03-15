/*
這段程式碼的註解說明了如何使用 ArduinoJson 庫，
將回傳的資料序列化成 JSON 格式。
 */
#include <ArduinoJson.h>  // Json 使用元件
StaticJsonDocument<200> doc; // 建立 JSON 文件物件

/*
{
"Device":"483FDACA32A3",
"Systime":"20250704063358",
"Result":"Added"
}
{
"Device":"483FDACA32A3",
"Systime":"20250704063057",
"Result":"Existed"
}
*/

void initjson()   // 初始化 JSON 資料
{

}
