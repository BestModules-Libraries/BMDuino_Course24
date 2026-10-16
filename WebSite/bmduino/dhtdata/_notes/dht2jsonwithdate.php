<?php     
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