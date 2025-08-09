
<?php
 
   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
	mysql_select_db($link, "ncnuiot") ;

	$s00=$_GET["MAC"];		//取得POST參數 : sysdatatime (unix datetime import)
	$qrystr=sprintf("select * from OW_curent where MAC = '%s' order by dataorder, sitename ", $s00);		//將dhtdata的資料找出來
/*
	$d00 = array();		// declare blank array of d00
	$d01 = array();	// declare blank array of d01
	$d02 = array();	// declare blank array of d01
	$d03 = array();	// declare blank array of d01
	$d04 = array();	// declare blank array of d01
	$d05 = array();	// declare blank array of d01
*/	
	$result=mysql_query($qrystr,$link);		//將dhtdata的資料找出來(只找最後5

/*
  if($result!==FALSE){
	 while($row = mysql_fetch_array($result)) 
	 {
			array_push($d00, $row["crtdatetime"]);		// $row[欄位名稱] 就可以取出該欄位資料
			array_push($d01, $row["siteid"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d02, $row["sitename"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d03, $row["country"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d04, $row["temp"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d05, $row["humidity"]);	// array_push(某個陣列名稱,加入的陣列的內容
		}// 會跳下一列資料

  }
			
	
	 mysql_free_result($result);	// 關閉資料集
 
	 mysql_close($link);		// 關閉連線

*/



?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Get Open Weather Curent Data List for NCNU </title>
<link href="webcss.css" rel="stylesheet" type="text/css" />

</head>
<body>
<?php
include '../title.php';
?>
<input type ="button" onclick="history.back()" value="BACK(回到上一頁)">
</input>
  <div align="center">
  	<?php
	echo $s00 ;
	echo "<br>" ;
	echo $qrystr;
	echo "<br>" ;	
	?>
   <table border="1" align = center cellspacing="1" cellpadding="1">		
		<tr>
			<td>日期時間</td>
			<td>站台編號</td>
			<td>站台名稱</td>
			<td>國別代碼</td>
			<td>溫度</td>
			<td>濕度</td>
			</tr>

      <?php 
	    if($result!==FALSE)
		{
		 while($row = mysql_fetch_array($result)) 
			 {

				echo sprintf("<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>", $row["crtdatetime"],$row["siteid"], $row["sitename"], $row["country"],  $row["temp"], $row["humidity"]);

			 }
		}
      	
	 mysql_free_result($result);	// 關閉資料集
 
	 mysql_close($link);		// 關閉連線

	  ?>

   </table>

</div>

</form>
<?php
include '../footer.php';
?>

</body>
</html>
