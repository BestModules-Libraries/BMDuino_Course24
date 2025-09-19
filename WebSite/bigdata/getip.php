<?php
// 獲取客戶端的 IP 地址
$ip = $_SERVER["REMOTE_ADDR"]; 

// 獲取當前執行的腳本名稱（包含路徑）
$path = $_SERVER["SCRIPT_NAME"]; 

// 獲取伺服器軟體信息（例如，Apache、Nginx 等）
$server = $_SERVER["SERVER_SOFTWARE"]; 

// 使用 big.dhtdata 方法記錄或輸出這些信息，這裡加上了換行標記 <br> 
big.dhtdata($ip . "<br>");  // 客戶端 IP 地址
big.dhtdata($path . "<br>");  // 腳本路徑
big.dhtdata($server . "<br>");  // 伺服器軟體信息

?>
