/*
程式原理說明與執行流程：
這是一個最基本的 Arduino LED 閃爍程式（Blink 範例），
用來讓內建 LED 持續以 1 秒為週期亮滅。

 主要元件與設定
LED_BUILTIN：
是一個 Arduino 預定義常數，代表板子上的內建 LED 腳位（大多數板子是 Pin 13）。
可直接用來控制那個小小的 SMD LED（通常是綠色或黃色）。
pinMode(pin, mode)：
設定腳位的工作模式。
pinMode(LED_BUILTIN, OUTPUT) 表示將該腳位設定為輸出（可以送出電壓）。

執行流程（在 loop() 裡反覆執行）：
digitalWrite(LED_BUILTIN, HIGH);
→ 將腳位輸出設為「高電位」，LED 接收到電壓而點亮。

delay(1000);
→ 暫停 1000 毫秒（1 秒），讓 LED 維持點亮狀態。

digitalWrite(LED_BUILTIN, LOW);
→ 將腳位輸出設為「低電位」，LED 熄滅。

delay(1000);
→ 暫停 1 秒，讓 LED 維持熄滅狀態。

然後從頭開始再次執行這 4 行，LED 就會每秒閃爍一次。

動作循環圖（時間軸）
時間 (秒)	LED 狀態
    0	ON（亮）
    1	OFF（暗）
    2	ON
    3	OFF
...	重複循環

總結
這段程式碼展示了 Arduino 如何使用 GPIO（通用輸出入）腳位控制 LED。藉由將腳位設定為高或低電位，達到開關 LED 的效果。這個程式常作為 Arduino 的入門範例，非常適合初學者理解以下幾個重點：
腳位模式設定（pinMode）
數位輸出控制（digitalWrite）
延遲控制（delay）
loop() 無限執行特性
*/