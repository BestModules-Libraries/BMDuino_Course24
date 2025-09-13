<!DOCTYPE html>
<?php
    // 包含 'comlib.php' 文件，這個檔案包含了一些共用的 PHP 函數
    include("comlib.php");
?> 
<html>
   <head>
    <!-- 設定網頁的標題，使用 'systemtitle' 函數動態生成標題 -->
    <title><?php systemtitle(); ?></title>
  </head>

  <body>
    <!-- 包含外部的 PHP 文件 'toptitle.php'，通常包含網頁的頂部設計元素 -->
    <?php
    include 'toptitle.php';
    ?>
    
    <!-- 空格行，用於區分不同的網頁區塊 -->
    <br>
    <div align="center">
    <!-- 這裡可以放置主要內容，例如顯示圖片 -->
     <img src="/bigdata/images/main.jpg" width = "60%" height = "60%" alt="my Team" />
    
    <!-- 再次添加空格行，通常用來分隔內容與頁腳 -->
    </div>

    <!-- 包含外部的 PHP 文件 'topfooter.php'，通常包含網頁的底部設計元素 -->
    <?php
    include 'topfooter.php';
    ?>
  </body>
</html>
