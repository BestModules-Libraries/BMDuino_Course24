/*
附註與建議（重要）

Serial 與 Serial4 的關係

BMC11(&Serial4)：表示 BMC11T001 與 Arduino 的 Serial4 通訊（通常為硬體序列埠）。

BMC11.begin(115200)：設定模組通訊使用 115200 bps（對 Serial4）。

Serial.begin(9600)：這是 Arduino 主序列埠（USB 到電腦）的速率，用來在 Serial Monitor 顯示資料。

若你想在序列監視器上看到人類可讀或正確的內容，建議把 Serial.begin(9600) 改成 Serial.begin(115200)，以避免 baud rate 不一致導致的亂碼。

UID 的顯示格式

範例程式採用 Serial.write(uid_buf, nlens) 直接輸出原始位元組，序列監視器可能會顯示不可見或亂碼字元。

如果你想以常見的十六進位格式 (e.g. 04 A2 1B 3C) 顯示，建議改成下面的顯示方式：

*/

/*
if (nlens > 0) {
  for (int i = 0; i < nlens; i++) {
    if (uid_buf[i] < 16) Serial.print("0"); // 補 0 以保持兩位十六進位
    Serial.print(uid_buf[i], HEX);
    if (i < nlens - 1) Serial.print(" ");
  }
  Serial.println();
}
*/
/*
錯誤處理

目前程式沒有檢查 nlens 的回傳是否為 0（表示沒讀到卡），
你可以加入判斷來避免無意義輸出：
nlens = BMC11.getUID_ISO14443A(uid_buf);
if (nlens > 0) {
  // 處理顯示 UID
} else {
  // 沒讀到卡，可以顯示提示或做其他事
}
硬體接線

請確認 BMduino UNO 的
 Serial4 腳位已正確接到 BMC11T001（library 
 與開發板定義會說明使用哪些 pins）。

若開發板或 core 不支援 Serial4，
請改用適合的 Serial
（例如 Serial1、Serial2）
或使用 SoftwareSerial（速度需求高時不建議）。

*/
