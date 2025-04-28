/*************************************************
File:             readDistance.ino
Description:      1.The Wire is used for IIC communication with the BML36M001.
                  2.Use a hardware serial port (BAUDRATE 115200) to communicate with the serial monitor.
                  When the distance data is ready, the distance data is read and displayed on the serial monitor.
connection method： i2cPort:Wire  intPin:D2         
**************************************************/
#include "BML36M001.h" // 引入 BML36M001 的庫

//BML36M001 BML36(2, &Wire); // 使用硬體 Wire（I2C 接口）和 intPin 2，若不使用 Wire 請註解掉這行
BML36M001 BML36(22, &Wire1); // 使用硬體 Wire1 接口和 intPin 22，若在 BMduino 上使用硬體 Wire1 請取消註解
//BML36M001 BML36(25, &Wire2); // 使用硬體 Wire2 接口和 intPin 25，若在 BMduino 上使用硬體 Wire2 請取消註解

void setup(void)
{
  Serial.begin(115200); // 設定序列監控的通訊速率為 115200
  BML36.begin(); // 初始化 BML36M001
  BML36.setDistanceModeLong(); // 設定為長距離測量模式，偵測範圍：40~4000（單位：毫米）
  BML36.startRanging(); // 開始測量距離
}

void loop(void)
{    
  // 確認數據是否準備好，0：數據未準備好，1：數據已準備好
  if (BML36.getINT()) 
  {
    Serial.print("Distance(mm):"); // 輸出字串：“Distance(mm):”
    Serial.println(BML36.readDistance()); // 獲取距離數據（單位：毫米）
    BML36.clearInterrupt(); // 清除 BML36M001 的中斷標誌位
  } 
}
