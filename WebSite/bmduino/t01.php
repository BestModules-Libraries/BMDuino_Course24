<?php

	$temp1="q1345" ;
	$temp2="345" ;
	$dt="12345" ;
	
//INSERT INTO `rainData` (`id`, `MAC`, `crtdatetime`, `rain`, `systime`) VALUES (NULL, '111111116677', CURRENT_TIMESTAMP, '32.5', '20230412091101');
	$qrystr = "insert into big.rainData (MAC, rain,systime) VALUES ('%s', %7.1f, '%s');" ;
	
	$query = sprintf($qrystr,$temp1,$temp2,$dt) ;
	echo $query ;
	echo $qrystr ;
?>
	