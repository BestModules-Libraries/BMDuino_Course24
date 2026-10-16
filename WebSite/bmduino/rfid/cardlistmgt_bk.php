<?php
/*
 這段 PHP 程式碼的主要功能是從資料庫中查詢 RFID 卡號清單，
 並依據卡號與啟用狀態（activate）進行分組與統計，
 最後將這些結果以 HTML 表格方式呈現在網頁上。
-------------------------
rfidstatus() 是一個自訂函數（可能在 comlib.php 中定義），
 * 用來將數值型的 activate 狀態（例如 0 或 1）
 * 轉換為人類可讀的文字（例如「啟用」、「停用」） 
 * 
 
*/

// 啟動 session，常用於管理登入狀態或傳遞頁面變數
session_start();

// 引入共用函式與資料庫連線參數
include("../comlib.php");        // 引入共用函數庫，如自訂函數
include("../Connections/iotcnn.php"); // 引入資料庫連線參數與連線函式

// 透過自定函數 Connection() 與資料庫建立連線
$link = Connection(); // 此函數定義於 iotcnn.php 檔案中

// 定義 HTML 表格中資料列的格式化字串 (使用 sprintf 格式化)
$subrow = "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>";
$admstr = "<a href='rfidformadd.php'>新增</a>、<a href='rfidformedt.php?id=%d'>修改</a>、<a href='rfidDatadel.php?id=%d'>刪除</a>" ;
// 定義 SQL 查詢語句：
// 從 rfidlist 資料表中查詢 rfidkey（RFID 卡號）、啟用狀態（activate），並統計每組筆數
$qrystr = "select * from rfidlist WHERE 1  order by rfidkey,activate,systime;";
//SQL 查詢語句中 group by rfidkey, activate 代表統計每個 RFID 卡號與其狀態出現的次數，常用於分析資料分佈。
// 初始化三個陣列，用來儲存查詢結果
$d01 = array(); // 儲存 RFID 卡號
$d02 = array(); // 儲存卡號狀態（啟用或停用）
$d03 = array(); // 儲存相同 RFID 卡號與狀態的筆數
$d04 = array(); // 儲存相同 RFID 卡號與狀態的筆數
$d05 = array(); // 儲存相同 RFID 卡號與狀態的筆數
$d06 = array(); // 儲存相同 RFID 卡號與狀態的筆數


// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

if ($result !== FALSE) 
{ // 如果查詢成功
    // 逐筆處理查詢結果
    while ($row = mysqli_fetch_array($result)) // 迴圈讀取
    {
        // 將查詢到的 RFID 卡號、狀態與數量分別存入對應陣列
        array_push($d01, $row["id"]);   //rfidlist資料表之主鍵
        array_push($d02, $row["rfidkey"]);  //RFID 卡號
        array_push($d03, rfidstatus((int)$row["activate"])); // 將 activate 狀態轉為對應的文字（自訂函數）
        array_push($d04, $row["systime"]); // 資料時間
        array_push($d05, $row["startdatetime"]); // 啟用時間
        array_push($d06, $row["enddatetime"]); // 結束時間
    }
}

// 釋放查詢記憶體資源
mysqli_free_result($result);

// 關閉資料庫連線
mysqli_close($link);
?>

<!-- 以下是 HTML 部分，負責將上方查詢到的資料呈現在網頁上 -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" 
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 設定網頁編碼 -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <!-- 設定網頁標題 -->
    <title>RFID Card List Management(RFID卡號管理)</title>
    <!-- 引入外部 CSS 樣式表 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <!-- 載入上方標題列（通常包含 LOGO、網站名稱等） -->
    <?php include("../toptitle.php"); ?>

    <!-- 主資料區塊：置中對齊 -->
    <div align="center">
        <!-- 建立表格，顯示 RFID 卡號資料 -->
        <table border="1" align="center" cellspacing="1" cellpadding="1">
             <!-- 資料表的第一階主標題欄位 -->
            <!-- 表格的標題列，底色為淡綠色 -->
            <tr bgcolor="#CFC">
                <td colspan='7'>
                    <div align='center'>Authorized RFID Card List(可通過之RFID卡號)</div>
                </td>
            </tr>
            <!-- 資料表的第二階主標題欄位 -->
            <tr>
                <td>id(主鍵)</td>
                <td>RFID Card(RFID卡號)</td>
                <td>Activate Status(狀態)</td>
                <td>Data Time(系統時間)</td>
                <td>Start Data Time(起始時間)</td>
                <td>End Data Time(結束時間)</td>
                <td>Management(管理)</td>
            </tr>

            <?php 
            // 如果查詢結果陣列不為空，表示有資料
            if (count($d01) > 0) 
            {
                // 用 for 迴圈輸出每一筆資料
                for ($i = 0; $i < count($d01); $i++) {
                    // 使用 sprintf 將每一筆資料格式化並輸出成 HTML 表格列
                    $mgestr = sprintf($admstr,$d01[$i],$d01[$i]) ;//產生管理列的連結
                    echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i],$d04[$i],$d05[$i],$d06[$i],$mgestr);
                }
            }
            ?>
        </table>
    </div>

    <!-- 載入網頁下方的頁尾（如聯絡資訊、版權聲明等） -->
    <?php include("../topfooter.php"); ?>
</body>
</html>
