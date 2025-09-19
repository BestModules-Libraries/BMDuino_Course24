<?php
    include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件

	$s00=$_GET["MAC"];		//取得POST參數 : sysdatatime (unix datetime import)
	$s01=$_GET["f01"];		//取得POST參數 : sysdatatime (unix datetime import)
	$s02=$_GET["f02"];		//取得POST參數 : timezone
	$s03=$_GET["f03"];		//取得POST參數 : siteid
	$s04=$_GET["f04"];		//取得POST參數 : sitename
	$s05=$_GET["f05"];		//取得POST參數 : country
	$s06=$_GET["f06"];		//取得POST參數 : 	lon
	$s07=$_GET["f07"];		//取得POST參數 : 	lat
	$s08=$_GET["f08"];		//取得POST參數 : temp
	$s09=$_GET["f09"];		//取得POST參數 : temp_min
	$s10=$_GET["f10"];		//取得POST參數 : temp_max
	$s11=$_GET["f11"];		//取得POST參數 : pressure
	$s12=$_GET["f12"];		//取得POST參數 : 	humidity
	$s13=$_GET["f13"];		//取得POST參數 : windspeed
	$s14=$_GET["f14"];		//取得POST參數 : 	winddeg
	echo $s00 ;
	echo $s01 ;
	echo $s02 ;
	echo $s03 ;
	echo $s04 ;
	echo $s05 ;
	echo $s06 ;
	echo $s07 ;
	echo $s08 ;
	echo $s09 ;
	echo $s10 ;
	echo $s11 ;
	echo $s12 ;
	echo $s13 ;
	echo $s14 ;
	//$sysdt ='2021/05/06 12:00:01' ;
	//$ddt ='20210506120001' ;
	
	$sysdt = uniztime2datetime($s01) ;
	$ddt = trandatetime0($sysdt) ;
	echo "/" ;
	echo $sysdt ;
	echo "/" ;
	echo $ddt ;
	//$ddt2 = getdataorder2() ;

//http://localhost:8888/
//http://localhost:8888/bigdata/openweather/owadd.php?MAC=Brucetsao&f01=1620265182&f02=28800&f03=1670310&f04=Puli&f05=TW&f06=120.9695&f07=23.9664&f08=29.39&f09=29.20&f10=30.12&f11=1016&f12=76&f13=0.51&f14=10
//web site ==>http://localhost:8888/
//DB agent ==>bigdata/openweather/owadd.php
//sensor data(parameters) ==>?MAC=Brucetsao&f01=1620265182&f02=28800&f03=1670310&f04=Puli&f05=TW&f06=120.9695&f07=23.9664&f08=29.39&f09=29.20&f10=30.12&f11=1016&f12=76&f13=0.51&f14=10

//  use restful as above 
//------------------
//INSERT INTO `OW_curent` (`id`, `crtdatetime`, `dataorder`, `sysdatatime`, `timezone`, `siteid`, `sitename`, `country`, `lon`, `lat`, `temp`, `temp_min`, `temp_max`, `pressure`, `humidity`, `windspeed`, `winddeg`) VALUES (NULL, CURRENT_TIMESTAMP, '20210506100701', '2021-05-06 10:12:00', '28800', '1670310', 'Puli', 'TW', '120.9695', '23.9664', '29.98', '29.82', '30.15', '1016', '65', '0.51', '10');

//-------------


	$qrystr = sprintf("INSERT INTO big.OW_curent (MAC,dataorder, sysdatatime,timezone, siteid, sitename, country ,lat, lon,temp ,temp_min, temp_max, pressure,humidity,windspeed, winddeg) VALUES ('%s','%s', '%s', '%s', '%s', '%s','%s','%s','%s',%f,%f,%f,%d,%f,%f,%f)"
	,$s00,$ddt,$sysdt,$s02,$s03,$s04,$s05,$s06,$s07,$s08,$s09,$s10,$s11,$s12,$s13,$s14);


	echo $qrystr ;
	echo "<br>" ;

		
	if (mysqli_query($link,$qrystr))
		{
				echo "Successful <br>" ;
		}
		else
		{
				echo "Fail <br>" ;
		}
		
			;			//執行SQL語法
	echo "<br>" ;
	mysqli_close($link);		//關閉Query

 
	

?>


