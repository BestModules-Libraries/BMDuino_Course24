/*
================================================================================
【程式名稱】course1015V1.ino
【功能概述】本程式為一套 Arduino 物聯網環境監控系統，整合了溫濕度感測器
            （BM25S2021-1）、OLED 顯示模組、WiFi 連線模組（TCP），以及
            雲端資料上傳（HTTP GET）等功能。系統會定期讀取環境溫濕度，
            顯示於 OLED 與序列埠，並將資料上傳至雲端伺服器。

【整體運作流程解說】
--------------------------------------------------------------------------------
一、setup() 初始化階段（開機後僅執行一次）：
    (1) 呼叫 initAll()：
        - 啟動序列埠（9600 bps），用於除錯與監控。
        - 呼叫 initSensor()，依序初始化 OLED 顯示模組與 DHT 溫濕度感測器。
    (2) 延遲 200 毫秒，等待感測器穩定。
    (3) 呼叫 ShowDHTInformation() 印出 DHT 感測器的產品資訊
        （序號 SN、產品 ID PID、韌體版本 VER）。
    (4) WiFi 初始化與連線流程：
        - 呼叫 initWiFi() 初始化 WiFi 模組並連線至基地台。
        - 取得 SSID（熱點名稱）並透過序列埠印出。
        - 取得 IP 位址並透過序列埠印出。
        - 取得 MAC 位址並透過序列埠印出。
    (5) OLED 顯示流程：
        - 清除螢幕。
        - 繪製 BEST MODULE 的 LOGO（128x64 全螢幕）。
        - 延遲 2 秒讓 LOGO 顯示。
        - 清除螢幕。
        - 將 MAC 位址顯示於 OLED 第 1 列。
        - 將 IP 位址顯示於 OLED 第 3 列。

二、loop() 主迴圈階段（每 30 秒執行一次，週而復始）：
    (1) 讀取 DHT 感測器的濕度值（HValue）與溫度值（TValue）。
    (2) 透過序列埠印出濕度與溫度數值，方便除錯與監控。
    (3) 將溫度顯示於 OLED 第 5 列左側，濕度顯示於第 5 列右側。
    (4) 檢查 WiFi 連線狀態：
        - 若連線正常（Wifi.getStatus() 為真），呼叫 SendtoClouding()
          將溫濕度資料以 HTTP GET 方式上傳至雲端。
    (5) 延遲 loopdelay（30000 毫秒 = 30 秒）後再次執行迴圈。

【硬體與函式庫對應】
- DHTLib.h    ：自訂溫濕度感測模組函式庫（BM25S2021-1，提供 initDHT、
                readHumidity、readTemperature、ShowDHTInformation 等）。
- TCP.h       ：TCP/WiFi 控制邏輯（含 initWiFi、GetSSID、GetIP、GetMAC、
                Wifi 物件、MacData 等）。
- clouding.h  ：雲端資料上傳（HTTP GET）自訂函式庫（SendtoClouding）。
- OledLib.h   ：OLED 顯示模組自訂函式庫（initOled、printText、clearScreen、
                drawPicture、BestModule_LOGO 等）。

【全域變數與常數】
- loopdelay   ：主迴圈延遲時間，預設 30000 毫秒（30 秒）。
- SSIDData    ：儲存 WiFi SSID 熱點名稱字串。
- IPData      ：儲存 WiFi 連線後取得的 IP 位址字串。
- MacData     ：儲存 WiFi 模組的 MAC 位址字串（定義於 TCP.h）。
- HValue      ：儲存讀取到的濕度值（定義於 DHTLib.h）。
- TValue      ：儲存讀取到的溫度值（定義於 DHTLib.h）。
- BMht        ：BM25S2021-1 感測器物件（定義於 DHTLib.h）。
- Wifi        ：WiFi 控制物件（定義於 TCP.h）。

【作者/版本】課程範例 course1015V1
================================================================================
*/

//-------外部函式宣告區-----------
#include "DHTLib.h"   // 引入自訂溫濕度感測模組的函式庫
#include "LuxLib.h"   // 引入自訂環境光感測模組（ALS）函式庫
#include "TCP.h"      // 引入 TCP 控制邏輯相關函式與變數定義（含 WiFi 初始化、MAC 取得等）
#include "clouding.h" // 引入傳送溫溼度資料到雲端自訂函式庫；HTTP GET 使用的函式庫
#include "clouding1.h" // 引入傳送光度資料到雲端自訂函式庫；HTTP GET 使用的函式庫
#include "OledLib.h"  // 引入 OLED 顯示模組自訂函式庫



//-------外部函式宣告區-----------
#define loopdelay 10000    // loop 迴圈延遲時間（單位：毫秒），即 30 秒


//-------自定義函式宣告區-----------
void initSensor();                    // 初始化所有感測模組（OLED + DHT+Lux）
void initAll();                       // 初始化整體系統（序列埠 + 感測模組）
void showTitleonOled(String ss);      // 將指定字串列印於 OLED 第 1 列（抬頭）
void showIPonOled(String ss);         // 將 IP 位址列印於 OLED 第 3 列
void showTemperatureonOled(float ss); // 將溫度值列印於 OLED 第 5 列左側
void showHumidityonOled(float ss);    // 將濕度值列印於 OLED 第 5 列右側
void showLuxonOled(float ss);    // 讀取第 1 個感測器的環境光照度並印出


//-------自定義全域變數宣告區-----------
String SSIDData; // 儲存 WiFi SSID 熱點名稱字串（註解原文誤寫為 MAC Address）
String IPData;   // 儲存 WiFi 連線後取得的 IP 位址字串（註解原文誤寫為 MAC Address）


//================================================================================
// setup()：系統初始化函式，Arduino 開機後僅執行一次
//================================================================================
void setup()
{
    initAll();              // 初始化整體系統（序列埠 + 感測模組）
    delay(200);             // 延遲 200 毫秒，等待感測器穩定
    ShowDHTInformation();   // 印出溫溼度感測器產品所有資訊

    // ------ WiFi 初始化與連線流程 --------------------
    initWiFi();             // 執行 WiFi 模組初始化與連線（定義於 TCP.h）
    Serial.println("");     // 印出空行，純粹格式美觀
    Serial.println("---wifi access point----"); // 分隔線，美觀用途

    SSIDData = GetSSID();     // 取得目前連線的 SSID 熱點字串
    Serial.println(SSIDData); // 印出 SSID 熱點字串

    // Serial.println(typeof(Wifi.SSID())) ; // 註解掉的除錯語句，用於查資料型別

    Serial.println("---Show IP Address----"); // 分隔線，美觀用途
    IPData = GetIP();       // 取得連線後分配到的 IP 位址
    Serial.println(IPData); // 印出取得的 IP 位址

    MacData = GetMAC();     // 取得 WiFi 模組的 MAC 位址字串（MacData 定義於 TCP.h）
    Serial.println("---MAC Address----"); // 分隔線，美觀用途
    Serial.println(MacData); // 印出取得的 MAC 位址

    //------------------------------- OLED 顯示流程
    clearScreen();          // 清除 OLED 螢幕

    // 畫 BEST MODULE LOGO
    drawPicture(0, 0, BestModule_LOGO, 128, 64); // 於 (0,0) 繪製 128x64 的 LOGO
    delay(2000);            // 延遲 2 秒，讓 LOGO 顯示於螢幕上
    clearScreen();          // 清除 OLED 螢幕

    showTitleonOled(MacData); // 將 MAC 位址顯示於 OLED 第 1 列
    showIPonOled(IPData);     // 將 IP 位址顯示於 OLED 第 3 列
    
}


//================================================================================
// loop()：主迴圈函式，Arduino 開機後會不斷重複執行
//================================================================================
void loop() {
    // 從感測器讀取濕度數值並顯示
    TValue = readTemperature();        // 讀取溫度數值（HValue 定義於 DHTLib.h）
    Serial.print("Temperature : ");
    Serial.print(TValue);           // 顯示溫度值
    Serial.print(" °C ");

    HValue = readHumidity();        // 讀取濕度數值（HValue 定義於 DHTLib.h）
    Serial.print("Humidity : ");
    Serial.print(HValue);           // 顯示濕度值
    Serial.print(" %    ");

    LValue = readLux() ; // 讀取第 1 個感測器的環境光照度並印出
    Serial.print("Light Lux : ");
    Serial.print(LValue); // 顯示環境光照度
    Serial.println(" Luxs ");               // 顯示光照度單位流明 Luxs

    showTemperatureonOled(TValue); // 列印溫度於 OLED 第 5 列左側
    showHumidityonOled(HValue);    // 列印濕度於 OLED 第 5 列右側
    showLuxonOled(LValue);    // 讀取第 1 個感測器的環境光照度並印出
   // updateScreen(); //顯示當前緩衝區的內容  

    if (Wifi.getStatus())           // 檢查 WiFi 是否已連線
    {
        Serial.println("WIFI OK");
        SendtoClouding();           // 傳送溫濕度資料到雲端
        SendtoClouding1();           // 傳送亮度資料到雲端
    }

    delay(loopdelay);               // 延遲 loopdelay（30 秒）後再次執行迴圈
}


//================================================================================
// initSensor()：初始化所有感測模組（OLED + DHT+Lux）
//================================================================================
void initSensor()
{
    initOled();     // 初始化 OLED 12864（0.96 吋 OLED 顯示模組 BMD31M090）
    delay(2000);    // 延遲 2 秒，等待 OLED 初始化完成
    initDHT();      // 初始化溫溼度感測器，啟動 I2C 通訊
    initLux();    // 初始化 BME82M131 環境光感測模組

}


//================================================================================
// initAll()：初始化整體系統（序列埠 + 感測模組）
//================================================================================
void initAll()
{
    Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
    initSensor();       // 初始化所有感測模組
}


//================================================================================
// showTitleonOled()：將指定字串列印於 OLED 第 1 列（抬頭）
// 參數：ss - 要顯示的字串
//================================================================================
void showTitleonOled(String ss)
{
    printText(0, 0, "              "); // 先以空白清空第 1 列（14 個空白字元）
    printText(0, 0, ss);               // 將字串寫入第 1 列
    Serial.print("Title on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}


//================================================================================
// showIPonOled()：將 IP 位址列印於 OLED 第 3 列
// 參數：ss - 要顯示的 IP 位址字串
//================================================================================
void showIPonOled(String ss)
{
    printText(0, 2, "              "); // 先以空白清空第 3 列（14 個空白字元）
    printText(0, 2, ss);               // 將字串寫入第 3 列
    Serial.print("IP Address on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}


//================================================================================
// showTemperatureonOled()：將溫度值列印於 OLED 第 5 列左側
// 參數：ss - 溫度值（浮點數）
//================================================================================
void showTemperatureonOled(float ss)
{
    printText(0, 4, "              ");   // 先以空白清空第 5 列左半部
    printText(0, 4, String(ss) + ".C");  // 將溫度字串加上 ".C" 寫入第 5 列左側
    Serial.print("Temperature on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}


//================================================================================
// showHumidityonOled()：將濕度值列印於 OLED 第 5 列右側
// 參數：ss - 濕度值（浮點數）
//================================================================================
void showHumidityonOled(float ss)
{
    printText(62, 4, "         ");       // 先以空白清空第 5 列右半部（從 x=62 開始）
    printText(62, 4, String(ss) + "%");  // 將濕度字串加上 "%" 寫入第 5 列右側
    Serial.print("Humidity on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}

//================================================================================
// showLuxonOled(float ss) ：將環境光照度列印於 OLED 第 5 列右側
// 參數：ss - 環境光照度（浮點數）
//================================================================================
void showLuxonOled(float ss)    // 讀取第 1 個感測器的環境光照度並印出
{
    printText(4, 6, "         ");       // 先以空白清空第 5 列右半部（從 x=62 開始）
    printText(4, 6, String(ss) + "lux");  // 將濕度字串加上 "%" 寫入第 5 列右側
    Serial.print("Light Lux on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}
