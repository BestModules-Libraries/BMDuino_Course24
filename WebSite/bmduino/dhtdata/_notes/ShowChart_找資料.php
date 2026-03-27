<?php
 
   	include("../comlib.php");		//使用資料庫的呼叫程式
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

    $mid=$_GET["MAC"];		//取得POST參數 : MAC address
    $dd1 = $_GET["dt1"] ;	
    $dd2 = $_GET["dt2"] ;	
	
    //select * from fcu.dhtdata where MAC = 'CC50E3B5BB20' and systime >= '20220101' and systime <= '20220103';
	$qry ="select * from fcu.dhtdata where MAC = '%s' and systime >= '%s' and systime <= '%s' order by systime desc " ;		//將dhtdata的資料找出來
	$qrystr=sprintf($qry,$mid,$dd1,$dd2) ;		//將dhtdata的資料找出來
			
	echo $qrystr."<br>" ;
//	int aa[] = {1,3,45,6,7} ;  == 	$aa = array(4,5,6,7,8,9);
//	int bb[] = {} ;  ==  $bb = array();
	$d00 = array();		// declare blank array of d00
	$d01 = array();	// declare blank array of d01
	$d02 = array();	// declare blank array of d02
	
	$result=mysqli_query($link,$qrystr);		//將dhtdata的資料找出來(只找最後5
//	echo "step 02 . <br>" ;
  if($result!==FALSE)
  {
	 while($row = mysqli_fetch_array($result)) 
	 {
			array_push($d00, $row["systime"]);		// $row[欄位名稱] 就可以取出該欄位資料
			array_push($d01, (double)$row["temperature"]);	// array_push(某個陣列名稱,加入的陣列的內容
			array_push($d02, (double)$row["humidity"]);
		}// 會跳下一列資料

  }
			


?>