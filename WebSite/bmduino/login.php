<!DOCTYPE html>
<!-- 
這個網頁的目的是提供一個登錄表單，用戶可以在其中輸入名字和密碼，然後將這些資料發送到 checkpass.php 以檢查登入資訊。表單中還包括重置和提交按鈕，以及註冊帳號的鏈接。頁面頂部和底部分別包含 title.php 和 footer.php，這些通常包含網頁的標題和頁腳信息
-->
<html>
<!-- 網頁的標準 HTML 結構 -->
<head>
    <!-- 設定網頁的標題 -->
    <title>登錄網站</title>
</head>

<body>
    <!-- 包含外部的 PHP 文件 'title.php'，通常包含標題和其他頁面的頂部元素 -->
    <?php include 'title.php'; ?>

    <!-- 創建一個登錄表單，將數據發送到 'checkpass.php'，使用 POST 方法 -->
    <form action="checkpass.php" method="POST">
        <!-- 表格用於對齊表單元素，並且將其置於網頁中央 -->
        <table align="center">
            <tr>
                <td>名字</td>
                <td>
                    <!-- 用於輸入用戶名 -->
                    <input type="text" name="fname">
                </td>
            </tr>
            <tr>
                <td>密碼</td>
                <td>
                    <!-- 用於輸入密碼，隱藏字符 -->
                    <input type="password" name="pwd">
                </td>
            </tr>
            <tr>
                <!-- 提供清除表單和提交表單的按鈕 -->
                <td align="center">
                    <input type="reset" value="清除">
                </td>
                <td align="center">
                    <input type="submit" value="送出">
                </td>
            </tr>
        </table>
    </form>

    <!-- 在表單下方提供註冊帳號的連結 -->
    <div align="center">
        <a href="./member/memberadd.php">註冊帳號</a>
    </div>

    <!-- 包含外部的 PHP 文件 'footer.php'，通常包含頁腳內容 -->
    <?php include 'footer.php'; ?>
</body>

</html>
