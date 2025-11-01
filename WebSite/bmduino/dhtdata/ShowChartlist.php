<?php
    // 引入共用函式與資料庫連線設定檔
    include("../comlib.php");              // 自訂通用函式庫（如日期轉換等工具）
    include("../Connections/iotcnn.php");  // 資料庫連線設定

    // 建立 MySQL 資料庫連線
    $link = Connection();

    // 判斷 MAC 資料是來自 GET 還是 POST，並取得 MAC 位址
    if (!isset($_GET["MAC"])) {
        $mid = $_POST["MAC"];
    } else {
        $mid = $_GET["MAC"];
    }

    // 判斷是否有開始時間，若無則取預設值（如 3 天前）
    if (!isset($_POST["dt1"])) {
        $sd1 = getshiftdataorder(-3 * 24);  // 自訂函式：取得 N 小時前的時間
        $dd1 = $sd1;
    } else {
        $dd1 = $_POST["dt1"];
        $sd1 = $_POST["dt1"];
    }

    // 判斷是否有結束時間，若無則取現在時間
    if (!isset($_POST["dt2"])) {
        $sd2 = getdataorder();  // 自訂函式：取得目前時間
        $dd2 = $sd2;
    } else {
        $dd2 = $_POST["dt2"];
        $sd2 = $_POST["dt2"];
    }

    // SQL 查詢語句樣板：選出指定 MAC 的資料，限制時間區間
    $qry = "SELECT * FROM big.dhtdata WHERE MAC = '%s' AND systime >= '%s' AND systime <= '%s' ORDER BY systime ASC";
    $qrystr = sprintf($qry, $mid, $dd1, $dd2);  // 使用 sprintf 格式化為完整查詢字串

    // 宣告陣列以儲存查詢結果
    $d00 = array();  // 時間（格式化）
    $d00a = array(); // 原始時間
    $d01 = array();  // 溫度
    $d02 = array();  // 濕度
    $d03 = array();  // MAC（冗餘）

    // 執行 SQL 查詢
    $result = mysqli_query($link, $qrystr);
    if ($result !== FALSE) {
        while ($row = mysqli_fetch_array($result)) {
            array_push($d00, trandatetime3($row["systime"]));  // 時間轉換顯示用
            array_push($d00a, $row["systime"]);  // 原始時間存檔用
            array_push($d01, (double)sprintf("%8.2f", (double)$row["temperature"]));  // 溫度格式化
            array_push($d02, (double)sprintf("%8.2f", (double)$row["humidity"]));     // 濕度格式化
            array_push($d03, $row["MAC"]);
        }
    }

    // 清除查詢結果資源與關閉連線
    mysqli_free_result($result);
    mysqli_close($link);

    // 寫入 CSV 檔案
    $myfile = fopen("../tmp/dhtdata.csv", "w");  // 開啟新檔案以寫入（會覆蓋）

    // 設定標題列與資料列格式
    $datah = "'%s', '%s', '%s'\n";  // CSV 標題格式
    $datar = "'%s', %10.3f, %5.2f\n";  // 資料列格式：時間、溫度、濕度

    // 寫入 CSV 標題
    $datarow = sprintf($datah, "DateTime", "Temperature", "Humidity");
    fwrite($myfile, $datarow);

    // 寫入每一筆資料到 CSV 檔案中
    for ($i = 0; $i < count($d00a); $i++) {
        $datarow = sprintf($datar, $d00a[$i], $d01[$i], $d02[$i]);  // 格式化資料
        fwrite($myfile, $datarow);
    }

    fclose($myfile);  // 關閉檔案
?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" ...>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta charset="utf-8" />
    <title>Display Temperature and Humidity Curve Chart by MAC</title>
    <link href="../webcss.css" rel="stylesheet" />

    <!-- 引入 Highcharts 所需模組 -->
    <script src="../code/highcharts.js"></script>
    <script src="../code/highcharts-more.js"></script>
    <script src="../code/modules/exporting.js"></script>
    <script src="../code/modules/export-data.js"></script>
    <script src="../code/modules/accessibility.js"></script>
</head>

<body>
<?php include("../toptitle.php"); ?>  <!-- 網頁頂部標題 -->

<!-- 查詢條件表單 -->
<form id="form1" method="post" action="">
    <table border="1" width="100%">
        <tr>
            <td colspan="5" align="center">Temperature & Humidity Sensor(溫濕度感測裝置)</td>
        </tr>
        <tr>
            <td>開始日期時間(YYYYMMDDHHMMSS)</td>
            <td>
                <input name="MAC" type="hidden" value="<?php echo $mid; ?>" />
                <input type="text" name="dt1" size="14" maxlength="14" value="<?php echo $sd1; ?>" />
            </td>
            <td>結束日期時間(YYYYMMDDHHMMSS)</td>
            <td>
                <input type="text" name="dt2" size="14" maxlength="14" value="<?php echo $sd2; ?>" />
            </td>
            <td>
                <input type="submit" value="送出" />
                <a href="/tmp/dhtdata.csv">下載</a>  <!-- 提供 CSV 下載 -->
            </td>
        </tr>
    </table>
</form>


    <!-- 容器，用於顯示 Highcharts 的溫度曲線圖 -->
    <div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>

    <!-- 容器，用於顯示 Highcharts 的濕度曲線圖 -->
    <div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>

    <!-- JavaScript，用於繪製圖表 -->
<!-- 溫度圖表 -->
<script type="text/javascript">
Highcharts.chart('container1', {
    chart: { zoomType: 'x' },
    title: { text: 'Temperature °C by MAC:<?php echo $mid ?>' },
    xAxis: {
        categories: <?php echo json_encode($d00, JSON_UNESCAPED_UNICODE); ?>
    },
    yAxis: {
        title: { text: '°C' }
    },
    legend: { enabled: false },
    plotOptions: {
        area: {
            fillColor: { linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 } },
            marker: { radius: 2 },
            lineWidth: 0.1,
            states: { hover: { lineWidth: 1 } },
            threshold: null
        }
    },
    series: [{
        name: 'Temperature',
        data: <?php echo json_encode($d01, JSON_UNESCAPED_UNICODE); ?>
    }]
});
</script>

<!-- 濕度圖表 -->
<script type="text/javascript">
Highcharts.chart('container2', {
    chart: { zoomType: 'x' },
    title: { text: 'Humidity Curve Chart by MAC:<?php echo $mid ?>' },
    xAxis: {
        categories: <?php echo json_encode($d00, JSON_UNESCAPED_UNICODE); ?>
    },
    yAxis: {
        title: { text: 'Percent(%)' }
    },
    legend: { enabled: false },
    plotOptions: {
        area: {
            fillColor: { linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 } },
            marker: { radius: 2 },
            lineWidth: 0.1,
            states: { hover: { lineWidth: 1 } },
            threshold: null
        }
    },
    series: [{
        name: 'Percent(%)',
        data: <?php echo json_encode($d02, JSON_UNESCAPED_UNICODE); ?>
    }]
});
</script>


    <?php
        // 包含頁面底部的共用內容
        include("../topfooter.php");  // 引入頁面底部
    ?> 
</body>
</html>
