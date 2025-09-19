<?php     
   //http://140.134.25.43:8088/dhtdata/getdht2tablelist.php?MAC=246F28248CE0&start=20200406005604&end=20200406010836
	//http://140.134.25.43:8088/ Host URL
	//Restful API 的程式：：fcu/dhtdata/getdht2tablelist.php
	//傳入的參數：?MAC=3C71BFFD882C&start=20210101&end=20211231

    //include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件
	$sid=$_GET["MAC"];		//取得POST參數 : 裝置的MAC 網路卡編號
	$s1=$_GET["start"];		//取得POST參數 : 開始日期  YYYYMMDDHHMMSS
	$s2=$_GET["end"];		//取得POST參數 : 結束日期  YYYYMMDDHHMMSS
	//SELECT * FROM dhtdata WHERE MAC = '246F28248CE0' and systime >= '20200406005604' and systime <= '20200406010836'; order by systime asc 
	//SELECT * FROM dhtdata WHERE MAC = '%s' and systime >= '%s' and systime <= '%s' order by systime asc 
	
	$qry1 = "select * from fcu.dhtdata where mac = '%s' and systime >= '%s' and systime <= '%s' order by systime asc " ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來

	//echo $qrystr."<br>" ;
	$result= mysqli_query($link,$qrystr);		//找出多少筆

	//echo $count."<br>";
	//以下是顯示用指令
	echo "<table width='98%' border='1'>"; 
	echo "<tr><td>序號</td><td>日期</td><td>溫度</td><td>濕度</td></tr>";
	$tablerow = "<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td></tr>" ;
	$count=1 ;	//序號變數
	while($row = mysqli_fetch_array($result)) 	//loop 用來讀資料
		{
			//$row  用loop讀出的每一列資料

			$tmp = sprintf($tablerow,$count,$row['crtdatetime'],$row['temperature'],$row['humidity'])  ;	
	
			echo $tmp;
		 	$count = $count +1 ;
		}		//end of while($row = mysqli_fetch_array($result)) 

	echo "</table>" ;
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線
	 

    ?>