#include "TCP.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
#include "counding.h"  // 引入 TCP 控制邏輯相關函式與變數定義（應包含 WiFi 初始化、MAC 取得等）
float Tvalue, Hvalue; //用溫溼度

void setup() 
{
  digitalWrite(LED, LOW);  // 將 LED 腳位設定為 LOW（熄滅），可視為開機狀態指示

  Serial.begin(9600);      // 啟動主機序列埠（USB）通訊，速率為 9600 bps，作為監控輸出
  initWiFi();              // 執行 WiFi 模組初始化與連線（定義於 TCP.h / BMC81M001.h）

  Serial.println("");      // 印出空行，純粹格式美觀
  Serial.println("---wifi access point----"); // 分隔線，美觀用途
  Serial.println(GetSSID()); //取得 SSID熱點字串
  // Serial.println(typeof(Wifi.SSID())) ; // 註解掉的除錯語句，用於查資料型別
    Serial.println("---Show IP Address----"); // 分隔線，美觀用途
  Serial.println(GetIP()); // 印出取得 連接上的SSID熱點之後閘道器IP位址
   if(!Wifi.connectTCP(IP,IP_Port))
  {
    Serial.print("IP fail");
  } 
  else
  {Serial.print("IP success"); }
}


void loop() {
    tcpBuff=Wifi.readDataTcp();
    if(tcpBuff!=0) 
    {
      Serial.println(tcpBuff);
    }
    while (Serial.available() > 0)
    {
      SerialBuff[resLen++] = Serial.read();
      delay(10);
    }
    if(resLen>0)
    {
      digitalWrite(LED, HIGH);
      if(Wifi.writeDataTcp(resLen,SerialBuff))    
      {
        Serial.println("Send  data sucess");
        digitalWrite(LED,LOW);
      }
      clearBuff();
    }
}
void clearBuff(){
  memset(SerialBuff,'\0',RES_MAX_LENGTH);
  resLen = 0;
}


