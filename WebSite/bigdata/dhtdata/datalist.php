
<?php
 
   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
/*
這個 PHP 程式碼的主要用途是根據 MAC 地址查詢溫溼度感測器的詳細資料，
 * 並將這些資訊顯示在 HTML 表格中。
 * 最初會檢查 GET 請求中是否存在 MAC 地址，
 * 然後根據該參數查詢資料庫。
 * 如果 MAC 地址不存在或為空，程式將終止。

接著，SQL 查詢根據 MAC 地址從 dhtdata 資料表中查詢相關資料，
 * 並將結果儲存到多個陣列中。
 * 最終，這些資料在 HTML 表格中顯示，
 * 包括 MAC 地址、IP 地址、更新時間、溫度、濕度等資訊。
 * 網頁的標題和頁腳使用了外部的 PHP 文件。 
 */
        
//	$subrow="<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>( %s , %s )</td><td>%s<br>%s<br>%s<br>%s<br>%s</td></tr>" ;
	$subrow="<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>( %s , %s )</td><td>%s<br>%s<br>%s<br>%s</td></tr>" ;
	$op1 = "<a href='devicelistadd.php'>Device Belong(裝置歸屬)</a>" ;
	$op2a = "<a href='ShowChartlist.php?MAC=%s'>Curve Chart</a>" ;
	$op3a = "<a href='ShowChartlistday.php?MAC=%s'>Dayily Curve Chart</a>" ;
	$op4a = "<a href='ShowBoxChart.php?MAC=%s'>Box Plot</a>" ;
	$op9a = "<a href='ShowSingleGuage.php?mac=%s'>Guage Display</a>" ;
	$qrystr="select a.*, b.*, c.* , a.id as mainid from big.sitelist as a, big.site as b, big.sensortype as c where a.sensortype = c.sid and a.Did = b.id and a.sensortype = '01' order by b.siteid asc , a.mac asc " ;		//將dhtdata的資料找出來

	$d00 = array();		// declare blank array of d00
	$d01 = array();	// declare blank array of d01
	$d02 = array();	// declare blank array of d02
	$d03 = array();	// declare blank array of d03
	$d04 = array();	// declare blank array of d04
	$d05 = array();	// declare blank array of d05
	$d06 = array();	// declare blank array of d06
	
	$result=mysqli_query($link,$qrystr);		//將dhtdata的資料找出來(只找最後5

  if($result!==FALSE){
	 while($row = mysqli_fetch_array($result)) 
	 {
			array_push($d00, $row["mainid"]);		// $row[欄位名稱] 就可以取出該欄位資料
			array_push($d01, $row["siteid"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d02, $row["sitename"]);
			array_push($d03, $row["address"]);
			array_push($d04, $row["mac"]);
			array_push($d05, $row["longitude"]);
			array_push($d06, $row["latitude"]);
		}// 會跳下一列資料

  }
			
	
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線





?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Temperature and Humidity Device  List</title>
<link href="webcss.css" rel="stylesheet" type="text/css" />

</head>
<body>
    <?php
       include '../toptitle.php';
    ?>
<input type ="button" onclick="history.back()" value="BACK(回到上一頁)">
</input>
  <div align="center">
   <table border="1" align = center cellspacing="1" cellpadding="1">		
	<tr bgcolor=#CFC >
		<td colspan='6'><div align='center'>Temperature & Humidity Sensor(溫溼度感測裝置)</div></td>
	</tr>
		<tr>
			<td>Site ID(站台編號)</td>
			<td>Site Name(站台名稱)</td>
			<td>Site Address(站台地址)</td>
			<td>MAC Adress(網路卡號)</td>
			<td>GPS</td>
			<td><a href='devicelistadd.php'>Manage(管理)</a>/Chart Display(圖表顯示)</td>
			</tr>

      <?php 
		  if(count($d00) >0)
		  {
				for($i=count($d00)-1;$i >=0  ;$i=$i-1)
					{
						$op2 = sprintf($op2a,$d04[$i]) ;
						$op3 = sprintf($op3a,$d04[$i]) ;
						//$op4 = sprintf($op4a,$d04[$i]) ;
						$op9 = sprintf($op9a,$d04[$i]) ;
						
						echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $d05[$i], $d06[$i],$op1 , $op2, $op3, $op9);
					 }
		 }
      ?>

   </table>

</div>

</form>
    <?php
        include '../topfooter.php';
    ?>

</body>
</html>
