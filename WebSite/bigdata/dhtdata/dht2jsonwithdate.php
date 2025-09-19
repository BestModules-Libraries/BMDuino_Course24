<?php     
<<<<<<< HEAD
// 範例網址（說明用）：這個 PHP 程式會根據 MAC、開始與結束時間查詢感測資料，並回傳 JSON 格式資料。
    // http://iot.arduino.org.tw:8888/bigdata/dhtdata/dht2jsonwithdate.php?MAC=246F28248CE0&start=20200406&end=20200407
// http://localhost:8888/bigdata/dhtdata/dht2jsonwithdate.php?MAC=246F28248CE0&start=20200406&end=20200407

//✅ 此 PHP 程式會讀取網址的三個參數：MAC、start、end。
//
//✅ 查詢 dhtdata 資料表中符合條件的資料。
//
//✅ 將查詢結果整理成 JSON 格式（符合你給的 JSON 格式），再輸出。
//
//✅ 適合用於 IoT 裝置要查詢過去的溫濕度資料時使用，例如用前端或 APP 顯示感測歷史資料圖表。

// 載入資料庫連線函式庫，這支檔案中會包含連線 MySQL 的 Connection() 函式
include("../Connections/iotcnn.php");

// 呼叫 Connection() 建立與 MySQL 資料庫的連線，並儲存在 $link 變數中
$link = Connection();

/*
輸出格式範例（JSON）：
{
  "Device": "246F28248CE0",
  "Datalist": [
    {
      "Datetime": "20220101",
      "Temperature": "23",
      "Humidity": "23"
    }
  ]
}
*/

// 定義一個類別 maindata，用來包裝裝置 MAC 與資料陣列
class maindata {
  public $Device;       // 裝置的 MAC 位址
  public $Datalist;     // 資料清單 (subdata 陣列)
}

// 定義子資料類別 subdata，用來表示每一筆資料記錄
class subdata {
  public $DateTime;       // 資料時間戳（字串）
  public $Temperature;    // 溫度（浮點數）
  public $Humidity;       // 濕度（浮點數）
}

// 建立一個 maindata 類別的實體，用來儲存查詢結果
$maindata = new maindata();

if (!isset($_GET["MAC"])) {
    echo "no MAC Data <br>"; // 如果 MAC 參數未設定，輸出錯誤訊息
}

if (!isset($_GET["start"])) {
    echo "no Start Date <br>"; // 如果開始日期參數未設定，輸出錯誤訊息
}

if (!isset($_GET["end"])) {
    echo "no End Date <br>"; // 如果結束日期參數未設定，輸出錯誤訊息
}

// 取得網址傳來的 GET 參數（感測器 MAC、開始時間、結束時間）
$sid = $_GET["MAC"];     // 裝置 MAC
$s1 = $_GET["start"];    // 查詢開始時間（格式：YYYYMMDD）
$s2 = $_GET["end"];      // 查詢結束時間（格式：YYYYMMDD）

// SQL 查詢語句樣板：從資料表 big.dhtdata 中查詢指定裝置 MAC，並且 systime 在起始與結束時間區間的資料，依時間排序
$qry1 = "select * from dhtdata where mac = '%s' and systime >= '%s' and systime <= '%s' order by systime asc";

// 使用 sprintf 將 MAC、開始與結束時間帶入 SQL 語句中，組成實際查詢字串
$qrystr = sprintf($qry1, $sid, $s1, $s2);

// 執行 SQL 查詢
$result = mysqli_query($link, $qrystr);

// 統計查詢結果的筆數
$count = mysqli_num_rows($result);

// 若有資料才進行處理
if ($count > 0) {
  $dd = array();  // 用來儲存所有 subdata 資料的陣列

  // 使用迴圈逐筆讀取查詢結果
while($row = mysqli_fetch_array($result)) 
{
  $subdata = new subdata();  // 建立一個 subdata 實體
  $subdata->DateTime = $row["systime"];  // 取得時間欄位（systime）
  $subdata->Temperature = (double)sprintf("%5.1f", (double)$row["temperature"]);  // 四捨五入溫度，保留一位小數
  $subdata->Humidity = (double)sprintf("%5.1f", (double)$row["humidity"]);        // 四捨五入濕度，保留一位小數

  // 將 subdata 實體加入資料陣列
  array_push($dd, $subdata);
}

  // 將裝置 MAC 及子資料陣列指派到 maindata 物件中
  $maindata->Device = $sid;
  $maindata->Datalist = $dd;

  // 將 maindata 物件轉成 JSON 格式並輸出
  // JSON_UNESCAPED_UNICODE 用來避免中文字被轉成 Unicode 編碼
  echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
}

    // 釋放查詢結果資源
    mysqli_free_result($result);

    // 關閉資料庫連線
    mysqli_close($link);

?>
=======
//http://localhost:8888/bigdata/dhtdata/dht2jsonwithdate.php?MAC=246F28248CE0&start=20200406&end=20200407
//include("../comlib.php");		//使用資料庫的呼叫程式
include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
$link=Connection();		//產生mySQL連線物件
/*
{
	"Device":"E89F6DE8F3BC",
	"Datalist":[
	{"Datetime":"20220101",
	"Temperature":"23",
	"Humidity":"23"
	}
	]
}

*/

class maindata{
	public $Device ;        //MAC裝置編號
	public $Datalist ;      //資料json 資料的內容
 }
 class subdata{
	public $DateTime ;      //日期時間的陣列
	public $Temperature ;     //溫度值陣列
	public $Humidity ;          //濕度值得陣列
 }
 $maindata = new maindata() ;   //產生class maindata 的實體

$sid=$_GET["MAC"];		//取得GET參數 : MAC
$s1=$_GET["start"];		//取得GET參數 : start
$s2=$_GET["end"];		//取得GET參數 : end
//select * FROM dhtdata where mac = '3C71BFFD882C' and systime >= "20210101" and systime <= "20211231" order by systime asc 
//select * FROM dhtdata where mac = '%s' and systime > '%s' and systime < '%s'

$qry1 = "select * from big.dhtdata where mac = '%s' and systime >= '%s' and systime <= '%s' order by systime asc " ;		//將dhtdata的資料找出來
$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來

//echo $qrystr."<br>" ;
$result= mysqli_query($link ,$qrystr );		//找出多少筆
$cnt= 1 ;
$count = mysqli_num_rows($result) ;
//echo $count."<br>";
if ($count >0)
{
	$dd = array() ;     //存$DateTime+$Temperatu +$Humidity之 json document 陣列


while($row = mysqli_fetch_array($result))   //進入讀取資料的迴圈
	{

		$subdata = new subdata() ;   //產生class subdata 的實體'
//		$subdata->DateTime = substr($row["systime"],4,10);	
		$subdata->DateTime = $row["systime"];	
		$subdata->Temperature = (double)sprintf("%5.1f",(double)$row["temperature"]);	
		$subdata->Humidity = (double)sprintf("%5.1f",(double)$row["humidity"] );
		array_push($dd ,$subdata ) ;

		 //將取得的濕度值，只留一位小數點，丟入$d3陣列  
	}
	$maindata->Device = $sid ;
	//$maindata->Device = "aaaaa" ;
  	 $maindata->Datalist = $dd;	
	echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
	//json_encode()把json物件，轉成json 文字，echo到網頁
}


 mysqli_free_result($result);	// 關閉資料集

 mysqli_close($link);		// 關閉連線
 

?>
>>>>>>> db2f7144f049a9ca8d54308bd388ef87fc78de9f
