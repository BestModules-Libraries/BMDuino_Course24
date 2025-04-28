/*
程式說明：
變數定義：

brightness 用於存儲 LED 的當前亮度，範圍從 0（完全關閉）到 255（最亮）。
fadeAmount 是每次亮度增減的量，用於控制亮度變化的速度。
setup() 函數：

設置內建 LED 腳位（通常是引腳 13，但在不同板上可能有所不同）為輸出模式。
loop() 函數：

不斷使用 analogWrite() 函數設置 LED 的亮度。
每次循環時，將亮度增加或減少 fadeAmount 值，讓 LED 漸漸變亮或變暗。
當亮度達到 0 或 255 時，反轉 fadeAmount，讓 LED 的亮度變化方向相反，從而實現來回漸變效果。
延遲 30 毫秒，讓 LED 漸變效果更平滑。
這段程式用於控制 LED 漸漸變亮和變暗，持續不斷地循環，形成漸變效果。
*/

int brightness = 0;  // 定義變數 brightness，表示 LED 的亮度，初始值為 0
int fadeAmount = 5;  // 定義變數 fadeAmount，每次調整 LED 亮度的增量，設為 5

// setup() 函數在按下重置鍵或電源啟動時執行一次
void setup() {
  // 將內建 LED (LED_BUILTIN) 的腳位設為輸出模式
  pinMode(LED_BUILTIN, OUTPUT);
}

// loop() 函數會一直重複執行
void loop() {
  // 設置內建 LED (LED_BUILTIN) 的亮度，範圍從 0 到 255
  analogWrite(LED_BUILTIN, brightness);

  // 調整亮度，為下一次循環做準備
  brightness = brightness + fadeAmount;

  // 如果亮度超過 255（最大值）或低於 0（最小值），則反轉增減方向
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;  // 將 fadeAmount 的值變為相反，實現亮度漸變反向
  }

  // 延遲 30 毫秒，使 LED 的亮度變化可見
  delay(30);
}

