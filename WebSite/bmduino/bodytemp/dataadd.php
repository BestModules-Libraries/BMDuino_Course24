
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

/*
這段程式示範如何使用 HTTP GET 方法接收從感測器設備送出的資料（如：MAC位址、溫度、濕度），
並將這些資料寫入 MySQL 資料庫中的 dhtdata 資料表。

HTTP GET 是用於從伺服器請求資料的方式，
透過 URL 中的參數（如 ?MAC=xxx&T=25&H=60）傳遞資訊。
*/

// 載入共用函式庫，例如自定義的工具函數
include("../comlib.php");		

// 載入資料庫連線設定與函數（應該定義有 Connection() 函數）
include("../Connections/iotcnn.php");

// 使用 Connection() 函式建立與資料庫的連線，結果存到 $link
$link = Connection();	

// 取得使用者端的 IP 位址
$ip = getenv("REMOTE_ADDR");        // 取得使用者端的 IP 位址
echo "ip:".$ip."<br>";	// 顯示目前連線的 IP

// 偵測是否透過 GET 傳入資料
echo "GET DATA passed <br>" ;

// 檢查是否有傳入 "MAC" 參數，如果沒有則顯示錯誤並中止執行
if(!isset($_GET["MAC"])) {
    echo "MAC address lost <br>" ;
    die(); // 結束程式
}

// 檢查是否有傳入 "bodytemp" 參數（溫度），否則中止
if(!isset($_GET["bodytemp"])) {
    echo "Body Temperature loss <br>" ;
    die();
}


// 從 GET 取得參數值：MAC 位址、溫度、濕度
$temp0 = $_GET["MAC"];		// MAC 位址
$temp1 = $_GET["bodytemp"];		// [身體溫度


// 顯示 MAC 位址，並去除空白字元
echo "(".trim($temp0).")<br>";

// 如果 MAC 是空字串（即只有空白），則中止執行
if (trim($temp0) == "") {
    echo "MAC Address is empty string <br>" ;
    die();
}

// 取得系統目前時間（格式化後，可能像 20250722124501），此函式應定義於 comlib.php
$sysdt = getdataorder();// get date+time as YYYYMMDDhhmmss Format

/*
以下為 HTTP GET 範例：

http://iot.arduino.org.tw:8888/bigdata/bodytemp/dataadd.php?MAC=AABBCCDDEEFF&bodytemp=35.6
這裡的參數意義為：
MAC=AABBCCDDEEFF	=> 感測器設備的 MAC 位址
bodytemp=35.6				=> 身體溫度值
 

將這些資料插入到資料表 dhtdata，例如：
INSERT INTO `bodytemp` (`id`, `MAC`, `crtdatetime`, `IP`, `bodytemperature`, `systime`) VALUES ('', 'AABBCCDDEEFF', CURRENT_TIMESTAMP, '192.168.99.100', '36.7', '20251018102901');


insert into bodytemp 
 ('MAC', 'IP', 'bodytemperature', 'systime') 
  values 
 ('%s', '%s', %f, '%s');


*/

// 建立 SQL 語法的模板字串，使用 %s 表示字串佔位符，%6.1f 表示浮點數佔位符（共6位，小數1位）
$cmdstr = "insert into bodytemp (MAC, IP, bodytemperature, systime) values ('%s', '%s', %f, '%s');" ;

// 使用 sprintf 將實際的變數值格式化進 SQL 語法中
$qrystr = sprintf($cmdstr, $temp0, $ip, $temp1, $sysdt);

// 印出 SQL 敘述（用於除錯）
echo $qrystr . "<br>";

// 使用 mysqli_query() 執行 SQL 指令，將資料寫入資料庫
if (mysqli_query($link, $qrystr)) 
{
    echo "Successful <br>" ;	// 若成功則顯示成功訊息
} 
else 
{
    echo "Fail <br>" ;		// 若失敗則顯示錯誤
}

// 關閉與資料庫的連線
mysqli_close($link);
?>
