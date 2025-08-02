<?php     
   //http://ncnu.arduino.org.tw:9999/dhtdata/dht2jsonwithdaylist.php?MAC=CC50E3B5BB20&start=20220101&end=20221231  
   //http://192.168.88.200:9999/dhtdata/dht2jsonwithdaylist.php?MAC=CC50E3B5BB20&start=20220101&end=20221231  
  
    //include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件
	/*
	{
	  "Device": "CC50E3B5BB20",
	  "Datalist": [
					{
					  "Datetime": "20220102",
					  "Temperature": ["16.63","12.3","23.3"],
					  "Humidity": ["84.83","55","35"]
					}	
					]
	}
	
	*/
	
	$jsonarray= "{\"Device\":\"%s\",\"Datalist\":[%s]}" ;
	$jsonrow= "{\"Datetime\":\"%s\",\"Temperature\":[%s],\"Humidity\":[%s]}" ;
	$jsonrow2= "%s" ;
	$sid=$_GET["MAC"];		//取得POST參數 : field1
	$s1=$_GET["start"];		//取得POST參數 : field1
	$s2=$_GET["end"];		//取得POST參數 : field1
	//select * FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= "20210101" and systime <= "20211231" order by systime asc 
	//select * FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= "20210101" and systime <= "20211231" group by left(systime,8) order by systime asc
	//select MAC,left(systime,8) as day, average(temperature) as temp, average(humidity) as humid   FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= "20210101" and systime <= "20211231" group by left(systime,8) order by systime asc
	//select MAC,left(systime,8) as day FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= '20210101' and systime <= '20211231' group by left(systime,8) order by systime asc;	
	//select MAC,left(systime,8) as day FROM ncnuiot.dhtData where mac = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc;
	//-----------------
	//select * FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and left(systime,8) = "20210619" order by systime asc;
	$qry1 = "select MAC,left(systime,8) as day, avg(temperature) as temp, avg(humidity) as humid FROM ncnuiot.dhtData where mac = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc" ;		//將dhtdata的資料找出來
	$qry2 = "select * FROM ncnuiot.dhtData where mac = '%s' and left(systime,8) = '%s' order by systime asc;" ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來
	
	//echo $qrystr."<br>" ;

	$result= mysql_query($qrystr,$link);		//找出多少筆
	$cnt= 1 ;
	$count = mysqli_num_rows($result) ;
	//echo $count."<br>";

	if ($count >0)
	{
		$t1="" ;

	while($row = mysqli_fetch_array($result)) 
		{
	
			
			$qrystr2 = sprintf($qry2 , $row['MAC'], $row['day']) ;		//將dhtdata的資料找出來
			$result2= mysql_query($qrystr2,$link);		//找出多少筆
			$cnt2= 1 ;
			$count2 = mysqli_num_rows($result2) ;
			$t2="" ;
			$t3="" ;
			//echo "1-----".$qrystr2."<br>";
			while($row2 = mysqli_fetch_array($result2)) 
			{
			//------------------
			$tmp2 = sprintf($jsonrow2,$row2['temperature'])  ;	
			$tmp3 = sprintf($jsonrow2,$row2['humidity'])  ;	
	
		//	echo $tmp."<br>";
			
			$t2 = $t2.$tmp2;
			$t3 = $t3.$tmp3;
				if ($cnt2 < $count2)
				{
					$t2 = $t2."," ;
					$t3 = $t3."," ;

				}
				$cnt2=$cnt2+1 ;
			//--------------
			}
				
			$tmp1 = sprintf($jsonrow,$row['day'],$t2,$t3)  ;	
	
			//echo "2-------".$tmp1."<br>";
			
			$t1 = $t1.$tmp1;

			if ($cnt < $count)
			{
				$t1 = $t1."," ;

			}
			$cnt=$cnt+1 ;
			//echo "-----".$cnt."<br>" ;
			//echo $t1."<br>";
		}		//end of while($row = mysqli_fetch_array($result)) 
		//echo "<br>=======================================<br><br>";
		$tmpp = sprintf($jsonarray,$sid,$t1) ;
		echo $tmpp ;
	}
	
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線


    ?>