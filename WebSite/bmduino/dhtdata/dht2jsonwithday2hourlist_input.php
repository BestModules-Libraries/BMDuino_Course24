<?php     
//http://localhost:8888/bigdata/dhtdata/dht2jsonwithday2hourlist.php?MAC=246F28248CE0&day=20200406
//http://iot.arduino.org.tw:8888/bigdata/dhtdata/dht2jsonwithday2hourlist.php?MAC=246F28248CE0&day=20200406

   //http://localhost:88888/ Host URL
 //Restful API 的程式：：/dhtdata/dht2jsonwithday2hourlist.php?MAC=E89F6DE8F3BC&day=20200406
 //傳入的參數：??MAC=E89F6DE8F3BC&start=20200101&end=20221231
 class maindata{
	 public $Device ; 
	 public $Day ; 
	 public $Datalist ; 
  }
  class subdata{
	public $Total ;
	public $Hour ;
	public $Temperature ; 
	public $Humidity ; 
  }
  class Temperature{
	public $Temperature ;
  }
  class Humidity{
	public $Humidity ;
  }
  $t = array();
  $h = array();
 
 //include("../comlib.php");		//使用資料庫的呼叫程式
	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
   $link=Connection();		//產生mySQL連線物件

 $maindata = new maindata() ;

 $sid=$_GET["MAC"];		//取得POST參數 : 裝置的MAC 網路卡編號
 $s1=$_GET["day"];		//取得POST參數 : 開始日期  YYYYMMDDHHMMSS
 //echo $sid."/".$s1."<br>" ;
	// 取一天：SELECT * FROM `dhtdata` WHERE left(systime,8) = '20200406';
	//取出時間的小時: SELECT * FROM `dhtdata` WHERE left(systime,8) = '20200406' group by left(systime,10);
	//取出小時: SELECT *, left(systime,8) as day , left(systime,10) as dayhour, right(left(systime,10),2) as hour FROM `dhtdata` WHERE left(systime,8) = '20200406' group by left(systime,10);
	
	//select MAC,left(systime,8) as systime, avg(temperature) as temperature, avg(humidity) as humidity FROM dhtData where MAC = 'CC50E3B5BB20' and systime >= '20210101' and systime <= '20211231' group by left(systime,8)  order by systime asc;
	//select MAC,left(systime,8) as systime, avg(temperature) as temperature, avg(humidity) as humidity FROM dhtData where MAC = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc;

 $qry1 = "select *, count(*) as cnt, left(systime,8) as day , left(systime,10) as dayhour, right(left(systime,10),2) as hour FROM big.dhtdata where MAC = '%s' and left(systime,8)= '%s' group by left(systime,10) order by hour asc ;" ;		//將dhtdata的資料找出來
 
 $qrystr = sprintf($qry1 , $sid, $s1) ;		//將dhtdata的資料找出來

//	echo $qrystr."<br>" ;

 $result= mysqli_query($link,$qrystr);		//找出多少筆
 $cnt= 1 ;
 $count = mysqli_num_rows($result) ;
 
 //echo $count."<br>";

 if ($count >0)
 {
	  $alldata=array() ;

 while($row = mysqli_fetch_array($result)) 
	 {
	// echo $row["dayhour"]."<br>";
		
		 $subdata = new subdata() ;
	//	 echo "1<br>" ;
		//$subdata->Count = (int)$row["cnt"] ;
	//	$subdata->Hour = $row["Hour"] ;	
		 //select * from dhtData where MAC='CC50E3B5BB20' and left(systime,8) = '20210619' order by systime asc;
		 $subqry="select temperature, humidity from big.dhtdata where MAC='%s' and left(systime,10) = '%s' order by systime asc;";
	//	 echo "2<br>" ;
		 $subqrystr = sprintf($subqry,$row["MAC"],$row["dayhour"]) ;
	//	 echo "3<br>" ;
	//	 echo  "-----".$subqrystr."<br>";
		 
		 $result2= mysqli_query($link,$subqrystr);
		 $t = array();
		 $h = array();
	//	 echo "=aaa===\<br>" ;
		
		 while($row2 = mysqli_fetch_array($result2)) 
		 {
			 //取每一個小時的temperature & Humidity
		//	$Temperature = new Temperature() ;
		//	$Humidity = new Humidity() ;
		//	$Temperature->Temperature = (double)sprintf("%8.3f",(double)$row2["temperature"]) ;
		//	$Humidity->Humidity = (double)sprintf("%8.3f",(double)$row2["humidity"]) ;
			array_push($t, sprintf("%8.3f",(double)$row2["temperature"]));	
			array_push($h, sprintf("%8.3f",(double)$row2["humidity"]));	
		//	echo "---".$t."/".$h."<br>";
		 }
		 
		
		 //把每一個小時的Temperature * Humidity 傳到陣列


		 $subdata->Total = $row["cnt"]  ;
		 $subdata->Hour = $row["hour"]  ;
		
		 $subdata->Temperature = $t  ;
		 $subdata->Humidity =  $h ;
		 
		array_push($alldata, $subdata);	
		 /*
		每一個小時的Temperature * Humidity 傳到每一個小時的資料陣列
		// array_push($dd , $subdata) ;
		
		*/
	 }
	  
	
	// echo "<br>=======================================<br><br>";
	$maindata->Device = $sid ;
	$maindata->Day = $s1 ;
	
	$maindata->Datalist = $alldata;	
	
	 //$maindata = utf8_encode($maindata) ;
	 echo json_encode($maindata, JSON_UNESCAPED_UNICODE);

	
 }
 
  mysqli_free_result($result);	// 關閉資料集

  mysqli_close($link);		// 關閉連線
 ?>