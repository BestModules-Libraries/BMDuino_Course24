<?php
// http://iot.arduino.org.tw:8888/bigdata/dhtdata/ShowChartlist.php?MAC=CC50E3B5BB20&dt1=20220101&dt2=20220201

include("../comlib.php");  // 引入資料庫的呼叫程式
include("../Connections/iotcnn.php");  // 引入資料庫的呼叫程式

$link = Connection();  // 產生 MySQL 連線物件

$mid = $_GET["MAC"];  // 取得 GET 參數: MAC 地址
$dd1 = $_GET["dt1"];  // 取得 GET 參數: 起始日期
$dd2 = $_GET["dt2"];  // 取得 GET 參數: 結束日期

// SQL 查詢語句，用於從資料庫中查找符合條件的資料
$qry = "select * from fcu.dhtdata where MAC = '%s' and systime >= '%s' and systime <= '%s' order by systime desc";
$qrystr = sprintf($qry, $mid, $dd1, $dd2);  // 格式化 SQL 查詢語句

$d00 = array();  // 宣告空陣列，用於儲存時間數據
$d01 = array();  // 宣告空陣列，用於儲存溫度數據
$d02 = array();  // 宣告空陣列，用於儲存濕度數據

$result = mysqli_query($link, $qrystr);  // 執行 SQL 查詢語句，獲取查詢結果

if ($result !== FALSE) {  // 如果查詢結果不為 FALSE
    while ($row = mysqli_fetch_array($result)) {  // 迭代查詢結果的每一行
        array_push($d00, trandatetime3($row["systime"]));  // 將時間數據加入陣列
        array_push($d01, (double)$row["temperature"]);  // 將溫度數據加入陣列
        array_push($d02, (double)$row["humidity"]);  // 將濕度數據加入陣列
    }
}

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Display Temperature and Humidity Curve Chart by MAC</title>
<link href="../webcss.css" rel="stylesheet" type="text/css" />
<script src="../code/highcharts.js"></script>
<script src="../code/modules/data.js"></script>
<script src="../code/modules/exporting.js"></script>
<script src="../code/modules/export-data.js"></script>
<script src="../code/modules/accessibility.js"></script>
</head>
<body>

<?php
include '../title.php';  // 引入標題檔案
?>

<div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>
<div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>

<script type="text/javascript">
// ----------- 顯示圖表的 JavaScript 代碼 --------------

Highcharts.getJSON(
    '<?php echo sprintf("http://140.134.25.43:8088/dhtdata/GetChartData.php?MAC=%s&dt1=%s&dt2=%s", $mid, $dd1, $dd2);?>',
    function (data) {

        Highcharts.chart('container1', {
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'USD to EUR exchange rate over time'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                    'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                type: 'datetime'
            },
            yAxis: {
                title: {
                    text: 'Exchange rate'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 0,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                        radius: 2
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'USD to EUR',
                data: data
            }]
        });
    }
);
// --------- 顏色溫度圖表的 JavaScript 代碼 ----------------
Highcharts.getJSON(
    'https://cdn.jsdelivr.net/gh/highcharts/highcharts@v7.0.0/samples/data/usdeur.json',
    function (data) {

        Highcharts.chart('container2', {
            chart: {
                zoomType: 'x'
            },
            title: {
                text: 'USD to EUR exchange rate over time'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                    'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                type: 'datetime'
            },
            yAxis: {
                title: {
                    text: 'Exchange rate'
                }
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 0,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                        radius: 2
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                }
            },

            series: [{
                type: 'area',
                name: 'USD to EUR',
                data: data
            }]
        });
    }
);

</script>

<?php
include '../footer.php';  // 引入底部檔案
?>      
</body>
</html>
