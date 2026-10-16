/*

整體程式原理與執行邏輯說明
程式功能概述
  使用者每按一下按鈕（接在 D7 腳位），就會讓一顆 LED 順序亮起（從第 1 顆到第 6 顆循環）。

  此程式只有一顆 LED 同時亮起，其他全都熄滅。
 硬體設定（前提）
接了 6 顆 LED 到 Arduino 數位腳位 D8 ~ D13。

各區塊原理詳解
1. pinMode(腳位,模式) 初始化
pinMode(ledpin[i], OUTPUT)：設定 LED 所接腳位為輸出模式。
  ledpin[i] ==> 8 ~ 13
pinMode(ButtoonPin, INPUT)：設定按鈕腳位為輸入，讀取按鈕狀態。
  ButtoonPin = 7
2. digitalRead(ButtoonPin) 偵測按鈕狀態
      digitalRead(ButtoonPin)：會讀到 HIGH（1） 代表按下、LOW（0） 代表沒按。
      用全域變數儲存按鈕狀態:keystatus = digitalRead(ButtoonPin);

    按鈕狀態:keystatus==>按下時就執行 num++，把亮燈編號加一。

3. if (num > 6) 判斷
    為了讓 LED 編號從 1~6 循環，超過 6 就重設為 1。

4. for 迴圈控制燈號
    6 顆燈透過一個 for 迴圈逐一控制。
    只有 num == j 的燈會被設為 HIGH（亮燈），其餘設為 LOW（關燈）。

5. delay(300) 延遲
    簡單達成「防彈跳效果」與 避免短時間多次偵測按鈕觸發。
    同時讓 LED 的亮滅切換更容易觀察。


*/