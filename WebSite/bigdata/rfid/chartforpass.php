<?php
// 引入資料庫連接和通用函數
include("../comlib.php");        // 通用函數庫
include("../Connections/iotcnn.php"); // 資料庫連接參數

// 建立與資料庫的連接
$link = Connection(); // Connection 是在 iotcnn.php 中定義的連接函數

// 確保 GET 參數中存在 "MAC"，如果沒有，則終止程式
if (!isset($_GET["KEY"])) {
    echo "Card Number lost <br>";
    die(); // 終止程式
}

// 獲取 MAC 地址參數，並移除首尾空格
$temp0 = trim($_GET["KEY"]);

// 如果 MAC 地址為空，則終止程式
if ($temp0 == "") {
    echo "Card Number is empty string <br>";
    die(); // 終止程式
}
 
// 表格標題，包含 MAC 地址
$tstr = "Logging Detail for Card Number %s(詳細每一筆資料)";

// 定義表格行的模板，用於顯示資料
$subrow = "<tr><td>%s</td><td>%s</td><td>%s</td><td>%4.2f</td></tr>";

// SQL 查詢字串，使用給定的 MAC 地址
$str = "select *, left(systime,10) as day ,count(*) as cnt from lockpass where cardnumber = '%s' group by left(systime,10),status order by systime asc ;";

// 產生真正SQL 查詢字串，格式化查詢字串，替換占位符
$qrystr = sprintf($str, $temp0);
//echo $qrystr."<br>";
// 建立空陣列，用於儲存查詢結果
$d01 = array(); // day
$d02 = array(); // IP
$d03 = array(); // status
$d04 = array(); // cnt
$d05 = array(); // cnt


// 執行 SQL 查詢，將結果存入 $result
$result = mysqli_query($link, $qrystr);

// 如果查詢成功，開始處理資料
if ($result !== FALSE) 
{
    // 逐行讀取資料，並將其存入對應的陣列
    while ($row = mysqli_fetch_array($result)) 
    {
        array_push($d01, $row["day"]);  // MAC 地址
        array_push($d02, $row["IP"]);   // IP 地址
        array_push($d03, rfidpassstatus((int)$row["status"])); // 更新時間
        array_push($d04, $row["cnt"]); // 測距裝置距離值
        array_push($d05, trandatetime2($row["systime"])); // 測距裝置距離值
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
    <title>Chart of Pass Logging for MAC(時段進出人數表)</title>
    <link href="../webcss.css" rel="stylesheet" type="text/css" /> <!-- 加載 CSS 樣式 -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/xlsx/dist/xlsx.full.min.js"></script>
</head>
<body>
    <!-- 包含標題內容 -->
    <?php include("../toptitle.php"); ?>
<form method="GET" action="passloglist.php">
    <!-- 隱藏欄位傳送 MAC 參數 -->
    <input type="hidden" name="MAC" value="<?php echo htmlspecialchars($temp0); ?>" />
    <div align="center">
        <button type="submit">回到裝置資料列表</button>
        ，<button type="button" onclick="exportTableToExcel('myTable', '門禁資料.xlsx')">匯出 Excel</button>,
        <button type="button" onclick="exportTableToCSV('pass_summary.csv')">匯出 CSV</button>
        ，<button type="button" onclick="exportChartAsImage1()">匯出登錄資料圖表為圖片</button>
    </div>
</form>      

    <!-- 建立居中對齊的表格 -->
    <div align="center">
        <table id="myTable" border="1" align="center" cellspacing="1" cellpadding="1">
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
                <td>DateTime(日期時間)</td>
                <td>IP Address(用戶端 IP)</td>
                <td>Status(狀態)</td>
                <td>Amount(數量)</td>
            </tr>

            <!-- 使用 PHP 迴圈生成表格內容 -->
            <?php 
            if (count($d01) > 0) { // 如果有資料
                for ($i = 0; $i < count($d01); $i++) { // 逐行顯示
                    // 使用 $subrow 模板和 sprintf 生成表格行
                    echo sprintf($subrow, $d05[$i], $d02[$i], $d03[$i], $d04[$i]);
                }
            }
            ?>
        </table>
    </div>
    <div style="width: 80%; margin: auto; padding-top: 20px;">
        <canvas id="chart1"></canvas>
    </div> 
    <!-- 包含頁腳內容 -->
    <?php include("../topfooter.php"); ?>
</body>
</html>
<!-- Chart.js 折線圖 -->
<script>
const ctx1 = document.getElementById('chart1').getContext('2d');
const chart1 = new Chart(ctx1, {
    type: 'line',
    data: {
        labels: <?php echo json_encode($d05); ?>,
        datasets: [
            {
                label: 'Access Amount(登錄次數)',
                data: <?php echo json_encode($d04); ?>,
                borderColor: 'rgba(75, 192, 192, 1)',
                backgroundColor: 'rgba(75, 192, 192, 0.2)',
                tension: 0.3
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            legend: { position: 'top' },
            title: { display: true, text: 'Pass Logging Chart for (<?php echo $temp0;?>)' }
        }
    }
});

function exportTableToExcel(tableID, filename = 'export.xlsx') {
    // 取得 HTML 表格元素
    var table = document.getElementById(tableID);

    // 使用 SheetJS 解析成工作簿物件
    var workbook = XLSX.utils.table_to_book(table, { sheet: "Sheet1" });

    // 產生下載檔案
    XLSX.writeFile(workbook, filename);
}
function exportTableToCSV(filename) {
    var csv = [];
    var rows = document.querySelectorAll("table tr");

    for (var i = 0; i < rows.length; i++) {
        var row = [], cols = rows[i].querySelectorAll("td, th");
        for (var j = 0; j < cols.length; j++) {
            // 如果欄位內含逗號，需加上引號處理
            var cellText = cols[j].innerText.replace(/"/g, '""'); // 轉義雙引號
            if (cellText.includes(",") || cellText.includes("\n")) {
                cellText = `"${cellText}"`;
            }
            row.push(cellText);
        }
        csv.push(row.join(","));
    }

    // 加上 UTF-8 BOM 頭（防止 Excel 中文亂碼）
    var csvContent = "\uFEFF" + csv.join("\n");

    var blob = new Blob([csvContent], { type: "text/csv;charset=utf-8;" });
    var downloadLink = document.createElement("a");
    downloadLink.download = filename;
    downloadLink.href = URL.createObjectURL(blob);
    downloadLink.style.display = "none";
    document.body.appendChild(downloadLink);
    downloadLink.click();
    document.body.removeChild(downloadLink);
}   
function exportChartAsImage1() {
    const chartCanvas = document.getElementById('chart1');
    const imageURL = chartCanvas.toDataURL("image/png");

    const downloadLink = document.createElement("a");
    downloadLink.href = imageURL;
    downloadLink.download = "chart1.jpg";
    document.body.appendChild(downloadLink);
    downloadLink.click();
    document.body.removeChild(downloadLink);
}
</script>