
<?php
   	include("../comlib.php");		//共用函式庫
	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
	$ip = getenv("REMOTE_ADDR");	//取得用戶端連線IP Address
	echo "ip:".$ip."<br>";
//	if(is_array($_GET)&&count($_GET)>0)//先判断是否通过get传值了
//    {
		echo "GET DATA passed <br>" ;
        if(!isset($_GET["MAC"]))//是否存在"MAC"的参数
        {
            echo "MAC address lost <br>" ;
			die();
        }
        if(!isset($_GET["bd"]))//是否存在"bd"的参数
        {
            echo "sPo2 loss <br>" ;
			die();
        }
        if(!isset($_GET["hb"]))//是否存在"hb"的参数
        {
            echo "heart beat loss <br>" ;
			die();
        }
	$temp0=$_GET["MAC"];		//取得POST參數 : MAC address
	$temp1=$_GET["bd"];		//取得POST參數 : blood oxyegn
	$temp2=$_GET["hb"];		//取得POST參數 : heart beat		
 //  }



//	if empty(trim($temp0))//MAC 是否空白的参数
	echo "(".trim($temp0).")<br>";
	if (trim($temp0) == "")//MAC 是否空白的参数
	{
		echo "MAC Address is empty string <br>" ;
		die();
	}

	$sysdt = getdataorder() ;

	
    // 範例傳入網址：
    // http://iot.arduino.org.tw:8888/bmduino/bloodoxygen/dataadd.php?MAC=AABBCCDDEEFF&bd=97.45&hb=83

	//Http GET程式：bmduino/bloodoxygen/dataadd.php
	//傳遞的參數：MAC=AABBCCDDEEFF&bd=97.45&hb=83
		//MAC=網卡編號(需大寫)
		//bd= 血氧
		//hb=  心跳

    // 範例資料表插入語法：
    // INSERT INTO `bloodoxygen` (`id`, `crtdatetime`, `systime`, `MAC`,
	//`IP`, `blood`, `heartbeat`) 
    // VALUES ('', current_timestamp(), '20250920155501', 'AABBCCDDEEFF',
	//'192.168.99.100', '96.76', '93');


	
    // 定義 SQL 新增語法的格式字串，%s 表示字串，%6.2f 表示浮點數(小數2位)，%d 表示整數
    //$qrycmd = "insert into bloodoxygen (MAC,IP,systime, blood, heartbeat) values ('%s', '%s','%s', %6.2f, %d);";
	$qrycmd  = "insert into bloodoxygen (MAC,IP,systime, blood, heartbeat) "
			 . "values ('%s', '%s','%s', "
			 . "%6.2f, %d);";
		 
	$qrystr = sprintf($cmdstr ,$temp0,$ip,$temp1,$temp2,$sysdt) ;
	//使用sprintf 將 插入到dhtdata 資料表的insert SQL 敘述產生出來，並回傳整個SQL敘述到變數$qrystr
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
