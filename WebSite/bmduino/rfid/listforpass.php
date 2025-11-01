<?php
// 引入資料庫連接和通用函數
include("../comlib.php");        // 通用函數庫
include("../Connections/iotcnn.php"); // 資料庫連接參數

// 建立與資料庫的連接
$link = Connection(); // Connection 是在 iotcnn.php 中定義的連接函數

// 確保 GET 參數中存在 "MAC"，如果沒有，則終止程式
if (!isset($_GET["KEY"])) {
    echo "KEY Number Lost <br>";
    die(); // 終止程式
}

// 獲取 MAC 地址參數，並移除首尾空格
$temp0 = trim($_GET["KEY"]);

// 如果 MAC 地址為空，則終止程式
if ($temp0 == "") {
    echo "KEY Number is empty string <br>";
    die(); // 終止程式
}
//ass Logging Detail(詳細每一筆資料)

$tstr = "Logging Detail for Card Number %s(詳細每一筆資料)";

// 定義表格行的模板，用於顯示資料
$subrow = "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>";

// SQL 查詢字串，使用給定的 MAC 地址
$str = "select * from lockpass WHERE cardnumber = '%s' order by systime desc , MAC asc;";

// 產生真正SQL 查詢字串，格式化查詢字串，替換占位符
$qrystr = sprintf($str, $temp0);

// 建立空陣列，用於儲存查詢結果
$d00 = array(); // id
$d01 = array(); // cardnumber
$d02 = array(); // systime
$d03 = array(); // MAC
$d04 = array(); // IP
$d05 = array(); // status

// 執行 SQL 查詢，將結果存入 $result
$result = mysqli_query($link, $qrystr);

// 如果查詢成功，開始處理資料
if ($result !== FALSE) 
{
    // 逐行讀取資料，並將其存入對應的陣列
    while ($row = mysqli_fetch_array($result)) 
    {
        array_push($d00, $row["id"]);  // MAC 地址
        array_push($d01, $row["cardnumber"]);  // MAC 地址
        array_push($d02, $row["systime"]);  // MAC 地址
        array_push($d03, $row["MAC"]);  // MAC 地址
        array_push($d04, $row["IP"]);  // MAC 地址
        array_push($d05, rfidpassstatus((int)$row["status"]));  // MAC 地址
    }
}

// 釋放查詢結果資源
mysqli_free_result($result);

// 關閉資料庫連接
mysqli_close($link);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 設定網頁的標題和數據字元碼  -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Logging Detail for Card Number %s(詳細每一筆資料)</title>
    <link href="../webcss.css" rel="stylesheet" type="text/css" /> <!-- 加載 CSS 樣式 -->
</head>
<body>
    <!-- 包含標題內容 -->
    <?php include("../toptitle.php"); ?>

    <!-- 建立居中對齊的表格 -->
    <div align="center">
        <table border="1" align="center" cellspacing="1" cellpadding="1">
            <!-- 表格標題行 -->
            <tr bgcolor="#CFC">
                <td colspan="6">
                    <div align="center">
                        <?php echo sprintf($tstr, $temp0); ?> <!-- 表格標題，包含 MAC 地址 -->
                    </div>
                </td>
            </tr>
            <!-- 表格列標題 -->
            <tr>
                <td>ID(序號)</td>
                <td>Card Number(卡號)</td>
                <td>DateTime(資料時間)</td>
                <td>MAC Address(裝置編號)</td>
                <td>IP(連線網址)</td>
                <td>Status(狀態)</td>
            </tr>

            <!-- 使用 PHP 迴圈生成表格內容 -->
            <?php 
            if (count($d01) > 0) { // 如果有資料
                for ($i = 0; $i < count($d01); $i++) { // 逐行顯示
                    // 使用 $subrow 模板和 sprintf 生成表格行
                    echo sprintf($subrow, $d00[$i], $d01[$i], $d02[$i], $d03[$i], $d04[$i], $d05[$i]);
                }
            }
            ?>
        </table>
    </div>

    <!-- 包含頁腳內容 -->
    <?php include("../topfooter.php"); ?>
</body>
</html>
