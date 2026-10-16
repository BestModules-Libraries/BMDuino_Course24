/*****************************************************************
File:         TCP.ino
Description: 1. Hardware serial port 1 is used for communication with BMC81M001, with a communication rate of 115200bps
             2. The serial port is used as a monitoring serial port to print and output some status of BMC81M001
             3. Automatically complete BMC81M001 networking action when powered on
             4. When serial port sends data, its data is directly transmitted through to TCP server
******************************************************************/
#include "TCP.h" // 引入 TCP 庫

//BMC81M001 Wifi(6,7);// 使用軟體序列（rxPin 6 , txPin 7），若不使用軟體序列請註解掉這行
//BMC81M001 Wifi(&Serial); // 使用硬體序列0（HW Serial），若使用 BMduino 的硬體序列，請取消這行的註解
BMC81M001 Wifi(&Serial1); // 使用硬體序列1（HW Serial1），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial2); // 使用硬體序列2（HW Serial2），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial3); // 使用硬體序列3（HW Serial3），若使用 BMduino 的硬體序列，請取消這行的註解
//BMC81M001 Wifi(&Serial4); // 使用硬體序列4（HW Serial4），若使用 BMduino 的硬體序列，請取消這行的註解

void setup() 
{
  digitalWrite(LED, LOW); // 將 LED 設定為低電平，關閉 LED
  Serial.begin(9600); // 設定序列通訊，鮑率為 9600
  Wifi.begin(); // 初始化 WiFi 模組
  Serial.print("TCP Connect Result："); // 顯示 TCP 連線結果

  // 嘗試連線至 WiFi AP
  if(!Wifi.connectToAP(WIFI_SSID, WIFI_PASS)) 
  {
    Serial.print("WIFI fail,"); // 顯示 WiFi 連線失敗訊息
  }  
  else
  {
    Serial.print("WIFI success,"); // 顯示 WiFi 連線成功訊息
  }

  // 嘗試連線至 TCP 伺服器
  if(!Wifi.connectTCP(IP, IP_Port))
  {
    Serial.print("IP fail"); // 顯示 IP 連線失敗訊息
  } 
  else
  {
    Serial.print("IP success"); // 顯示 IP 連線成功訊息
  }
}

void loop() {
    // 讀取 TCP 傳來的數據
    tcpBuff = Wifi.readDataTcp();
    if(tcpBuff != 0) 
    {
      Serial.println(tcpBuff); // 若收到數據則顯示於序列監控
    }

    // 讀取 Serial 輸入數據
    while (Serial.available() > 0)
    {
      SerialBuff[resLen++] = Serial.read(); // 將接收的數據儲存至 SerialBuff 緩衝區
      delay(10); // 延遲 10 毫秒
    }

    // 若緩衝區有數據，則送出至 TCP
    if(resLen > 0)
    {
      digitalWrite(LED, HIGH); // 打開 LED，表示開始傳輸
      if(Wifi.writeDataTcp(resLen, SerialBuff))    
      {
        Serial.println("Send data success"); // 顯示數據傳輸成功訊息
        digitalWrite(LED, LOW); // 關閉 LED
      }
      clearBuff(); // 清空緩衝區
    }
}

// 清空緩衝區函數
void clearBuff() {
  memset(SerialBuff, '\0', RES_MAX_LENGTH); // 將 SerialBuff 清空
  resLen = 0; // 重置緩衝區長度
}
