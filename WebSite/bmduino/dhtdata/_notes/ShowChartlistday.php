<?php
/*
 這段 PHP/HTML 程式的主要功能是顯示指定 MAC 地址及
 * 日期範圍內的溫濕度數據並以圖表形式展示
 * 
 */
// http://iot.arduino.org.tw:8888/bigdata/dhtdata/ShowChartlistday.php?MAC=E89F6DE8F3BC&dt1=20200101&dt2=20221231	
include("../comlib.php");  // 使用資料庫的呼叫程式
include("../Connections/iotcnn.php");  // 使用資料庫的呼叫程式

$link = Connection();  // 產生 MySQL 連線物件

// 檢查是否有 MAC 參數，如果沒有就從 POST 參數中取得
if (!isset($_GET["MAC"])) {
    $mid = $_POST["MAC"];  // 取得 POST 參數: MAC 地址
} else {
    $mid = $_GET["MAC"];  // 取得 GET 參數: MAC 地址
}

// 檢查是否有 dt1 參數，如果沒有就設為 7 天前的日期
if (!isset($_POST["dt1"])) {
    $sd1 = getshiftdayorder(-7); 
    $dd1 = $sd1;	
} else {
    $dd1 = $_POST["dt1"];	
    $sd1 = $_POST["dt1"];
}

// 檢查是否有 dt2 參數，如果沒有就設為當前日期
if (!isset($_POST["dt2"])) {
    $sd2 = getdayorder();    
    $dd2 = $sd2;	
} else {
    $dd2 = $_POST["dt2"];	
    $sd2 = $_POST["dt2"];
}

// SQL 查詢語句，用於從資料庫中查找符合條件的資料
$qry = "select MAC, left(systime,8) as systime, avg(temperature) as temperature, avg(humidity) as humidity from big.dhtdata where MAC = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc";
$qrystr = sprintf($qry, $mid, $dd1, $dd2);  // 格式化 SQL 查詢語句

$d00 = array();  // 宣告空陣列，用於儲存日期數據
$d00a = array();  // 宣告空陣列，用於儲存原始日期數據
$d01 = array();  // 宣告空陣列，用於儲存溫度數據
$d02 = array();  // 宣告空陣列，用於儲存濕度數據
$d03 = array();  // 宣告空陣列，用於儲存 MAC 地址數據

$result = mysqli_query($link, $qrystr);  // 執行 SQL 查詢語句，獲取查詢結果

if ($result !== FALSE) {  // 如果查詢結果不為 FALSE
    while ($row = mysqli_fetch_array($result)) {  // 迭代查詢結果的每一行
        array_push($d00, trandate($row["systime"]));  // 將日期數據加入陣列
        array_push($d00a, $row["systime"]);  // 將原始日期數據加入陣列
        array_push($d01, (double)sprintf("%8.2f", (double)$row["temperature"]));  // 將溫度數據加入陣列
        array_push($d02, (double)sprintf("%8.2f", (double)$row["humidity"]));  // 將濕度數據加入陣列
        array_push($d03, $row["MAC"]);  // 將 MAC 地址數據加入陣列
    }
}

mysqli_free_result($result);  // 關閉資料集
mysqli_close($link);  // 關閉連線

/*
$myfile = fopen("../tmp/dhtdata.csv", "w");
$datah = "'%s' , '%s' , '%s' \n";
$datar = "'%s' , %10.3f , %5.2f \n";
$datarow = sprintf($datah,"DateTime",'Temperature','Humidity');
fwrite($myfile, $datarow);
for($i=0 ; $i < count($d00a); $i=$i+1) {
    $datarow = sprintf($datar,$d00a[$i],$d01[$i],$d02[$i]);
    fwrite($myfile, $datarow);
}
fclose($myfile);
*/
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Display Temperature and Humidity Data as Curve Chart by MAC</title>
<link href="webcss.css" rel="stylesheet" type="text/css" />

<script src="../code/highcharts.js"></script>
<script src="../code/highcharts-more.js"></script>
<script src="../code/modules/exporting.js"></script>
<script src="../code/modules/export-data.js"></script>
<script src="../code/modules/accessibility.js"></script>	
</head>
<body>
<?php
include '../toptitle.php';  // 引入標題檔案
?>

<form id="form1" name="form1" method="post" action="">
  <table width="100%" border="1">
	<tr bgcolor=#CFC >
		<td colspan='6'><div align='center'>Temperature & Humidity Sensor(溫溼度感測裝置)</div></td>
	</tr>
    <tr>
      <td width="220">開始日期時間(YYYYMMDD)
         <input name="MAC" type="hidden" id="MAC" value="<?php echo $mid ?>" />
      </td>
      <td width="200"><label for="strdt"></label>
      <input type="text" name="dt1" id="dt1" size="8" maxlength="8" value = <? echo $sd1; ?> /></td>
      <td width="220">結束日期時間(YYYYMMDD)</td>
      <td width="200"><input type="text" name="dt2" id="dt2" size="8" maxlength="8" value = <? echo $sd2; ?> /></td>
      <td width="200"><input type="submit" name="button" id="button" value="送出" /><a href="/tmp/dhtdata.csv">下載</a></td>
    </tr>
  </table>
</form>

<div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>
<div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>

<script type="text/javascript">

// 顯示溫度圖表
Highcharts.chart('container1', {
    chart: {
        zoomType: 'x'
    },
    title: {
        text: 'Temperature °C by MAC:<? echo $mid?>',
        fontsize: 30
    },
    subtitle: {
        text: ''
    },
    xAxis: {
        categories: <?php echo json_encode($d00, JSON_UNESCAPED_UNICODE); ?>
    },
    yAxis: {
        title: {
            text: '°C'
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
            },
            marker: {
                radius: 2
            },
            lineWidth: 0.1,
            states: {
                hover: {
                    lineWidth: 1
                }
            },
            threshold: null
        }
    },
    series: [{
        name: 'Temperature',
        data: <?php echo json_encode($d01, JSON_UNESCAPED_UNICODE); ?> 
    }]
});

// 顯示濕度圖表
Highcharts.chart('container2', {
    chart: {
        zoomType: 'x'
    },
    title: {
        text: 'Humidity Curve Chart by MAC:<? echo $mid?>',
        fontsize: 30
    },
    subtitle: {
        text: ''
    },
    xAxis: {
        categories: <?php echo json_encode($d00, JSON_UNESCAPED_UNICODE); ?>
    },
    yAxis: {
        title: {
            text: 'Percent(%)'
        }
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
            },
            marker: {
                radius: 2
            },
            lineWidth: 0.1,
            states: {
                hover: {
                    lineWidth: 1
                }
            },
            threshold: null
        }
    },
    series: [{
        name: 'Percent',
        data: <?php echo json_encode($d02, JSON_UNESCAPED_UNICODE); ?> 
    }]
});
</script>

<?php
include '../topfooter.php';  // 引入頁腳檔案
?>      
</body>
</html>
