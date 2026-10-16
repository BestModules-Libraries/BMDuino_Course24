<?php
/*
 這段程式碼顯示了一個頁面，列出了溫溼度感測器的 MAC 地址、溫度、濕度、最後更新時間等信息。
 * SQL 查詢從 big.dhtdata 資料表中獲取數據，
 * 然後在 HTML 表格中顯示這些信息。超連結提供了查看特定裝置詳細資料的途徑。
 * 表格內含標題欄和資料行，並使用 PHP 的 sprintf 函數進行格式化。
 */


// 開啟 PHP 會話
session_start();  // 開啟或繼續 PHP 會話，用於保存用戶的會話資料

// 會話是一種在多個頁面之間保存用戶資料的方法。
// 通過 session，可以在使用者瀏覽網站時保持他們的狀態和資料。
// session_start() 用於初始化或恢復會話。
// 必須在任何會話資料讀取或寫入之前調用這個函數。
// 它通常放在每個 PHP 檔案的開頭，這樣才能確保會話資料可用。

    // 包含共用資料庫函式庫和連線設置
    include("../comlib.php");  // 通用的資料庫函式庫
    include("../Connections/iotcnn.php");  // 資料庫連線設定

    // 建立到 MySQL 資料庫的連線
    $link = Connection();  // 取得一個 MySQL 連線物件
    
    // 格式化表格每一行的模板
    $subrow = "<tr><td>%s</td><td>%d</td><td>%s</td><td>%s</td></tr>" ;

    // 格式化超連結的模板，用於查詢特定 MAC 地址的詳細資料
    $op1 = "<a href='ShowChartlist.php?MAC=%s'>Curve Chart Display(曲線表顯示)</a>";

    // 定義 SQL 查詢語句，用於獲取設備的 MAC 地址、記錄總數以及最後更新時間
    $qrystr = "SELECT MAC, count(MAC) as tt, max(systime) as systime FROM big.dhtdata WHERE 1 GROUP BY MAC ORDER BY MAC";

    // 初始化空陣列以儲存 MAC 地址、記錄數量和最後更新時間
    $d00 = array();  // 儲存 MAC 地址
    $d01 = array();  // 儲存記錄總數
    $d02 = array();  // 儲存最後更新時間

    // 執行 SQL 查詢
    $result = mysqli_query($link, $qrystr);  // 執行查詢並將結果存入 $result
    if ($result !== FALSE) 
    {  // 檢查查詢是否成功
        // 遍歷查詢結果，將數據加入對應的陣列中
        while ($row = mysqli_fetch_array($result)) 
        {  // 迭代查詢結果
            array_push($d00, $row["MAC"]);  // 將 MAC 地址加入陣列
            array_push($d01, $row["tt"]);  // 將記錄數量加入陣列
            array_push($d02, $row["systime"]);  // 將最後更新時間加入陣列
        }
    }

    // 釋放查詢結果資源
    mysqli_free_result($result);  // 釋放記憶體中分配的資源

    // 關閉資料庫連線
    mysqli_close($link);  // 關閉 MySQL 連線
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">  <!-- XHTML DOCTYPE -->
<html xmlns="http://www.w3.org/1999/xhtml">  <!-- XHTML 命名空間 -->
<head>
    <!-- 頁面字符編碼 -->
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />  
<title>Query Temperature and Humidity Data by MAC</title>  <!-- 頁面標題 -->
<link href="../webcss.css" rel="stylesheet" type="text/css" />
<!-- 連結外部 CSS 檔案 -->
</head>

<body>
<?php
// 引入網頁頂部標題區域
include '../toptitle.php';  // 引入網頁頂部標題
?>
  <div align="center">  <!-- 內容置中 -->
   <table border="1" align = "center" cellspacing="1" cellpadding="1">  <!-- 表格結構 -->
        <tr>  <!-- 表格標題列 -->
            <td>MAC Address</td>  <!-- MAC 地址欄 -->
            <td>Record Counts</td>  <!-- 記錄數量欄 -->
            <td>Last Update</td>  <!-- 最後更新時間欄 -->
            <td>Curve Chart Display</td>  <!-- 顯示詳細資料欄 -->
        </tr>

    <?php 
        // 如果有 MAC 地址記錄
        if (count($d00) > 0) {  
            // 逆序迭代，以顯示記錄
            for ($i = count($d00) - 1; $i >= 0; $i--) 
            {  
                // 使用 echo 和 sprintf 建立表格行，並包含詳細資料的連結
                echo sprintf($subrow,
                    $d00[$i],  // MAC 地址
                    $d01[$i],  // 記錄數量
                    $d02[$i],  // 最後更新時間
                    sprintf($op1,$d00[$i])  // 用於顯示詳細資料的 MAC 地址
                );
            }
        }
    ?>

   </table>
  </div>  <!-- 結束置中內容區域 -->
</form>

<?php 
// 包含網頁底部區域
include("../topfooter.php");  // 引入網頁底部
?>
</body>
</html>
