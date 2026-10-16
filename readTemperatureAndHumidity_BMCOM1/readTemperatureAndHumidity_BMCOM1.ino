/*****************************************************************
  檔案名稱: readTemperatureAndHumidityWithIIC
  說明:
    通電後，僅執行一次以取得裝置資訊（包含序號 SN、產品ID PID、韌體版本 VER），
    接著透過 I2C 介面持續重複讀取溫濕度資料，並透過序列埠（Serial）顯示結果。
******************************************************************/

#include "BM25S2021-1.h"   // 引入 BM25S2021-1 感測器專用函式庫

// 初始化感測器物件，參數為使用的 I2C 介面
// 根據實際連接的 I2C 接口（Wire, Wire1, Wire2）決定使用哪一行初始化
// BM25S2021_1 BMht(&Wire);   // 若使用預設 I2C 接口（Wire）可使用這一行
// BM25S2021_1 BMht(&Wire1);  // 若使用 Wire1 介面（例如某些ESP32腳位）可使用這一行
// BM25S2021_1 BMht(&Wire2);  // 若使用 Wire2 介面則使用這一行
BM25S2021_1 BMht(&Wire1);     // 本範例中選擇使用 Wire1 介面來初始化感測器

void setup() 
{
    BMht.begin();               // 啟動並初始化感測器
    Serial.begin(9600);         // 設定序列埠傳輸速率為 9600 bps，供電腦端顯示資料
    Serial.println("=======DeviceInfo(HEX)========= "); // 顯示裝置資訊區塊開頭
    
    Serial.print("SN : ");              // 顯示感測器序號標籤（Serial Number）
    Serial.println(BMht.getSN(), HEX); // 使用 getSN() 方法取得序號，並以16進位方式顯示
    
    Serial.print("PID : ");             // 顯示產品 ID 標籤（Product ID）
    Serial.println(BMht.getPID(), HEX); // 使用 getPID() 方法取得產品 ID，並以16進位方式顯示
    
    Serial.print("VER : ");             // 顯示韌體版本標籤（Firmware Version）
    Serial.println(BMht.getFWVer(), HEX); // 使用 getFWVer() 方法取得韌體版本，並以16進位方式顯示

    Serial.println("=============================== "); // 顯示裝置資訊區塊結尾
    delay(2000);                // 延遲 2 秒，讓使用者有時間閱讀裝置資訊
}

void loop() 
{
      Serial.print("Humidity : ");           // 顯示濕度資訊標籤
      Serial.print(BMht.readHumidity());     // 讀取並顯示目前濕度值（單位：%）
      Serial.print(" %    ");                // 顯示濕度單位符號與間隔

      Serial.print("Temperature : ");        // 顯示溫度資訊標籤
      Serial.print(BMht.readTemperature());  // 讀取並顯示目前溫度值（單位：攝氏）
      Serial.println(" °C ");                // 顯示攝氏溫度單位與換行

    delay(2000);   // 延遲 2 秒後再重複讀取溫濕度資料
}
