
<?php
 
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
			
			
	$qry ="select * from ncnuiot.dhtData where MAC = '%s' and systime >= '%s' and systime <= '%s' order by systime desc " ;		//將dhtdata的資料找出來
	$qrystr=sprintf($qry,$mid,$dd1,$dd2) ;		//將dhtdata的資料找出來
			
	//echo $qrystr."<br>" ;
//	int aa[] = {1,3,45,6,7} ;  == 	$aa = array(4,5,6,7,8,9);
//	int bb[] = {} ;  ==  $bb = array();
	$d00 = array();		// declare blank array of d00
	$d01 = array();	// declare blank array of d01
	$d02 = array();	// declare blank array of d02
	$d03 = array();	// declare blank array of d03
	
	$result=mysql_query($qrystr,$link);		//將dhtdata的資料找出來(只找最後5
//	echo "step 02 . <br>" ;
  if($result!==FALSE){
	 while($row = mysqli_fetch_array($result)) 
	 {
			array_push($d00, $row["systime"]);		// $row[欄位名稱] 就可以取出該欄位資料
			array_push($d01, $row["temperature"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d02, $row["humidity"]);
			array_push($d03, $row["mac"]);
		}// 會跳下一列資料

  }
			
//	echo "step 03 . <br>" ;		
	 mysqli_free_result($result);	// 關閉資料集
//	echo "step 04 . <br>" ;	 
	 mysqli_close($link);		// 關閉連線
//	echo "step 05 . <br>" ;	 	


		$myfile = fopen("../tmp/dhtdata.csv", "w");
		$datar = "%s , %10.3f , %5.2f \n" ;
			for($i=count($d00)-1;$i >=0  ;$i=$i-1)
				{
					$datarow = sprintf($datar,$d00[$i],$d01[$i],$d02[$i]) ;
					fwrite($myfile, $datarow);
				}

		fclose($myfile);



?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Display Temperature and Humidity Data as Curve Chart by MAC</title>
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
    <tr>
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


</form>


<script type="text/javascript">

 //-----------particle--------------

Highcharts.chart('container1', {
    chart: {
        type: 'spline'
    },
    title: {
        text: 'Temperature °C by MAC:<? echo $mid?>',
		fontsize: 30
    },
    subtitle: {
        text: ''
    },
    xAxis: {
        categories: [
		<?php
	for($i=count($d00)-1;$i >=0  ;$i=$i-1)
		{
			echo "'";
			echo trandatetime3($d00[$i]);
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
        name: 'Temperaturte',
        data: [
		<?php
	for($i=count($d00)-1;$i >=0  ;$i=$i-1)
		{
			echo $d01[$i];
			echo ",";
		}
		?>
        ]} ]
});
 //---------Color Temperaturte----------------
Highcharts.chart('container2', {
    chart: {
        type: 'spline'
    },
    title: {
        text: 'Humidity Curve Chart by MAC:<? echo $mid?>',
		fontsize: 30
    },
    subtitle: {
        text: ''
    },
    xAxis: {
        categories: [
		<?php
		for($i=0;$i < count($d00);$i=$i+1)
		{
			echo "'";
			echo trandatetime3($d00[$i]);
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
		for($i=0;$i < count($d02);$i=$i+1)
		{
			echo $d02[$i];
			echo ",";
		}
		?>
        ]} ]
});
		</script>
<div align="center">

	<?php echo "Data Gererated by Device's MAC:".$mid ?>;
   <table border="1" cellspacing="1" cellpadding="1">		
		<tr>
			<td>Date Time</td>
			<td>Temperature(Celsius °C)</td>
			<td>Humidity(Percent %)</td>
		</tr>

      <?php 
		  if(count($d00) >0)
		  {
				for($i=count($d00)-1;$i >=0  ;$i=$i-1)
					{
						echo sprintf("<tr><td>%s</td><td>%4.1f</td><td>%3.1f</td></tr>", 
						  trandatetime4($d00[$i]), $d01[$i], $d02[$i]);
					 }
		 }
      ?>

   </table>

</div>  
<?php
include '../footer.php';
?>      
</body>
</html>
