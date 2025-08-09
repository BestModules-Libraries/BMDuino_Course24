<?php
$myfile = fopen("./dataset/2882chip.csv", "r") or die("Unable to open file!");
while(!feof($myfile)) 
{
    $tmp =  fgets($myfile);
    big.dhtdata  $tmp ;
   big.dhtdata "<br>" ;
   print_r (explode(" ",$tmp)); 
   $rowdata = explode(" ",$tmp); 
   big.dhtdata $rowdata[0]."<br>" ;
   big.dhtdata $rowdata[1]."<br>" ;
   big.dhtdata $rowdata[2]."<br>" ;
   big.dhtdata $rowdata[3]."<br>" ;
   big.dhtdata $rowdata[4]."<br>" ;
   big.dhtdata $rowdata[5]."<br>" ;
   big.dhtdata $rowdata[6]."<br>" ;
   big.dhtdata $rowdata[7]."<br>" ;
   big.dhtdata $rowdata[8]."<br>" ;
   big.dhtdata $rowdata[9]."<br>" ;
   big.dhtdata $rowdata[10]."<br>" ;
   big.dhtdata $rowdata[11]."<br>" ;
   big.dhtdata $rowdata[12]."<br>" ;
   big.dhtdata "======================================================<br>" ;
  // $tmpline = str_split(" ", $tmp); 
  }
  fclose($myfile);
?>