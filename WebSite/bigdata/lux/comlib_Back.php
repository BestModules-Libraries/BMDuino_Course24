<?php

function getqtyinfo($dd)	//$dd must be integer
{
	$rr= "value='%d' min='%d' max='%d'" ;
	if ($dd <1)
	{
			return sprintf($rr,0,0,0) ;
	}
	else
	{
			return sprintf($rr,1,1,$dd) ;
	}
		

}
function GetYear($dd)
{
    //echo $dd."-----<br>" ;
    //echo gettype($dd)."====<br>" ;
    $tmp = DateTime::createFromFormat('Y-m-d', $dd);

    return $tmp->format('Y');

}
function GetMonth($dd)
{
   // echo $dd."-----<br>" ;
    $tmp = DateTime::createFromFormat('Y-m-d', $dd);

    return $tmp->format('m');

}
function GetDay($dd)
{
    //echo $dd."-----<br>" ;
    $tmp = DateTime::createFromFormat('Y-m-d', $dd);

    return $tmp->format('d');

}
		function makekey($cc)
		{
			
			return md5($cc) ;
		}
		function cryptkey($cc)
		{
			
			return crypt($cc,rand(0,99)) ;
		}
		function comparekey($c1,$c2)
		{
			if ($c1 == $c2)
				return 1 ;
			else
				return 1 ;
				
		}

		 function twdate2date($ddt) 
		 {
          		$yyyy = (string)((int)substr($ddt,0,3)+1911);
				$mm  =  (string)((int)substr($ddt,4,2));
				$dd  =  (string)((int)substr($ddt,7,2));;

			return ($yyyy."/".$mm."/".$dd)  ;
         }
         /* Defining a PHP Function */
         function getdataorder() {
           $dt = getdate() ;
				$yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($dt['minutes'] ,2,"0",STR_PAD_LEFT);
				$sec  =  str_pad($dt['seconds'] ,2,"0",STR_PAD_LEFT);

			return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }
         function getdataorder2() {
           $dt = getdate() ;
				$yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad($dt['minutes'] ,2,"0",STR_PAD_LEFT);

			return ($yyyy.$mm.$dd.$hh.$min)  ;
         }
         function getdataorderten() {
           $dt = getdate() ;
				$yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
				$mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
				$dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
				$hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
				$min  =  str_pad((integer)($dt['minutes']/10) ,1,"0",STR_PAD_LEFT);

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

			return ($yyyy."/".$mm."/".$dd." ".$hh.":".$min.":".$sec)  ;
         }
		
         function trandatetime0($dt) 
		 {
			 	//2021-05-06 09:39:42
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,5,2);
				$dd  =   substr($dt,8,2);
				$hh  =   substr($dt,11,2);
				$min  =   substr($dt,14,2);
				$sec  =   substr($dt,17,2);

			return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
         }		

         function trandatetime($dt) {
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,4,2);
				$dd  =   substr($dt,6,2);
				$hh  =   substr($dt,8,2);
				$min  =   substr($dt,10,2);
				$sec  =   substr($dt,12,2);

			return ($yyyy."/".$mm."/".$dd." ".$hh.":".$min)  ;
		 }
         function trandatetime2($dt) {
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,4,2);
				$dd  =   substr($dt,6,2);
				$hh  =   substr($dt,8,2);
				$min  =   substr($dt,10,2);
				$sec  =   substr($dt,12,2);

			return ($mm."/".$dd." ".$hh.":".$min)  ;
		 }

         function trandatetime3($dt) {
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,4,2);
				$dd  =   substr($dt,6,2);
				$hh  =   substr($dt,8,2);
				$min  =   substr($dt,10,2);
				$sec  =   substr($dt,12,2);

			return ($mm."/".$dd." <br>".$hh.":".$min)  ;
		 }
         function trandatetime4($dt) {
				$yyyy =  substr($dt,0,4);
				$mm  =   substr($dt,4,2);
				$dd  =   substr($dt,6,2);
				$hh  =   substr($dt,8,2);
				$min  =   substr($dt,10,2);
				$sec  =   substr($dt,12,2);

			return ($mm."/".$dd."  ".$hh.":".$min)  ;
		 }

		function uniztime2datetime($udt)
		{
			return date("Y-m-d H:i:s", $udt);	
		}
		
		function strzero($no,$len)
		{
			$tmp = (string)$no ;
			$l = strlen($tmp) ;
			if ($len > $l)
			{
				return str_repeat("0",$len-$l).$tmp ;
			}
			else
			{
				return $tmp ;
			}
		}
	
function getToday(){
	$today = getdate();
	date("Y/m/d H:i");  //日期格式化
	$year=$today["year"]; //年 
	$month=$today["mon"]; //月
	$day=$today["mday"];  //日
 
	if(strlen($month)=='1')$month='0'.$month;
	if(strlen($day)=='1')$day='0'.$day;
	$today=$year."-".$month."-".$day;
	//echo "今天日期 : ".$today;
 
	return $today;
}	
function getThisToday()
{
	$today = getdate();
	date("Y/m/d H:i");  //日期格式化
	$year=$today["year"]; //年 
	$month=$today["mon"]; //月
	$day=$today["mday"];  //日
 
	if(strlen($month)=='1')$month='0'.$month;
	if(strlen($day)=='1')$day='0'.$day;
	$today=$year."/".$month."/".$day;
	//echo "今天日期 : ".$today;
 
	return $today;
}
function getThisYear(){
	$today = getdate();
	date("Y/m/d H:i");  //日期格式化
	$year=$today["year"]; //年 
 
 
	return $year;
}
?>

<script>
function goBack() {
  window.history.back();
}
function goForward() {
  window.history.forward();
}

</script>