<?php
    // 範例網址（用於測試啟用 RFID 卡片的功能）：
<<<<<<< HEAD
    // http://iot.arduino.org.tw:8888/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC
    // http://localhost:8888/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC

    include("../Connections/iotcnn.php"); // 引入資料庫連線檔案，通常內含 Connection() 函式

    $link = Connection(); // 建立資料庫連線

    // 定義一個資料類別，存放回傳的結果
    class maindata {
        public $Device;    // 儲存裝置的 MAC 位址
        public $Card;      // 儲存 RFID 卡片的卡號
        public $Result;    // 回傳的結果，例如：已啟用、成功啟用、已存在等
    }

    $maindata = new maindata(); // 建立 maindata 類別的實例

    // 取得用戶端的 IP 位址（供記錄用）
    $ip = getenv("REMOTE_ADDR");

    // 取得目前系統時間，格式為 yyyymmddhhmmss（例如 20250527153045）
    $sysdt = getdatetime();

    // 檢查是否有傳入 MAC（裝置位址）
    if (!isset($_GET["MAC"])) {
        echo "Device MAC lost <br>";
        die(); // 若未傳入 MAC，終止程式
    }

    // 檢查是否有傳入 KEY（RFID 卡號）
    if (!isset($_GET["KEY"])) {
        echo "RFID KEY lost <br>";
        die(); // 若未傳入 KEY，終止程式
    }

    // 取出網址中的參數值
    $temp0 = $_GET["MAC"]; // MAC 位址
    $temp1 = $_GET["KEY"]; // RFID 卡號


    // 檢查 MAC 是否為空字串
    if (trim($temp0) == "") {
        echo "MAC is empty string <br>";
        die();
    }

    // 檢查 RFID 卡號是否為空字串
    if (trim($temp1) == "") {
        echo "Card number is empty string <br>";
        die();
    }

    // 準備查詢語句，找出已存在的卡片（activate 為 0 或 1）
    $qry = "select * from rfidlist where rfidkey = '%s' and activate in (0,1)";
    $qrystr = sprintf($qry, $temp1); // 將卡號代入 SQL 語句中

    $result = mysqli_query($link, $qrystr); // 執行查詢
    $num = mysqli_num_rows($result); // 取得查詢結果筆數

    // 若查無資料，表示卡號不存在，新增資料並啟用該卡
    if ($num == 0) 
    {
        $qry = "insert into rfidlist (systime,IP,rfidkey,activate,startdatetime,enddatetime) values ('%s' ,'%s', '%s',1,'%s','%s') ;";
        $qrystr = sprintf($qry, $sysdt, $ip, $temp1, $sysdt, '20991231235959'); // 設定起始與結束時間
        $result = mysqli_query($link, $qrystr); // 執行新增語句
        $passok = "Data Added and Ativated"; // 記錄新增成功
    }

    // 若卡號已存在，檢查是否為啟用或未啟用狀態
    if ($num >= 1) 
    {
        $row = mysqli_fetch_array($result); // 取得資料列

        if ((int)$row["activate"] == 0) 
        {
            // 若尚未啟用，則更新為啟用狀態
            $qry = "update rfidlist set activate = 1, systime = '%s' , IP = '%s' where rfidkey = '%s' and activate = 0";
            $qrystr = sprintf($qry, $sysdt, $ip, $temp1);
            $result = mysqli_query($link, $qrystr);
            $passok = "Ativated";
        }

        if ((int)$row["activate"] == 1) {
            // 若已啟用，僅回報狀態
            $passok = "had Ativated";
        }
    }

    // 設定回傳資料
    $maindata->Device = $temp0;
    $maindata->Card = $temp1;
    $maindata->Result = $passok;

    // 釋放資源
    if ($result && $result instanceof mysqli_result) {
        mysqli_free_result($result);
    }

    // 關閉資料庫連線
    mysqli_close($link);

    // 回傳 JSON 格式資料，供前端或 API 呼叫使用
    echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
 /*
 {
 "Device":"B8D61A68DAFC",
 "Card":"0219390674",
 "Result":"Data Added and Ativated"
 }
 {
 "Device":"B8D61A68DAFC",
 "Card":"0219390674",
 "Result":"Ativated"
 }
  {
 "Device":"B8D61A68DAFC",
 "Card":"0219390674",
 "Result":"had Ativated"
 }  */   
    
?>


<?php
    // 將日期時間轉換為指定格式（yyyymmddhhmmss）
    function getdataorder($dt) {
        $splitTimeStamp = explode(" ", $dt);
        $vdate = explode('-', $splitTimeStamp[0]);
        $vtime = explode(':', $splitTimeStamp[1]);
        $yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
        $min = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
        $sec = str_pad($vtime[2], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min.$sec);
    }

    // 同上，但不含秒數
    function getdataorder2($dt) {
        $splitTimeStamp = explode(" ", $dt);
        $vdate = explode('-', $splitTimeStamp[0]);
        $vtime = explode(':', $splitTimeStamp[1]);
        $yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
        $min = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min);
    }

    // 取得目前系統時間（yyyymmddhhmmss）
    function getdatetime() {
        $dt = getdate();
        $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
        $min = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);
        $sec = str_pad($dt['seconds'], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min.$sec);
    }

    // 將 yyyymmddhhmmss 格式轉回為可讀取的時間格式 yyyy/mm/dd hh:mm:ss
    function trandatetime1($dt) {
        $yyyy = substr($dt, 0, 4);
        $mm = substr($dt, 4, 2);
        $dd = substr($dt, 6, 2);
        $hh = substr($dt, 8, 2);
        $min = substr($dt, 10, 2);
        $sec = substr($dt, 12, 2);
        return ($yyyy . "/" . $mm . "/" . $dd . " " . $hh . ":" . $min . ":" . $sec);
    }
?>
=======
    // http://localhost:8888/bigdata/rfid/act.php?KEY=1234567890
    // http://iot.arduino.org.tw:8888/bigdata/rfid/act.php?KEY=1234567890

    include("../Connections/iotcnn.php"); // 引入資料庫連線程式
    // 通常此檔案中包含 Connection() 函式以建立與資料庫的連線

    $link = Connection(); // 呼叫 Connection() 函式，取得資料庫連線物件

    // 取得用戶端的 IP 位址，記錄使用者來源
    $ip = getenv("REMOTE_ADDR");
    echo "ip:" . $ip . "<br>";

    // 呼叫自訂函式 getdatetime()，取得目前系統的日期時間，格式為 yyyymmddhhmmss
    $sysdt = getdatetime();

    // 驗證網址是否有傳入 GET 參數 "KEY"（代表 RFID 卡號）
    if (!isset($_GET["KEY"])) {
        echo "RFID KEY lost <br>"; // 若沒有傳入 KEY 參數，顯示錯誤訊息
        die(); // 終止程式執行
    }
    echo "GET KEY DATA passed <br>"; // 表示 KEY 參數已成功接收

    // 再次取得用戶端 IP（這裡其實是重複，不必要的動作）
    $ip = getenv("REMOTE_ADDR");
    echo "ip:" . $ip . "<br>";

    // 將從網址取得的 KEY 參數存入變數 $temp0，代表 RFID 卡號
    $temp0 = $_GET["KEY"];

    // 再次取得目前系統時間
    $sysdt = getdatetime();

    // 驗證 RFID 卡號是否為空字串（雖然 GET 有傳入，但值可能是空的）
    if (trim($temp0) == "") {
        echo "RFID KEY is empty string <br>"; // 顯示錯誤
        die(); // 終止程式
    }

    // 準備查詢語句，從資料表 rfidlist 中找出尚未啟用 (activate = 0) 且符合 RFID 卡號的紀錄
    $qry = "select * from rfidlist where rfidkey = '%s' and activate = 0";
    $qrystr = sprintf($qry, $temp0); // 將 RFID 卡號代入查詢語句
    echo $qrystr . "<br>"; // 顯示查詢語句（除錯用）

    // 執行查詢語句
    $result = mysqli_query($link, $qrystr);
    $num = mysqli_num_rows($result); // 取得符合條件的紀錄數量

    if ($num >= 1) 
    {
        // 若有找到尚未啟用的 RFID 卡號，進行啟用（將 activate 欄位更新為 1）
        $qry = "update rfidlist set activate = 1, systime = '%s' , IP = '%s' where rfidkey = '%s' and activate = 0";
        $qrystr = sprintf($qry, $sysdt,$ip,$temp0); // 將 RFID 卡號代入更新語句
        echo $qrystr . "<br>"; // 顯示更新語句（除錯用）

        $result = mysqli_query($link, $qrystr); // 執行更新語句
        mysqli_close($link); // 關閉資料庫連線
        echo "Activate Successful <br>"; // 顯示啟用成功訊息
    } else {
        // 若找不到符合條件的 RFID 卡號，顯示錯誤訊息
        mysqli_close($link); // 關閉資料庫連線
        echo "Key is Wrong and Activate fail <br>"; // 顯示啟用失敗訊息
    }
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
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
