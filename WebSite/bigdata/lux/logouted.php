<?php 
session_start();
include './util.php' ; 
include("./comlib.php");		//使用資料庫的呼叫程式
$dt= getdataorder() ;
$ip = $_SERVER["REMOTE_ADDR"];
?>
<?php
//SELECT * FROM fcu.user WHERE user ='brucetsao' and pwd = '12345678'
//SELECT * FROM fcu.user WHERE user ='%s' and pwd = '%s'

   	include("./Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

$usr = $_POST["userid"]  ;


	userlog($link, $usr,"09",$dt,$ip ,"login out from web system successful")  ;


?>
<script>
// 這一區
	// php would run before javascript
	// better solution to catch error
	window.location = '/fcu/'
</script>";
