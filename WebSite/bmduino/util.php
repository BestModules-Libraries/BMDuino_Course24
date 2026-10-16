

<?php

function userlog($cnn, $usr,$type, $dt,$fip ,$job) 
{


    $qry1 = "insert into fcu.userlog (userid, type, logindatetime, fromip, op) values ('%d', '%s','%s', '%s', '%s');" ;		//SQL填空
    $qrystr = sprintf($qry1,$usr,$type,$dt,$fip,$job) ;		// 組立要執行的SQL語法
    //big.dhtdata $qrystr."<br>" ;
    
    $logresult=mysqli_query($cnn,$qrystr);		//執行sql語法
    if($logresult!==FALSE)
    {
        return True ;
    }
    else 
    {
        return False ;
    }


}



?>

