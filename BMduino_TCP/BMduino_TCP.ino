/*****************************************************************
檔案名稱(File):       TCP.ino
功能說明(Description): 
1. 使用硬體序列埠 Serial2 與 WiFi 模組 BMC81M001 進行通訊，傳輸速率為 115200bps。
2. 使用 Serial (USB 序列埠) 作為監控輸出，顯示模組的網路狀態與 TCP 連線狀況。
3. 通電啟動時，自動完成 BMC81M001 模組的 WiFi 連線與 TCP 伺服器連線。
4. 當序列埠接收到資料時，直接將資料透過 TCP 傳送至伺服器。
******************************************************************/

#include "TCP.h"  // 引入 TCP 函式庫，包含 WiFi 物件類別與相關控制方法

//BMC81M001 Wifi(6,7);         // (選項1) 若使用「軟體序列埠」(Software Serial)，則 RX 接 6 腳，TX 接 7 腳
//BMC81M001 Wifi(&Serial1);    // (選項2) 若使用硬體序列埠 Serial1，請取消註解本行
BMC81M001 Wifi(&Serial2);      // (選項3) 使用硬體序列埠 Serial2，連接至 BMC81M001 模組（建議 BMduino 使用此設定）

//-------------------------------------------------------------
// 初始化設定區（setup 函式）
//-------------------------------------------------------------
void setup() 
{
  digitalWrite(LED, LOW);     // 將 LED 初始化為低電位（熄滅狀態）
  Serial.begin(9600);         // 啟動 USB 序列埠監控視窗，速率為 9600bps
  Wifi.begin();               // 啟動 WiFi 模組（進行內部初始化與序列通訊設置）

  Serial.print("TCP Connect Result：");  // 提示訊息 - 開始顯示 WiFi / TCP 連線結果

  //===========================
  // 步驟 1：連線至 WiFi 熱點
  //===========================
  Serial.print("SSID:(") ;
  Serial.print(WIFI_SSID) ;
  Serial.print(")\n") ;
  Serial.print("WIFI_PASS:(") ;
  Serial.print(WIFI_PASS) ;
  Serial.print(")\n") ;  
  //===========================
  if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS))   // 嘗試連線至定義於 BMC81M001.h 之 WiFi 熱點
  {
    Serial.print("WIFI fail,");   // 若回傳 false，表示 WiFi 連線失敗
  }  
  else
  {
    Serial.print("WIFI success,"); // WiFi 連線成功
  }

  //===========================
  // 步驟 2：連線至 TCP 伺服器
  //===========================
    //===========================
  // 步驟 1：連線至 WiFi 熱點
  //===========================
  Serial.print("IP:(") ;
  Serial.print(IP) ;
  Serial.print(")\n") ;
  Serial.print("IP_Port:(") ;
  Serial.print(IP_Port) ;
  Serial.print(")\n") ;  
  //===========================

  if(!Wifi.connectTCP(IP, IP_Port))   // 嘗試建立 TCP 連線至指定 IP 與 Port
  {
    Serial.print("IP fail");          // 若失敗，印出提示訊息
  } 
  else
  {
    Serial.print("IP success");       // 若成功，印出提示訊息
  }
}

//-------------------------------------------------------------
// 主執行迴圈區（loop 函式）
//-------------------------------------------------------------
void loop() {
    tcpBuff = Wifi.readDataTcp();   // 從 WiFi 模組讀取 TCP 接收緩衝區資料

    if(tcpBuff != 0)   // 若伺服器有傳回資料
    {
      Serial.println(tcpBuff);   // 將接收到的 TCP 資料顯示於序列監控視窗
    }

    //===========================
    // 讀取使用者輸入的序列資料
    //===========================
    while (Serial.available() > 0)   // 若 USB 序列埠有可讀取的資料
    {
      SerialBuff[resLen++] = Serial.read();   // 逐字節讀取資料存入 SerialBuff 緩衝區
      delay(10);                              // 短暫延遲，確保資料讀取完整
    }

    //===========================
    // 若序列緩衝區有資料要發送
    //===========================
    if(resLen > 0)
    {
      digitalWrite(LED, HIGH);  // 點亮 LED 表示正在傳送資料中

      // 嘗試透過 WiFi 模組將緩衝區資料寫入 TCP 通道
      if(Wifi.writeDataTcp(resLen, SerialBuff))    
      {
        Serial.println("Send data success");  // 傳送成功提示訊息
        digitalWrite(LED, LOW);               // 傳送完成後熄滅 LED
      }

      // 清除暫存緩衝區
      clearBuff();
    }
}

//-------------------------------------------------------------
// 緩衝區清除函式 (將緩衝資料歸零)
//-------------------------------------------------------------
void clearBuff(){
  memset(SerialBuff, '\0', RES_MAX_LENGTH);  // 將整個序列緩衝區以空字元 '\0' 清除
  resLen = 0;                                // 重設緩衝資料長度計數
}
