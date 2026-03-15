<?php
    // http://iot.arduino.org.tw:8888/bigdata/dhtdata/ShowChartlistday.php?MAC=CC50E3B5BB20
    
    include("../comlib.php"); // 使用資料庫的呼叫程式
    include("../Connections/iotcnn.php"); // 使用資料庫的呼叫程式

    $link = Connection(); // 產生 MySQL 連線物件

    // $mid = $_GET["mac"]; // 取得 GET 參數 : mac address
    if (!isset($_GET["MAC"])) {
        $mid = $_POST["MAC"]; // 取得 POST 參數 : MAC address
    } else {
        $mid = $_GET["MAC"]; // 取得 GET 參數 : MAC address
    }

    // 檢查是否有提供開始日期，若無則設為7天前
    if (!isset($_POST["dt1"])) {
        $sd1 = getshiftdayorder(-7);
        $dd1 = $sd1;
    } else {
        $dd1 = $_POST["dt1"];
        $sd1 = $_POST["dt1"];
    }

    // 檢查是否有提供結束日期，若無則設為今天
    if (!isset($_POST["dt2"])) {
        $sd2 = getdayorder();
        $dd2 = $sd2;
    } else {
        $dd2 = $_POST["dt2"];
        $sd2 = $_POST["dt2"];
    }

    // 查詢語句：從 big.dhtdata 表中查詢資料，並按日期分組
    $qry = "select MAC, left(systime,8) as systime, avg(temperature) as temp, avg(temperature) as humid from big.dhtdata where MAC = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime desc";
    $qrystr = sprintf($qry, $mid, $dd1, $dd2); // 格式化查詢語句

    // 宣告空陣列以存放查詢結果
    $d00 = array();
    $d01 = array();
    $d02 = array();
    $d03 = array();

    $result = mysqli_query($link ,$qrystr ); // 執行查詢

    if ($result !== FALSE) {
        while ($row = mysqli_fetch_array($result)) {
            array_push($d00, $row["systime"]); // 將系統時間加入 d00 陣列
            array_push($d01, $row["temp"]); // 將溫度加入 d01 陣列
            array_push($d02, $row["humid"]); // 將濕度加入 d02 陣列
            array_push($d03, $row["MAC"]); // 將 MAC 地址加入 d03 陣列
        }
    }

    mysqli_free_result($result); // 釋放查詢結果
    mysqli_close($link); // 關閉資料庫連線

    $myfile = fopen("../tmp/dhtdata.csv", "w"); // 打開/創建 CSV 文件以寫入
    $datar = "%s , %10.3f , %5.2f \n";
    for ($i = count($d00) - 1; $i >= 0; $i--) {
        $datarow = sprintf($datar, $d00[$i], $d01[$i], $d02[$i]);
        fwrite($myfile, $datarow); // 將資料寫入 CSV 文件
    }

    fclose($myfile); // 關閉 CSV 文件
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Display Temperature and Humidity Data as Curve Chart by MAC</title>
<link href="../webcss.css" rel="stylesheet" type="text/css" />

    <script src="../code/highcharts.js"></script>
    <script src="../code/highcharts-more.js"></script>
    <script src="../code/modules/exporting.js"></script>
    <script src="../code/modules/export-data.js"></script>
    <script src="../code/modules/accessibility.js"></script>    
</head>
<body>
<?php include '../title.php'; ?>
<input type="button" onclick="history.back()" value="BACK(回到上一頁)">
</input>
<form id="form1" name="form1" method="post" action="">
  <table width="100%" border="1">
    <tr>
      <td width="220">開始日期時間(YYYYMMDD)
         <input name="MAC" type="hidden" id="MAC" value="<?php echo $mid ?>" />
      </td>
      <td width="200">
        <label for="strdt"></label>
        <input type="text" name="dt1" id="dt1" size="8" maxlength="8" value=<?php echo $sd1; ?> />
      </td>
      <td width="220">結束日期時間(YYYYMMDD)</td>
      <td width="200">
        <input type="text" name="dt2" id="dt2" size="8" maxlength="8" value=<?php echo $sd2; ?> />
      </td>
      <td width="200">
        <input type="submit" name="button" id="button" value="送出" /><a href="/tmp/dhtdata.csv">下載</a>
      </td>
    </tr>
  </table>
</form>
<div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>
<div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>

<script type="text/javascript">
// 繪製溫度圖表
Highcharts.chart('container1', {
    chart: {
        type: 'spline'
    },
    title: {
        text: 'Temperature °C by MAC:<?php echo $mid ?>',
        fontsize: 30
    },
    subtitle: {
        text: ''
    },
    xAxis: {
        categories: [
        <?php
            for ($i = count($d00) - 1; $i >= 0; $i--) {
                echo "'";
                echo trandate($d00[$i]);
                echo "',";
            }
        ?>
        ]
    },
    yAxis: {
        title: {
            text: '°C'
        }
    },
    plotOptions: {
        line: {
            dataLabels: {
                enabled: true
            },
            enableMouseTracking: false
        }
    },
    series: [{
        name: 'Temperature',
        data: [
        <?php
            for ($i = count($d00) - 1; $i >= 0; $i--) {
                echo $d01[$i];
                echo ",";
            }
        ?>
        ]
    }]
});

// 繪製濕度圖表
Highcharts.chart('container2', {
    chart: {
        type: 'spline'
    },
    title: {
        text: 'Humidity Curve Chart by MAC:<?php echo $mid ?>',
        fontsize: 30
    },
    subtitle: {
        text: ''
    },
    xAxis: {
        categories: [
        <?php
            for ($i = 0; $i < count($d00); $i++) {
                echo "'";
                echo trandate($d00[$i]);
                echo "',";
            }
        ?>
        ]
    },
    yAxis: {
        title: {
            text: 'Percent(%)'
        }
    },
    plotOptions: {
        line: {
            dataLabels: {
                enabled: true
            },
            enableMouseTracking: false
        }
    },
    series: [{
        name: 'Percent',
        data: [
        <?php
            for ($i = 0; $i < count($d02); $i++) {
                echo $d02[$i];
                echo ",";
            }
        ?>
        ]
    }]
});
</script>

<div align="center">
    <?php echo "Data Generated by Device's MAC:".$mid ?>;
    <table border="1" cellspacing="1" cellpadding="1">
        <tr>
            <td>Date Time</td>
            <td>Temperature(Celsius °C)</td>
            <td>Humidity(Percent %)</td>
        </tr>
        <?php 
            if (count($d00) > 0) {
                for ($i = count($d00) - 1; $i >= 0; $i--) {
                    echo sprintf("<tr><td>%s</td><td>%4.1f</td><td>%3.1f</td></tr>", 
                        trandatetime4($d00[$i]), $d01[$i], $d02[$i]);
                }
            }
        ?>
    </table>
</div>

<?php include '../footer.php'; ?>
</body>
</html>
