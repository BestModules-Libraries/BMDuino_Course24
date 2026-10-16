/*
程式運作流程

按鈕觸發

按下 LED 按鈕 → 觸發中斷 → 設定 int_flag = 1

主迴圈 loop()

偵測到 int_flag == 1

呼叫 getAllButtonStatus() → 取得按鈕狀態字串（例："0101"）

呼叫 showButtononOled() → 在 OLED 顯示該字串

呼叫 genJSON() → 將字串轉換成 JSON 資料 → 準備送到 MQTT

資料轉換

genJSON("0101") → 分解成：

按鈕 1 → 0 → OFF

按鈕 2 → 1 → ON

按鈕 3 → 0 → OFF

按鈕 4 → 1 → ON
{
  "Device":"AABBCC123456",
  "RelayNumber":1,
  "Command":"ON"
  }

{
  "Device":"AABBCC123456",
  "RelayNumber":2,
  "Command":"ON"
  }
  
*/