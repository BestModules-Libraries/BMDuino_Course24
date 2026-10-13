<!DOCTYPE html> <!-- 宣告 HTML5 文件類型，讓瀏覽器用標準模式解析網頁 -->
<?php
    // 引入 'comlib.php' 檔案，通常包含一些共用的 PHP 函式
    // 例如：頁面標題、日期時間格式、系統設定等
    include("comlib.php");

    // 引入 'iotcnn.php' 檔案，通常用來設定與資料庫的連線資訊
    // 例如：資料庫帳號、密碼、主機位址，以及建立連線的函式 Connection()
    include("iotcnn.php");

    // 呼叫 Connection() 函式建立與資料庫的連線
    // 執行後會回傳一個連線物件（$link），可用來查詢資料庫
    $link = Connection();
?> 

<html>
   <head>
    <!-- 設定網頁標題 -->
    <!-- 這裡使用 systemtitle() 函式，可能會動態輸出系統名稱或公司名稱 -->
    <title><?php systemtitle(); ?></title>
  </head>

  <body>
    <!-- 引入 'toptitle.php' 檔案，通常包含網頁的頂部設計元素 -->
    <!-- 例如網站 LOGO、標題列、導覽列（Menu）等 -->
    <?php
    include 'toptitle.php';
    ?>
    
    <!-- <br>：換行，用來在視覺上分隔不同內容區塊 -->
    <br>

    <!-- <div align="center">：建立一個區塊並讓內容置中 -->
    <div align="center">
        <!-- 顯示圖片 -->
        <!-- src="/bigdata/images/main.jpg"：圖片路徑 -->
        <!-- width="60%" height="60%"：設定圖片寬高為原圖的 60%（百分比可自適應裝置大小） -->
        <!-- alt="my Team"：當圖片無法載入時顯示的替代文字 -->
        <img src="/bigdata/images/main.jpg" width="60%" height="60%" alt="my Team" />
    </div>

    <!-- 引入 'topfooter.php' 檔案，通常包含網站的底部設計元素 -->
    <!-- 例如版權聲明、聯絡資訊、頁尾連結等 -->
    <?php
    include 'topfooter.php';
    ?>
  </body>
</html>
