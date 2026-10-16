
<?php
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件

//SELECT * FROM `rfidlist` WHERE rfidkey = '2345678901' and activate = 1 and '20250509200101' >= startdatetime and '20250509200101' <= enddatetime;
/*
{
	"Device":"2345678901",
	"Result" :"Find"/"notFind"
}
*/
class maindata
{
	public $Device ;        //RFID裝置卡號
	public $Result ;      //是否准許進入，"Find" is pass, "notFind" is not valid
 }
 $maindata = new maindata() ;   //產生class maindata 的實體
 
 if(!isset($_GET["KEY"]))//是否存在"KEY"的参数
        {
           // echo "RFID KEY lost <br>" ;
			die();
        }        
        //echo "GET KEY DATA passed <br>" ;       
        

      
//	mysqli_select_db($link, "ncnuiot") ;
	$temp0=$_GET["KEY"];		//取得POST參數 : MAC address

	$sysdt = getdatetime() ;

//	$ddt = getdataorder() ;
	//echo "(".trim($temp0).")<br>";
	if (trim($temp0) == "")//KEY 是否空白的参数
	{
		//echo "RFID KEY is empty string <br>" ;
		die();
	}

//	
	//http://iot.arduino.org.tw:8888/bigdata/rfid/dataadd.php?KEY=1234567890&ACT=1
	//http://localhost:8888/bigdata/rfid/rfidpass.php?KEY=2345678901
	
	//SELECT * FROM `rfidlist` WHERE rfidkey = '2345678901' and activate = 1 and '20250509200101' >= startdatetime and '20250509200101' <= enddatetime;
	//select * from rfidlist where rfidkey = '%s' and activate = 1 and '%s' >= startdatetime and '%s' <= enddatetime;
	

            
     $qry1 = "select * from rfidlist where rfidkey = '%s' and activate = 1 and '%s' >= startdatetime and '%s' <= enddatetime;" ;
	$qrystr = sprintf($qry1,$temp0,$sysdt,$sysdt) ;
	//組成新增到dhtdata資料表的SQL語法
	//echo $qrystr."<br>" ;
	
	$result= mysqli_query($link ,$qrystr );		//找出多少筆
	$cnt= 1 ;
	$count = mysqli_num_rows($result) ;
	if ($count >0)
	{
		$passok = "Find" ;
	}
	else
	{	
		$passok = "notFind" ;
	}
	$maindata->Device = $temp0 ;
	//$maindata->Device = "aaaaa" ;
  	 $maindata->Result = $passok ;



			;			//執行SQL語法
	//	echo "<br>" ;

	echo json_encode($maindata, JSON_UNESCAPED_UNICODE);	
 
	 mysqli_free_result($result);	// 關閉資料集

	 mysqli_close($link);		// 關閉連線
?>
    <?php
         /* Defining a PHP Function */
         function getdataorder($dt) {
        	//   $dt = getdate() ;
				$splitTimeStamp = explode(" ",$dt);
				$ymd = $splitTimeStamp[0] ;
				$hms = $splitTimeStamp[1] ;
				$vdate = explode('-', $ymd);
				$vtime = explode(':', $hms);
				$yyyy =  str_pad($vdate[0],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($vdate[1] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($vdate[2] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($vtime[0] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($vtime[1] ,2,"0",STR_PAD_LEFT);
				$sec  =  str_pad($vtime[2] ,2,"0",STR_PAD_LEFT);
			/*
				echo "***(" ;
				echo $dt ;
				echo "/" ;
				echo $yyyy ;
				echo "/" ;
				echo $mm ;
				echo "/" ;
				echo $dd ;
				echo "/" ;
				echo $hh ;
				echo "/" ;
				echo $min ;
				echo "/" ;
				echo $sec ;
				echo ")<br>" ;
			*/
			return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
         function getdataorder2($dt) {
        	//   $dt = getdate() ;
				$splitTimeStamp = explode(" ",$dt);
				$ymd = $splitTimeStamp[0] ;
				$hms = $splitTimeStamp[1] ;
				$vdate = explode('-', $ymd);
				$vtime = explode(':', $hms);
				$yyyy =  str_pad($vdate[0],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($vdate[1] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($vdate[2] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($vtime[0] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($vtime[1] ,2,"0",STR_PAD_LEFT);
				
			return ($yyyy.$mm.$dd.$hh.$min)  ;
         }
         function getdatetime() {
           $dt = getdate() ;
				$yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($dt['minutes'] ,2,"0",STR_PAD_LEFT);
				$sec  =  str_pad($dt['seconds'] ,2,"0",STR_PAD_LEFT);

			return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
		          function trandatetime1($dt) {
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,4,2);
				$dd  =   substr($dt,6,2);
				$hh  =   substr($dt,8,2);
				$min  =   substr($dt,10,2);
				$sec  =   substr($dt,12,2);

			return ($yyyy."/".$mm."/".$dd." ".$hh.":".$min.":".$sec)  ;
		 }
      ?>
