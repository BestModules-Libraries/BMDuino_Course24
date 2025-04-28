/*
  Blink
程式說明：
setup() 函數：

將內建的 LED 腳位（LED_BUILTIN）設置為輸出模式，這樣我們可以控制其開關。
loop() 函數：

先將 LED 設置為高電平（HIGH），使其亮起，並保持亮起 1 秒（delay(1000)）。
然後將 LED 設置為低電平（LOW），使其熄滅，並保持熄滅 1 秒。
這段程式會讓內建的 LED 以 1 秒間隔持續閃爍，形成一閃一滅的效果。
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // 將內建 LED 腳位（LED_BUILTIN，通常是引腳 13）初始化為輸出模式
  pinMode(LED_BUILTIN, OUTPUT);
}

// loop() 函數會一直重複執行
void loop() {
  // 將內建 LED 設置為高電平 (HIGH)，點亮 LED
  digitalWrite(LED_BUILTIN, HIGH);  
  delay(1000);  // 延遲 1000 毫秒（1 秒），保持 LED 亮著

  // 將內建 LED 設置為低電平 (LOW)，熄滅 LED
  digitalWrite(LED_BUILTIN, LOW);   
  delay(1000);  // 延遲 1000 毫秒（1 秒），保持 LED 關閉
}
