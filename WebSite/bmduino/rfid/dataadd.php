<?php

    //http://iot.arduino.org.tw:8888/bigdata/rfid/dataadd.php?KEY=1234567890&ACT=1
    //http://localhost:8888/bigdata/rfid/dataadd.php?KEY=1234567890&ACT=1
    /*
        這支 PHP 程式檔主要用來處理來自網址參數（GET）所傳來的 RFID 卡片資訊，
       並根據卡號 (KEY) 與啟用狀態 (ACT) 查詢或更新資料庫中的 RFID 卡片記錄，
       最後將使用者導回卡片清單頁面 cardlistmgt.php。
        使用者透過 GET 請求送來參數 KEY（RFID卡號）與 ACT（啟用狀態）。
        程式驗證參數是否正確、存在。
        查詢資料庫是否已存在此卡號與對應啟用狀態的資料。
        若已存在 → 直接導回卡片列表頁面。
        若不存在 → 根據 ACT 值進行卡號啟用或停用狀態更新，並記錄使用者 IP 與系統時間。
      */

    // 引用資料庫連線設定
    include("../Connections/iotcnn.php"); // 載入連線資料庫的函數

    $link = Connection(); // 呼叫 Connection() 函數取得資料庫連線物件

    // 取得用戶端 IP 位址（可記錄用戶來源）
    $ip = getenv("REMOTE_ADDR");	//取得用戶端連線IP Address
    echo "ip:".$ip."<br>";
    $temp0 = $_GET["KEY"];       // 從網址參數取得 RFID 卡號
    $temp1 = (int)$_GET["ACT"];  // 從網址參數取得啟用狀態（轉為整數）
    $sysdt = getdatetime(); // 取得目前系統時間（格式：yyyymmddhhmmss）
 
    if (!isset($_GET["KEY"])) {
        echo "RFID KEY lost <br>" ;
        die(); // 停止執行程式
    }

    echo "GET KEY DATA passed <br>" ;

    if (!isset($_GET["ACT"])) {
        echo "Activate lost <br>" ;
        die();
    }

    echo "GET Active DATA passed <br>" ;

    if (trim($temp0) == "") {
        echo "RFID KEY is empty string <br>" ;
        die();
    }
 

    $qry = "select * from rfidlist where rfidkey = '%s' and activate = %d" ;
    $qrystr = sprintf($qry, $temp0, $temp1); // 套入參數
    echo $qrystr."<br>";

    $result = mysqli_query($link, $qrystr);
    $num = mysqli_num_rows($result); // 查詢回傳筆數
    if ($num >= 1 ) 
    {
        mysqli_close($link);  
        die() ; //Stop Here
    }
     
    if ($temp1 == 0) 
    {
        // 若設定為停用，更新 activate 為 0，條件是原本是啟用中
        $qry = "update rfidlist set systime = '%s', IP = '%s' ,activate = 0 where rfidkey = '%s' and activate = 1" ;
        $qrystr = sprintf($qry, $sysdt, $ip , $temp0) ;
        echo $qrystr."<br>";
        $result = mysqli_query($link, $qrystr);
    }

    if ($temp1 == 1) 
    {
        // 若設定為啟用，更新 activate 為 1，條件是原本是停用中
        $qry = "update rfidlist set systime = '%s', IP = '%s' ,activate = 1 where rfidkey = '%s' and activate = 0" ;
        $qrystr = sprintf($qry, $sysdt, $ip , $temp0) ;
        echo $qrystr."<br>";
        $result = mysqli_query($link, $qrystr);
    }


    // 重新導向使用者到卡片列表頁面
    mysqli_close($link);    
   // 重新導向使用者到卡片列表頁面
   die() ; //Stop Here     

?>
<?php
         /* Defining a PHP Function */
// 取得完整日期時間字串 yyyymmddhhmmss
function getdatetime() {
    $dt = getdate();
    return str_pad($dt['year'],4,"0",STR_PAD_LEFT).
           str_pad($dt['mon'],2,"0",STR_PAD_LEFT).
           str_pad($dt['mday'],2,"0",STR_PAD_LEFT).
           str_pad($dt['hours'],2,"0",STR_PAD_LEFT).
           str_pad($dt['minutes'],2,"0",STR_PAD_LEFT).
           str_pad($dt['seconds'],2,"0",STR_PAD_LEFT);
}

// 將字串格式日期轉為 "yyyy/mm/dd hh:mm:ss"
function trandatetime1($dt) {
    $yyyy = substr($dt,0,4);
    $mm   = substr($dt,4,2);
    $dd   = substr($dt,6,2);
    $hh   = substr($dt,8,2);
    $min  = substr($dt,10,2);
    $sec  = substr($dt,12,2);
    return "$yyyy/$mm/$dd $hh:$min:$sec";
}

// 把日期時間格式字串整理成 yyyymmddhhmmss
function getdataorder($dt) {
    $splitTimeStamp = explode(" ", $dt);
    $vdate = explode('-', $splitTimeStamp[0]);
    $vtime = explode(':', $splitTimeStamp[1]);
    return str_pad($vdate[0],4,"0",STR_PAD_LEFT).
           str_pad($vdate[1],2,"0",STR_PAD_LEFT).
           str_pad($vdate[2],2,"0",STR_PAD_LEFT).
           str_pad($vtime[0],2,"0",STR_PAD_LEFT).
           str_pad($vtime[1],2,"0",STR_PAD_LEFT).
           str_pad($vtime[2],2,"0",STR_PAD_LEFT);
}

// 另一種省略秒數的版本 yyyymmddhhmm
function getdataorder2($dt) {
    $splitTimeStamp = explode(" ", $dt);
    $vdate = explode('-', $splitTimeStamp[0]);
    $vtime = explode(':', $splitTimeStamp[1]);
    return str_pad($vdate[0],4,"0",STR_PAD_LEFT).
           str_pad($vdate[1],2,"0",STR_PAD_LEFT).
           str_pad($vdate[2],2,"0",STR_PAD_LEFT).
           str_pad($vtime[0],2,"0",STR_PAD_LEFT).
           str_pad($vtime[1],2,"0",STR_PAD_LEFT);
}

?>
