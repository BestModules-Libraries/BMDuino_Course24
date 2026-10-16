<?php
	session_start();		//必須要用這段，方能使用session的所有功能

//insert into fcu.user (user, username, pwd, email) values ( 'abc', '李大德', '123456789', 'lee@gmail.com');
//insert into fcu.user (user, username, pwd, email) values ( '%s', '%s', '%s', '%s');
   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

?>
<!DOCTYPE html>
      <html>
        <head>
          <title>選擇美味肉圓</title>
       <meta http-equiv="Content-Type" content="text/html"; charset="utf-8">
      </head>
        <body>
		<?php include '../title.php' ; ?>
   <table border="1"  align = center cellspacing="1" cellpadding="1">		


<?php
$qrystr = "select * from fcu.Vitems where 1 ;" ;		//SQL填空

//$result=mysqli_query($link,$qrystr);	


$result=mysqli_query($link,$qrystr);		//將dhtdata的資料找出來(只找最後5
//執行抓資料的SQL敘述
?>
<table align="center" border = "1">
<?php	
$cnt = 1 ;
$goddata = "<td><form id='form1' name='form1' method='post' action='balling.php?sid=%s'  target = '_self'><p><div align = 'center'><img src='/fcu/images/meatball/%s' width='300' height='300' /></div></p><div align = 'center'><p>肉圓名稱:%s</p></div><div align = 'center'><p>價格:%s</p></div><div align = 'center'><p><input type='number' id='num' name='num' value='0' min='0' max='100'></p></div><div align = 'center'><input type='submit' name='button' id='button' value='加入購物車' /></div></p></form></td>\n" ;

if($result!==FALSE)	//判斷是否有資料
{
 while($row = mysqli_fetch_array($result)) //每次抓一筆資料，有抓到資料傳回true， 沒有傳回false
 {
	if (($cnt % 3)== 1)
	{
		big.dhtdata "<tr>" ;
	}
	$goddatastr = sprintf($goddata,$row["id"],$row["pic"],$row["name"],$row["price"]) ;
	big.dhtdata $goddatastr  ;
		if (($cnt % 4)== 0)
		{
			big.dhtdata "</tr>" ;
		}
		$cnt =$cnt +1 ;
	}// 會跳下一列資料

}
		

 mysqli_free_result($result);	// 關閉資料集

 mysqli_close($link);		// 關閉連線
?>
	</table>
	<?php include '../footer.php' ; ?>	
	</body>
</html>

<script>
// 這一區
// php would run before javascript
// better solution to catch error
// ajax request in led.php request for this lamp.php than handle alert() based on callback
//window.location = '/fcu/movie/movielist.php'
</script>";