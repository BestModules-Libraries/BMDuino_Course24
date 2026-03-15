<?php 
    // 引入資料庫連線設定（iotcnn.php 應該是資料庫連線細節設定）
    include("../Connections/iotcnn.php");
    // 呼叫自定的 Connection() 函式，建立與 MySQL 資料庫的連線
    $link = Connection();
    $sid = $_GET["id"]; //取得目前用戶端的 id 主鍵 from cardlistmgt.php

    /*
     建立 SQL 插入語法模板：
     將表單中取得的資料插入至 rfidlist 資料表中
     delete from rfidlist WHERE id = 4;
     */
    
    $qry = "delete from rfidlist where id = %d ;";
    
    // 套用實際的資料進入 SQL 字串
    $qrystr = sprintf($qry,$sid);
    echo $qrystr."<br>" ;
    // 顯示組合完成的 SQL 語句（除錯用，可移除）
    //echo $qrystr."<br>";

    // 執行 SQL 插入指令，如果成功，則導向至 cardlistmgt.php 頁面
    if (mysqli_query($link, $qrystr)) {
        // 若資料新增成功，先關閉與資料庫的連線
        mysqli_close($link);

        // 重新導向使用者到卡片列表頁面
        header("Location: cardlistmgt.php");
        exit(); // 確保導向後不再執行後續程式
    }

    // 若 SQL 執行失敗（不論是否成功執行上面 if），最終都關閉資料庫連線
    mysqli_close($link);
?>
