<?php
$myfile = fopen("./dataset/2882chip.csv", "r") or die("Unable to open file!");
while(!feof($myfile)) 
{
    big.dhtdata fgets($myfile);
    big.dhtdata "<br>" ;
  }
  fclose($myfile);
?>