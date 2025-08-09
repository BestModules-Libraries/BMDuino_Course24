/*
程式原理與運作解釋
1. 程式概述
這段程式使用 BMduino-UNO 開發板（BM53A367A）搭配 BMH08002-4 血氧感測器模組，通過序列通訊（Serial）實現血氧飽和度（SpO2）、心率和灌注指數（PI）的測量。程式支援兩種工作模式：查詢模式（Query Response Mode）和連續模式（Continuous Timing Mode），並通過序列埠監視器輸出結果。

2. 硬體與通訊設定
序列埠配置：
硬體序列埠（Serial）設為 9600 波特率，用於與電腦的序列監視器通訊，顯示測量數據。
軟體序列埠（SoftwareSerial）或硬體序列埠（Serial1、Serial2 等）設為 38400 波特率，用於與血氧感測器通訊。程式中預設使用 Serial1（腳位 22 作為啟用引腳）。
感測器初始化：
mySpo2.begin() 初始化感測器。
mySpo2.setModeConfig(0x02) 設定為查詢模式，當檢測到手指時紅燈亮起。
mySpo2.setTimeInterval(300) 設定測量間隔為 300 毫秒。
3. 工作模式
查詢模式 (Mode = 1)：
通過 mySpo2.requestInfoPackage(rBuf) 主動請求感測器返回數據。
狀態碼 Status 表示測量狀態：
0x02：測量完成，輸出 SpO2、Heart Rate 和 PI，並進入休眠模式。
0x01：測量中，提示用戶不要移動手指（使用 flag 避免重複提示）。
0x00：未檢測到手指，提示用戶重新放置手指。
數據解釋：
rBuf[0]：血氧飽和度 (SpO2)，單位為 %。
rBuf[1]：心率，單位為 BPM (Beats Per Minute)。
rBuf[2]：灌注指數 (PI)，需除以 10 轉為小數，單位為 %。
連續模式 (Mode = 0)：
通過 mySpo2.isInfoAvailable() 檢查是否有數據可用，然後用 mySpo2.readInfoPackage(rBuf) 讀取數據。
將 rBuf 陣列中的所有 15 個位元組逐一輸出，適合調試或查看原始數據。
4. 運作流程
初始化階段 (setup)：
啟動序列埠和感測器，設定為查詢模式，提示用戶放置手指並等待 2 秒。
根據模式配置決定執行 Mode_ask 或 Mode_continuous_timing。
主循環 (loop)：
根據 Mode 變數選擇執行相應的模式函數。
測量與輸出：
查詢模式下，主動請求數據並根據狀態碼處理結果。
連續模式下，持續監聽並輸出原始數據。
5. 注意事項
手指放置：感測器需要正確檢測到手指才能開始測量，否則會返回 0x00 狀態。
數據精度：PI 值的計算需要除以 10，確保單位正確。
電源管理：測量完成後，程式會呼叫 mySpo2.endMeasure() 和 mySpo2.sleep() 節省能源。



*/