#include "TCP.h"  //將WIFI模組加入
#include "OledLib.h"  //將OLED12864顯示模組加入
#include <String.h>    // Arduino 內建字串處理函式庫
//#include "commlib.h"   // 通訊相關的共用函式庫（可能包含封包處理、緩衝區管理等）

// 包含 BMH08002-4 庫，用於與血氧感測器進行通訊
#include <BMH08002-4.h>
    
// 定義 mySpo2 物件，根據硬體序列埠選擇不同的配置
// 如果不使用軟體序列埠，請註解以下行
 BMH08002_4 mySpo2(8, 7, 6);       // enPin, rxPin, txPin

// 如果使用硬體序列埠 Serial1，請取消註解此行（適用於 BMduino 的 Serial1）
//BMH08002_4 mySpo2(22, &Serial1);  // enPin 設為 22，與 Serial1 連結

// 如果使用硬體序列埠 Serial2，請取消註解此行（適用於 BMduino 的 Serial2）
// BMH08002_4 mySpo2(25, &Serial2);

// 如果使用硬體序列埠 Serial3，請取消註解此行
// BMH08002_4 mySpo2(2, &Serial3);

// 如果使用硬體序列埠 Serial4，請取消註解此行
// BMH08002_4 mySpo2(2, &Serial4);

// 定義變數
uint8_t Mode = 0;                  // 模式變數，0 表示連續模式，1 表示查詢模式
uint8_t rBuf[15] = {0};            // 接收緩衝區陣列，用於儲存感測器返回的數據（長度 15 位元組，初始值為 0）
uint8_t Status = 0;                // 狀態變數，用於儲存感測器返回的狀態碼
uint8_t flag = 0;                  // 旗標變數，用於追蹤手指移動狀態

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
void INITtWIFI();                  // 初始化 WiFi 網路連線
void showDeviceonOled(String ss,int row);  //列印Device ID於OLED上
void showTitleonOled(String ss,int row); //列印抬頭於OLED上
void showIPonOled(String ss,int row); //列印IP Address於OLED上
//-------列印ＭＡＣ　Ａｄｄｒｅｓｓ於OLED上---------------
void showMACeonOled(String ss,int row); //列印MAC Address於OLED上
//-------列印ＭＡＣ　Ａｄｄｒｅｓｓ於OLED上---------------
void showSSIDeonOled(String ss,int row); //列印SSID於OLED上

// ------- 全域變數宣告區 -----------
String SSIDData;   // 儲存 WiFi 熱點名稱 (SSID)
String IPData;     // 儲存 WiFi 分配到的 IP 位址
String MacData;     // 儲存 WiFi 分配到的 MAC Address
// MacData 在其他函式中宣告，作為全域變數

// ------------------ 初始化 ------------------




void setup()
{
   initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
  INITtWIFI();     // 初始化 WiFi 網路，並取得 SSID、IP 與 MAC 資料

 //---------------------------------
  clearScreen();  // 清除 OLED 螢幕
  // 顯示 BEST MODULES 的 LOGO
  drawPicture(0, 0, BestModule_LOGO, 128, 64);
  delay(3000);    // LOGO 顯示 3 秒
  clearScreen();  // 再次清除螢幕

  // 顯示系統資訊於 OLED
  showTitleonOled(MacData,0);  // 在 OLED 顯示 MAC 位址
  showIPonOled(IPData,2);      // 在 OLED 顯示 IP 位址
  //----------------------------
  Serial.println("Enter Loop()"); // 提示已經進入主迴圈 loop()
  mySpo2.begin();                  // 初始化血氧感測器模組
  mySpo2.setModeConfig(0x02);      // 設定感測器為查詢回應模式（0x02），手指檢測到時紅燈亮起
  mySpo2.setTimeInterval(300);     // 設定測量間隔為 300 毫秒
  Serial.println("請放置您的手指");  // 提示用戶放置手指
  delay(2000);                     // 等待 2 秒，讓用戶有時間放置手指
  mySpo2.beginMeasure();           // 開始測量

  Mode = mySpo2.getModeConfig();   // 獲取當前模式配置
  if (Mode == 0x02 || Mode == 0x03) // 如果模式為 0x02（查詢模式）或 0x03，則設 Mode 為 1
  {
    Mode = 1;
  }
  else Mode = 0;                   // 否則設為 0（連續模式）
}

void loop()
{
  switch (Mode)                    // 根據 Mode 變數選擇執行模式
  {
    case 1:
      Mode_ask();                  // 執行查詢模式
      break;
    default:
      Mode_continuous_timing();    // 執行連續模式
  }
}

void Mode_ask()
{
  Status = mySpo2.requestInfoPackage(rBuf); // 請求感測器返回數據包，存入 rBuf，並獲取狀態碼
  if (Status == 0x02)                      // 如果狀態為 0x02，表示測量完成
  {
    Serial.println("測量完成，可以移開手指");  // 提示測量完成
    Serial.print("SpO2:");                 // 輸出血氧飽和度
    Serial.print(rBuf[0], DEC);            // 顯示 rBuf[0]（血氧值），以十進位顯示
    Serial.println("%");                   // 單位為百分比
    Serial.print("心率:");                 // 輸出心率
    Serial.print(rBuf[1], DEC);            // 顯示 rBuf[1]（心率值），以十進位顯示
    Serial.println("BMP");                 // 單位為每分鐘跳動次數（Beats Per Minute）
    Serial.print("PI:");                   // 輸出灌注指數
    Serial.print((float)rBuf[2] / 10);     // 顯示 rBuf[2]（灌注指數），除以 10 轉為小數
    Serial.println("%");                   // 單位為百分比
    mySpo2.endMeasure();                   // 停止測量
    mySpo2.sleep();                        // 進入低功耗休眠模式
  }
  if (Status == 0x01 && flag != 1)         // 如果狀態為 0x01（測量中，手指移動），且 flag 不是 1
  {
    Serial.println("請勿移動您的手指");      // 提示用戶不要移動手指
    flag = 1;                              // 設定 flag 為 1，避免重複提示
  }
  if (Status == 0x00 && flag != 0)         // 如果狀態為 0x00（未檢測到手指），且 flag 不是 0
  {
    Serial.println("請重新放置您的手指");    // 提示用戶重新放置手指
    flag = 0;                              // 將 flag 設為 0
  }
}

void Mode_continuous_timing()
{
  if (mySpo2.isInfoAvailable() == true)    // 檢查序列埠接收緩衝區是否有數據可用
  {
    mySpo2.readInfoPackage(rBuf);         // 讀取感測器返回的數據包到 rBuf
    for (uint8_t i = 0; i < 15; i++)      // 遍歷 rBuf 陣列的所有 15 個位元組
    {
      Serial.print(i);                    // 輸出索引
      Serial.print(":");                  // 分隔符
      Serial.println(rBuf[i]);            // 輸出對應的數據值
    }
  }
}
// ------------------ 系統初始化區 ------------------

// 初始化所有感測模組
void initSensor()
{
  initOled();    // 初始化 OLED 12864 (0.96吋 OLED BMD31M090)
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
  
}

// 初始化整體系統
void initAll()
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  initSensor();        // 呼叫初始化感測模組
}

// 初始化 WiFi
void INITtWIFI()
{
  initWiFi();   // 初始化 WiFi 功能
  Serial.println("");
  Serial.println("---wifi access point----");

  SSIDData = GetSSID();   // 取得連線的 WiFi 熱點名稱
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得裝置分配到的 IP 位址
  Serial.println(IPData);

  MacData = GetMAC();     // 取得裝置的 MAC 位址
  Serial.println("---MAC Address----");
  Serial.println(MacData);
}










