<?php 
    // 引入共用的資料庫函式庫（comlib.php 應該包含一些通用函式，例如時間格式轉換）
    include("../comlib.php");

    // 引入資料庫連線設定（iotcnn.php 應該是資料庫連線細節設定）
    include("../Connections/iotcnn.php");

    // 呼叫自定的 Connection() 函式，建立與 MySQL 資料庫的連線
    $link = Connection();

    // 取得目前用戶端的 IP 位址，透過環境變數 REMOTE_ADDR
    //$ip = getenv("REMOTE_ADDR");
    $ip = $_POST["ip"]; //取得目前用戶端的 IP 位址from rfidformadd.php
    // 呼叫自定義的函式 getdataorder()，取得系統時間（格式可能為 yyyymmddhhmmss）
    $sysdt = getdataorder();

    // 從表單 POST 方法取得使用者輸入的 RFID 卡片編號
    $rfidkey = $_POST["rfidkey"];

    // 取得卡片是否啟用的狀態（通常為 0 或 1）
    $activate = $_POST["activate"];

    // 取得卡片的啟用起始時間（格式為 yyyymmddhhmmss）
    $startdatetime = $_POST["startdatetime"];

    // 取得卡片的啟用結束時間（格式為 yyyymmddhhmmss）
    $enddatetime = $_POST["enddatetime"];
     // 準備查詢語句，從資料表 rfidlist 確定是否重複資料
    $qry = "select * from rfidlist where rfidkey = '%s' and activate = %d" ;
    // 套用實際的資料進入 SQL 字串
    $qrystr = sprintf($qry, $rfidkey, $activate);

    echo $qrystr . "<br>"; // 顯示查詢語句（除錯用）

    // 執行查詢語句
    $result = mysqli_query($link, $qrystr);
    $num = mysqli_num_rows($result); // 取得符合條件的紀錄數量

    if ($num == 0) 
    {
        /*
         建立 SQL 插入語法模板：
         將表單中取得的資料插入至 rfidlist 資料表中
         欄位依序為：systime, IP, rfidkey, activate, startdatetime, enddatetime
         使用 sprintf 格式化 SQL 語句，以避免直接字串拼接
         */
        $qry = "insert into rfidlist (systime, IP, rfidkey, activate, startdatetime, enddatetime) values ('%s', '%s', '%s', %d, '%s', '%s');";

        // 套用實際的資料進入 SQL 字串
        $qrystr = sprintf($qry, $sysdt, $ip, $rfidkey, $activate, $startdatetime, $enddatetime);


        // 顯示組合完成的 SQL 語句（除錯用，可移除）
        //echo $qrystr."<br>";

        // 執行 SQL 插入指令，如果成功，則導向至 cardlist.php 頁面
        if (mysqli_query($link, $qrystr)) 
        {
            // 若資料新增成功，先關閉與資料庫的連線
            mysqli_close($link);

            // 重新導向使用者到卡片列表頁面
            header("Location: cardlistmgt.php");
            exit(); // 確保導向後不再執行後續程式
        }
    }    
    // 若 SQL 執行失敗（不論是否成功執行上面 if），最終都關閉資料庫連線
    mysqli_close($link);
    header("Location: cardlistmgt.php");
    exit(); // 確保導向後不再執行後續程式 
?>
