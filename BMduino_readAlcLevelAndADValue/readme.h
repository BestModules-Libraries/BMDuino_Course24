/*
註解重點：

初始化部分 說明了模組的不同連線方式（軟體序列 vs 硬體序列）。

封包解析 說明了 moduleInfo[] 陣列中各 byte 的用途
（第 5-6 byte = A/D 值，
第 7 byte = 酒精濃度等級，
第 10 byte = 預熱倒數）。

預熱機制 解釋了 AUTO_MODE 與 CMD_MODE 的差異。
*/