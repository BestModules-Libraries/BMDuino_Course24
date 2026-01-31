
<?php
/*

HTTP GET 是 HTTP 協定中的一種請求方法，
用於獲取（或查詢）特定資源。
它通常會由瀏覽器發送到 Web 伺服器，以檢索網頁、圖片、音頻、影片、API 等資源。

以下是 HTTP GET 的基本語法：
GET /path/to/resource HTTP/1.1
Host: www.example.com

在這個範例中，GET 是請求方法，
/path/to/resource 是要請求的資源路徑，
HTTP/1.1 是協定版本，
Host 標頭指定了要請求的主機名稱或 IP 位址。

HTTP GET 的工作原理是，
瀏覽器或客戶端向伺服器發送一個請求，
請求中包含了要獲取的資源的路徑和一些其他相關的訊息。
伺服器收到請求後，會根據路徑找到對應的資源，
並將該資源的內容返回給客戶端。
客戶端收到伺服器的回應後，
會解析回應的資料，
並將其顯示在瀏覽器中（例如，網頁內容）或者進行其他操作。

總的來說，
HTTP GET 是一個基本但重要的 HTTP 請求方法，
可用於獲取各種資源，
包括網頁、圖片、音頻、影片、API 等。它是 Web 應用程式中最常見的請求方法之一，
也是 RESTful API 的核心請求方法之一。

格式：
   Http GET的的程式 ? 參數1&參數2&參數3&參數4&參數5......
	參數ｎ的格式：
	等號（＝）左方為傳入參數名稱
	等號（＝）右方為傳入該參數的內容（Ｖａｌｕｅ）
	
*/
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
        if(!isset($_GET["T"]))//是否存在"Temperature"的参数
        {
            echo "Temperature loss <br>" ;
			die();
        }
        if(!isset($_GET["H"]))//是否存在"humidity"的参数
        {
            echo "humidity loss <br>" ;
			die();
        }
	$temp0=$_GET["MAC"];		//取得POST參數 : MAC address
	$temp1=$_GET["T"];		//取得POST參數 : temperature
	$temp2=$_GET["H"];		//取得POST參數 : humidity		
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
	
	//http://localhost:8888/bigdata/dhtdata/dhDatatadd.php?MAC=AABBCCDDEEFF&T=34&H=34
	//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dhDatatadd.php?MAC=AABBCCDDEEFF&T=34&H=34
	// 主機：http://localhost:8888/
	//Http GET程式：dhtdata/dhDatatadd.php
	//傳遞的參數：MAC=AABBCCDDEEFF&T=34&H=34
		//MAC=網卡編號(需大寫)
		//T= 溫度
		//H=  濕度
		//INSERT INTO `dhtdata` (`id`, `MAC`, `crtdatetime`, `temperature`, `humidity`, `systime`) VALUES (NULL, 'AABBCCDDEEFF', current_timestamp(), '25.3', '88.9', '20221026085601');
	/*
	INSERT INTO `dhtdata` 
	(`MAC`,  `temperature`, `humidity`, `systime`) 
	VALUES 
	( 'AABBCCDDEEFF', 25.3, 88.9, '20221026085601');
	
	*/
	
	//$qrystr = "insert into big.dhtdata (mac,systime,temperature,humidity) VALUES ('".$temp0."','".$sysdt."',".$temp1.",".$temp2.")"; 
	//組成新增到dhtdata資料表的SQL語法
	//          INSERT INTO `dhtdata` (`id`, `MAC`, `crtdatetime`, `temperature`, `humidity`, `systime`) VALUES (NULL, '111111111111', CURRENT_TIMESTAMP, '26.8', '65', '20220318100901');
	//          INSERT INTO `dhtdata` (MAC, temperature, humidity, systime) VALUES ('111111111111','26.8', '65', '20220318100901');

	//select * from dhtdata order by id desc;
//--------------

	//新增資料的sql 語法:INSERT INTO dhtdata (MAC, temperature, humidity, systime) VALUES ( 'AAAAAAAAAAAA',  '45.2', '88.9', '20230324114801');
	$cmdstr = "insert into big.dhtdata (MAC,IP, temperature, humidity, systime) VALUES ( '%s',  '%s', %6.1f, %6.1f, '%s');" ;
//	$qrystr = sprintf("insert into big.dhtdata (MAC,temperature, humidity, systime) VALUES ( '%s', '%s', %f, %f, '%s');" ,$temp0,$IP,$temp1,$temp2,$sysdt) ;
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
