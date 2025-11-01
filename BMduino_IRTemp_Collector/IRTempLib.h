// IR Temperature紅外線溫度模組自訂函式
#include <BMH06203.h>   // 引入 BMH06203 感測模組的函式庫，以便呼叫其 API 函式

//------------------- 模組初始化設定區 -------------------
// 下列三行只需保留一行依實際接線設定而定
// BMH06203 mytherm(&Wire);   // 若使用主 I2C 匯流排 (Wire)，請取消註解此行
BMH06203 mytherm(&Wire1);     // 若使用副 I2C 匯流排 (Wire1)，請取消註解此行
// BMH06203 mytherm(&Wire2);  // 若使用第三組 I2C 匯流排 (Wire2)，請取消註解此行
//--------------------------------------------------------

//-----準備全域變數-------------
float bodytemp = 0 ;  //體溫全域變數
//-----------------------------
// ------- 自定義函式宣告區 -----------
void initIRTemperature();    //初始化紅外線模組
float readIRTemperature(); //讀取紅外線模組取得量測溫度

//------自訂函式------
void initIRTemperature()    //初始化紅外線模組
{
  /* ===== 系統初始化階段 ===== */
  mytherm.begin();           // 啟動感測器的初始化程序，使其進入可通訊狀態 (設定 I2C 位址、檢查模組是否存在)
  Serial.println("初始化紅外線模組") ;
}

float readIRTemperature() //讀取紅外線模組取得量測溫度
{
    return mytherm.readTemperature(OBJ_TEMP);
    //回傳讀取紅外線模組取得量測溫度
}


