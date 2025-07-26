<?php
    // 範例網址（用於增加門禁機器清單的功能）：
    // http://localhost:8888/bigdata/rfid/reglockdevice.php?MAC=B8D61A68DAFC
    // http://iot.arduino.org.tw:8888/bigdata/rfid/reglockdevice.php?MAC=B8D61A68DAFC

    include("../Connections/iotcnn.php"); // 引入資料庫連線程式
    // 通常此檔案中包含 Connection() 函式以建立與資料庫的連線

    $link = Connection(); // 呼叫 Connection() 函式，取得資料庫連線物件
    class maindata
    {
            public $Device ;        //MAC裝置卡號
            public $Systime ;        //RFID裝置卡號
            public $Result ;      //是否准許進入，"Find" is pass, "notFind" is not valid
     }
     $maindata = new maindata() ;   //產生class maindata 的實體

    // 取得用戶端的 IP 位址，記錄使用者來源
    $ip = getenv("REMOTE_ADDR");
    //echo "ip:" . $ip . "<br>";

    // 呼叫自訂函式 getdatetime()，取得目前系統的日期時間，格式為 yyyymmddhhmmss
    $sysdt = getdatetime();

    // 再次取得用戶端 IP（這裡其實是重複，不必要的動作）
    // 驗證網址是否有傳入 GET 參數 "MAC"（代表 RFID 卡號）
    if (!isset($_GET["MAC"])) {
        echo "Device MAC lost <br>"; // 若沒有傳入 KEY 參數，顯示錯誤訊息
        die(); // 終止程式執行
    }
  //  echo "GET MAC DATA passed <br>"; // 表示 KEY 參數已成功接收

    // 將從網址取得的 KEY 參數存入變數 $temp0，代表 RFID 卡號
    $temp0 = $_GET["MAC"];

    // 再次取得目前系統時間
    $sysdt = getdatetime();

    // 驗證 RFID 卡號是否為空字串（雖然 GET 有傳入，但值可能是空的）
    if (trim($temp0) == "") {
        echo "MAC is empty string <br>"; // 顯示錯誤
        die(); // 終止程式
    }

    // 準備查詢語句，從資料表 rfidlist 中找出尚未啟用 (activate = 0) 且符合 RFID 卡號的紀錄
    $qry = "select * from lockdevice where deviceid = '%s' and activate = 1";
    $qrystr = sprintf($qry, $temp0); // 將 RFID 卡號代入查詢語句
    //echo $qrystr . "<br>"; // 顯示查詢語句（除錯用）

    // 執行查詢語句
    $result = mysqli_query($link, $qrystr);
    $num = mysqli_num_rows($result); // 取得符合條件的紀錄數量

    if ($num >= 1) 
    {
        // 若有找到尚未啟用的 RFID 卡號，進行啟用（將 activate 欄位更新為 1）
        //echo "Lock Device is existed <br>"; // 顯示啟用成功訊息
        $passok = "Existed" ;
        
    } 
    else 
    {
        //INSERT INTO `lockdevice` (`id`, `crtdatetime`, `systime`, `IP`, `deviceid`, `devicename`, `activate`, `location`, `building`) VALUES (NULL, CURRENT_TIMESTAMP, '20250514010101', '140.12.45.67', 'AABBCCDDEEFF', '測試機', '1', '老師家門口', '老師家');
<<<<<<< HEAD
        $qry = "insert into lockdevice (systime, IP, deviceid, devicename, activate, location, building) values ('%s', '%s', '%s', '卡號%s', '1', '請補述', '請補述');" ;
        $qrystr = sprintf($qry,$sysdt,$ip,$temp0,$temp0) ;
=======
        $qry = "insert into lockdevice (systime, IP, deviceid, devicename, activate, location, building) values ('%s', '%s', '%s', '未給名字', '1', '老師家門口', '老師家');" ;
        $qrystr = sprintf($qry,$sysdt,$ip,$temp0) ;
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
        $result = mysqli_query($link, $qrystr);        
        //echo "Lock Device is added <br>"; // 顯示啟用失敗訊息
        $passok = "Added" ;
    }
	$maindata->Device = $temp0 ;
	$maindata->Systime = $sysdt ;
  	 $maindata->Result = $passok ;
         
    if ($result && $result instanceof mysqli_result) {
        mysqli_free_result($result);	// 關閉資料集
    }

     mysqli_close($link);		// 關閉連線
    echo json_encode($maindata, JSON_UNESCAPED_UNICODE);	
    
    
    
?>

    <?php
         /* Defining a PHP Function */
         function getdataorder($dt) {
                //   $dt = getdate() ;
                                $splitTimeStamp = explode(" ",$dt);
                                $ymd = $splitTimeStamp[0] ;
                                $hms = $splitTimeStamp[1] ;
                                $vdate = explode('-', $ymd);
                                $vtime = explode(':', $hms);
                                $yyyy =  str_pad($vdate[0],4,"0",STR_PAD_LEFT);
                                $mm  =  str_pad($vdate[1] ,2,"0",STR_PAD_LEFT);
                                $dd  =  str_pad($vdate[2] ,2,"0",STR_PAD_LEFT);
                                $hh  =  str_pad($vtime[0] ,2,"0",STR_PAD_LEFT);
                                $min  =  str_pad($vtime[1] ,2,"0",STR_PAD_LEFT);
                                $sec  =  str_pad($vtime[2] ,2,"0",STR_PAD_LEFT);
                        /*
                                echo "***(" ;
                                echo $dt ;
                                echo "/" ;
                                echo $yyyy ;
                                echo "/" ;
                                echo $mm ;
                                echo "/" ;
                                echo $dd ;
                                echo "/" ;
                                echo $hh ;
                                echo "/" ;
                                echo $min ;
                                echo "/" ;
                                echo $sec ;
                                echo ")<br>" ;
                        */
                        return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
         function getdataorder2($dt) {
                //   $dt = getdate() ;
                                $splitTimeStamp = explode(" ",$dt);
                                $ymd = $splitTimeStamp[0] ;
                                $hms = $splitTimeStamp[1] ;
                                $vdate = explode('-', $ymd);
                                $vtime = explode(':', $hms);
                                $yyyy =  str_pad($vdate[0],4,"0",STR_PAD_LEFT);
                                $mm  =  str_pad($vdate[1] ,2,"0",STR_PAD_LEFT);
                                $dd  =  str_pad($vdate[2] ,2,"0",STR_PAD_LEFT);
                                $hh  =  str_pad($vtime[0] ,2,"0",STR_PAD_LEFT);
                                $min  =  str_pad($vtime[1] ,2,"0",STR_PAD_LEFT);

                        return ($yyyy.$mm.$dd.$hh.$min)  ;
         }
         function getdatetime() {
           $dt = getdate() ;
                                $yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
                                $mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
                                $dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
                                $hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
                                $min  =  str_pad($dt['minutes'] ,2,"0",STR_PAD_LEFT);
                                $sec  =  str_pad($dt['seconds'] ,2,"0",STR_PAD_LEFT);

                        return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
                          function trandatetime1($dt) {
                                $yyyy =  substr($dt,0,4);
                                $mm  =   substr($dt,4,2);
                                $dd  =   substr($dt,6,2);
                                $hh  =   substr($dt,8,2);
                                $min  =   substr($dt,10,2);
                                $sec  =   substr($dt,12,2);

                        return ($yyyy."/".$mm."/".$dd." ".$hh.":".$min.":".$sec)  ;
                 }
      ?>
