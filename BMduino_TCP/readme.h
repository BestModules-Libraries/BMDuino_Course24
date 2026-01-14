/*
程式執行邏輯說明：
階段	功能	詳細說明
初始化 (setup)	WiFi 模組啟動與自動連線	程式上電後會呼叫 Wifi.begin() 初始化，接著自動嘗試連線 WiFi (connectToAP) 及 TCP (connectTCP)
主迴圈 (loop)	雙向資料通訊	不斷檢查是否有 TCP 資料從伺服器回傳，並監控序列輸入（例如從 PC 輸入的文字）
資料轉送	UART ↔ TCP 橋接	若從 USB 序列埠輸入文字，會直接透過 BMC81M001 模組轉發到 TCP 伺服器
緩衝清理	清除暫存記憶體	透過 clearBuff() 將已使用的緩衝清空，避免資料混亂或重複傳送
⚙️ 硬體連接示意：
裝置	引腳說明	連接對象
BMduino UNO / SuperMini	TX2 (GPIO17)、RX2 (GPIO16)	連接至 BMC81M001 模組的 RX 與 TX
LED 指示燈	GPIO13	顯示傳送狀態 (亮表示傳送中，熄表示待機)

程式說明重點：

Header Guard (#ifndef / #define / #endif)
防止同一份標頭檔在多個檔案中被重複引入而造成重複定義錯誤。

WiFi 與 TCP 設定區

WIFI_SSID 與 WIFI_PASS 用來設定模組要連線的無線網路。

IP 與 IP_Port 用於指定 TCP 伺服器的主機與通訊埠，這裡的 "www.hinet.net" 可能只是測試用途，可改成實際 IoT 平台伺服器。

LED 腳位設定

設為 13 腳，在大多數 Arduino 板上會直接連到內建 LED，可用於顯示網路狀態或 Debug 訊號。

緩衝區變數

SerialBuff：接收 UART 資料。

data：暫存特定輸入值（例如 RFID、鍵盤值等）。

DATA_BUF / tcpBuff：主要用於儲存字串封包（可搭配 String 操作，如 +=、indexOf() 等）。

resLen / nKeyBuf：作為資料長度管理變數。


此程式完整流程（圖像化理解）
階段	動作
⏹ 開機 → setup()	初始化 Serial、WiFi 模組
📶 connectToAP()	嘗試連線至 WiFi 熱點
🌐 connectTCP()	建立 TCP Client 連線
🔁 loop()	等待：伺服器資料 or 使用者輸入
📤 writeDataTcp()	將序列輸入送往 TCP server
📥 readDataTcp()	顯示伺服器回覆

這是一種典型 IoT 設備 TCP client 傳感器/人機資料 → 雲端伺服器 的模式。4
*/