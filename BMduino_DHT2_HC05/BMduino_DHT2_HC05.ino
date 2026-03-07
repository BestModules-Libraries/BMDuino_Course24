/*************************************************
 * 檔案：        BMduino_DHT2_HC05.ino

 * 中文註解：      ChatGPT
 * 開發板：       BMDuino-UNO (相容於 Arduino UNO)
 *************************************************/

// ---------- 感測模組函式與外部函式引用宣告區 -----------
// 引用自訂的腳位初始化標頭檔，其中包含板載 LED 控制函式
#include "initPins.h"
#include "OledLib.h"
#include "DHTLib.h"


// 引用 SoftwareSerial 函式庫，用於在非硬體序列埠腳位上模擬序列通訊
// 此函式庫允許我們使用任意數位腳位進行序列通訊
#include <SoftwareSerial.h>
// 若需使用硬體序列埠（如 Uno 的 0(RX)、1(TX)），可引入 HardwareSerial，但本例未使用

// ================================================================
// =============== 全域變數宣告區 (Global Variables) ===============
// ================================================================
// ================== 腳位設定 (請依 BMCOM2 實際接線調整) ==================
#define BT_RX_PIN 23   // Arduino 的接收腳位，接 HC-05 的 TX 腳（接收來自藍牙模組的資料）
#define BT_TX_PIN 24   // Arduino 的傳送腳位，接 HC-05 的 RX 腳（傳送資料至藍牙模組）

// ================================================================
// =============== 感測器物件實例化區 (Sensor Object) =============
// ================================================================
// 建立藍牙軟體序列埠物件，指定 RX 與 TX 腳位
// 此物件將用於與 HC-05 藍牙模組進行通訊
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

//-------自定義函式宣告區-----------
void initSensor();   // 初始化所有感測模組
void initAll() ;  // 初始化整體系統
void showTemperatureonOled(float ss); //列印溫度於OLED上
void showHumidityonOled(float ss);  //列印濕度於OLED上-

// ================== 初始化設定 (setup) ==================
// 此函式在 Arduino 啟動時執行一次，用於初始化設定
void setup()
{
    initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
  //---------------------------------
  clearScreen();  // 清除 OLED 螢幕
  // 顯示 BEST MODULES 的 LOGO
  drawPicture(0, 0, BestModule_LOGO, 128, 64);
  delay(3000);    // LOGO 顯示 3 秒
  clearScreen();  // 再次清除螢幕

  //----------------------------
  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()
}

// ================== 主迴圈，持續執行 (loop) ==================
// 此函式會不斷重複執行，為程式的主邏輯區
void loop()
{

    // 從感測器讀取濕度數值並顯示
    HValue = readHumidity();        //讀取濕度數
    Serial.print("Humidity : ");
    Serial.print(HValue);       // 顯示濕度值
    Serial.print(" %    ");   

    // 從感測器讀取溫度數值並顯示
    TValue = readTemperature();        //讀取濕度數
    Serial.print("Temperature : ");
    Serial.print(BMht.readTemperature());    // 顯示溫度值
    Serial.println(" °C ");                 // 顯示溫度單位 °C
    //秀溫溼度資訊在OLED上
    showTemperatureonOled(TValue); //列印溫度於OLED上
    showHumidityonOled(HValue);  //列印濕度於OLED上- 
    //傳送溫溼度資訊到藍芽裝置  
    btSerial.println("Temperature:  "+String(TValue)+".C");
    btSerial.println("Humidity:  "+String(HValue)+" %");
     delay(30000) ;              // 每隔 30 秒讀取並更新一次溫濕度資料
}


//-----初始化所有感測模組----------
void initSensor()	// 初始化所有感測模組
{
  initOled();    // 初始化 OLED 12864 (0.96吋 OLED BMD31M090)
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
  initDHT();   // 初始化溫溼度感測器，啟動 I2C 通訊
  ShowDHTInformation(); //印出溫溼度感測器的完整產品資訊
}

// ------------------ 系統初始化區 ------------------
// 初始化整體系統
void initAll()	// 初始化整體系統
{
  // 初始化硬體序列埠（USB 連接至電腦），鮑率設定為 9600
  // 此序列埠用於與電腦上的 Arduino IDE 序列埠監控視窗通訊
  Serial.begin(9600);
  // 初始化 HC-05 藍牙模組的軟體序列埠，鮑率設定為 9600（HC-05 預設值）
  btSerial.begin(9600);
  // 在序列埠監控視窗顯示啟動訊息，提示使用者程式已開始執行
  Serial.println("=== HC-05 Bluetooth Test ===");
  Serial.println("Waiting for Bluetooth data...");
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor(); 
}
//-------列印溫度於OLED上---------------
void showTemperatureonOled(float ss) //列印溫度於OLED上
{
      printText(0,4,"              ")  ;  //清空
    printText(0,4,String(ss)+".C")  ;  //轉換string字串到char array
    Serial.print("Temperature on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印濕度於OLED上---------------
void showHumidityonOled(float ss)  //列印濕度於OLED上-
{
   printText(62,4,"         ")  ;  //清空
    printText(62,4,String(ss)+"%")  ;  //轉換string字串到char array
    Serial.print("Humidity on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

