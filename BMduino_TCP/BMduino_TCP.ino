/*****************************************************************
檔案名稱(File):       TCP.ino
功能說明(Description): 
⚫ 此程式透過硬體序列埠 Serial2 與 WiFi 模組 BMC81M001 建立通訊。
⚫ USB 序列埠 Serial 作為監控視窗，顯示 WiFi、TCP 連線與資料傳送狀況。
⚫ 程式啟動後，會自動執行：
    (1) WiFi 熱點連線
    (2) TCP Server 連線
⚫ 當使用者於 USB 序列輸入訊息，會透過 WiFi 模組的 TCP Client 送出至伺服器。
******************************************************************/

#include "TCP.h"  // 引入 TCP 函式庫，內含 BMC81M001 模組控制類別、WiFi、TCP 功能封裝

//==============================================================
// 建立 BMC81M001 WiFi 模組物件（依不同序列埠方式而定）
//==============================================================

//BMC81M001 Wifi(6,7);         // (選項1) 使用「軟體序列埠」SoftwareSerial，RX 接 D6, TX 接 D7
//BMC81M001 Wifi(&Serial1);    // (選項2) 使用硬體序列埠 Serial1（視開發板支援腳位而定）
BMC81M001 Wifi(&Serial2);      // (選項3) 建議 BMduino：使用硬體序列埠 Serial2 與模組通訊，最穩定最推薦


//-------------------------------------------------------------
// 初始化設定區（setup 函式）- 開機後只執行一次
//-------------------------------------------------------------
void setup() 
{
  digitalWrite(LED, LOW);     // LED 設為關閉(LOW)，做為指示燈使用（亮表示正在傳送資料）
  Serial.begin(9600);         // 啟動 USB Serial，用於與電腦序列視窗通訊
  Wifi.begin();               // 初始化 WiFi 模組內部通訊、波特率同步等設定

  Serial.print("TCP Connect Result：");  // 顯示程序開始進行 TCP/WiFi 連線流程

  //===========================================================
  // Step 1：嘗試連線至 WiFi 熱點 (SSID + Password)
  //===========================================================
  Serial.print("SSID:(") ;
  Serial.print(WIFI_SSID) ;
  Serial.print(")\n") ;
  Serial.print("WIFI_PASS:(") ;
  Serial.print(WIFI_PASS) ;
  Serial.print(")\n") ;  

  // 呼叫函式 connectToAP() 嘗試連線 AP，回傳布林值
  if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS))   // 若回傳 false
  {
    Serial.print("WIFI fail,");   // WiFi 連線失敗
  }  
  else
  {
    Serial.print("WIFI success,"); // WiFi 連線成功
  }

  //===========================================================
  // Step 2：使用 TCP Client 連線至指定伺服器 IP & Port
  //===========================================================
  Serial.print("IP:(") ;
  Serial.print(IP) ;     // 顯示 TCP 欲連線的 Server IP
  Serial.print(")\n") ;
  Serial.print("IP_Port:(") ;
  Serial.print(IP_Port) ;// 顯示 TCP Server 的 Port
  Serial.print(")\n") ;  

  // 嘗試建立 TCP 連線
  if(!Wifi.connectTCP(IP, IP_Port))   // 若回傳 false 即失敗
  {
    Serial.print("IP fail");          // TCP 連線失敗
  } 
  else
  {
    Serial.print("IP success");       // TCP 成功與伺服器建立連線
  }
}


//-------------------------------------------------------------
// 主執行迴圈（loop）- 不斷重複執行
//-------------------------------------------------------------
void loop() {

    //===========================================================
    // 檢查是否有 TCP Server 傳回資料 → 若有，則讀出
    //===========================================================
    tcpBuff = Wifi.readDataTcp();   // 從 WiFi 模組讀取 TCP 接收緩衝資料

    if(tcpBuff != 0)               // 若收到資料(不為0)
    {
      Serial.println(tcpBuff);     // 顯示伺服器回傳資料至序列監控視窗
    }

    //===========================================================
    // 讀取 USB 序列傳入資料（使用者從 Serial Monitor 輸入）
    //===========================================================
    while (Serial.available() > 0)     // 若 USB Serial 緩衝中有資料存在
    {
      SerialBuff[resLen++] = Serial.read();  // 逐字讀取，存入 SerialBuff 緩衝
      delay(10);                              // 延遲避免資料流過快讀不全
    }

    //===========================================================
    // 若有序列來源資料 → 準備透過 TCP 傳送出去
    //===========================================================
    if(resLen > 0)
    {
      digitalWrite(LED, HIGH);  // 點亮 LED (HIGH)，表示正在進行資料傳送

      // writeDataTcp() 將 USB 序列資料透過 TCP 發送至伺服器
      if(Wifi.writeDataTcp(resLen, SerialBuff))    
      {
        Serial.println("Send data success");  // 傳送成功提示
        digitalWrite(LED, LOW);               // 傳送完成 → 熄滅 LED
      }

      // 清除資料緩衝區，等待下一次傳送
      clearBuff();
    }
}


//-------------------------------------------------------------
// 緩衝區清除函式：將 SerialBuff 重製為空字元
//-------------------------------------------------------------
void clearBuff(){
  memset(SerialBuff, '\0', RES_MAX_LENGTH);  // 清空整個暫存陣列內容
  resLen = 0;                                // 緩衝區長度計數變數歸零
}
