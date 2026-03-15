/*************************************************
 * 檔案：        HC05-read.ino
 * 功能描述： 
 *   本程式搭配 BMDuino-UNO 開發板與 BMCOM2 擴充板，
 *   連接 HC-05 藍牙模組，實現藍牙通訊功能。
 *   將從藍牙接收到的資料即時顯示於序列埠監控視窗，
 *   並可將序列埠監控視窗輸入的資料傳送至藍牙裝置。
 *   
 * 中文註解：      ChatGPT
 * 開發板：       BMDuino-UNO (相容於 Arduino UNO)
 *************************************************/

// 引入 SoftwareSerial 函式庫，用於在非硬體序列埠腳位上模擬序列通訊
#include <SoftwareSerial.h>
// 如果需使用硬體序列埠（如 Uno 的 0(RX)、1(TX)），可引入 HardwareSerial，但本例未使用

// ================== 腳位設定 (請依 BMCOM2 實際接線調整) ==================
#define BT_RX_PIN 23   // Arduino 的接收腳位，接 HC-05 的 TX 腳
#define BT_TX_PIN 24   // Arduino 的傳送腳位，接 HC-05 的 RX 腳

// 建立藍牙軟體序列埠物件，指定 RX 與 TX 腳位
SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

// ================== 初始化設定 ==================
void setup()
{
  // 初始化硬體序列埠（USB 連接至電腦），鮑率設定為 9600
  // 此序列埠用於與電腦上的 Arduino IDE 序列埠監控視窗通訊
  Serial.begin(9600);
  // 初始化 HC-05 藍牙模組的軟體序列埠，鮑率設定為 9600（HC-05 預設值）
  btSerial.begin(9600);
  // 在序列埠監控視窗顯示啟動訊息
  Serial.println("=== HC-05 Bluetooth Test ===");
  Serial.println("Waiting for Bluetooth data...");
}

// ================== 主迴圈，持續執行 ==================
void loop()
{
  // 檢查藍牙序列埠是否有資料可讀（即 HC-05 是否收到資料）
  if (btSerial.available())
  {
    // 當藍牙序列埠的接收緩衝區中有資料時，持續讀取
    while(btSerial.available() > 0)
    {
      // 讀取一個位元組的資料（以無符號字元形式儲存）
      unsigned char btData = btSerial.read();
      // 將讀取到的資料轉為字元形式，顯示於序列埠監控視窗
      Serial.print(char(btData));   // 以可視字元形式印出
      // 亦可使用以下方式直接寫入原始位元組資料（不進行字元轉換）：
      // Serial.write(btData);
    }
  }

  // （可選功能）若序列埠監控視窗有輸入資料，則將其轉發至藍牙裝置
  if (Serial.available())
  {
    // 讀取電腦序列埠監控視窗輸入的一個位元組資料
    unsigned char pcData = Serial.read();
    
    // 將該資料透過藍牙序列埠傳送至 HC-05（再傳至配對的藍牙裝置）
    btSerial.write(pcData);
  }
}


