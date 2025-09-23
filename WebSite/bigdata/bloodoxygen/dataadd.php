
<?php
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
        $ip = getenv("REMOTE_ADDR");	//取得用戶端連線IP Address
        echo "ip:".$ip."<br>";
//	mysqli_select_db($link, "ncnuiot") ;
	$temp0=$_GET["MAC"];		//取得POST參數 : MAC address
	$temp1=$_GET["bd"];		//取得POST參數 : 距離值
	$temp1=$_GET["hb"];		//取得POST參數 : 距離值

	$sysdt = getdatetime() ;
//	$ddt = getdataorder() ;

	//http://iot.arduino.org.tw:8888/bigdata/bloodoxygen/dataadd.php?MAC=AABBCCDDEEFF&bd=97.45&hb=83
	
	//INSERT INTO `bloodoxygen` (`id`, `crtdatetime`, `systime`, `MAC`, `blood`, `heartbeat`) VALUES (NULL, current_timestamp(), '20250516200301', 'AABBCCDDEEFF', '97.5', '87');
        //$qrycmd="insert into bloodoxygen (systime, MAC, blood, heartbeat) values ('%s', '%s', '%6.2f', '%d');";
	$query = "INSERT INTO sonic (mac,IP,systime,distance) VALUES ('".$temp0."','".$ip."','".$sysdt."',".$temp1.")"; 
	//組成新增到dhtdata資料表的SQL語法

	echo $query ;
	echo "<br>" ;


	if (mysqli_query($link, $query))
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
    <?php
         /* Defining a PHP Function */
         function getdataorder($dt) {
        	//   $dt = getdate() ;
				$splitTimeStamp = explode(" ",$dt);
				$ymd = $splitTimeStamp[0] ;
				$hms = $splitTimeStamp[1] ;
				$vdate = explode('-', $ymd);
				$vtime = explode(':', $hms);
				$yyyy =  str_pad($vdate[0],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($vdate[1] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($vdate[2] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($vtime[0] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($vtime[1] ,2,"0",STR_PAD_LEFT);
				$sec  =  str_pad($vtime[2] ,2,"0",STR_PAD_LEFT);
			/*
				echo "***(" ;
				echo $dt ;
				echo "/" ;
				echo $yyyy ;
				echo "/" ;
				echo $mm ;
				echo "/" ;
				echo $dd ;
				echo "/" ;
				echo $hh ;
				echo "/" ;
				echo $min ;
				echo "/" ;
				echo $sec ;
				echo ")<br>" ;
			*/
			return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
         function getdataorder2($dt) {
        	//   $dt = getdate() ;
				$splitTimeStamp = explode(" ",$dt);
				$ymd = $splitTimeStamp[0] ;
				$hms = $splitTimeStamp[1] ;
				$vdate = explode('-', $ymd);
				$vtime = explode(':', $hms);
				$yyyy =  str_pad($vdate[0],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($vdate[1] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($vdate[2] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($vtime[0] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($vtime[1] ,2,"0",STR_PAD_LEFT);
				
			return ($yyyy.$mm.$dd.$hh.$min)  ;
         }
         function getdatetime() {
           $dt = getdate() ;
				$yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($dt['minutes'] ,2,"0",STR_PAD_LEFT);
				$sec  =  str_pad($dt['seconds'] ,2,"0",STR_PAD_LEFT);

			return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
		          function trandatetime1($dt) {
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,4,2);
				$dd  =   substr($dt,6,2);
				$hh  =   substr($dt,8,2);
				$min  =   substr($dt,10,2);
				$sec  =   substr($dt,12,2);

			return ($yyyy."/".$mm."/".$dd." ".$hh.":".$min.":".$sec)  ;
		 }
      ?>
