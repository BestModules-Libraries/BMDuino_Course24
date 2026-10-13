<?php
 
   	include("../comlib.php");		        // 引入共用程式庫 (comlib.php)，裡面通常包含一些常用的函數
   	include("../Connections/iotcnn.php");	// 引入資料庫連線程式 (iotcnn.php)，用來建立與 MySQL 的連線
	//	Connection() ;                     // 備註，呼叫資料庫連線函式
  	$link=Connection();		                // 呼叫 Connection() 建立 MySQL 連線，並存到 $link 變數中

/*
這個 PHP 程式的主要功能：
1. 從資料庫 big.sitelist, big.site, big.sensortype 三個資料表查詢屬於感測器類型 "01" (即溫溼度感測器) 的裝置清單。
2. 取得站台編號、站台名稱、站台地址、MAC 位址、GPS 經緯度等資訊。
3. 將查詢結果存放在陣列中，並用 HTML 表格格式顯示在網頁上。
4. 每個裝置資料列後面提供多種操作超連結 (例如管理、顯示曲線圖、顯示日曲線圖、顯示儀表板)。
5. 頁首與頁尾分別使用外部的 PHP 模組 (toptitle.php, topfooter.php)，方便版面一致性。
*/

	// 定義 HTML 表格中的一列輸出格式 (使用 sprintf 填入數據)
	// %s 對應 sprintf 代入的字串，這裡有站台編號、站台名稱、站台地址、MAC、GPS(經緯度) 及操作超連結
	$subrow="<tr><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>( %s , %s )</td><td>%s<br>%s<br>%s<br>%s</td></tr>" ;

	// 定義操作連結範本
	$op1 = "<a href='devicelistadd.php'>Device Belong(裝置歸屬)</a>" ;  
	$op2a = "<a href='ShowChartlist.php?MAC=%s'>Curve Chart</a>" ;            // 顯示歷史曲線圖
	$op3a = "<a href='ShowChartlistday.php?MAC=%s'>Dayily Curve Chart</a>" ;  // 顯示日曲線圖
	$op4a = "<a href='ShowBoxChart.php?MAC=%s'>Box Plot</a>" ;                // 顯示箱形圖 (目前未使用)
	$op9a = "<a href='ShowSingleGuage.php?mac=%s'>Guage Display</a>" ;        // 顯示單一裝置儀表板

	// SQL 查詢語法：  
	// 連結三個資料表 sitelist (裝置清單)、site (站台資訊)、sensortype (感測器類型)
	// 條件：sitelist.sensortype = '01' (代表溫溼度感測器)
	// 並按照 siteid 與 mac 位址排序
	$qrystr="select a.*, b.*, c.* , a.id as mainid 
            from big.sitelist as a, big.site as b, big.sensortype as c 
            where a.sensortype = c.sid 
            and a.Did = b.id 
            and a.sensortype = '01' 
            order by b.siteid asc , a.mac asc " ;		

	// 宣告多個空陣列，用來存放查詢結果
	$d00 = array();	// 存放主鍵 id
	$d01 = array();	// 存放 siteid (站台編號)
	$d02 = array();	// 存放 sitename (站台名稱)
	$d03 = array();	// 存放 address (站台地址)
	$d04 = array();	// 存放 mac (裝置 MAC 地址)
	$d05 = array();	// 存放 longitude (經度)
	$d06 = array();	// 存放 latitude (緯度)
	
	// 執行 SQL 查詢
	$result=mysqli_query($link,$qrystr);		

  if($result!==FALSE){
	 // 逐列讀取查詢結果
	 while($row = mysqli_fetch_array($result)) 
	 {
			array_push($d00, $row["mainid"]);	 // 裝置主鍵 ID
			array_push($d01, $row["siteid"]);	 // 站台編號
			array_push($d02, $row["sitename"]); // 站台名稱
			array_push($d03, $row["address"]);	 // 站台地址
			array_push($d04, $row["mac"]);	     // MAC 位址
			array_push($d05, $row["longitude"]); // GPS 經度
			array_push($d06, $row["latitude"]);	 // GPS 緯度
		}
  }
			
	// 釋放查詢結果記憶體
	mysqli_free_result($result);	
 
	// 關閉 MySQL 連線
	mysqli_close($link);		

?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Temperature and Humidity Device  List</title>
<link href="webcss.css" rel="stylesheet" type="text/css" />   <!-- 引用外部 CSS 樣式 -->
</head>
<body>

<?php include("../toptitle.php"); ?>   <!-- 引入共用的網頁標題區塊 -->

<!-- 提供返回上一頁的按鈕 -->
<input type ="button" onclick="history.back()" value="BACK(回到上一頁)">
</input>

<div align="center">
   <!-- 建立顯示感測器清單的表格 -->
   <table border="1" align="center" cellspacing="1" cellpadding="1">		
	<tr bgcolor=#CFC >
		<td colspan='6'><div align='center'>Temperature & Humidity Sensor(溫溼度感測裝置)</div></td>
	</tr>
		<tr>
			<td>Site ID(站台編號)</td>
			<td>Site Name(站台名稱)</td>
			<td>Site Address(站台地址)</td>
			<td>MAC Adress(網路卡號)</td>
			<td>GPS</td>
			<td><a href='devicelistadd.php'>Manage(管理)</a>/Chart Display(圖表顯示)</td>
		</tr>

      <?php 
		  // 如果有查詢到裝置資料
		  if(count($d00) >0)
		  {
				// 從最後一筆開始往前顯示 (反向列出)
				for($i=count($d00)-1;$i >=0  ;$i=$i-1)
					{
						// 建立每個操作的超連結 (帶入 MAC 位址)
						$op2 = sprintf($op2a,$d04[$i]) ;  // 曲線圖
						$op3 = sprintf($op3a,$d04[$i]) ;  // 日曲線圖
						//$op4 = sprintf($op4a,$d04[$i]) ; // 箱形圖 (未使用)
						$op9 = sprintf($op9a,$d04[$i]) ;  // 儀表板顯示
						
						// 使用 sprintf 將資料帶入 HTML 表格列格式
						echo sprintf($subrow, $d01[$i], $d02[$i], $d03[$i], $d04[$i], $d05[$i], $d06[$i],$op1 , $op2, $op3, $op9);
					}
		  }
      ?>
   </table>
</div>

</form>

<!-- 引入網頁底部區塊 -->
<?php include("../topfooter.php"); ?>

</body>
</html>
