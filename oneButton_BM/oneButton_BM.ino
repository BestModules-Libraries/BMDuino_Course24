#define ButtoonPin  7


// setup() 函式：當 Arduino 上電或按下重設鍵時只會執行一次
void setup() 
{
  Serial.begin(9600);  // 啟動序列通訊，設定傳輸速率為 9600 bps（與電腦端同步）
  
  // 顯示啟動訊息到序列監控器
  Serial.println("Start System");  
  // 將所有 LED 所接的腳位設為「輸出模式」並先全部關閉
    pinMode(ButtoonPin, INPUT);     // 將ButtoonPin的腳位設為輸入
}

// loop() 函式：主要的重複執行區，每秒跑數百次（Arduino 核心迴圈）
void loop() 
{
  // 檢查是否有資料從序列埠輸入（例如使用者從電腦序列監控器輸入）
  if (digitalRead(ButtoonPin) == 0)
  {
    Serial.println("Button is Pressed");
  }
    else
  {
    Serial.println("Button is Pressed");
  }
  delay(300) ;
}
