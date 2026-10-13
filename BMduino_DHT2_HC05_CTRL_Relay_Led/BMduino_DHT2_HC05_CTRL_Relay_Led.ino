/*************************************************
 * 檔案：        BMduino_DHT2_HC05_CTRL_Relay_Led.ino
 * 功能描述：    使用 BMDuino-UNO 控制 DHT 溫濕度感測器、OLED 顯示、藍牙通訊 (HC-05) 與繼電器模組
 *               可透過藍牙接收指令控制板載 LED 與繼電器，並定期上傳溫濕度資料至藍牙裝置
 * 硬體需求：     BMDuino-UNO (相容 Arduino UNO)、DHT 溫濕度感測器、0.96 吋 OLED 顯示器、HC-05 藍牙模組、繼電器模組
 * 通訊協定：     序列埠 (UART)、軟體序列埠 (SoftwareSerial)、I2C (OLED、DHT)
 * 藍牙指令對應： 
 *               0x4f ('O')：關閉板載 LED
 *               0x50 ('P')：開啟板載 LED
 *               0x51 ('Q')：開啟第 1 個繼電器
 *               0x52 ('R')：關閉第 1 個繼電器
 * 中文註解：ChatGPT 中文註解
 * 版本：         1.0
 * 更新日期：     2024-12-12
 *************************************************/

// ---------- 感測模組函式與外部函式引用宣告區 -----------
// 引用自訂的腳位初始化標頭檔，其中包含板載 LED 控制函式
#include "initPins.h"   // 控制板載 LED 的初始化與操作函式
#include "OledLib.h"    // OLED 顯示器控制函式庫
#include "DHTLib.h"     // DHT 溫濕度感測器函式庫
#include "RelayLib.h"   // 繼電器模組控制函式庫

// 引用 SoftwareSerial 函式庫，用於在非硬體序列埠腳位上模擬序列通訊
// 此函式庫允許我們使用任意數位腳位進行序列通訊（本例用於藍牙 HC-05 模組）
#include <SoftwareSerial.h>
// 若需使用硬體序列埠（如 Uno 的 0(RX)、1(TX)），可引入 HardwareSerial，但本例未使用

// ================================================================
// =============== 全域變數宣告區 (Global Variables) ===============
// ================================================================

// ================== 腳位設定 (請依 BMCOM2 實際接線調整) ==================
#define BT_RX_PIN 23   // Arduino 的接收腳位，接 HC-05 的 TX 腳（接收來自藍牙模組的資料）
#define BT_TX_PIN 24   // Arduino 的傳送腳位，接 HC-05 的 RX 腳（傳送資料至藍牙模組）
int loopcnt = 0;      // 延遲計數器，用於控制每 30 秒更新一次溫濕度資料並傳送至藍牙

// ================================================================
// =============== 感測器物件實例化區 (Sensor Object) =============
// ================================================================
// 建立藍牙軟體序列埠物件，指定 RX 與 TX 腳位
// 此物件將用於與 HC-05 藍牙模組進行通訊
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

// ================== 自定義函式宣告區 (Function Prototypes) ==================
void initSensor();             // 初始化所有感測模組（OLED、DHT、繼電器）
void initAll();                // 初始化整體系統（序列埠、藍牙、感測器）
void showTemperatureonOled(float ss);  // 在 OLED 上顯示溫度數值
void showHumidityonOled(float ss);     // 在 OLED 上顯示濕度數值
void judgeKeyCommand(unsigned char kk) ;// 參數：kk - 接收到的無符號字元資料（來自藍牙）


// ================== 初始化設定 (setup) ==================
// 此函式在 Arduino 啟動時執行一次，用於初始化設定硬體與通訊介面
void setup()
{
    initAll();          // 初始化整體系統（啟動序列埠、藍牙通訊、感測器模組）
    delay(200);         // 延遲 200ms，確保硬體模組穩定啟動

    // 顯示 BEST MODULES 的 LOGO 於 OLED 上
    clearScreen();      // 清除 OLED 螢幕
    drawPicture(0, 0, BestModule_LOGO, 128, 64); // 在 (0,0) 位置繪製 128x64 像素的 LOGO
    delay(3000);        // LOGO 顯示 3 秒
    clearScreen();      // 再次清除螢幕，準備顯示溫濕度資料

    // 提示已經進入主迴圈 loop()
    Serial.println("Enter Loop()"); 
    loopcnt = 0;        // 初始化迴圈計數器
}

// ================== 主迴圈，持續執行 (loop) ==================
// 此函式會不斷重複執行，主要功能為：
// 1. 監聽藍牙指令並執行對應控制動作
// 2. 每 30 秒讀取一次溫濕度並顯示於 OLED 與序列埠，同時傳送至藍牙裝置
void loop()
{
    // ---------- 檢查藍牙序列埠是否有資料可讀 ----------
    // 判斷 HC-05 藍牙模組是否接收到來自外部裝置（如手機、電腦）的資料
    if (btSerial.available())
    {
        // 當藍牙序列埠的接收緩衝區中有資料時，持續讀取直到清空緩衝區
        while (btSerial.available() > 0)
        {
            // 讀取一個位元組的資料（以無符號字元形式儲存）
            unsigned char btData = btSerial.read();
            // 將讀取到的資料轉為字元形式，顯示於序列埠監控視窗（用於除錯）
            Serial.print(char(btData));   // 以可視字元形式印出
            // 呼叫自訂函式，根據接收到的資料執行對應控制命令（如控制 LED、繼電器）
            judgeKeyCommand(btData);
        }
    }

    // 每 30 秒（loopcnt 從 0 到 30，每次 loop 延遲 1 秒）更新一次溫濕度資料
    if (loopcnt == 0)
    {
        // ---------- 讀取並顯示濕度數值 ----------
        HValue = readHumidity();        // 呼叫 DHT 函式讀取濕度值
        Serial.print("Humidity : ");
        Serial.print(HValue);          // 顯示濕度值於序列埠
        Serial.print(" %    ");

        // ---------- 讀取並顯示溫度數值 ----------
        TValue = readTemperature();    // 呼叫 DHT 函式讀取溫度值
        Serial.print("Temperature : ");
        Serial.print(TValue);          // 顯示溫度值於序列埠（註：此處原程式碼誤寫為 BMht.readTemperature()，已修正為 TValue）
        Serial.println(" °C ");        // 顯示溫度單位 °C

        // ---------- 顯示溫濕度資訊於 OLED ----------
        showTemperatureonOled(TValue); // 在 OLED 上顯示溫度
        showHumidityonOled(HValue);    // 在 OLED 上顯示濕度

        // ---------- 傳送溫濕度資訊到藍芽裝置 ----------
        btSerial.println("Temperature:  " + String(TValue) + " °C");  // 傳送溫度資料至藍牙裝置
        btSerial.println("Humidity:  " + String(HValue) + " %");      // 傳送濕度資料至藍牙裝置
    }

    // 主迴圈每次執行後延遲 1 秒，並更新計數器
    delay(1000);
    loopcnt++;

    // 當計數器超過 30 秒（即 30 次迴圈）時重置，實現每 30 秒更新一次溫濕度
    if (loopcnt >= 30)
        loopcnt = 0;
}

// ================== 自訂函式：判斷指令並執行對應動作 ==================
// 參數：kk - 接收到的無符號字元資料（來自藍牙）
void judgeKeyCommand(unsigned char kk)  // 參數：kk - 接收到的無符號字元資料（來自藍牙）
{
    // 若接收到的資料為 0x4f（對應 ASCII 字元 'O'，代表關閉 LED）
    if (kk == 0x4f)
    {
        Serial.println("設定 LED Pin 為低電位，關閉 LED");
        TurnOffLed13();  // 呼叫函式關閉板載 LED（位於腳位 13）
        btSerial.println("設定 LED Pin 為低電位，關閉 LED");
    }

    // 若接收到的資料為 0x50（對應 ASCII 字元 'P'，代表開啟 LED）
    if (kk == 0x50)
    {
        Serial.println("設定 LED Pin 為高電位，開啟 LED");
        TurnOnLed13();   // 呼叫函式開啟板載 LED（位於腳位 13）
        btSerial.println("設定 LED Pin 為高電位，開啟 LED");
    }

    // 若接收到的資料為 0x51（對應 ASCII 字元 'Q'，代表開啟繼電器）
    if (kk == 0x51)
    {
        Serial.println("開啟第 1 個繼電器");
        TurnonRelay(1);  // 開啟第 1 個繼電器（依 RelayLib 實作而定）
        btSerial.println("開啟第 1 個繼電器");
    }

    // 若接收到的資料為 0x52（對應 ASCII 字元 'R'，代表關閉繼電器）
    if (kk == 0x52)
    {
        Serial.println("關閉第 1 個繼電器");
        TurnoffRelay(1); // 關閉第 1 個繼電器（依 RelayLib 實作而定）
        btSerial.println("關閉第 1 個繼電器");
    }
}

// ================== 自訂函式：初始化所有感測模組 ==================
void initSensor()
{
    initOled();            // 初始化 OLED 12864 (0.96吋 OLED BMD31M090)
    delay(2000);           // 延遲 2 秒，等待顯示模組穩定
    initDHT();             // 初始化溫溼度感測器，啟動 I2C 通訊
    ShowDHTInformation();  // 印出溫溼度感測器的完整產品資訊（型號、版本等）
    initRealy();           // 初始化繼電器模組（設定腳位模式等）
}

// ================== 自訂函式：初始化整體系統 ==================
void initAll()
{
    // 初始化硬體序列埠（USB 連接至電腦），鮑率設定為 9600
    // 此序列埠用於與電腦上的 Arduino IDE 序列埠監控視窗通訊
    Serial.begin(9600);
   // 初始化 HC-05 藍牙模組的軟體序列埠，鮑率設定為 9600（HC-05 預設值）
    btSerial.begin(9600);
    // 在序列埠監控視窗顯示啟動訊息，提示使用者程式已開始執行
    Serial.println("=== HC-05 Bluetooth Test ===");
    Serial.println("Waiting for Bluetooth data...");
    Serial.println("System Start.....");
    // 初始化所有感測模組（OLED、DHT、繼電器）
    initSensor();
}

// ================== 自訂函式：在 OLED 上顯示溫度 ==================
// 參數：ss - 溫度數值（浮點數）
void showTemperatureonOled(float ss)
{
    printText(0, 4, "              ");  // 清空 OLED 上對應的顯示區域
    printText(0, 4, String(ss) + " °C"); // 將溫度轉為字串並顯示於 OLED (x=0, y=4)
    Serial.print("Temperature on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}

// ================== 自訂函式：在 OLED 上顯示濕度 ==================
// 參數：ss - 濕度數值（浮點數）
void showHumidityonOled(float ss)
{
    printText(62, 4, "         ");     // 清空 OLED 上對應的顯示區域
    printText(62, 4, String(ss) + " %"); // 將濕度轉為字串並顯示於 OLED (x=62, y=4)
    Serial.print("Humidity on OLED:(");
    Serial.print(ss);
    Serial.print(")\n");
}