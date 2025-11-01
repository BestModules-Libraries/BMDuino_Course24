<?php
	//http://nuk.arduino.org.tw:8888/dhtdata/ShowChartlistday.php?MAC=E89F6DE8F3BC
	
   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
	echo $link."<br>";
?>