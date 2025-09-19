<?php
/*
 這段程式碼顯示了一個頁面，列出了RFID Access logging 等信息。
這個程式可以視為 刷卡紀錄系統的總覽頁：

資料來源：從 lockpass 表格中彙整資料（這是 RFID 門禁刷卡資料表）。
統計用途：針對每張卡，統計進出紀錄筆數，以及最早與最晚的一筆資料時間。
互動性：提供每張卡片的明細連結，導向 listforpass.php?KEY=卡號 進一步查看單一卡的所有刷卡紀錄。
前後端分離清晰：PHP 處理邏輯與 HTML 顯示格式分開，使系統更易維護。 
 */

// 啟動 PHP 會話，確保使用者的 Session 能夠追蹤登入或其他狀態
session_start();

// 引入自訂的函式庫 comlib.php（例如共用函數）
include("../comlib.php");        

// 引入與資料庫連線的設定與函數（通常在 iotcnn.php 裡會定義 mysqli_connect 的細節）
include("../Connections/iotcnn.php"); 

// 透過自訂的函式 Connection() 建立資料庫連線
$link = Connection(); 

// 定義一個 HTML 表格列的模板（每行五個欄位）
$subrow = "<tr><td>%s</td><td>%s</td><td>%s</td><td>%d</td><td>%s</td></tr>";

// 定義每筆紀錄對應的超連結模板，點擊後會跳轉至該卡片的詳細紀錄頁面
<<<<<<< HEAD
$op1 = "<a href='listforpass.php?KEY=%s'>Pass Logging Detail(詳細每一筆資料)</a>/<a href='chartforpass.php?KEY=%s'>Pass Logging Chart(資料統計圖表)</a>";
=======
$op1 = "<a href='listforpass.php?KEY=%s'>Pass Logging Detail(詳細每一筆資料)</a>";
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f

// 建立 SQL 查詢：
// - 查詢所有卡號(cardnumber)
// - 統計每張卡的刷卡次數(count(*))
// - 查出該卡的最早(min(systime))與最晚(max(systime))刷卡時間
// - GROUP BY 將相同卡號的資料整合在一起
// - ORDER BY max(systime) 依照最後刷卡時間倒序排序（越近期的排前面）
$qrystr = "select cardnumber , count(*) as cnt , max(systime) as max, min(systime) as min from lockpass WHERE 1 group by cardnumber order by max desc;";

// 建立四個陣列來儲存資料
$d01 = array(); // 卡號列表
$d02 = array(); // 每張卡的最早刷卡時間
$d03 = array(); // 每張卡的最晚刷卡時間
$d04 = array(); // 每張卡的刷卡次數（紀錄筆數）

// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

// 如果查詢成功
if ($result !== FALSE) {
    // 使用 while 迴圈遍歷所有查詢結果
    while ($row = mysqli_fetch_array($result)) {
        // 將每列結果存入對應的陣列中
        array_push($d01, $row["cardnumber"]); // 卡號
        array_push($d02, $row["min"]);        // 最早刷卡時間
        array_push($d03, $row["max"]);        // 最晚刷卡時間
        array_push($d04, $row["cnt"]);        // 刷卡次數
    }
}

// 釋放資料庫查詢資源
mysqli_free_result($result);

// 關閉資料庫連線
mysqli_close($link);

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 設定網頁編碼與標題 -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>User Pass Logging Record by RFID Number (RFID裝置之開門紀錄)</title>
    <!-- 載入 CSS 樣式檔 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <!-- 引入頁面上方標題 -->
    <?php include("../toptitle.php"); ?>

    <div align="center">
        <table border="1" align="center" cellspacing="1" cellpadding="1">
            <!-- 表格標題列 -->
            <tr bgcolor="#CFC">
                <td colspan='5'><div align='center'>User Pass Record(刷卡號開門紀錄篇)</div></td>
            </tr>
            <tr>
                <td>RFID Card(RFID卡號)</td>
                <td>Earliest Date(最早日期)</td>
                <td>Latest Date(最晚日期)</td>
                <td>Amount(進出次數)</td>
                <td>Detail(查詢明細)</td>
            </tr>

            <?php 
            // 如果有資料要顯示
            if (count($d01) > 0) {
                // 用迴圈把每筆資料列印出來
                for ($i = 0; $i < count($d01); $i++) {
                    // 建立超連結（跳轉到每張卡片詳細紀錄）
<<<<<<< HEAD
                    $op1a = sprintf($op1, $d01[$i], $d01[$i]);
=======
                    $op1a = sprintf($op1, $d01[$i]);
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
                    // 印出一行表格資料
                    echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $op1a);
                }
            }
            ?>
        </table>
    </div>

    <!-- 引入頁面下方版權或結尾區塊 -->
    <?php include("../topfooter.php"); ?>
</body>
</html>
