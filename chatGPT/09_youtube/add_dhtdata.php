<?php
// 引入 iotcnn.php 檔案，該檔案通常包含資料庫連線的設定與函式
include("iotcnn.php");

// 呼叫 Connection() 函式取得資料庫連線物件
$link = Connection();

// 判斷請求方法是否為 POST（代表是表單送出的資料，而非直接開網頁 GET）
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // 從 POST 表單接收欄位資料，若欄位不存在則設為空字串
    $mac = $_POST['mac'] ?? '';              // MAC 位址（必填，長度 12）
    $ip = $_POST['ip'] ?? '';                // IP 位址（非必填，長度 20）
    $temperature = $_POST['temperature'] ?? ''; // 溫度（必填，浮點數）
    $humidity = $_POST['humidity'] ?? '';       // 濕度（必填，浮點數）
    $systime = $_POST['systime'] ?? '';         // 系統時間（必填，長度 14，可能是 yyyymmddHHMMSS 格式）

    // 基本驗證：確認必填欄位都不為空
    if (!empty($mac) && !empty($temperature) && !empty($humidity) && !empty($systime)) {
        // SQL 語法，使用「?」作為參數佔位符，防止 SQL Injection
        $query = "INSERT INTO dhtdata (MAC, IP, temperature, humidity, systime) VALUES (?, ?, ?, ?, ?)";

        // 使用 MySQLi 預備語句（Prepared Statement）建立查詢
        $stmt = $link->prepare($query);

        // 綁定參數到查詢（sssdd 代表字串、字串、字串、浮點數、浮點數）
        // 這裡可能需注意：systime 是字串，temperature / humidity 才是浮點數
        $stmt->bind_param("sssdd", $mac, $ip, $temperature, $humidity, $systime);
        
        // 執行 SQL 語句
        if ($stmt->execute()) {
            echo "新紀錄已成功創建"; // 執行成功
        } else {
            echo "錯誤: " . $link->error; // 執行失敗時輸出錯誤訊息
        }

        // 關閉預備語句
        $stmt->close();
    } else {
        // 若必填欄位有缺，顯示錯誤訊息
        echo "請填寫所有必填欄位";
    }
}
?>


<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>新增資料</title>
</head>
<body>
    <!-- 引入頁面頂部樣式與標題（toptitle.php 通常包含網站標題、LOGO 或選單） -->
    <?php include 'toptitle.php'; ?>

    <h2>新增溫濕度資料</h2>

    <!-- 表單送出方式為 POST，送到自己這個 PHP 頁面 -->
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <!-- MAC 位址輸入欄（必填，長度限制 12 個字元） -->
        <label>MAC: <input type="text" name="mac" maxlength="12" required></label><br>

        <!-- IP 位址輸入欄（非必填，長度限制 20 個字元） -->
        <label>IP: <input type="text" name="ip" maxlength="20"></label><br>

        <!-- 溫度輸入欄（必填，允許小數點 1 位，HTML5 number 型別） -->
        <label>溫度: <input type="number" step="0.1" name="temperature" required></label><br>

        <!-- 濕度輸入欄（必填，允許小數點 1 位） -->
        <label>濕度: <input type="number" step="0.1" name="humidity" required></label><br>

        <!-- 系統時間輸入欄（必填，長度限制 14 位數字） -->
        <label>系統時間: <input type="text" name="systime" maxlength="14" required></label><br>

        <!-- 提交按鈕 -->
        <input type="submit" value="提交">
    </form>

    <!-- 引入頁面底部資訊（topfooter.php 通常包含版權、聯絡資訊等） -->
    <?php include 'topfooter.php'; ?>
</body>
</html>
