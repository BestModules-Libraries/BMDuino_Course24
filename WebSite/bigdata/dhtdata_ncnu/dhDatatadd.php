
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
   	include("../comlib.php");		//使用資料庫的呼叫程式
	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
//	mysql_select_db($link, "ncnuiot") ;
	$temp0=$_GET["MAC"];		//取得POST參數 : MAC address
	$temp1=$_GET["T"];		//取得POST參數 : temperature
	$temp2=$_GET["H"];		//取得POST參數 : humidity

	$sysdt = getdataorder() ;
//	$ddt = getdataorder() ;
	$ip = $_SERVER['REMOTE_ADDR'];
	
	//http://ncnu.arduino.org.tw:9999/dhtdata/dhDatatadd.php?MAC=AABBCCDDEEFF&T=34&H=34
        //http://192.168.88.200:9999/dhtdata/dhDatatadd.php?MAC=AABBCCDDEEFF&T=34&H=34
	// 主機：http://ncnu.arduino.org.tw:9999/
	//Http GET程式：dhtdata/dhDatatadd.php
	//傳遞的參數：MAC=AABBCCDDEEFF&T=34&H=34
		//MAC=網卡編號(需大寫)
		//T= 溫度
		//H=  濕度
	$query = "insert into ncnuiot.dhtData (mac,IP,systime,temperature,humidity) VALUES ('".$temp0."','".$ip."','".$sysdt."',".$temp1.",".$temp2.")"; 
	//組成新增到dhtdata資料表的SQL語法
	//          INSERT INTO `dhtData` (`id`, `MAC`, `crtdatetime`, `temperature`, `humidity`, `systime`) VALUES (NULL, '111111111111', CURRENT_TIMESTAMP, '26.8', '65', '20220318100901');
	//          INSERT INTO `dhtData` (MAC, temperature, humidity, systime) VALUES ('111111111111','26.8', '65', '20220318100901');

	echo $query ;
	echo "<br>" ;


	if (mysqli_query($link,$query))
		{
				echo "Successful <br>" ;
		}
		else
		{
				echo "Fail <br>" ;
		}
		
			;			//執行SQL語法
	echo "<br>" ;
	mysqli_close($link);		// 關閉連線

 
?>
