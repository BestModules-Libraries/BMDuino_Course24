<?php
/*
這是一個 PHP 網頁系統，
 用於顯示特定 MAC 地址（裝置）所上傳的 血氧與心跳紀錄資料，
 並以表格方式呈現在網頁上。

流程概述：
確認網址是否有提供 MAC 參數。
根據該 MAC 參數從資料庫查詢 bloodoxygen 表中的資料（含時間、IP、血氧值、心跳值）。
使用 HTML 表格動態產出結果頁面顯示給使用者。 
 */



// 引入資料庫連線與通用函數檔案
include("../comlib.php");        // 載入共用函數庫，內含可能會用到的工具函數
include("../Connections/iotcnn.php"); // 載入資料庫連線設定檔

// 建立與資料庫的連線，Connection() 是 iotcnn.php 中定義的函數
$link = Connection();

// 檢查是否從網址參數中提供了 MAC（裝置位址）
if (!isset($_GET["MAC"])) {
    echo "MAC address lost <br>"; // 若無提供 MAC，顯示錯誤訊息
    die(); // 停止程式執行
}

// 取得網址中 GET 參數的 MAC 值，並移除前後空白
$temp0 = trim($_GET["MAC"]);

// 如果 MAC 是空字串，也停止執行
if ($temp0 == "") {
    echo "MAC Address is empty string <br>";
    die();
}

$act = "listforMAC.php?MAC=%s" ;
$actstr = sprintf($act,$temp0) ;
// 建立表格標題顯示字串，含 MAC
$tstr = "Blood Oxygen Device for MAC %s(血氧測量裝置)";

// 定義表格每一列的格式（時間、IP、血氧、心跳）
$subrow = "<tr><td>%s</td><td>%s</td><td>%s</td><td>%4.2f</td></tr>";

// 建立 SQL 查詢語法，根據 MAC 值查詢 bloodoxygen 資料表，並依時間降冪排序
$str = "select * from bloodoxygen where MAC = '%s' order by systime desc";

// 使用 sprintf 將 MAC 帶入 SQL 字串中
$qrystr = sprintf($str, $temp0);

// 建立陣列用來存放資料庫查詢的欄位值
$d01 = array(); // 資料時間
$d01a = array(); // 資料時間
$d02 = array(); // IP 位址
$d03 = array(); // 血氧值
$d04 = array(); // 心跳值

// 執行查詢，取得查詢結果集
$result = mysqli_query($link, $qrystr);

// 如果查詢成功
if ($result !== FALSE) {
    // 逐筆取出查詢結果，分別存入對應陣列
    while ($row = mysqli_fetch_array($result)) {
        array_push($d01, $row["systime"]);
        array_push($d01a, trandatetime($row["systime"]));
        
        array_push($d02, $row["IP"]);
        array_push($d03, $row["blood"]);
        array_push($d04, $row["heartbeat"]);
    }
}

// 若結果是 mysqli_result 的物件型態，釋放資源（雖然這通常用在 SELECT 查詢）
if ($result instanceof mysqli_result) {
    mysqli_free_result($result);
}

// 關閉與資料庫的連線
mysqli_close($link);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 設定編碼與網頁標題 -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Blood Oxygen Device List(血氧測量裝置)</title>
    <!-- 套用外部樣式表，改善版面設計 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/xlsx/dist/xlsx.full.min.js"></script>
</head>
<body>
    <!-- 引入網頁頂端標題欄 -->
    <?php include("../toptitle.php"); ?>
<form method="GET" action="devicelist.php">
    <!-- 隱藏欄位傳送 MAC 參數 -->
    <input type="hidden" name="MAC" value="<?php echo htmlspecialchars($temp0); ?>" />
    <div align="center">
        <button type="submit">回到裝置資料列表</button>
        ，<button type="button" onclick="exportTableToExcel('myTable', '血氧資料.xlsx')">匯出 Excel</button>,
        <button type="button" onclick="exportTableToCSV('bloodoxygen_summary.csv')">匯出 CSV</button>
        ，<button type="button" onclick="exportChartAsImage1()">匯出血氧資料圖表為圖片</button>
        ，<button type="button" onclick="exportChartAsImage2()">匯出心跳資料圖表為圖片</button>
    </div>
</form>      
    <!-- 建立中央對齊的表格來顯示結果 -->
    <div align="center">
        <table id="myTable" border="1" align="center" cellspacing="1" cellpadding="1">
            <!-- 表格標題列 -->
            <tr bgcolor="#CFC">
                <td colspan="5">
                    <div align="center">
                        <?php echo sprintf($tstr, $temp0); ?> <!-- 插入 MAC 標題 -->
                    </div>
                </td>
            </tr>
            <!-- 表格欄位名稱列 -->
            <tr>
                <td>DateTime(資料時間)</td>
                <td>IP Address(用戶端 IP)</td>
                <td>Blood Oxygen(血氧值)</td>
                <td>Heart Beats(心跳值)</td>
            </tr>

            <!-- 用 PHP 動態輸出每一列的資料 -->
            <?php 
            if (count($d01) > 0) { // 有資料才顯示
                for ($i = 0; $i < count($d01); $i++) {
                    echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i]);
                }
            }
            ?>
        </table>
    </div>
    <div style="width: 80%; margin: auto; padding-top: 20px;">
        <canvas id="chart1"></canvas>
    </div> 
    <div style="width: 80%; margin: auto; padding-top: 20px;">
    <canvas id="chart2"></canvas>
    </div> 
    <!-- 引入頁腳 -->
    <?php include("../topfooter.php"); ?>
</body>
</html>
<!-- Chart.js 折線圖 -->
<script>
const ctx1 = document.getElementById('chart1').getContext('2d');
const chart1 = new Chart(ctx1, {
    type: 'line',
    data: {
        labels: <?php echo json_encode($d01a); ?>,
        datasets: [
            {
                label: '血氧',
                data: <?php echo json_encode($d03); ?>,
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
            title: { display: true, text: '裝置血氧值(<?php echo $temp0;?>)' }
        }
    }
});
const ctx2 = document.getElementById('chart2').getContext('2d');
const chart2 = new Chart(ctx2, {
    type: 'line',
    data: {
        labels: <?php echo json_encode($d01a); ?>,
        datasets: [
             {
                label: '心跳',
                data: <?php echo json_encode($d04); ?>,
                borderColor: 'rgba(255, 99, 132, 1)',
                backgroundColor: 'rgba(255, 99, 132, 0.2)',
                tension: 0.3
            }
        ]
    },
    options: {
        responsive: true,
        plugins: {
            legend: { position: 'top' },
            title: { display: true, text: '裝置心跳值(<?php echo $temp0;?>)' }
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
function exportChartAsImage2() {
    const chartCanvas = document.getElementById('chart2');
    const imageURL = chartCanvas.toDataURL("image/png");

    const downloadLink = document.createElement("a");
    downloadLink.href = imageURL;
    downloadLink.download = "chart2.jpg";
    document.body.appendChild(downloadLink);
    downloadLink.click();
    document.body.removeChild(downloadLink);
}
</script>