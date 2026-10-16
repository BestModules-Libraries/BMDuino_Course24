<?php
		//session_start();
	function Connection() 
	{

		$server="127.0.0.1";		//因為apache server and db server(mysql) 安裝在同一台機器，所以用本機端
									//如果apache server and db server(mysql) 安裝在不同一台機器，請改為DB SERVER那台機器的IP ADDRESS
		$user="big";			// DB SERVER user account 
		$pass="12345678";		// DB SERVER user password	
		$db="big";				//系統使用著資料庫名稱
		$dbport = 3306 ;		// DB SERVER 安裝與使用的通訊埠
   		//echo "cnn is ok 01"."<br>" ;	   	
		$connection = new mysqli($server,$user,$pass,$db , $dbport);
		//$connection = mysqli_connect($server, $user, $pass, $db ) ;
   		//echo "cnn is ok 02"."<br>" ;	   	
		//透過上述資訊，建立mysql的資料連線，並把連線回傳到$connection 變數

		if ($connection -> connect_errno) 	//判斷mysql 連線失敗
		{
		  echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
		  exit();		//終止系統
		}
		
   		//echo "cnn is ok 03"."<br>" ;	   	
		mysqli_select_db($connection,$db); 	//切換資料庫到系統變數$db所指的資料庫
   		//echo "cnn is ok 04"."<br>" ;	   	
		$connection -> query("SET NAMES UTF8");		//設定連線資料庫使用語系(unicode UTF8)
   		//echo "cnn is ok 05"."<br>" ;	   	

   		//echo "cnn is ok 06"."<br>" ;	   	
	
		return $connection  ;			//回傳連線
	}
	
?>