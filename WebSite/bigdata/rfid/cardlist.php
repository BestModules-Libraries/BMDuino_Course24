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

<<<<<<< HEAD
// 啟動 PHP 會話，確保使用者的 Session 能夠追蹤登入或其他狀態
session_start();

// 引入自訂的函式庫 comlib.php（例如共用函數）
include("../comlib.php");        // 引入共用函數庫，如自訂函數  

// 引入與資料庫連線的設定與函數（通常在 iotcnn.php 裡會定義 mysqli_connect 的細節）
include("../Connections/iotcnn.php"); 

// 透過自訂的函式 Connection() 建立資料庫連線
$link = Connection(); 
=======
// 啟動 session，常用於管理登入狀態或傳遞頁面變數
session_start();

// 引入共用函式與資料庫連線參數
include("../comlib.php");        // 引入共用函數庫，如自訂函數
include("../Connections/iotcnn.php"); // 引入資料庫連線參數與連線函式

// 透過自定函數 Connection() 與資料庫建立連線
$link = Connection(); // 此函數定義於 iotcnn.php 檔案中
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f

// 定義 HTML 表格中資料列的格式化字串 (使用 sprintf 格式化)
$subrow = "<tr><td>%s</td><td>%s</td><td>%d</td></tr>";

// 定義 SQL 查詢語句：
// 從 rfidlist 資料表中查詢 rfidkey（RFID 卡號）、啟用狀態（activate），並統計每組筆數
$qrystr = "select rfidkey, activate, count(*) as cnt FROM rfidlist WHERE 1 group by rfidkey,activate;";
//SQL 查詢語句中 group by rfidkey, activate 代表統計每個 RFID 卡號與其狀態出現的次數，常用於分析資料分佈。
// 初始化三個陣列，用來儲存查詢結果
$d01 = array(); // 儲存 RFID 卡號
$d02 = array(); // 儲存卡號狀態（啟用或停用）
$d03 = array(); // 儲存相同 RFID 卡號與狀態的筆數

// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

if ($result !== FALSE) 
{ // 如果查詢成功
    // 逐筆處理查詢結果
    while ($row = mysqli_fetch_array($result)) 
    {
        // 將查詢到的 RFID 卡號、狀態與數量分別存入對應陣列
        array_push($d01, $row["rfidkey"]);
        array_push($d02, rfidstatus((int)$row["activate"])); // 將 activate 狀態轉為對應的文字（自訂函數）
        array_push($d03, $row["cnt"]); // 存入數量
    }
}
//釋放資料庫連線使用記憶體
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
    <title>RFID Card List (RFID卡號清單)</title>
    <!-- 引入外部 CSS 樣式表 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <!-- 載入上方標題列（通常包含 LOGO、網站名稱等） -->
    <?php include("../toptitle.php"); ?>

<<<<<<< HEAD

=======
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
    <!-- 主資料區塊：置中對齊 -->
    <div align="center">
        <!-- 建立表格，顯示 RFID 卡號資料 -->
        <table border="1" align="center" cellspacing="1" cellpadding="1">
             <!-- 資料表的第一階主標題欄位 -->
            <!-- 表格的標題列，底色為淡綠色 -->
            <tr bgcolor="#CFC">
                <td colspan='3'>
                    <div align='center'>Authorized RFID Card List(可通過之RFID卡號)</div>
                </td>
            </tr>
            <!-- 資料表的第二階主標題欄位 -->
            <tr>
                <td>RFID Card(RFID卡號)</td>
                <td>Activate Status(狀態)</td>
                <td>Amount(卡號數量)</td>
            </tr>

            <?php 
            // 如果查詢結果陣列不為空，表示有資料
            if (count($d01) > 0) 
            {
                // 用 for 迴圈輸出每一筆資料
                for ($i = 0; $i < count($d01); $i++) {
                    // 使用 sprintf 將每一筆資料格式化並輸出成 HTML 表格列
                    echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i]);
                }
            }
            ?>
        </table>
    </div>

    <!-- 載入網頁下方的頁尾（如聯絡資訊、版權聲明等） -->
<<<<<<< HEAD
    <!-- 引入頁面下方版權或結尾區塊 -->
=======
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
    <?php include("../topfooter.php"); ?>
</body>
</html>
