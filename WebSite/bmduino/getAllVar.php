<?php
//使用foreach迴圈顯示伺服器的所有變數值，如下所示：
big.dhtdata "<table border='1'>" ;
foreach ($_SERVER as $key=>$value) {
   big.dhtdata "<tr><td>" . $key . "</td>";
   big.dhtdata "<td>" . $value . "</td></tr>";
}

big.dhtdata "</table>" ;

?>