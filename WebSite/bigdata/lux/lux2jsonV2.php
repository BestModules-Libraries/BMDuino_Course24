<?php     
//http://localhost:8888/bigdata/dhtdata/dht2jsonV2.php?MAC=246F28248CE0&start=20200406&end=20200407
    //include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件
	/*
{
  "Device": "3C71BFFD882C",
  
  "DateTime": [
    "20200301",
    "20200303",
    "20200304",
    "20200305",
    "20200309"
  ],
  "Lux": [
    12,
    33,
    45,
    34,
    34
  ]
}
	
	*/

    class maindata{
        public $Device ;        //MAC裝置編號
        public $StartDateTime ;        //資料開始日期時間
		public $EndDateTime ;        //資料結束日期時間		        
        public $DateTime ;      //日期時間的陣列
        public $Lux ;     //亮度值陣列
     }

     $sid=$_GET["MAC"];		//取得GET參數 : MAC
     $s1=$_GET["start"];		//取得GET參數 : start
     $s2=$_GET["end"];		//取得GET參數 : end

     $maindata = new maindata() ;   //產生class maindata 的實體
   // echo $sid."/".$s2."/".$s2."<br>" ;
    //將使用get變數將外部mac==裝置編號，start==開始時間， end = 結束時間
	//select * FROM big.dhtData where mac = '246F28248CE0' and systime > "20200406" and systime < "20200407" order by systime asc 
	//select * FROM big.dhtData where mac = '%s' and systime > '%s' and systime < '%s'
	//select * FROM big.dhtData where mac = '246F28248CE0' order by `systime` asc 

	$qry1 = "select * from big.lux where mac = '%s' and systime > '%s' and systime < '%s' order by systime asc " ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來

	//echo $qrystr."<br>" ;
	$result= mysqli_query($link, $qrystr , );		//找出多少筆
	$count = mysqli_num_rows($result) ;
	//echo $count."<br>";
	
	if ($count >0)
	{
        $d1 = array() ;     //存$DateTime 之陣列
        $d2 = array() ;     //存$Lux 之陣列

	while($row = mysqli_fetch_array($result))   //進入讀取資料的迴圈
		{

            array_push($d1 , substr($row["systime"],4,10)) ;
            //將取得的日期時間，去掉yyyy， 丟入$d1陣列
            array_push($d2 , (double)sprintf("%8.1f",(double)$row["luxvalue"])) ;
            //將取得的LUX，只留一位小數點，丟入$d2陣列  
        }
        $maindata->Device = $sid ;
        $maindata->StartDateTime = $s1 ;
        $maindata->EndDateTime = $s2 ;        
        //$maindata->Device = "aaaaa" ;
        $maindata->DateTime = $d1;	
       $maindata->Lux = $d2;	
        echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
        //json_encode()把json物件，轉成json 文字，echo到網頁
	}
	
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線
    ?>