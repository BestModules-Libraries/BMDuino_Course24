<?php     
  //  http://localhost:8888/bigdata/dhtdata/dht2json.php?MAC=246F28248CE0&start=20200406&end=20200407
  //  http://localhost:8888/bigdata/dhtdata/dht2json.php?MAC=246F28248CE0&start=20200406&end=20200407
    //include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件
	/*
	{
		"Device":"3C71BFFD882C",
		"Temperature":[12,33,45,34,34],
		"Humidity":[12,33,45,34,34]
	}
	
	*/

    class maindata{
        public $Device ;        //MAC裝置編號
        public $Temperature ;     //溫度值陣列
        public $Humidity ;          //濕度值得陣列
     }

	$sid=$_GET["MAC"];		//取得POST參數 : field1
	$s1=$_GET["start"];		//取得POST參數 : field1
	$s2=$_GET["end"];		//取得POST參數 : field1
    //將使用get變數將外部mac==裝置編號，start==開始時間， end = 結束時間
	//select * FROM big.dhtdata where mac = '246F28248CE0' and systime > "20200406" and systime < "20200407" order by systime asc 
	//select * FROM big.dhtdata where mac = '%s' and systime > '%s' and systime < '%s'
	//select * FROM big.dhtdata where mac = '246F28248CE0' order by `systime` asc 
	$maindata = new maindata() ;   //產生class maindata 的實體

	$qry1 = "select * from big.dhtdata where mac = '%s' and systime > '%s' and systime < '%s' order by systime asc " ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來

	//echo $qrystr."<br>" ;
	//mysqli_query 用法 ==> mysqli_query(連線變數，SQL 敘述變數)
	$result= mysqli_query($link ,$qrystr);		//找出多少筆
	$cnt= 1 ;
	$count = mysqli_num_rows($result) ;
	//echo $count."<br>";
	
	if ($count >0)
	{

        $d2 = array() ;     //存$Temperature 之陣列
        $d3 = array() ;     //存$Humidity 之陣列

	while($row = mysqli_fetch_array($result))   //進入讀取資料的迴圈
		{


            array_push($d2 , (double)sprintf("%5.1f",(double)$row["temperature"])) ;
            //將取得的溫度值，只留一位小數點，丟入$d2陣列  
            array_push($d3 , (double)sprintf("%5.1f",(double)$row["humidity"] )) ;
             //將取得的濕度值，只留一位小數點，丟入$d3陣列  
        }
        $maindata->Device = $sid ;
        //$maindata->Device = "aaaaa" ;
       $maindata->Temperature = $d2;	
       $maindata->Humidity = $d3;	
        echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
        //json_encode()把json物件，轉成json 文字，echo到網頁
	}
	
	
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線
    ?>