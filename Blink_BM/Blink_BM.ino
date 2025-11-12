// setup() 函式只在 Arduino 上電或按下重設（Reset）按鈕時執行一次
void setup() 
{
  // 設定 LED_BUILTIN 腳位為「輸出」模式
  // LED_BUILTIN 是 Arduino 內建 LED 通常接在腳位 13（視板子而定）
  pinMode(LED_BUILTIN, OUTPUT); //設定LED_BUILTIN(開發板預設燈號)為輸出狀態
  digitalWrite(LED_BUILTIN, LOW); //將 LED_BUILTIN 腳位設為 LOW（低電位）
}

// loop() 函式會無限重複執行，是 Arduino 的主要執行迴圈
void loop() 
{
  // 將 LED_BUILTIN 腳位設為 HIGH（高電位），點亮 LED 燈
  digitalWrite(LED_BUILTIN, HIGH);

  // 延遲 1000 毫秒（1 秒），讓 LED 保持亮著
  delay(1000);

  // 將 LED_BUILTIN 腳位設為 LOW（低電位），熄滅 LED 燈
  digitalWrite(LED_BUILTIN, LOW);

  // 再次延遲 1000 毫秒（1 秒），讓 LED 保持關閉
  delay(1000);
}
