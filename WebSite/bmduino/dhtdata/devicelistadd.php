<?php 
   	include("../Connections/iotcnn.php");		// 引入資料庫連線程式 (iotcnn.php)
		//	Connection() ;                    // 備註：呼叫資料庫連線的函式
  	$link=Connection();		                // 建立 MySQL 連線物件並存到 $link
//	mysqli_select_db($link, "ncnuiot") ;     // (已註解) 切換資料庫
?>
<?php
// 定義 SQL 字串處理函式 (避免 SQL Injection)
// 這個函式會依照欄位型別 (文字、數字、日期...) 決定如何包裝輸入值
if (!function_exists("GetSQLValueString")) {
function GetSQLValueString($theValue, $theType, $theDefinedValue = "", $theNotDefinedValue = "") 
{
  // 舊版本 PHP 需移除自動跳脫字元
  if (PHP_VERSION < 6) {
    $theValue = get_magic_quotes_gpc() ? stripslashes($theValue) : $theValue;
  }

  // 使用 mysqli_real_escape_string 避免特殊字元造成 SQL 注入攻擊
  $theValue = function_exists("mysqli_real_escape_string") ? mysqli_real_escape_string($theValue) : mysqli_escape_string($theValue);

  // 依據不同型態處理
  switch ($theType) {
    case "text":     // 字串型態，加上單引號
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;    
    case "long":
    case "int":      // 整數型態，轉為 int
      $theValue = ($theValue != "") ? intval($theValue) : "NULL";
      break;
    case "double":   // 浮點數
      $theValue = ($theValue != "") ? doubleval($theValue) : "NULL";
      break;
    case "date":     // 日期型態，加上單引號
      $theValue = ($theValue != "") ? "'" . $theValue . "'" : "NULL";
      break;
    case "defined":  // 已定義的值/未定義的值
      $theValue = ($theValue != "") ? $theDefinedValue : $theNotDefinedValue;
      break;
  }
  return $theValue;
}
}

// 建立目前程式的表單提交路徑 (包含 Query String)
$editFormAction = $_SERVER['PHP_SELF'];
if (isset($_SERVER['QUERY_STRING'])) {
  $editFormAction .= "?" . htmlentities($_SERVER['QUERY_STRING']);
}

// 如果表單被送出 (form1)，則執行新增資料
if ((isset($_POST["MM_insert"])) && ($_POST["MM_insert"] == "form1")) {
  // 組合 SQL 新增語法，將感測器裝置 (MAC) 與所屬站台 (Did) 寫入 sitelist
  $insertSQL = sprintf("insert into big.sitelist (mac, Did, datatbl,ps,sensortype) VALUES (%s, %d, %s, %s, '%s')",
                       GetSQLValueString($_POST['select'], "text"),   // select (感測器 MAC)
                       GetSQLValueString($_POST['select2'], "int"),   // select2 (站台 ID)
                       GetSQLValueString("dhtdata", "text"),          // 預設資料表名稱 dhtdata
                       GetSQLValueString($_POST['textfield'], "text"),// 備註
					   '01');                                         // 感測器型態 (01=溫溼度)

// 偵錯用：輸出 SQL 語法
//echo $insertSQL."<br>";

  mysqli_select_db($db, $link);                        // 選擇資料庫
  $Result1 = mysqli_query($link, $insertSQL) or die(mysqli_error());  // 執行 SQL，若失敗則輸出錯誤
 		
  // 成功新增後，導向 datalist.php 頁面
  $insertGoTo = "datalist.php";
  if (isset($_SERVER['QUERY_STRING'])) {
    $insertGoTo .= (strpos($insertGoTo, '?')) ? "&" : "?";
    $insertGoTo .= $_SERVER['QUERY_STRING'];
  }
  header(sprintf("Location: %s", $insertGoTo));
}
?>

<?php
	// 查詢未被加入 sitelist 的感測器 (避免重複登錄)
	$str2 =  "select mac from big.dhtdata where mac not in 
              (select mac from big.sitelist where sensortype = '01' group by mac) group by mac" ;

	// 查詢所有站台 (由 siteid 降冪排序)
	$str3 =  "select * from big.site order by siteid desc" ;

	// 執行 SQL 查詢
	$result2=mysqli_query($link, $str2);	
	$result3=mysqli_query($link,$str3);		
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Add Temperature and Humidity Device into Site belonged</title>
<link href="../webcss.css" rel="stylesheet" type="text/css" /> <!-- 網頁樣式表 -->
</head>

<body>
<?php
include '../title.php';   // 引入標題區塊
?>
<input type ="button" onclick="history.back()" value="回到上一頁"> <!-- 回上一頁按鈕 -->
</input>

<!-- 表單：將新的感測器 (MAC) 加入站台 -->
<form id="form1" name="form1" method="POST" action="<?php echo $editFormAction; ?>">
  <table width="100%" border="1">
    <!-- 選擇感測器 MAC -->
    <tr>
      <td width="20%">Device MAC(機器網路碼)</td>
      <td width="80%">
        <select name="select" id="select">
          <?php 
		  if($result2 !==FALSE){
		     while($row = mysqli_fetch_array($result2)) {
		        // 下拉選單：顯示所有尚未登錄的 MAC
		        printf(" <option value='%s'>%s</option>", 
		           $row["mac"], $row["mac"]);
		     }
 		     mysqli_free_result($result2);
		 }
   	   ?>
        </select>
      </td>
    </tr>

    <!-- 選擇站台 -->
    <tr>
      <td>Site Name(站台名稱)</td>
      <td width="80%">
        <select name="select2" id="select2">
          <?php 
		  if($result3 !==FALSE){
		     while($row = mysqli_fetch_array($result3)) {
		        // 下拉選單：顯示 siteid/sitename
		        printf(" <option value='%s'>%s/%s</option>", 
		           $row["id"], $row["siteid"], $row["sitename"]);
		     }
 		     mysqli_free_result($result3);
		 }
		?>
        </select>
      </td>
    </tr>

    <!-- 備註欄位 -->
    <tr>
      <td>PS(備註)</td>
      <td width="80%">
      <input name="textfield" type="text" id="textfield2" size="120" maxlength="300" />
      </td>
    </tr>	

    <!-- 送出按鈕 -->
    <tr>
      <td>&nbsp;</td>
      <td><input type="submit" name="button" id="button" value="Submit(送出)" /></td>
    </tr>
  </table>
  
  <!-- 隱藏欄位，確認是 form1 的提交 -->
  <input type="hidden" name="MM_insert" value="form1" />
</form>

<!-- jQuery，未來可擴充表單檢查 -->
<script src="https://code.jquery.com/jquery-3.4.1.min.js"></script>

<?php
include '../footer.php';   // 引入頁尾
?>
</body>
</html>
