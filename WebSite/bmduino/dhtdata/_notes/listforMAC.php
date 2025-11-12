<?php
// 引入資料庫連接和通用函數
include("../comlib.php");        // 通用函數庫
include("../Connections/iotcnn.php"); // 資料庫連接參數

// 建立與資料庫的連接
$link = Connection(); // Connection 是在 iotcnn.php 中定義的連接函數

// 確保 GET 參數中存在 "MAC"，如果沒有，則終止程式
if (!isset($_GET["MAC"])) {
    echo "MAC address lost <br>";
    die(); // 終止程式
}

// 獲取 MAC 地址參數，並移除首尾空格
$temp0 = trim($_GET["MAC"]);

// 如果 MAC 地址為空，則終止程式
if ($temp0 == "") {
    echo "MAC Address is empty string <br>";
    die(); // 終止程式
}

// 表格標題，包含 MAC 地址
$tstr = "Temperature & Humidity Sensor for MAC %s(溫溼度感測裝置)";

// 定義表格行的模板，用於顯示資料
$subrow = "<tr><td>%s</td><td>%s</td><td>%s</td><td>%4.2f</td><td>%4.2f</td></tr>";

// SQL 查詢字串，使用給定的 MAC 地址
$str = "SELECT MAC, temperature, humidity, systime, IP FROM big.dhtdata WHERE MAC = '%s' order by IP, systime asc";

// 產生真正SQL 查詢字串，格式化查詢字串，替換占位符
$qrystr = sprintf($str, $temp0);

// 建立空陣列，用於儲存查詢結果
$d01 = array(); // MAC 地址
$d02 = array(); // IP 地址
$d03 = array(); // 更新時間
$d04 = array(); // 溫度
$d05 = array(); // 濕度

// 執行 SQL 查詢，將結果存入 $result
$result = mysqli_query($link, $qrystr);

// 如果查詢成功，開始處理資料
if ($result !== FALSE) 
{
    // 逐行讀取資料，並將其存入對應的陣列
    while ($row = mysqli_fetch_array($result)) 
    {
        array_push($d01, $row["MAC"]);  // MAC 地址
        array_push($d02, $row["IP"]);   // IP 地址
        array_push($d03, $row["systime"]); // 更新時間
        array_push($d04, $row["temperature"]); // 溫度
        array_push($d05, $row["humidity"]); // 濕度
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
    <title>Temperature and Humidity List for MAC Address</title>
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
                <td colspan="5">
                    <div align="center">
                        <?php echo sprintf($tstr, $temp0); ?> <!-- 表格標題，包含 MAC 地址 -->
                    </div>
                </td>
            </tr>
            <!-- 表格列標題 -->
            <tr>
                <td>MAC Address(網卡編號)</td>
                <td>IP Address(用戶端 IP)</td>
                <td>Update Time(更新時間)</td>
                <td>Temperature(溫度)</td>
                <td>Humidity(濕度)</td>
            </tr>

            <!-- 使用 PHP 迴圈生成表格內容 -->
            <?php 
            if (count($d01) > 0) { // 如果有資料
                for ($i = 0; $i < count($d01); $i++) { // 逐行顯示
                    // 使用 $subrow 模板和 sprintf 生成表格行
                    echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $d05[$i]);
                }
            }
            ?>
        </table>
    </div>

    <!-- 包含頁腳內容 -->
    <?php include("../topfooter.php"); ?>
</body>
</html>
