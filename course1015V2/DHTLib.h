/*
================================================================================
【程式名稱】DHTLib.h
【功能概述】本檔案為溫濕度感測模組（BM25S2021-1）的自訂函式庫，主要負責
            透過 I2C 介面與感測器通訊，提供初始化、讀取溫度、讀取濕度、
            讀取產品資訊（序號 SN、產品 ID PID、韌體版本 VER）等功能。

【整體運作流程解說】
--------------------------------------------------------------------------------
1. 引入與宣告區：
   (1) 引入 "String.h" 與 "BM25S2021-1.h" 函式庫。
   (2) 宣告全域變數：
       - TValue：儲存讀取到的溫度值（浮點數）。
       - HValue：儲存讀取到的濕度值（浮點數）。
       - DHTStr：儲存感測器產品、序號、編號等文字資訊。
   (3) 建立 BM25S2021_1 物件 BMht，並指定使用 Wire1（第一組 I2C）進行通訊。
   (4) 宣告自定義函式原型：initDHT、readTemperature、readHumidity、
       getVersion、getProductNumbetr、getSNNumbetr、ShowDHTInformation。

2. 函式運作流程：
   (1) initDHT()：
       - 呼叫 BMht.begin() 初始化感測器，啟動 I2C 通訊。
       - 透過序列埠印出 "init Temerature OK" 表示初始化完成。
   (2) readTemperature()：
       - 呼叫 BMht.readTemperature() 讀取溫度值並回傳。
   (3) readHumidity()：
       - 呼叫 BMht.readHumidity() 讀取濕度值並回傳。
   (4) getVersion()：
       - 呼叫 BMht.getFWVer() 讀取韌體版本，並轉為 String 回傳。
   (5) getProductNumbetr()：
       - 呼叫 BMht.getPID() 讀取產品 ID，並轉為 String 回傳。
       - 註：函式名稱拼字有誤（Numbetr → Number），但為保持與原程式一致，
         此處保留原名。
   (6) getSNNumbetr()：
       - 呼叫 BMht.getSN() 讀取產品序號，並轉為 String 回傳。
       - 註：函式名稱拼字有誤（Numbetr → Number），但為保持與原程式一致，
         此處保留原名。
   (7) ShowDHTInformation()：
       - 依序透過序列埠印出感測器的序號（SN）、產品 ID（PID）、
         韌體版本（VER），並以分隔線美化輸出格式。

【硬體與函式庫對應】
- BM25S2021-1.h：BM25S2021-1 溫濕度感測模組官方函式庫，提供
                 begin、readTemperature、readHumidity、getFWVer、
                 getPID、getSN 等方法。
- Wire1        ：Arduino 的第一組 I2C 介面（依開發板不同可能為 Wire、
                 Wire1 或 Wire2）。

【作者/版本】課程範例 DHTLib.h
================================================================================
*/

/*****************************************************************
File:        readTemperatureAndHumidityWithIIC
Description: 開機後透過 I2C 介面讀取一次裝置資訊（包含序號SN、產品ID PID、韌體版本VER），
             接著不斷重複讀取溫濕度資料，並透過序列埠輸出顯示。
******************************************************************/

//------外部引入函式區--------------------------
#include "String.h"       // 引入 Arduino String 函式庫（字串處理）
#include "BM25S2021-1.h"  // 引入 BM25S2021-1 溫濕度感測模組的函式庫


//------全域變數區--------------------------
float TValue;    // 溫度值全域變數（供主程式與雲端上傳使用）
float HValue;    // 濕度值全域變數（供主程式與雲端上傳使用）
String DHTStr;   // 取得產品、序號、編號等文字（預留使用）


//-----------------感測元件物件區-------------------------
// 建立一個 BM25S2021_1 的物件，命名為 BMht，並使用 I2C（Wire）通訊方式
//BM25S2021_1 BMht(&Wire);    // 透過 IIC 讀取溫溼度感測模組
BM25S2021_1 BMht(&Wire1);     // 透過第一組 IIC 讀取溫溼度感測模組
//BM25S2021_1 BMht(&Wire2);   // 透過第二組 IIC 讀取溫溼度感測模組


//----------自定義函式區宣告--------------
void initDHT();              // 初始化溫溼度感測器，啟動 I2C 通訊
float readTemperature();     // 讀取溫度值
float readHumidity();        // 讀取濕度值
String getVersion();         // 讀取版本號
String getProductNumbetr();  // 讀取產品編號
String getSNNumbetr();       // 讀取產品序列號
void ShowDHTInformation();   // 印出溫溼度感測器產品所有資訊


//--------初始化溫溼度感測器，啟動 I2C 通訊-----------
void initDHT()  // 初始化溫溼度感測器，啟動 I2C 通訊
{
    BMht.begin();                          // 初始化感測器，啟動 I2C 通訊
    Serial.println("init Temerature OK");  // 印出初始化完成訊息（原文拼字為 Temerature）
}


//-------讀取溫度值------------
float readTemperature()  // 讀取溫度值
{
    return BMht.readTemperature();  // 讀取溫度值並回傳
}


//-------讀取濕度值------------
float readHumidity()  // 讀取濕度值
{
    return BMht.readHumidity();  // 讀取濕度值並回傳
}


//-----讀取版本號-------------
String getVersion()  // 讀取版本號
{
    return String(BMht.getFWVer());  // 讀取韌體版本，轉為 String 後回傳
}


//-----讀取產品編號-------------
String getProductNumbetr()  // 讀取產品編號
{
    return String(BMht.getPID());  // 讀取產品 ID，轉為 String 後回傳
}


//-----讀取產品序列號-------------
String getSNNumbetr()  // 讀取產品序列號
{
    return String(BMht.getSN());  // 讀取產品序號，轉為 String 後回傳
}


//-----印出溫溼度感測器產品所有資訊-------
void ShowDHTInformation()  // 印出溫溼度感測器產品所有資訊
{
    Serial.println("=======DeviceInfo(HEX)========= ");
    Serial.print("SN : ");
    Serial.println(getSNNumbetr());       // 顯示產品序號（十六進位格式）
    Serial.print("PID : ");
    Serial.println(getProductNumbetr());  // 顯示產品 ID
    Serial.print("VER : ");
    Serial.println(getVersion());         // 顯示韌體版本
    Serial.println("=============================== ");
}