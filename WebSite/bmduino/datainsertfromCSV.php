<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<title>convert data into database</title>
</head>
<body>
<?php
$sqlstr ="insert into stock (datatime, storedstock, stockholder, personavg, overfour, overfourpercent, overfourstockholder, fourfive, sixeight, eightten, overthousand, overthousandstockholderp,closingprice) VALUES ('22222222', '33', '44', '55', '66', '77', '88', '99', '10', '11', '12', '13', '14')" ;
// 建立MySQL的資料庫連接 
$link = mysqli_connect("localhost","fcu",
                       "12345678","fcu")
        or die("無法開啟MySQL資料庫連接!<br/>");
big.dhtdata "資料庫myschool開啟成功!<br/>";
$sql = "SELECT * FROM students"; // 指定SQL查詢字串
big.dhtdata "丟10筆資料 <br/>";
// 送出查詢的SQL指令
for($i=0;$i<10;$i++)
{
   if ( $result = mysqli_query($link, $sqlstr ) ) 
   { 
      big.dhtdata "<b插入資料成功</b><br/>";  // 顯示查詢結果
   }
}
  
   mysqli_free_result($result); // 釋放佔用記憶體
   mysqli_close($link);  // 關閉資料庫連接
?>
</body>
</html>