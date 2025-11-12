/*****************************************************************
File:         readTemperature.ino
Description:  1.I2C(100k) is used to communicate with BMH63K203.
              2.hardware Serial (BAUDRATE 9600) is used to communicate with Serial port monitor.            
Note:
******************************************************************/
#include <BMH06203.h>

//BMH06203 mytherm(&Wire); // 如果在 BMduino 上使用 Wire，請取消註解此行
BMH06203 mytherm(&Wire1); // 如果在 BMduino 上使用 Wire1，請取消註解此行
// BMH06203 mytherm(&Wire2); // 如果在 BMduino 上使用 Wire2，請取消註解此行

void setup() 
{
  /* 請使用 IIC 模式 */
   mytherm.begin(); // 初始化溫度感測器   
   Serial.begin(9600); // 設定 Serial 通訊速率為 9600
}

void loop()
{
  // 取得物體溫度資料
   Serial.print("物體溫度為: ");
   Serial.print(mytherm.readTemperature(OBJ_TEMP)); // 讀取並顯示物體溫度
   Serial.println("℃"); // 顯示單位℃（攝氏度）
}
