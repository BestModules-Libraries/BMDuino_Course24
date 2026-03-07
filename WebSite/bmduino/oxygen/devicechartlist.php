<?php
session_start();
include("../comlib.php");
include("../Connections/iotcnn.php");
$link = Connection();

$subrow = "<tr><td>%s</td><td>%6.2f</td><td>%6.2f</td><td>%d</td><td>%s</td><td>%s</td></tr>";
$op1 = "<a href='listforMAC.php?MAC=%s&start=%s&end=%s'>詳細資料</a>";

$startDate = isset($_GET['start']) ? $_GET['start'] : date("Y-m-01");
$endDate = isset($_GET['end']) ? $_GET['end'] : date("Y-m-d");

$qry = "select MAC, AVG(blood) as bd, AVG(heartbeat) as hb, COUNT(*) as cnt, MAX(systime) as maxtime 
        FROM bloodoxygen 
        WHERE systime BETWEEN '%s' AND '%s' group by MAC order by maxtime DESC;";

$qrystr = sprintf($qry, TranDate($startDate)."000000", TranDate($endDate)."235959");

$d01 = $d02 = $d03 = $d04 = $d05 = array();
$result = mysqli_query($link, $qrystr);

if ($result !== FALSE) {
    while ($row = mysqli_fetch_array($result)) {
        array_push($d01, $row["MAC"]);
        array_push($d02, $row["bd"]);
        array_push($d03, $row["hb"]);
        array_push($d04, $row["cnt"]);
        array_push($d05, $row["maxtime"]);
    }
}
if ($result instanceof mysqli_result) mysqli_free_result($result);
mysqli_close($link);
?>
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <title>血氧測量裝置</title>
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
<?php include("../toptitle.php"); ?>
<div align="center">
    <form method="GET" action="">
        起始日期: <input type="date" name="start" value="<?php echo $startDate; ?>">
        結束日期: <input type="date" name="end" value="<?php echo $endDate; ?>">
        <input type="submit" value="篩選">
    </form>
    <br />
    <button onclick="exportTableToCSV('bloodoxygen_summary.csv')">匯出 CSV</button>

    <table border="1" cellspacing="1" cellpadding="1">
        <tr bgcolor="#CFC">
            <td colspan='6'><div align='center'>血氧測量裝置統計資料</div></td>
        </tr>
        <tr>
            <td>MAC 地址</td>
            <td>平均血氧</td>
            <td>平均心跳</td>
            <td>筆數</td>
            <td>查詢明細</td>
            <td>上線狀態</td>
        </tr>
        <?php 
        $now = strtotime("now");
        for ($i = 0; $i < count($d01); $i++) {
            $online = ((strtotime($d05[$i]) + 60) > $now) ? "<span style='color:green;'>上線</span>" : "<span style='color:red;'>離線</span>";
            $op1a = sprintf($op1, $d01[$i], $startDate, $endDate);
            echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $op1a, $online);
        }
        ?>
    </table>
    
    <div style="width: 80%; margin: auto; padding-top: 20px;">
        <canvas id="summaryChart"></canvas>
    </div>   
</div>

<!-- 匯出 CSV 的 JavaScript -->
<script>
function exportTableToCSV(filename) {
    var csv = [];
    var rows = document.querySelectorAll("table tr");
    for (var i = 0; i < rows.length; i++) {
        var row = [], cols = rows[i].querySelectorAll("td, th");
        for (var j = 0; j < cols.length; j++) row.push(cols[j].innerText);
        csv.push(row.join(","));
    }
    var csvFile = new Blob([csv.join("\n")], { type: "text/csv" });
    var downloadLink = document.createElement("a");
    downloadLink.download = filename;
    downloadLink.href = window.URL.createObjectURL(csvFile);
    downloadLink.style.display = "none";
    document.body.appendChild(downloadLink);
    downloadLink.click();
}
</script>

<!-- Chart.js 折線圖 -->
<script>
const ctx = document.getElementById('summaryChart').getContext('2d');
const chart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: <?php echo json_encode($d01); ?>,
        datasets: [
            {
                label: '平均血氧',
                data: <?php echo json_encode($d02); ?>,
                borderColor: 'rgba(75, 192, 192, 1)',
                backgroundColor: 'rgba(75, 192, 192, 0.2)',
                tension: 0.3
            },
            {
                label: '平均心跳',
                data: <?php echo json_encode($d03); ?>,
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
            title: { display: true, text: '各裝置 MAC 血氧與心跳平均值' }
        }
    }
});
</script>

<?php include("../topfooter.php"); ?>
</body>
</html>
