<?php 
   	include("../Connections/iotcnn.php");		//使用資料庫的呼叫程式
		//	Connection() ;
  	$link=Connection();		//產生mySQL連線物件
//	mysqli_select_db($link, "ncnuiot") ;


?>
<?php
if (!function_exists("GetSQLValueString")) {
function GetSQLValueString($theValue, $theType, $theDefinedValue = "", $theNotDefinedValue = "") 
{
  if (PHP_VERSION < 6) {
    $theValue = get_magic_quotes_gpc() ? stripslashes($theValue) : $theValue;
  }

  $theValue = function_exists("mysqli_real_escape_string") ? mysqli_real_escape_string($theValue) : mysqli_escape_string($theValue);

  switch ($theType) {
    case "text":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;    
    case "long":
    case "int":
      $theValue = ($theValue != "") ? intval($theValue) : "NULL";
      break;
    case "double":
      $theValue = ($theValue != "") ? doubleval($theValue) : "NULL";
      break;
    case "date":
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;
    case "defined":
      $theValue = ($theValue != "") ? $theDefinedValue : $theNotDefinedValue;
      break;
  }
  return $theValue;
}
}

$editFormAction = $_SERVER['PHP_SELF'];
if (isset($_SERVER['QUERY_STRING'])) {
  $editFormAction .= "?" . htmlentities($_SERVER['QUERY_STRING']);
}

if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {
  $insertSQL = sprintf("insert into big.sitelist (mac, Did, datatbl,ps,sensortype) VALUES (%s, %d, %s, %s, '%s')",
                       GetSQLValueString($_POST['select'], "text"),
                       GetSQLValueString($_POST['select2'], "int"),
                       GetSQLValueString("dhtdata", "text"),
                       GetSQLValueString($_POST['textfield'], "text"),
					   '01');
//echo $insertSQL."<br>";
//insert into big.sitelist (mac, Did, datatbl,ps,sensortype) VALUES (''441793F001AC'', 11, ''dhtdata'', ''院長室空氣盒子_溫溼度感測器'', '01')
  mysqli_select_db($db, $link);
  $Result1 = mysqli_query($link, $insertSQL) or die(mysqli_error());
 		
  $insertGoTo = "datalist.php";
  if (isset($_SERVER['QUERY_STRING'])) {
    $insertGoTo .= (strpos($insertGoTo, '?')) ? "&" : "?";
    $insertGoTo .= $_SERVER['QUERY_STRING'];
  }
  header(sprintf("Location: %s", $insertGoTo));
}
?>
<?php

	
	$str2 =  "select mac from big.dhtdata where mac not in (select mac from big.sitelist where sensortype = '01' group by mac) group by mac" ;
	$str3 =  "select * from big.site order by  siteid  desc" ;
	$result2=mysqli_query($link, $str2);	
	$result3=mysqli_query($link,$str3);		
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Add Temperature and Humidity Device into Site belonged</title>
<link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>

<body>
<?php
include '../title.php';
?>
<input type ="button" onclick="history.back()" value="回到上一頁">
</input>
<form id="form1" name="form1" method="POST" action="<?php echo $editFormAction; ?>">
  <table width="100%" border="1">
    <tr>
      <td width="20%">Device MAC(機器網路碼)</td>
      <td width="80%"><label for="textfield">
        <select name="select" id="select">
          <?php 
		  if($result2 !==FALSE){
		     while($row = mysqli_fetch_array($result2)) {
		        printf(" <option value='%s'>%s</option>", 
		           $row["mac"], $row["mac"]);
		     }
 		     mysqli_free_result($result2);
		 }
   	   ?>
        </select>
      </label></td>
    </tr>
    <tr>
      <td>Site Name(站台名稱)</td>
      <td width="80%"><label for="textfields2">
        <select name="select2" id="select2">
          <?php 
		  if($result3 !==FALSE){
		     while($row = mysqli_fetch_array($result3)) {
		        printf(" <option value='%s'>%s/%s</option>", 
		           $row["id"], $row["siteid"], $row["sitename"]);
		     }
 		     mysqli_free_result($result3);
		 }
		?>
        </select>
      </label></td>
    </tr>
    <tr>
      <td>PS(備註)</td>
      <td width="80%"><label for="textfield2"></label>
      <input name="textfield" type="text" id="textfield2" size="120" maxlength="300" /></td>
    </tr>	
    <tr>
      <td>&nbsp;</td>
      <td><input type="submit" name="button" id="button" value="Submit(送出)" /></td>
    </tr>
  </table>
  <p>
    <input type="hidden" name="MM_insert" value="form1" />
</p>
</form>
<script src="https://code.jquery.com/jquery-3.4.1.min.js"></script>

<?php
include '../footer.php';
?>
</body>
</html>