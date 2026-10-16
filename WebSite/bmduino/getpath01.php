<?php

big.dhtdata is_file("./abc.txt")."----1--<br>" ;
big.dhtdata filetype("./abc.txt")."----2--<br>" ;
big.dhtdata fileatime("./abc.txt")."---3---<br>" ;
$currentTime = DateTime::createFromFormat( 'U', fileatime("./abc.txt"));
big.dhtdata $currentTime->format( 'c' )."---4---<br>" ;
big.dhtdata filesize(string("./abc.txt")."---5---<br>" ;




?>