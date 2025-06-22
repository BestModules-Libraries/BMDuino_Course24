#define ButtoonPin 7  // 定義按鈕接在數位腳位 7，之後使用變數 ButtoonPin 代替硬編碼的數字 7

int keystatus = 0;    // 用來儲存按鈕目前的狀態（0：未按下，1：按下）

// 宣告整數陣列 ledpin[]，存放 6 顆 LED 燈所接的腳位（D8 ~ D13）
int ledpin[] = {8, 9, 10, 11, 12, 13};  // 六顆 LED 燈的對應腳位

int num = 1;         // num 為目前要點亮的 LED 編號（從 1 到 6）
int i, j;            // 迴圈變數

// setup()：只在 Arduino 開機或重設時執行一次
void setup() 
{
  Serial.begin(9600);  // 啟用序列通訊，設定傳輸速率為 9600 bps

  // 在序列監控器中顯示啟動提示文字
  Serial.println("Start System");
  Serial.println("Please Input a Number n to Turn on Ledn");

  // 將所有 LED 的腳位設定為「輸出模式」並預設為關閉（LOW）
  for (i = 0; i <= 5; i++)  // 陣列 ledpin 有 6 個元素，索引為 0~5
  {
    pinMode(ledpin[i], OUTPUT);     // 設定第 i 顆 LED 所接的腳位為輸出
    digitalWrite(ledpin[i], LOW);   // 將該 LED 預設為關閉狀態
  }

  // 將按鈕腳位設為輸入模式，準備接收按鈕狀態
  pinMode(ButtoonPin, INPUT);
}

// loop()：主程式區塊，會持續不斷執行（每秒可能重複幾百次）
void loop() 
{
  // 讀取按鈕的目前狀態（HIGH = 被按下，LOW = 未按下）
  keystatus = digitalRead(ButtoonPin);

  // 如果按鈕被按下（HIGH）
  if (keystatus == 1)
  {
    num++;           // 將 num 增加 1，代表要亮下一顆燈
    if (num > 6)     // 如果超過第 6 顆，從頭再來（回到第 1 顆）
    {
      num = 1;
    }
  }

  // 用 for 迴圈控制 6 顆 LED 的狀態
  for (j = 1; j <= 6; j++)  // j 代表第幾顆 LED（1 到 6）
  {
    if (num == j)
    {
      // 只點亮目前 num 對應的那一顆燈
      digitalWrite(ledpin[j - 1], HIGH);
    }
    else
    {
      // 其餘的 LED 全部關閉
      digitalWrite(ledpin[j - 1], LOW);
    }
  }

  delay(300); // 暫停 300 毫秒，用來降低按鈕彈跳或避免重複觸發太快
}
