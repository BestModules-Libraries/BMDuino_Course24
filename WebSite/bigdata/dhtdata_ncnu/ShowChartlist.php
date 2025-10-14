
<?php
 	//http://ncnu.arduino.org.tw:9999/dhtdata/ShowChartlist.php?MAC=CC50E3B5BB20
	//http://192.168.88.200:9999/dhtdata/ShowChartlist.php?MAC=34945435152C&d1=19990101&d2=20250201

   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

	//$mid=$_GET["mac"];		//取得POST參數 : mac address
	if  (!isset($_GET["MAC"])) 
	{
		$mid=$_POST["MAC"];		//取得POST參數 : MAC address
	}
	else
	{
		$mid=$_GET["MAC"];		//取得POST參數 : MAC address
	}
		
	if (!isset($_POST["dt1"])) 
	{
		$sd1 = getshiftdataorder(-3*24); 
		$dd1 = $sd1 ;	
	}
	else
	{
		$dd1 = $_POST["dt1"] ;	
		$sd1 = $_POST["dt1"] ;
	}
	

	
	if (!isset($_POST["dt2"])) 
	{
		$sd2 = getdataorder();    
		$dd2 = $sd2 ;	

	}	
	else
	{
		$dd2 = $_POST["dt2"] ;	
		$sd2 =  $_POST["dt2"] ;
	}
			
			
	//$qry ="select * from ncnuiot.dhtData where MAC = '%s' and systime >= '%s' and systime <= '%s' order by systime asc " ;		//將dhtdata的資料找出來
	$qry ="select left(systime,8) as systime ,avg(temperature) as t, avg(humidity) as h from ncnuiot.dhtData where MAC = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc " ;		
	$qrystr=sprintf($qry,$mid,$dd1,$dd2) ;		//將dhtdata的資料找出來
	//echo $qrystr."<br>" ;
//	int aa[] = {1,3,45,6,7} ;  == 	$aa = array(4,5,6,7,8,9);
//	int bb[] = {} ;  ==  $bb = array();
	$d00 = array();		// declare blank array of d00
	$d00a = array();		// declare blank array of d00
	$d01 = array();	// declare blank array of d01
	$d02 = array();	// declare blank array of d02
	$d03 = array();	// declare blank array of d03
	
	$result=mysqli_query($link,$qrystr);		//將dhtdata的資料找出來(只找最後5
//	echo "step 02 . <br>" ;
  if($result!==FALSE)
  {
	 while($row = mysqli_fetch_array($result)) 
	 {
			array_push($d00, trandatetime3($row["systime"]));		// $row[欄位名稱] 就可以取出該欄位資料
			array_push($d00a, $row["systime"]);		// $row[欄位名稱] 就可以取出該欄位資料
			array_push($d01, (double)sprintf("%8.2f",(double)$row["t"]));	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d02, (double)sprintf("%8.2f",(double)$row["h"]));
			array_push($d03, $row["MAC"]);

		}// 會跳下一列資料

  }
			
//	echo "step 03 . <br>" ;		
	 mysqli_free_result($result);	// 關閉資料集
//	echo "step 04 . <br>" ;	 
	 mysqli_close($link);		// 關閉連線
//	echo "step 05 . <br>" ;	 	


		$myfile = fopen("../tmp/dhtdata.csv", "w");
		$datah = "'%s' , '%s' , '%s' \n" ;
		$datar = "'%s' , %10.3f , %5.2f \n" ;
			$datarow = sprintf($datah,"DateTime",'Temperature','Humidity') ;
			fwrite($myfile, $datarow);
			for($i=0 ; $i < count($d00a);$i=$i+1)
				{
					$datarow = sprintf($datar,$d00a[$i],$d01[$i],$d02[$i]) ;
					fwrite($myfile, $datarow);
				}

		fclose($myfile);



?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Display Temperature and Humidity Curve Chart by MAC</title>
<link href="webcss.css" rel="stylesheet" type="text/css" />

	<script src="/code/highcharts.js"></script>
    <script src="/code/highcharts-more.js"></script>
    <script src="/code/modules/exporting.js"></script>
    <script src="/code/modules/export-data.js"></script>
    <script src="/code/modules/accessibility.js"></script>	
</head>
<body>
<?php
include '../title.php';
?>
<input type ="button" onclick="history.back()" value="BACK(回到上一頁)">
</input>
<form id="form1" name="form1" method="post" action="">
  <table width="100%" border="1">
	<tr backcolor >
		<td colspan='6'><div align='center'>Temperature & Humidity Sensor(溫溼度感測裝置)</div></td>
	</tr>
      <td width="220">開始日期時間(YYYYMMDDHHMMSS)
         <input name="MAC" type="hidden" id="MAC" value="<?php echo $mid ?>" />

	  </td>
      <td width="200"><label for="strdt"></label>
      <input type="text" name="dt1" id="dt1" size="14" maxlength="14" value = <? echo $sd1; ?> /></td>
      <td width="220">結束日期時間(YYYYMMDDHHMMSS)</td>
      <td width="200"><input type="text" name="dt2" id="dt2" size="14" maxlength="14" value = <? echo $sd2; ?> /></td>
      <td width="200"><input type="submit" name="button" id="button" value="送出" /><a href="/tmp/dhtdata.csv">下載</a></td>
      </tr>
  </table>

</form>
<div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>
<div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
<p>

<script type="text/javascript">

 //-----------particle--------------

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
        name: 'Temperaturte',
        data:<?php echo json_encode($d01, JSON_UNESCAPED_UNICODE); ?> 
			} ]
});
 //---------Color Temperaturte----------------
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
        name: 'Percent',
        data:<?php echo json_encode($d02, JSON_UNESCAPED_UNICODE); ?> 
			} ]
});
		</script>

<?php
include '../footer.php';
?>      
</body>
</html>
