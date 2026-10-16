<?php
/*
 這段程式碼顯示了一個頁面，列出了溫溼度感測器的 MAC 地址、溫度、濕度、最後更新時間等信息。
 * SQL 查詢從 big.dhtdata 資料表中獲取數據，
 * 然後在 HTML 表格中顯示這些信息。超連結提供了查看特定裝置詳細資料的途徑。
 * 表格內含標題欄和資料行，並使用 PHP 的 sprintf 函數進行格式化。
 */
// 啟動 PHP 會話，確保在連線和查詢資料庫前正確初始化
session_start();

// 引入外部檔案，這些檔案包含資料庫連接和其他功能
include("../comlib.php");        // 通用函數庫
include("../Connections/iotcnn.php"); // 資料庫連接參數

// 建立與資料庫的連接
$link = Connection(); // Connection 是在 iotcnn.php 中定義的連接函數

// 格式化表格每一行的模板
    $subrow = "<tr><td>%s</td><td>%s</td><td>%d</td></tr>";

// 格式化超連結的模板，用於查詢特定 MAC 地址的詳細資料
$op1 = "<a href='listforMAC.php?MAC=%s'>Device Detail(詳細每一筆資料)</a>";

// SQL 查詢字串，用於按 MAC 地址分組並查詢距離值和其他資訊
$qrystr = "SELECT rfidkey, activate, count(*) as cnt FROM rfidlist WHERE 1 group by rfidkey,activate;";

// 建立空陣列，用於儲存查詢結果
$d01 = array(); // 用於儲存 rfidkey 地址
$d02 = array(); // 用於儲存activate
$d03 = array(); // 用於amount


// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

if ($result !== FALSE) { // 如果查詢成功，開始處理結果
    // 逐行遍歷查詢結果
    while ($row = mysqli_fetch_array($result)) {
        // 將查詢結果添加到各個陣列中
        array_push($d01, $row["rfidkey"]);
        array_push($d02, rfidstatus((int)$row["activate"]));
        array_push($d03, $row["cnt"]);

    }
}

// 釋放查詢結果
mysqli_free_result($result);

// 關閉資料庫連接
mysqli_close($link);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>RFID Card List (RFID卡號清單)</title>
    <link href="../webcss.css" rel="stylesheet" type="text/css" /> <!-- 加載 CSS 樣式 -->
</head>
<body>
    <!-- 包含網頁的標題內容 -->
    <?php include("../toptitle.php"); ?>

    <div align="center"> <!-- 居中對齊 -->
        <table border="1" align="center" cellspacing="1" cellpadding="1"> <!-- 建立表格 -->
            <tr bgcolor="#CFC"> <!-- 標題行，帶有背景色 -->
                <td colspan='5'><div align='center'>Ultrasonic HC-SR04 Device(超音波測距裝置)</div></td> <!-- 合併六個單元格 -->
            </tr>
            <tr> <!-- 表格的標題欄 -->
                <td>RFID Card(RFID卡號)</td>
                <td>Activate Status(狀態)</td>
                <td>Amount(卡號數量)</td>
            </tr>

            <?php 
            if (count($d01) > 0) { // 如果有數據
                // 使用 for 迴圈遍歷陣列
                for ($i = 0; $i < count($d01); $i++) {
                    $op1a = sprintf($op1, $d01[$i]); // 生成每行的詳細資料連結
                    echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i]); 
                    // 使用格式化字串顯示每行的內容
                }
            }
            ?>
        </table>
    </div>

    <!-- 包含網頁的頁尾內容 -->
    <?php
    include("../topfooter.php"); 
    ?>
</body>
</html>
