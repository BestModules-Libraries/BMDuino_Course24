<?php 

   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
   	
  	$link=Connection();		//產生mySQL連線物件

        $ip = getenv("REMOTE_ADDR");	//取得用戶端連線IP Address
        $sysdt = getdataorder() ;
        $rfidkey = $_POST["rfidkey"]  ;
        $activate = $_POST["activate"]  ;
        $startdatetime = $_POST["startdatetime"]  ;
        $enddatetime = $_POST["enddatetime"]  ;

        //INSERT INTO `rfidlist` (`id`, `crtdatetime`, `systime`, `IP`, `rfidkey`, `activate`, `startdatetime`, `enddatetime`) VALUES (NULL, CURRENT_TIMESTAMP, '20250514010101', '120.9.9.9', '1236547654', '1', '20250514010101', '99991231235959');
        $qry = "insert into rfidlist (systime, IP, rfidkey, activate, startdatetime, enddatetime) values ('%s', '%s', '%s', %d, '%s', '%s');" ;
        $qrystr = sprintf($qry ,$sysdt ,$ip,$rfidkey,$activate,$startdatetime,$enddatetime) ;
	echo $qrystr."<br>" ;
        if (mysqli_query($link,$qrystr))		//連線的資料庫($link)，在該資料庫上執行變數$qrystr 的SQL 敘述(插入資料)
        {
            mysqli_close($link);		// 關閉連線
            header("Location: rfid_display.php");
            exit();
        }
        mysqli_close($link);		// 關閉連線
?>