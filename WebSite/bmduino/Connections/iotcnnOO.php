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
                $conn = new PDO("mysql:host=$server;dbname=$db", $user, $pass);
                $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
                $stmt = $conn->prepare("select database "+$db);//切換資料庫到系統變數$db所指的資料庫
                $stmt->execute();//切換資料庫到系統變數$db所指的資料庫
                $stmt = $conn->prepare("SET NAMES UTF8");//設定連線資料庫使用語系(unicode UTF8)
                $stmt->execute();//設定連線資料庫使用語系(unicode UTF8)

		return $conn  ;			//回傳連線
	}
	
?>