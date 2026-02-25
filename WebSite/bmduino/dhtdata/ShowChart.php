<?php
/*
 這段程式碼顯示了一個頁面，列出了溫溼度感測器的 MAC 地址、溫度、濕度、最後更新時間等信息。
 * SQL 查詢從 big.dhtdata 資料表中獲取數據，
 * 然後在 HTML 表格中顯示這些信息。超連結提供了查看特定裝置詳細資料的途徑。
 * 表格內含標題欄和資料行，並使用 PHP 的 sprintf 函數進行格式化。
 */
?>

<?php
// 開啟或繼續 PHP 會話，用來在使用者瀏覽過程中儲存狀態資訊（如登入狀態）
session_start();  

// 引入共用程式：comlib.php 是常用的函式庫，iotcnn.php 是資料庫連線設定
include("../comlib.php");             
include("../Connections/iotcnn.php"); 

// 建立與資料庫的連線，回傳 MySQLi 連線物件
$link = Connection();                

// 定義表格內每一列的格式：4 個欄位（MAC、記錄數、更新時間、曲線圖連結）
$subrow = "<tr><td>%s</td><td>%d</td><td>%s</td><td>%s</td></tr>" ;

// 定義詳細資料的超連結模板：會帶入 MAC 作為網址參數
$op1 = "<a href='ShowChartlist.php?MAC=%s'>Curve Chart Display(曲線表顯示)</a>";

// 建立 SQL 查詢語句：取得每個 MAC 裝置的總筆數與最新資料時間
$qrystr = "SELECT MAC, count(MAC) as tt, max(systime) as systime FROM big.dhtdata WHERE 1 GROUP BY MAC ORDER BY MAC";

// 初始化三個陣列：用來儲存查詢出來的 MAC、筆數、與時間
$d00 = array();  // 儲存 MAC 地址
$d01 = array();  // 儲存總記錄數量
$d02 = array();  // 儲存最後更新時間

// 執行 SQL 查詢，結果存入 $result
$result = mysqli_query($link, $qrystr);

// 如果查詢成功
if ($result !== FALSE) 
{
    // 使用 while 迴圈逐筆讀取查詢結果
    while ($row = mysqli_fetch_array($result)) 
    {
        // 把每一筆資料的欄位存入各自的陣列
        array_push($d00, $row["MAC"]);      // 儲存 MAC
        array_push($d01, $row["tt"]);       // 儲存統計筆數
        array_push($d02, $row["systime"]);  // 儲存最新時間
    }
}

// 查詢完畢後，釋放資源以節省記憶體
mysqli_free_result($result);

// 關閉與資料庫的連線
mysqli_close($link);
?>


<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Query Temperature and Humidity Data by MAC</title> <!-- 頁面標題 -->

<!-- 引用外部樣式表 -->
<link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>

<body>
<?php
// 引入網頁上方的標題區塊（例如 logo 或標語）
include '../toptitle.php';
?>

<!-- 建立主要表格區塊，置中顯示 -->
<div align="center">
<table border="1" align="center" cellspacing="1" cellpadding="1">  
        <tr> <!-- 標題列 -->
            <td>MAC Address</td>         <!-- 裝置 MAC -->
            <td>Record Counts</td>       <!-- 資料筆數 -->
            <td>Last Update</td>         <!-- 最後更新時間 -->
            <td>Curve Chart Display</td> <!-- 查看曲線圖表 -->
        </tr>

        <?php 
        // 如果資料筆數大於 0，則輸出所有資料列
        if (count($d00) > 0) {
            // 倒序輸出資料（最新的在上面）
            for ($i = count($d00) - 1; $i >= 0; $i--) 
            {
                // 使用 sprintf 將資料填入 HTML 模板，並輸出每一列
            echo sprintf($subrow,
                $d00[$i],                 // MAC
                $d01[$i],                 // 筆數
                $d02[$i],                 // 最後時間
                sprintf($op1,$d00[$i])    // 曲線圖表的連結（帶入 MAC）
                );
            }
        }
        ?>
    </table>
</div>

<!-- 頁尾 -->
<?php 
include("../topfooter.php");  // 引入頁腳
?>
</body>
</html>
