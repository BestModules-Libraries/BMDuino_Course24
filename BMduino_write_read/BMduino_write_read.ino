/*****************************************************************
File:         write_read.ino
Description:  
  1. 應用電路：模組連接至 BMduino UNO 的 BMCOM1 
  2. 說明：
     - UART port1 傳送 1 個位元組 (byte) 給 UART port2，然後 port2 接收並將資料顯示於序列埠監控視窗。
     - UART port1 傳送 16 個位元組給 UART port2，然後 port2 接收並將資料顯示於序列埠監控視窗。
******************************************************************/
#include "BMB22M210.h"  // 引入 BMduino 專用的 UART 通訊模組函式庫

// 以下為 UART 模組初始化的方式 (依需求選擇正確的 Serial 埠)
// 使用 BMduino 上的 Serial1 硬體序列埠 (HW Serial1)，並且使用 pin 22 作為控制腳位

// 如果要使用其他硬體 UART，可以取消註解以下任一行：
//BMB22M210 mySerial(22, &Serial1);   
BMB22M210 mySerial(25, &Serial2); // 使用 Serial2
//BMB22M210 mySerial(3,  &Serial3); // 使用 Serial3
//BMB22M210 mySerial(3,  &Serial4); // 使用 Serial4

// 定義要傳送的 16 筆資料 (內容為數字 1 ~ 16)，型態為 uint8_t (8位元無號整數)
uint8_t sendbuff[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

// 接收緩衝區，用來存放從 UART port2 讀到的資料
int receivebuff[16] = {0};  

// 用來當作迴圈計數器
uint8_t i = 0;  

// 用來暫存單一筆資料
int data = 0;  

void setup() 
{ 
   Serial.begin(9600);            // 啟動 Serial (USB 虛擬序列埠)，用於 Arduino IDE 序列埠監控視窗
   mySerial.begin(9600);          // 啟動 BMB22M210 模組，設定傳輸速率為 9600 bps
   mySerial.beginPort(1, 9600);   // 設定 UART port1 的鮑率 (baud rate) 為 9600
   mySerial.beginPort(2, 9600);   // 設定 UART port2 的鮑率 (baud rate) 為 9600
}

void loop() 
{
   // ===== 範例 1：傳送與接收單一位元組 =====
   Serial.println("port1 write 1 byte:0x75");
   mySerial.write(1, 0x75);          // 由 UART port1 傳送一個位元組資料 (16進位 0x75 = 十進位 117)
   delay(2);                         // 稍微延遲，讓 port2 有時間接收資料

   Serial.print("num of port2 can be read:");
   Serial.println(mySerial.available(2)); // 顯示 UART port2 的可讀取資料數量

   Serial.print("port2 read:");
   if (mySerial.available(2) >= 1)   // 若 port2 的接收緩衝區至少有 1 筆資料
   {
     Serial.print(mySerial.read(2)); // 讀取 UART port2 的 1 筆資料並輸出至序列埠監控視窗
     Serial.print(" ");
   }
   Serial.println("");

   // ===== 範例 2：傳送與接收 16 個位元組 =====
   Serial.println("port1 write 16 bytes:1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16");
   mySerial.writeBytes(1, sendbuff, 16);   // 由 UART port1 一次傳送 16 筆資料
   delay(20);                              // 延遲 20ms，確保資料已傳輸完成

   Serial.print("num of port2 can be read:");
   Serial.println(mySerial.available(2));  // 顯示 UART port2 的可讀取資料數量

   Serial.print("port2 read:");
   if (mySerial.available(2) >= 16)        // 確認 port2 接收到至少 16 筆資料
   {
     // 將接收到的 16 筆資料存入 receivebuff[]
     mySerial.readBytes(2, receivebuff, 16);  

     // 使用迴圈逐筆印出接收到的資料
     for (i = 0; i < 16; i++)
     {
       Serial.print(receivebuff[i]);  // 輸出每一筆接收到的數字
       Serial.print(" ");
     }
   }
   Serial.println("");
   Serial.println("");

   delay(1000);   // 延遲 1 秒再進行下一次傳輸測試
}
