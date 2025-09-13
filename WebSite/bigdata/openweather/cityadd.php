<?php
    include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
//	mysql_select_db($link, "nkustiot") ;

	$s00=$_GET["MAC"];		//取得POST參數 :MAC)
	$s01=$_GET["f01"];		//取得POST參數 :sid
	$s02=$_GET["f02"];		//取得POST參數 :sname
	$s03=$_GET["f03"];		//取得POST參數 : country
	$s04=$_GET["f04"];		//取得POST參數 : state
	$s05=$_GET["f05"];		//取得POST參數 : lon
	$s06=$_GET["f06"];		//取得POST參數 : 	lat

	echo $s00 ;
	echo $s01 ;
	echo $s02 ;
	echo $s03 ;
	echo $s04 ;
	echo $s05 ;
	echo $s06 ;

	//$sysdt ='2021/05/06 12:00:01' ;
	//$ddt ='20210506120001' ;
	
	$sysdt = uniztime2datetime($s01) ;
	$ddt = trandatetime0($sysdt) ;
	echo "/" ;
	echo $sysdt ;
	echo "/" ;
	echo $ddt ;
	//$ddt2 = getdataorder2() ;
//http://ncnu.arduino.org.tw:9999/iot.php
//http://ncnu.arduino.org.tw:9999/openweather/cityadd.php?MAC=Brucetsao&f01=11&f02=taippp&f03=TW&f04=&f05=120.9695&f06=23.9664
//web site ==>http://ncnu.arduino.org.tw:9999/
//DB agent ==>openweather/cityadd.php
//sensor data(parameters) ==>?MAC=Brucetsao&f01=11&f02=taippp&f03=TW&f04=&f05=120.9695&f06=23.9664

//  use restful as above 
//------------------

//INSERT INTO ncnuiot.OW_citylist (MAC,sid, sname, country ,state ,lat, lon) VALUES ('Brucetsao','11', 'taippp', 'TW', '', '120.9695','23.9664')
//-------------


	$query = sprintf("INSERT INTO ncnuiot.OW_citylist (MAC,sid, sname, country ,state ,lat, lon) VALUES ('%s','%s', '%s', '%s', '%s', '%s','%s')"
	,$s00,$s01,$s02,$s03,$s04,$s05,$s06);


	echo $query ;
	echo "<br>" ;


	if (mysql_query($query,$link))
		{
				echo "Successful <br>" ;
		}
		else
		{
				echo "Fail <br>" ;
		}
		
			;			//執行SQL語法
	echo "<br>" ;
	mysql_close($link);		//關閉Query

 
	

?>


