<?php
<<<<<<< HEAD
/*
這個 PHP 程式是從網址的 GET 參數中取得一張 RFID 卡的卡號（KEY），
然後從資料表 lockpass 中查詢出該卡片的所有刷卡紀錄，
顯示出：
    刷卡 ID（流水號）
    卡號
    刷卡時間
    使用的裝置 MAC
    來源 IP
    狀態（轉換為中文說明）

*/
// 啟動 session，常用於管理登入狀態或傳遞頁面變數
session_start();


// 引入資料庫連線與共用函式庫
include("../comlib.php");        // 一般用途的共用函式，例如 rfidpassstatus()
include("../Connections/iotcnn.php"); // 包含資料庫連線參數與連線函式

// 呼叫自定的連線函式 Connection()，取得 MySQL 連線物件
$link = Connection(); 

// 檢查 GET 參數中是否有傳入名為 "KEY" 的值（也就是卡號）
if (!isset($_GET["KEY"])) {
    echo  "KEY Number Lost <br>"; // 若未傳入，顯示錯誤訊息
    die(); // 結束程式執行
}

// 取得 GET 傳入的卡號參數值，並去除前後空白
$temp0 = trim($_GET["KEY"]);

// 若卡號為空字串，也視為錯誤
if ($temp0 == "") {
    echo "KEY Number is empty string <br>";
    die(); // 結束程式
}

// 設定表格標題顯示樣板（包含卡號）
$tstr = "Logging Detail for Card Number %s(詳細每一筆資料)";

// 定義每列表格資料的輸出樣板，包含6個欄位
$subrow = "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>";

// SQL 查詢樣板：查詢 lockpass 資料表中所有屬於指定卡號的資料
// 依 systime 降序排列，若時間相同則依 MAC 升序排列
//select * from lockpass WHERE cardnumber = '1938332656' order by systime desc , MAC asc
$str = "select * from lockpass WHERE cardnumber = '%s' order by systime desc , MAC asc;";

// 將卡號帶入 SQL 字串中，產生實際查詢語句
$qrystr = sprintf($str, $temp0);

// 建立多個陣列用來儲存查詢到的欄位資料
$d00 = array(); // id 欄位
=======
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
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
$d01 = array(); // cardnumber
$d02 = array(); // systime
$d03 = array(); // MAC
$d04 = array(); // IP
<<<<<<< HEAD
$d05 = array(); // status（會經過文字轉換）

// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

// 如果查詢成功
if ($result !== FALSE) {
    // 一筆一筆讀出查詢結果，存入陣列
    while ($row = mysqli_fetch_array($result)) {
        array_push($d00, $row["id"]);
        array_push($d01, $row["cardnumber"]);
        array_push($d02, $row["systime"]);
        array_push($d03, $row["MAC"]);
        array_push($d04, $row["IP"]);
        // 使用 rfidpassstatus 函數將數字狀態轉為說明文字（例如 "成功"、"失敗"）
        array_push($d05, rfidpassstatus((int)$row["status"]));
    }
}

// 釋放查詢資源
mysqli_free_result($result);

// 關閉資料庫連線
mysqli_close($link);
?>

<!DOCTYPE html ...>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 頁面編碼與標題 -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Logging Detail for Card Number %s(詳細每一筆資料)</title>
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <!-- 頁首區塊（如標題Logo） -->
    <?php include("../toptitle.php"); ?>

    <div align="center">
        <table border="1" align="center" cellspacing="1" cellpadding="1">
        <!-- 表格主標題 -->
        <tr bgcolor="#CFC">
            <td colspan="6">
                <div align="center">
                    <?php echo sprintf($tstr, $temp0); ?>
                </div>
            </td>
        </tr>
        <!-- 表格欄位名稱 -->
        <tr>
            <td>ID(序號)</td>
            <td>Card Number(卡號)</td>
            <td>DateTime(資料時間)</td>
            <td>MAC Address(裝置編號)</td>
            <td>IP(連線網址)</td>
            <td>Status(狀態)</td>
        </tr>

<!-- 使用 PHP 印出所有資料列 -->
<?php 
if (count($d01) > 0) { // 有資料才顯示
    for ($i = 0; $i < count($d01); $i++) {
        echo sprintf($subrow, $d00[$i], $d01[$i], $d02[$i], $d03[$i], $d04[$i], $d05[$i]);
    }
}
?>
    </table>
</div>

    <!-- 頁尾區塊 -->
=======
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
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
    <?php include("../topfooter.php"); ?>
</body>
</html>
