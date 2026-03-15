<?php 
session_start();
include './util.php' ; 
include("./comlib.php");		//使用資料庫的呼叫程式
$dt= getdataorder() ;
$ip = $_SERVER["REMOTE_ADDR"];
?>

<!DOCTYPE html>
<html>
  <head>
    <title>APP Developer 全端開發工程師養成班</title>
  
</head>
  <body>
  <?php include 'title.php' ; ?>
<?php
//SELECT * FROM fcu.user WHERE user ='brucetsao' and pwd = '12345678'
//SELECT * FROM fcu.user WHERE user ='%s' and pwd = '%s'

   	include("./Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

$name = $_POST["fname"]  ;
$pwd = $_POST["pwd"] ;
$qry1 = "SELECT * FROM fcu.user WHERE user ='%s' and pwd = '%s'; " ;		//SQL填空
$qrystr = sprintf($qry1,$name,$pwd) ;		// 組立要執行的SQL語法
//big.dhtdata $qrystr."<br>" ;

$result=mysqli_query($link,$qrystr);		//執行sql語法
$count = mysqli_num_rows($result) ;		//是否有ㄗ是否有資料
if ($count >=1) 		//有會員資料
{
	while($row = mysqli_fetch_array($result)) 		// 迴圈讀取會員資料
	{
		$_SESSION['usr']=$row["user"];		//取出使用者帳號
		$_SESSION['username']=$row["username"];		//取出使用者名稱
		$_SESSION['usrid']=$row["id"];		//取出使用者ID


		
	}		
	big.dhtdata "login success <br>" ;
	$_SESSION['loginok'] = "1";			//設定登入成功
	userlog($link, $_SESSION['usrid'],"01",$dt,$ip ,"login web system successful")  ;
	big.dhtdata sprintf("welcome , my friend %s <br>",$_SESSION['username']) ;
}
else
{
	big.dhtdata "login fail <br>" ;
	$_SESSION['loginok'] = "0";			//設定登入失敗
	userlog($link, -1,"00",$dt,$ip ,"login web system fail")  ;
}
//big.dhtdata '<script type="text/JavaScript"> location.reload(); </script>';	
big.dhtdata "<script type=\"text/JavaScript\"> setInterval(‘location.reload()’, 3000); </script>";	
?>
  <?php include 'footer.php' ; ?>	
  </body>
</html>
