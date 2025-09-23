<?php
    // 引入資料庫連線設定檔 (iotcnn.php)，這個檔案內含有 Connection() 函式可建立與 MySQL 的連線
    include("../Connections/iotcnn.php");		

    // 呼叫自訂函式 Connection() 來建立與 MySQL 資料庫的連線，並回傳一個連線物件給 $link
    $link = Connection();		

    // 使用 getenv() 取得目前使用者端的連線 IP 位址
    $ip = getenv("REMOTE_ADDR");	
    echo "ip:".$ip."<br>";   // 輸出取得的 IP 位址到網頁上，方便偵錯或確認來源

    // 透過 GET 方式取得網址參數 (來自感測器或裝置上傳的資料)
    $temp0 = $_GET["MAC"];   // MAC Address (裝置唯一位址)
    $temp1 = $_GET["bd"];    // 血氧濃度值 (blood oxygen)
    $temp2 = $_GET["hb"];    // 心跳值 (heartbeat)

    // 呼叫自訂函式 getdatetime() 來取得系統目前的時間，格式為 YYYYMMDDHHMMSS
    $sysdt = getdatetime() ;

    // 範例傳入網址：
    // http://iot.arduino.org.tw:8888/bmduino/bloodoxygen/dataadd.php?MAC=AABBCCDDEEFF&bd=97.45&hb=83
    
    // 範例資料表插入語法：
    // INSERT INTO `bloodoxygen` (`id`, `crtdatetime`, `systime`, `MAC`, `IP`, `blood`, `heartbeat`) 
    // VALUES ('', current_timestamp(), '20250920155501', 'AABBCCDDEEFF', '192.168.99.100', '96.76', '93');

    // 定義 SQL 新增語法的格式字串，%s 表示字串，%6.2f 表示浮點數(小數2位)，%d 表示整數
    $qrycmd = "insert into bloodoxygen (MAC,IP,systime, blood, heartbeat) values ('%s', '%s','%s', %6.2f, %d);";

    // 使用 sprintf() 將上面 $qrycmd 的格式字串依序填入變數，組合出完整的 SQL 語法
    $qrystr = sprintf($qrycmd, $temp0, $ip, $sysdt, $temp1, $temp2); 

    // 顯示產生的 SQL 語法，方便偵錯
    echo $qrystr ;
    echo "<br>" ;

    // 執行 SQL 新增語法，若成功則顯示 "Successful"，失敗則顯示 "Fail"
    if (mysqli_query($link, $qrystr))
    {
        echo "Successful <br>" ;
    }
    else
    {
        echo "Fail <br>" ;
    }

    echo "<br>" ;
    // 關閉與 MySQL 資料庫的連線
    mysqli_close($link);		
?>
    
<?php
    /* 定義自訂 PHP 函式 */

// 將日期字串轉換成排序用格式 (YYYYMMDDHHMMSS)
function getdataorder($dt) {
	// 將字串依空白分割成日期(ymd) 與 時間(hms)
	$splitTimeStamp = explode(" ", $dt);
	$ymd = $splitTimeStamp[0] ;
	$hms = $splitTimeStamp[1] ;

	// 將日期拆解成年、月、日
	$vdate = explode('-', $ymd);
	// 將時間拆解成時、分、秒
	$vtime = explode(':', $hms);

	// 使用 str_pad() 補齊字串長度，例如 "9" → "09"
	$yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
	$mm   = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
	$dd   = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
	$hh   = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
	$min  = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
	$sec  = str_pad($vtime[2], 2, "0", STR_PAD_LEFT);

	// 回傳合併後的時間字串 (YYYYMMDDHHMMSS)
	return ($yyyy.$mm.$dd.$hh.$min.$sec) ;
}

// 與 getdataorder 類似，但回傳結果只到分鐘 (YYYYMMDDHHMM)
function getdataorder2($dt) {
	$splitTimeStamp = explode(" ", $dt);
	$ymd = $splitTimeStamp[0] ;
	$hms = $splitTimeStamp[1] ;

	$vdate = explode('-', $ymd);
	$vtime = explode(':', $hms);

	$yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
	$mm   = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
	$dd   = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
	$hh   = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
	$min  = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);

	return ($yyyy.$mm.$dd.$hh.$min) ;
}

// 取得目前系統時間，並回傳格式為 YYYYMMDDHHMMSS
function getdatetime() {
	$dt = getdate() ;
	$yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
	$mm   = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
	$dd   = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
	$hh   = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
	$min  = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);
	$sec  = str_pad($dt['seconds'], 2, "0", STR_PAD_LEFT);

	return ($yyyy.$mm.$dd.$hh.$min.$sec) ;
}

// 將 YYYYMMDDHHMMSS 格式轉換成標準可讀格式 (YYYY/MM/DD HH:MM:SS)
function trandatetime1($dt) {
	$yyyy = substr($dt, 0, 4);
	$mm   = substr($dt, 4, 2);
	$dd   = substr($dt, 6, 2);
	$hh   = substr($dt, 8, 2);
	$min  = substr($dt, 10, 2);
	$sec  = substr($dt, 12, 2);

	return ($yyyy."/".$mm."/".$dd." ".$hh.":".$min.":".$sec) ;
}
?>
