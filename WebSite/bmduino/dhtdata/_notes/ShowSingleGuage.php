<?php
    // 包含資料庫的呼叫程式
    include("../comlib.php");
    include("../Connections/iotcnn.php");

    // 產生mySQL連線物件
    $link = Connection();

    // 取得GET參數 : mac address
    $mid = $_GET["mac"];

    // 建立查詢字串，從 big.dhtdata 表格中選取指定 MAC 地址的最新一筆資料
    $qrystr = sprintf("SELECT * FROM big.dhtdata where mac = '%s' order by systime desc limit 1 ", $mid);

    // 宣告空陣列
    $d00 = array();
    $d01 = array();
    $d02 = array();
    $d03 = array();

    // 執行查詢並取得結果
    $result = mysqli_query($link, $qrystr);

    // 如果查詢結果不為空，將資料存入陣列
    if ($result !== FALSE) {
        while ($row = mysqli_fetch_array($result)) {
            // 將每一列的資料存入對應的陣列中
            array_push($d00, $row["systime"]);
            array_push($d01, $row["temperature"]);
            array_push($d02, $row["humidity"]);
            array_push($d03, $row["mac"]);
        }
    }

    // 釋放查詢結果
    mysqli_free_result($result);
    // 關閉連線
    mysqli_close($link);
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Display Temperature and Humidity Data as Guage by MAC</title>
<link href="../webcss.css" rel="stylesheet" type="text/css" />

<!-- 引入Highcharts相關的JavaScript檔案 -->
<script src="../code/highcharts.js"></script>
<script src="../code/highcharts-more.js"></script>
<script src="../code/modules/exporting.js"></script>
<script src="../code/modules/export-data.js"></script>
<script src="../code/modules/accessibility.js"></script>

</head>
<body>
<?php
include '../title.php';
?>
<!-- 返回上一頁的按鈕 -->
<input type="button" onclick="history.back()" value="BACK(回到上一頁)"></input>

<!-- 顯示溫度計圖表的容器 -->
<div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>
<!-- 顯示濕度計圖表的容器 -->
<div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>

<?php
include '../footer.php';
?>

<script type="text/javascript">
// 繪製溫度計圖表
Highcharts.chart('container1', {
    chart: {
        type: 'gauge', // 設定圖表類型為儀表盤
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },
    title: {
        text: 'Temperature by MAC:<? echo $mid?> at <? echo trandatetime4($d00[0])?>',
    },
    pane: {
        startAngle: -120,
        endAngle: 120,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },
    yAxis: {
        min: -10, // 設定Y軸最小值
        max: 50, // 設定Y軸最大值
        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',
        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 1,
            rotation: 'auto'
        },
        title: {
            text: '°C(Celsius)'
        },
        plotBands: [{
            from: -10,
            to: 22,
            color: '#DDDF0D' // 黃色
        }, {
            from: 22,
            to: 30,
            color: '#55BF3B' // 綠色
        }, {
            from: 30,
            to: 50,
            color: '#DF5353' // 紅色
        }]
    },
    series: [{
        name: 'Celsius',
        data: [
            <?php
            for ($i = count($d00) - 1; $i >= 0; $i--) {
                echo $d01[$i];
                echo ",";
            }
            ?>
        ],
        tooltip: {
            valueSuffix: ' °C'
        }
    }]
});

// 繪製濕度計圖表
Highcharts.chart('container2', {
    chart: {
        type: 'gauge', // 設定圖表類型為儀表盤
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false
    },
    title: {
        text: 'Humidity by MAC:<? echo $mid?> at <? echo trandatetime4($d00[0])?>',
    },
    pane: {
        startAngle: -150,
        endAngle: 150,
        background: [{
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#FFF'],
                    [1, '#333']
                ]
            },
            borderWidth: 0,
            outerRadius: '109%'
        }, {
            backgroundColor: {
                linearGradient: { x1: 0, y1: 0, x2: 0, y2: 1 },
                stops: [
                    [0, '#333'],
                    [1, '#FFF']
                ]
            },
            borderWidth: 1,
            outerRadius: '107%'
        }, {
            // default background
        }, {
            backgroundColor: '#DDD',
            borderWidth: 0,
            outerRadius: '105%',
            innerRadius: '103%'
        }]
    },
    yAxis: {
        min: 0, // 設定Y軸最小值
        max: 100, // 設定Y軸最大值
        minorTickInterval: 'auto',
        minorTickWidth: 1,
        minorTickLength: 10,
        minorTickPosition: 'inside',
        minorTickColor: '#666',
        tickPixelInterval: 30,
        tickWidth: 2,
        tickPosition: 'inside',
        tickLength: 10,
        tickColor: '#666',
        labels: {
            step: 1,
            rotation: 'auto'
        },
        title: {
            text: '%(Percent)'
        },
        plotBands: [{
            from: 0,
            to: 50,
            color: '#55BF3B' // 綠色
        }, {
            from: 50,
            to: 85,
            color: '#DDDF0D' // 黃色
        }, {
            from: 85,
            to: 100,
            color: '#DF5353' // 紅色
        }]
    },
    series: [{
        name: 'humidity',
        data: [
            <?php
            for ($i = count($d00) - 1; $i >= 0; $i--) {
                echo $d02[$i];
                echo ",";
            }
            ?>
        ],
        tooltip: {
            valueSuffix: ' %'
        }
    }]
});
</script>

</body>
</html>
