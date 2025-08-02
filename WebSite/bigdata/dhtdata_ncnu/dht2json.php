<?php     
  //http://ncnu.arduino.org.tw:9999/dhtdata/dht2json.php?MAC=3C71BFFD882C&dt1=20210101&dt2=20211231  
  //http://192.168.88.200:9999/dhtdata/dht2json.php?MAC=3C71BFFD882C&dt1=20210101&dt2=20211231
    include("../jsonlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件
	/*
	{
		"Device":"3C71BFFD882C",
		"Temperature":[12,33,45,34,34],
		"Humidity":[12,33,45,34,34]
	}
	
	*/
  class maindata
  {
	public $Device ;        //MAC裝置編號
	public $Temperature ;        //MAC裝置編號
	public $Humidity ;        //MAC裝置編號
 }

 $maindata = new maindata() ;   //產生class maindata 的實體       
        
	//$jsonarray= "{\"Device\":\"%s\",\"Temperature\":[%s],\"Humidity\":[%s]}" ;

 if  (!isset($_GET["MAC"])) 
	{
		echo "no MAC Data <br>" ;
	}
		
	if (!isset($_GET["dt1"])) 
	{
		echo "no Start DateTime <br>" ;	
	}

	
	if (!isset($_GET["dt2"])) 
	{
            echo "no End DateTime <br>" ;		
	}

	$sid=$_GET["MAC"];		//取得POST參數 : field1
	$s1=$_GET["dt1"];		//取得POST參數 : field1
	$s2=$_GET["dt2"];		//取得POST參數 : field1
	//select * FROM ncnuiot.dhtData where mac = '3C71BFFD882C' and systime > "20210101" and systime < "20211231" order by systime asc 
	//select * FROM ncnuiot.dhtData where mac = '%s' and systime > '%s' and systime < '%s'
	
	$qry1 = "select * from ncnuiot.dhtData where mac = '%s' and systime > '%s' and systime < '%s' order by systime asc " ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來

	//echo $qrystr."<br>" ;
	$result= mysqli_query($link,$qrystr);		//找出多少筆
	$count = mysqli_num_rows($result) ;
	//echo $count."<br>";
	
	if ($count >0)
	{
            $d1 = array() ;  
            $d2 = array() ;  
	while($row = mysqli_fetch_array($result)) 
		{
	//$jsonrow = "{\"stockno\":\"%s\",\"companyname\":\"%s\"}" ;

			$tmp1 = sprintf("%10.2f",(float)($row['temperature']))  ;	
			$tmp2 = sprintf("%10.2f",(float)($row['humidity']))  ;	
			//$tmp1 = string2float($row['temperature'],8,3)  ;	
			//$tmp2 =  string2float($row['humidity'],8,3)  ;	
			//$tmp1 = sprintf("%6.2f",$row['temperature'])  ;	
			//$tmp2 = sprintf("%6.2f",$row['humidity'])  ;                     //   echo $tmp1."/".$tmp2."<br>" ;
           //             $t1 = (float)((int)((float)$tmp1*100))/10 ;  
           //             $t2 = (float)((int)((float)$tmp2*100))/10 ; 
            //            echo $t1."/".$t2."<br>" ;
			//array_push($d1 ,(float)$tmp1) ;
                        //array_push($d2 ,(float)$tmp2) ;
			array_push($d1 ,$tmp1) ;
                        array_push($d2 ,$tmp2) ;

		}
		//echo "<br>=======================================<br><br>";
                $maindata->Device = $sid ;
                $maindata->Temperature = $d1 ;
                $maindata->Humidity = $d2 ;
             echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
            //json_encode()把json物件，轉成json 文字，echo到網頁
	}
	
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線
    ?>