#define ButtoonPin 7  // 定義按鈕接在數位腳位 7（之後用變數 ButtoonPin 取代數值 7）
int keystatus = 0;    // 宣告一個整數變數 keystatus，用來儲存按鈕目前的狀態（0 或 1）

// setup() 函式：開機或按下重設鍵時執行一次
void setup() 
{
  Serial.begin(9600);  // 啟動序列通訊，設定速率為 9600 bps（位元/秒），與序列監控器同步

  // 輸出初始訊息到序列監控器（Serial Monitor）
  Serial.println("Start System");

  // 設定腳位 7（ButtoonPin）為輸入模式，用來讀取按鈕的狀態
  pinMode(ButtoonPin, INPUT);
}

// loop() 函式：主執行迴圈，Arduino 會不斷重複執行這裡的程式碼
void loop() 
{
  // 從按鈕腳位讀取目前狀態，HIGH（1）表示按下，LOW（0）表示未按下
  keystatus = digitalRead(ButtoonPin);

  // 判斷按鈕是否被按下，並印出對應訊息
  if (keystatus == 1) // HIGH 狀態 → 按鈕被按下
  {
    Serial.println("Button is Pressed");  // 顯示「按鈕被按下」
  }
  else // LOW 狀態 → 按鈕未被按下
  {
    Serial.println("Button is not Pressed");  // 顯示「按鈕未被按下」
  }

  delay(300); // 暫停 300 毫秒，避免資訊更新太快（有助於閱讀序列輸出）
}
