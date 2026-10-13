<?php 
// 開啟 PHP 會話，這樣可以使用 $_SESSION 變數
session_start();
?>
<!DOCTYPE html>
<!-- 定義網頁為 HTML5 文件 -->
<html>

<head>
    <!-- 設定網頁的標題 -->
    <title>登錄網站</title>
</head>

<body>
    <!-- 包含外部的 PHP 文件 'title.php'，通常是網頁的標題或頂部內容 -->
    <?php include 'title.php'; ?>
    
    <!-- 建立一個按鈕，用於返回到上一頁 -->
    <input type="button" onclick="history.back()" value="BACK(回到上一頁)">
    
    <!-- 建立一個表單，用於登出 -->
    <form action="logouted.php" method="POST">
        <!-- 表格用於表單元素的對齊，並將其置於中央 -->
        <table align="center">
            <tr>
                <td>帳號</td>
                <td>
                    <!-- 使用隱藏字段傳遞用戶 ID -->
                    <input type="hidden" name="userid" value="<?php echo $_SESSION['usrid']; ?>">
                    <!-- 顯示當前登入用戶的帳號 -->
                    <?php echo $_SESSION['usr']; ?>
                </td>
            </tr>
            <tr>
                <td>名字</td>
                <td>
                    <!-- 顯示當前登入用戶的名字 -->
                    <?php echo $_SESSION['username']; ?>
                </td>
            </tr>
            <tr>
                <td align="center">
                    <!-- 空白單元格，用於平衡表格布局 -->
                </td>
                <td align="center">
                    <!-- 提供提交按鈕，用於登出 -->
                    <input type="submit" value="登出">
                </td>
            </tr>
        </table>
    </form>

    <!-- 包含外部的 PHP 文件 'footer.php'，通常包含頁腳內容 -->
    <?php include 'footer.php'; ?>
</body>

</html>
