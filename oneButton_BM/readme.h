/*

整體程式原理與執行邏輯說明
程式原理說明：
目的：
這個程式的功能是：
偵測使用者是否按下接在腳位 D8 的按鈕，
並透過序列監控器（Serial Monitor）即時顯示狀態。

腳位設定：
  pinMode(ButtoonPin, INPUT);：
  將腳位 8 設為「輸入模式」，
  這表示 Arduino 會從這個腳位讀取電壓狀態（HIGH 或 LOW）。

讀取按鈕狀態：
  digitalRead(ButtoonPin)：這行會回傳 0 或 1（分別代表 LOW 或 HIGH）來表示按鈕是否被按下。

序列輸出：

    按鈕被按下:Serial.println("Button is Pressed");  // 顯示「按鈕被按下」
    按鈕未被按下:Serial.println("Button is not Pressed");  // 顯示「按鈕未被按下」

延遲機制：
  delay(300)：每次偵測後等 300 毫秒，
  避免訊息在序列監控器上快速刷屏，
  也防止按鈕雜訊造成誤判（簡單的 debounce 效果）。
  
*/