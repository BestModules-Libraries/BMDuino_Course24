<?php     
   //http://ncnu.arduino.org.tw:9999/dhtdata/dht2jsonwithday.php?MAC=CC50E3B5BB20&start=20220101&end=20221231  
  //dht2jsonwithday
    //http://192.168.88.200:9999/dhtdata/dht2jsonwithday.php?MAC=CC50E3B5BB20&start=20220101&end=20221231  
    //include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
  	$link=Connection();		//產生mySQL連線物件
	/*
    {
      "Device": "3C71BFFD882C",
      "Datalist": [
        {
          "Datetime": "20210429",
          "Temperature": " 64.18",
          "Humidity": " 28.94"
        }
      ]
}
	
	*/
class maindata
{
	public $Device ;        //MAC裝置編號
        public $Datalist ;        //MAC裝置編號
 }
 class subdata{
	public $Datetime ;          //濕度值得陣列
	public $Temperature ;          //濕度值得陣列
	public $Humidity ;          //濕度值得陣列
 }
 $maindata = new maindata() ;   //產生class maindata 的實體        
	//$jsonarray= "{\"Device\":\"%s\",\"Datalist\":[%s]}" ;
	//$jsonrow= "{\"Datetime\":\"%s\",\"Temperature\":\"%s\",\"Humidity\":\"%s\"}" ;
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
	//select * FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= "20210101" and systime <= "20211231" order by systime asc 
	//select * FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= "20210101" and systime <= "20211231" group by left(systime,8) order by systime asc
	//select MAC,left(systime,8) as day, average(temperature) as temp, average(humidity) as humid   FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= "20210101" and systime <= "20211231" group by left(systime,8) order by systime asc
	//select MAC,left(systime,8) as day, avg(temperature) as temp, avg(humidity) as humid FROM ncnuiot.dhtData where mac = 'CC50E3B5BB20' and systime >= '20210101' and systime <= '20211231' group by left(systime,8) order by systime asc;	
	//select MAC,left(systime,8) as day, avg(temperature) as temp, avg(humidity) as humid FROM ncnuiot.dhtData where mac = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc;
	
	$qry1 = "select MAC,left(systime,8) as day, avg(temperature) as temp, avg(humidity) as humid FROM ncnuiot.dhtData where mac = '%s' and systime >= '%s' and systime <= '%s' group by left(systime,8) order by systime asc" ;		//將dhtdata的資料找出來
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;		//將dhtdata的資料找出來
	
	//echo $qrystr."<br>" ;

	$result= mysqli_query($link,$qrystr);		//找出多少筆
	$cnt= 1 ;
	$count = mysqli_num_rows($result) ;
	//echo $count."<br>";

	if ($count >0)
	{
	  $dd = array() ;  

	while($row = mysqli_fetch_array($result)) 
            {
    //$jsonrow = "{\"stockno\":\"%s\",\"companyname\":\"%s\"}" ;
                    $tmp1 = sprintf("%10.2f",(float)($row['temp']))  ;	
                    $tmp2 = sprintf("%10.2f",(float)($row['humid']))  ;
                    $subdata = new subdata() ;   //產生class subdata 的實體'
                    $subdata->Datetime = $row['day'];	
                    $subdata->Humidity = $tmp1;	
                    $subdata->Temperature = $tmp2;	
                    array_push($dd ,$subdata ) ;    
            }
		//echo "<br>=======================================<br><br>";
                $maindata->Device = $sid ;
                $maindata->Datalist = $dd ;

            echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
	}
	
	 mysqli_free_result($result);	// 關閉資料集
 
	 mysqli_close($link);		// 關閉連線


    ?>