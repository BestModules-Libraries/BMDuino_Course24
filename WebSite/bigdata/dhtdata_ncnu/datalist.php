<?php
include "../comlib.php"; // 包含 "comlib.php" 檔案，該檔案包含資料庫操作的通用函數。
include "../Connections/iotcnn.php"; // 包含 "iotcnn.php" 檔案，該檔案包含資料庫連接函數。

$link = Connection(); // 呼叫 Connection 函數建立資料庫連接，並將連接物件指派給 $link。

// 定義表格行的模板，使用 sprintf 格式化資料。
$subrow = "<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>( %s , %s )</td><td>%s<br>%s<br>%s<br>%s</td></tr>";
// 定義裝置管理的連結。
$op1 = "<a href='devicelistadd.php'>Device Belong(裝置歸屬)</a>";
// 定義曲線圖連結的模板。
$op2a = "<a href='ShowChartlist.php?MAC=%s'>Curve Chart</a>";
// 定義每日曲線圖連結的模板。
$op3a = "<a href='ShowChartlistday.php?MAC=%s'>Daily Curve Chart</a>";
// 定義盒鬚圖連結的模板（未使用）。
$op4a = "<a href='ShowBoxChart.php?MAC=%s'>Box Plot</a>";
// 定義量表顯示連結的模板。
$op9a = "<a href='ShowSingleGuage.php?mac=%s'>Guage Display</a>";

// SQL 查詢，從 sitelist (a)、site (b) 和 sensortype (c) 表格中選擇資料，條件為 a.sensortype 等於 c.sid，a.Did 等於 b.id，且 a.sensortype 為 '01'，按 b.siteid 和 a.mac 升序排序。
$qrystr = "select a.*, b.*, c.* , a.id as mainid from ncnuiot.sitelist as a, ncnuiot.site as b, ncnuiot.sensortype as c where a.sensortype = c.sid and a.Did = b.id and a.sensortype = '01' order by b.siteid asc , a.mac asc";

// 初始化陣列，用於儲存查詢結果的特定欄位。
$d00 = array(); // 儲存 mainid
$d01 = array(); // 儲存 siteid
$d02 = array(); // 儲存 sitename
$d03 = array(); // 儲存 address
$d04 = array(); // 儲存 mac
$d05 = array(); // 儲存 longitude
$d06 = array(); // 儲存 latitude

// 執行 SQL 查詢，原始註解「僅找最後 5 筆資料」不正確，此查詢未有限制。
$result = mysqli_query($link, $qrystr);
if ($result !== FALSE) {
    while ($row = mysqli_fetch_array($result)) {
        array_push($d00, $row["mainid"]); // 將 mainid 加入陣列
        array_push($d01, $row["siteid"]); // 將 siteid 加入陣列
        array_push($d02, $row["sitename"]); // 將 sitename 加入陣列
        array_push($d03, $row["address"]); // 將 address 加入陣列
        array_push($d04, $row["mac"]); // 將 mac 加入陣列
        array_push($d05, $row["longitude"]); // 將 longitude 加入陣列
        array_push($d06, $row["latitude"]); // 將 latitude 加入陣列
    } // 結束 while 迴圈
}

// 釋放結果集，關閉資料庫連接。
mysqli_free_result($result);
mysqli_close($link);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Temperature and Humidity Device List</title>
<link href="webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>
<?php
include '../title.php'; // 包含標題檔案。
?>
<input type="button" onclick="history.back()" value="BACK(回到上一頁)">
<div align="center">
<table border="1" align="center" cellspacing="1" cellpadding="1">
<tr bgcolor="#CFC">
<td colspan='6'><div align='center'>Temperature & Humidity Sensor(溫溼度感測裝置)</div></td>
</tr>
<tr>
<td>Site ID(站台編號)</td>
<td>Site Name(站台名稱)</td>
<td>Site Address(站台地址)</td>
<td>MAC Address(網路卡號)</td>
<td>GPS</td>
<td><a href='devicelistadd.php'>Manage(管理)</a>/Chart Display(圖表顯示)</td>
</tr>
<?php
if (count($d00) > 0) {
    for ($i = count($d00) - 1; $i >= 0; $i = $i - 1) {
        $op2 = sprintf($op2a, $d04[$i]); // 使用 MAC 地址創建曲線圖連結
        $op3 = sprintf($op3a, $d04[$i]); // 使用 MAC 地址創建每日曲線圖連結
        // $op4 = sprintf($op4a, $d04[$i]); // 盒鬚圖連結（已註解）
        $op9 = sprintf($op9a, $d04[$i]); // 使用 MAC 地址創建量表顯示連結
        echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $d05[$i], $d06[$i], $op1, $op2, $op3, $op9); // 輸出表格行，包含資料和連結
    }
}
?>
</table>
</div>
</form>
<?php
include '../footer.php'; // 包含頁腳檔案。
?>
</body>
</html>