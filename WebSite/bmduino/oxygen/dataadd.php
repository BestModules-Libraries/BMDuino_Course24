
<?php
    include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
            //	Connection() ;
    $link=Connection();		//產生mySQL連線物件
//----------------------------------------------
    if (!isset($_GET["MAC"]))   //Check MAC Address is passed
    {
        echo "MAC Address lost <br>" ;
        die(); // 停止執行程式
    }
    echo "MAC Address DATA  passed <br>" ;
//----------------------------------------------
    if (!isset($_GET["O"]))   //Check MAC Address is passed
    {
        echo "Blood Oxygen data lost <br>" ;
        die(); // 停止執行程式
    }

    echo "Blood Oxygen data  passed <br>" ;
    
//----------------------------------------------
    if (!isset($_GET["HB"]))   //Check MAC Address is passed
    {
        echo "Heart Beat is  lost <br>" ;
        die(); // 停止執行程式
    }

    echo "Heart Beat DATA passed <br>" ;    
//----------------------------------------------

    $ip = getenv("REMOTE_ADDR");	//取得用戶端連線IP Address
    echo "ip:".$ip."<br>";
    $sysdt = getdatetime() ;    //取得系統日期時間
//----------------------------------------------

    $temp0=$_GET["MAC"];		//取得POST參數 : MAC address
    $temp1=$_GET["O"];		//取得POST參數 : 距離值
    $temp2=$_GET["HB"];		//取得POST參數 : 距離值

//----------------------------------------------
    if (trim($temp0) == "") {
        echo "MAC Address DATA empty string <br>" ;
        die();
    }
 
//----------------------------------------------

    //http://iot.arduino.org.tw:8888/bigdata/oxygen/dataadd.php?MAC=AABBCCDDEEFF&O=96.89&HB=89
    //http://localhost:8888/bigdata/oxygen/dataadd.php?MAC=AABBCCDDEEFF&O=96.89&HB=89


    //INSERT INTO `bloodoxygen` (`id`, `crtdatetime`, `systime`, `MAC`, `IP`, `blood`, `heartbeat`) VALUES (NULL, current_timestamp(), '20250530191601', 'AABBCCDDEEFF', '192.168.99.100', '97.5', '89');

    $qry = "insert into bloodoxygen (systime, MAC, IP, blood, heartbeat) values ('%s', '%s', '%s', %6.2f, %d);"; 
    //組成新增到dhtdata資料表的SQL語法
    $qrystr = sprintf($qry,$sysdt,$temp0,$ip,$temp1,$temp2) ;   //產生加入bloodoxygen資料表新一筆資料
    echo $qrystr ;
    echo "<br>" ;
//----------------------------------------------
    
    if (mysqli_query($link, $qrystr))    //執行SQL語法
        {
            echo "Successful <br>" ;
        }
        else
        {
            echo "Fail <br>" ;
        }

    // 關閉資料庫連線
    mysqli_close($link);


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
