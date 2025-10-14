
<?php
   	include("../comlib.php");		//使用資料庫的呼叫程式
	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

//	if(is_array($_GET)&&count($_GET)>0)//先判断是否通过get传值了
//    {
		echo "GET DATA passed <br>" ;
        if(!isset($_GET["MAC"]))//是否存在"MAC"的参数
        {
            echo "MAC address lost <br>" ;
			die();
        }
        if(!isset($_GET["L"]))//是否存在"Lux value"的参数
        {
            echo "Lux value loss <br>" ;
			die();
        }

	$temp0=$_GET["MAC"];		//取得POST參數 : MAC address
	$temp1=$_GET["L"];		//取得POST參數 : Lux value
	
 //  }



//	if empty(trim($temp0))//MAC 是否空白的参数
	echo "(".trim($temp0).")<br>";
	if (trim($temp0) == "")//MAC 是否空白的参数
	{
		echo "MAC Address is empty string <br>" ;
		die();
	}

//	$temp0=$_GET["MAC"];		//取得POST參數 : MAC address
//	$temp1=$_GET["T"];		//取得POST參數 : temperature
//	$temp2=$_GET["H"];		//取得POST參數 : humidity	
	$sysdt = getdataorder() ;
//	$ddt = getdataorder() ;
	
	//http://localhost:8888/bigdata/lux/luxdataadd.php?MAC=AABBCCDDEEFF&L=987
	// 主機：http://localhost:8888/
	//Http GET程式：bigdata/lux/luxdataadd.php?MAC=AABBCCDDEEFF&L=987
	//傳遞的參數：MAC=AABBCCDDEEFF&L=987
		//MAC=網卡編號(需大寫)
		//L= 亮度

		//INSERT INTO `lux` (`id`, `MAC`, `crtdatetime`, `luxvalue`, `systime`) VALUES (NULL, 'AABBCCDDEEFF', current_timestamp(), '1023.4', '20230406104401');
	/*
		INSERT INTO big.lux (MAC, luxvalue,systime) VALUES ('%s', %10.1f, '%s');
	
	*/
	
	//$qrystr = "INSERT INTO big.lux (MAC, luxvalue,systime) VALUES ('%s', %10.1f, '%s');"; 
	//組成新增到dhtdata資料表的SQL語法
	//          INSERT INTO `dhtData` (`id`, `MAC`, `crtdatetime`, `temperature`, `humidity`, `systime`) VALUES (NULL, '111111111111', CURRENT_TIMESTAMP, '26.8', '65', '20220318100901');
	//          INSERT INTO `dhtData` (MAC, temperature, humidity, systime) VALUES ('111111111111','26.8', '65', '20220318100901');

	//select * from dhtdata order by id desc;
//--------------

	//新增資料的sql 語法:INSERT INTO dhtdata (MAC, temperature, humidity, systime) VALUES ( 'AAAAAAAAAAAA',  '45.2', '88.9', '20230324114801');
	$cmdstr = "INSERT INTO big.lux (MAC, luxvalue,systime) VALUES ('%s', %10.1f, '%s');" ;
//	$qrystr = sprintf("insert into big.dhtData (MAC,temperature, humidity, systime) VALUES ( '%s', %f, %f, '%s');" ,$temp0,$temp1,$temp2,$sysdt) ;
	$qrystr = sprintf($cmdstr ,$temp0,$temp1,$sysdt) ;
	//使用sprintf 將 插入到dhtData 資料表的insert SQL 敘述產生出來，並回傳整個SQL敘述到變數$qrystr
	echo $qrystr ;		//印出$qrystr 內容
	echo "<br>" ;
	if (mysqli_query($link,$qrystr))		//連線的資料庫($link)，在該資料庫上執行變數$qrystr 的SQL 敘述(插入資料)
		{
				echo "Successful <br>" ;
		}
		else
		{
				echo "Fail <br>" ;
		}
			;			//執行SQL語法
	mysqli_close($link);		// 關閉連線

?>
