#include "BME33M251Lib.h" //溫溼度感測器自訂函式區塊區

void setup() 
{
    Serial.begin(9600) ;  //初始化監控視窗，設定速率為9600
    initBME33M251() ; //初始化溫溼度感測器
    delay(2000);                // 延遲 2 秒，讓使用者有時間閱讀裝置資訊
}

void loop() 
{

    readSensor() ;//讀取溫溼度感測器值
    printSensorData() ; //感測模組：列印感測器物件之感測資料

    delay(2000);   // 延遲 2 秒後再重複讀取溫濕度資料
}
