<?php
/*
這段 PHP 程式是一個 網頁介面後台模組，用來：
查詢資料庫中所有血氧裝置（以 MAC 分組）
顯示每個裝置的：
    MAC 地址（裝置編號）
    平均血氧值
    平均心跳值
    總紀錄筆數
提供「查看詳細資料」的連結
這可用於健康監測系統中做「裝置總覽」，
 * 點進去才能看單一裝置歷史紀錄。 
 */


// 啟動 PHP Session，用於儲存或傳遞登入資訊等（雖然這裡未使用 session 變數，但是良好習慣）
session_start();

// 引入公用程式與資料庫連線設定
include("../comlib.php");        // comlib.php 為通用函式庫
include("../Connections/iotcnn.php"); // 資料庫連線參數與連線函數定義

// 建立與 MySQL 的連線，Connection() 是在 iotcnn.php 中定義的函數
$link = Connection();

// 定義表格中每列輸出的格式：MAC地址、血氧平均、心跳平均、筆數、超連結
$subrow = "<tr><td>%s</td><td>%6.2f</td><td>%6.2f</td><td>%d</td><td>%s</td></tr>";

// 定義查詢詳細資料的超連結樣板
$op1 = "<a href='listforMAC.php?MAC=%s'>Device Detail(詳細每一筆資料)</a>";

// SQL 查詢語句：
// 查詢 bloodoxygen 表中的資料，按 MAC 分組，統計平均血氧值與平均心跳值與記錄筆數，並依時間降冪排序
$qrystr = "
    SELECT 
        MAC, 
        AVG(`blood`) AS bd, 
        AVG(`heartbeat`) AS hb, 
        COUNT(*) AS cnt 
    FROM bloodoxygen 
    GROUP BY MAC 
    ORDER BY systime DESC;
";

// 宣告陣列用來存放查詢結果（分別存 MAC、血氧、心跳、筆數）
$d01 = array(); // MAC
$d02 = array(); // 平均血氧值
$d03 = array(); // 平均心跳值
$d04 = array(); // 記錄筆數

// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

// 若查詢成功，開始將資料放入陣列
if ($result !== FALSE) {
    while ($row = mysqli_fetch_array($result)) {
        array_push($d01, $row["MAC"]);     // 存 MAC 地址
        array_push($d02, $row["bd"]);      // 存平均血氧值
        array_push($d03, $row["hb"]);      // 存平均心跳值
        array_push($d04, $row["cnt"]);     // 存紀錄筆數
    }
}

// 若結果是 mysqli_result 類型，釋放記憶體（針對 SELECT 查詢才需要）
if ($result instanceof mysqli_result) {
    mysqli_free_result($result);
}

// 關閉資料庫連線
mysqli_close($link);
?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 設定網頁的字元編碼與標題 -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Blood Oxygen Device List(血氧測量裝置)</title>
    <!-- 引入外部 CSS 樣式檔案 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>

<!-- 引入頂部標題（可能包含 LOGO、網站名稱） -->
<?php include("../toptitle.php"); ?>

<!-- 中央對齊的容器 -->
<div align="center">
    <!-- 建立表格 -->
    <table border="1" align="center" cellspacing="1" cellpadding="1">
        <!-- 表格標題列 -->
        <tr bgcolor="#CFC">
            <td colspan='5'>
                <div align='center'>
                    Blood Oxygen Device (血氧測量裝置)
                </div>
            </td>
        </tr>

        <!-- 資料表頭欄位 -->
        <tr>
            <td>MAC Address(網卡編號)</td>
            <td>Average of Blood Oxygen(平均血氧值)</td>
            <td>Average of Heart Beats(平均心跳值)</td>
            <td>Records Count(紀錄筆數)</td>
            <td>Query Detail(明細查詢)</td>
        </tr>

        <!-- 使用 PHP 動態顯示資料 -->
        <?php 
        if (count($d01) > 0) {
            for ($i = 0; $i < count($d01); $i++) {
                // 生成查詢明細的超連結（點擊可查看該 MAC 裝置所有詳細紀錄）
                $op1a = sprintf($op1, $d01[$i]);

                // 輸出該列的所有欄位資料
                echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $op1a);
            }
        }
        ?>
    </table>
</div>

<!-- 引入網頁下方頁腳 -->
<?php include("../topfooter.php"); ?>

</body>
</html>
