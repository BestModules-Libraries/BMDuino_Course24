/*****************************************************************
File:         readTemperature.ino
Description:  
  1. 本範例程式使用 I2C（100kHz）介面與 BMH63K203 紅外線溫度感測模組進行通訊。  
  2. 使用硬體序列埠（Serial）以 9600 bps 的鮑率（BAUDRATE）與電腦端的序列監控視窗進行資料輸出顯示。            
Note:
  - 本程式可運行於 BMduino 系列主板，需依實際 I2C 接腳（Wire、Wire1、Wire2）選擇對應物件。
******************************************************************/

#include <BMH06203.h>   // 引入 BMH06203 感測器的函式庫，用於與紅外線溫度模組溝通

// 宣告一個 BMH06203 物件 mytherm，使用主板內建的 I2C 匯流排 Wire 進行通訊。
// 若使用不同 I2C 接腳（如 BMduino 上的 Wire1 或 Wire2），可取消註解對應行。
BMH06203 mytherm(&Wire);     // 若使用 Wire 介面，啟用此行
//BMH06203 mytherm(&Wire1);  // 若使用 Wire1 介面，請取消此行註解
//BMH06203 mytherm(&Wire2);  // 若使用 Wire2 介面，請取消此行註解

void setup() 
{
  /* 初始化設定階段 */
  
  mytherm.begin();        // 初始化 BMH06203 感測器，設定 I2C 通訊模式與感測器參數
  Serial.begin(9600);     // 啟動序列埠通訊，設定鮑率為 9600，方便輸出資料到電腦監控視窗
}

void loop()
{
  // 主迴圈：持續讀取紅外線感測器量測的物體溫度（Object Temperature, OBJ_TEMP）

  Serial.print("The OBJTemp is:");          // 印出提示字串
  Serial.print(mytherm.readTemperature(OBJ_TEMP)); // 讀取感測器所測得的物體溫度值，單位為攝氏度
  Serial.println("℃");                      // 印出單位符號「℃」並換行

  // 提示：可在此加入 delay() 以降低輸出頻率，例如 delay(1000);
  // 以每秒輸出一次測得溫度
}
