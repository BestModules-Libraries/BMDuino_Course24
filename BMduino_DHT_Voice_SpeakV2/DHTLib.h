/*****************************************************************
File:        readTemperatureAndHumidityWithIIC
Description: 開機後透過 I2C 介面讀取一次裝置資訊（包含序號SN、產品ID PID、韌體版本VER），
             接著不斷重複讀取溫濕度資料，並透過序列埠輸出顯示。
******************************************************************/
//------外部引入函式區--------------------------
#include "String.h"
#include "BM25S2021-1.h"  // 引入 BM25S2021-1 溫濕度感測模組的函式庫


//------全域變數區--------------------------
float TValue = 0 ; //溫度全域變數
float HValue = 0 ; //濕度全域變數
String DHTStr ;  //取得產品、序號、編號等文字
//-------------  end of 全域變數 ------------------
//-----------------感測元件物件區-------------------------
// 建立一個 BM25S2021_1 的物件，命名為 BMht，並使用 I2C（Wire）通訊方式
//BM25S2021_1 BMht(&Wire);    //透過IIC 讀取溫溼度感測模組
BM25S2021_1 BMht(&Wire1);     //透過第一組IIC 讀取溫溼度感測模組
//BM25S2021_1 BMht(&Wire2);   //透過第二組IIC 讀取溫溼度感測模組

//----------自定義函式區宣告--------------
void initDHT() ;// 初始化溫溼度感測器，啟動 I2C 通訊
float readTemperature(); // 讀取溫度值
float readHumidity(); // 讀取濕度值
String getVersion(); //讀取版本號
String getProductNumbetr(); //讀取產品編號
String getSNNumbetr(); //讀取產品序列號
void ShowDHTInformation();   //印出溫溼度感測器產品所有資訊


//--------初始化溫溼度感測器，啟動 I2C 通訊-----------
void initDHT()// 初始化溫溼度感測器，啟動 I2C 通訊
{
    BMht.begin();               // 初始化感測器，啟動 I2C 通訊
    Serial.println("init Temerature OK") ;
}

//-------讀取溫度值------------
float readTemperature() // 讀取溫度值
{
  return BMht.readTemperature() ;       // 讀取溫度值,回傳溫度值，
}

//-------讀取濕度值------------
float readHumidity() // 讀取濕度值
{
  return BMht.readHumidity();       // 讀取濕度值,回傳濕度值，
}

//-----讀取版本號-------------
String getVersion() //讀取版本號
{
  return String(BMht.getFWVer()) ;  // 讀取版本號,回傳版本號，
}

//-----讀取產品編號-------------
String getProductNumbetr() //讀取產品編號
{
  return String(BMht.getPID()) ;  // 讀取產品編號,回傳產品編號，
}

//-----讀取產品序列號-------------
String getSNNumbetr() //讀取產品序列號
{
  return String(BMht.getSN()) ;  // 讀取讀取產品序列號,回傳產品序列號，
}

//-----印出溫溼度感測器產品所有資訊-------
void ShowDHTInformation()   //印出溫溼度感測器產品所有資訊
{
    Serial.println("=======DeviceInfo(HEX)========= ");
    Serial.print("SN : ");
    Serial.println(getSNNumbetr());       // 以十六進位格式顯示序號
    Serial.print("PID : ");
    Serial.println(getProductNumbetr());      // 顯示產品 ID
    Serial.print("VER : ");
    Serial.println(getVersion());    // 顯示韌體版本
    Serial.println("=============================== ");   
}
