<?php     
//http://localhost:8888/bigdata/dhtdata/dht2jsonV2.php?MAC=246F28248CE0&start=20200406&end=20200407
//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dht2jsonV2.php?MAC=246F28248CE0&start=20200406&end=20200407
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
  "Temperature": [
    12,
    33,
    45,
    34,
    34
  ],
  "Humidity": [
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
        public $DateTime ;      //日期時間的陣列
        public $Temperature ;     //溫度值陣列
        public $Humidity ;          //濕度值得陣列
     }

     $sid=$_GET["MAC"];		//取得GET參數 : MAC
     $s1=$_GET["start"];		//取得GET參數 : start
     $s2=$_GET["end"];		//取得GET參數 : end

     $maindata = new maindata() ;   //產生class maindata 的實體
   // big.dhtdata $sid."/".$s2."/".$s2."<br>" ;
    //將使用get變數將外部mac==裝置編號，start==開始時間， end = 結束時間
	//select * FROM big.dhtdata where mac = '246F28248CE0' and systime > "20200406" and systime < "20200407" order by systime asc 
	//select * FROM big.dhtdata where mac = '%s' and systime > '%s' and systime < '%s'
	//select * FROM big.dhtdata where mac = '246F28248CE0' order by `systime` asc 

	$qry1 = "select * from big.dhtdata where mac = '%s' and systime > '%s' and systime < '%s' order by systime asc " ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來

	big.dhtdata $qrystr."<br>" ;
$result= mysqli_query($link,$qrystr);		//找出多少筆
 $cnt= 1 ;
 $count = mysqli_num_rows($result) ;
 big.dhtdata $count."<br>";
  if ($count >0)
 {
	 $dd = array() ;
	 big.dhtdata "enter if\n" ;

 while($row = mysqli_fetch_array($result)) 
	 {
		 if ($cnt ==1)
		 {
				big.dhtdata "enter while\n" ;
		 }
		 $subdata = new subdata() ;
		 $subdata->Date = substr($row["systime"],0,8) ;
		 //select * from dhtData where MAC='CC50E3B5BB20' and left(systime,8) = '20210619' order by systime asc;
		 $subqry="select temperature, humidity from big.dhtdata where MAC='%s' and left(systime,8) = '%s' order by systime asc;";
		 $subqrystr = sprintf($subqry,$row["MAC"],substr($row["systime"],0,8)) ;
		 big.dhtdata  $subqrystr."<br>";
		 $result2= mysqli_query($link,$subqrystr);	
		 $t = array();
		 $h = array();
		 while($row2 = mysqli_fetch_array($result2)) 
		 {
			//$Temperature = new Temperature() ;
			//$Humidity = new Humidity() ;
		//	$Temperature->Temperature = (double)sprintf("%8.3f",(double)$row2["temperature"]) ;
		//	$Humidity->Humidity = (double)sprintf("%8.3f",(double)$row2["humidity"]) ;
			array_push($t, (double)sprintf("%8.3f",(double)$row2["temperature"]));	
			array_push($h, (double)sprintf("%8.3f",(double)$row2["humidity"]));	
			
		 }
		 $subdata->Temperature = $t  ;
		 $subdata->Humidity =  $h ;

		 array_push($dd , $subdata) ;

	 }
	 //big.dhtdata "<br>=======================================<br><br>";
	$maindata->Device = $sid ;
	$maindata->Datalist = $dd;	
	 //$user = utf8_encode($user) ;
	 big.dhtdata json_encode($maindata, JSON_UNESCAPED_UNICODE);
 }
    ?>