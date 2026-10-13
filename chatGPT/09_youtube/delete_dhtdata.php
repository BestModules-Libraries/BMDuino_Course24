<?php
// 引入 iotcnn.php 檔案，該檔案通常包含資料庫連線的設定與函式
include("iotcnn.php");

// 取得資料庫連線物件
$link = Connection();

// 預設 $row 為 null，用來存放查詢到的單筆資料
$row = null;

// ----------- 情境 1：GET 請求（查詢要刪除的資料） -----------
if (isset($_GET['id'])) { // 如果網址參數有傳入 id
    $id = (int)$_GET['id']; // 轉型成整數，避免 SQL Injection 與型別錯誤

    // 建立 SQL 語法，使用 ? 作為參數佔位符
    $query = "SELECT * FROM dhtdata WHERE id = ?";

    // 建立預備語句（Prepared Statement）
    $stmt = $link->prepare($query);

    // 綁定參數（"i" 表示整數型別）
    $stmt->bind_param("i", $id);

    // 執行 SQL 查詢
    $stmt->execute();

    // 取得查詢結果集
    $result = $stmt->get_result();

    // 如果查詢到至少 1 筆資料
    if ($result->num_rows > 0) {
        // 將查詢結果存入 $row（關聯式陣列）
        $row = $result->fetch_assoc();
    } else {
        // 若找不到對應資料，顯示提示訊息
        echo "未找到記錄";
    }

    // 關閉預備語句
    $stmt->close();

// ----------- 情境 2：POST 請求（確認刪除資料） -----------
} elseif (isset($_POST['confirm']) && $_POST['confirm'] === 'yes') {
    // 取得表單傳入的 id，並轉型為整數
    $id = (int)$_POST['id'];

    // 刪除資料的 SQL 語法
    $query = "DELETE FROM dhtdata WHERE id = ?";

    // 建立預備語句
    $stmt = $link->prepare($query);

    // 綁定參數（整數型別）
    $stmt->bind_param("i", $id);
    
    // 執行刪除動作
    if ($stmt->execute()) {
        echo "記錄已成功刪除"; // 成功訊息
    } else {
        echo "錯誤: " . $link->error; // 失敗訊息
    }

    // 關閉預備語句
    $stmt->close();
}
?>


<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>刪除資料</title>
</head>
<body>
    <!-- 引入頁面標題與導覽列 -->
    <?php include 'toptitle.php'; ?>

    <h2>刪除溫濕度資料</h2>

    <?php if ($row): ?>
        <!-- 如果 $row 有資料，顯示刪除確認畫面 -->
        <p>確定要刪除以下記錄嗎？</p>
        <p>
            ID: <?php echo $row['id']; ?>, 
            MAC: <?php echo htmlspecialchars($row['MAC']); ?>, 
            IP: <?php echo htmlspecialchars($row['IP'] ?? ''); ?>, 
            溫度: <?php echo $row['temperature']; ?>, 
            濕度: <?php echo $row['humidity']; ?>, 
            系統時間: <?php echo htmlspecialchars($row['systime']); ?>
        </p>

        <!-- 刪除確認表單（用 POST 送出） -->
        <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <!-- 隱藏欄位：記錄 ID -->
            <input type="hidden" name="id" value="<?php echo $row['id']; ?>">
            <!-- 隱藏欄位：確認刪除標誌 -->
            <input type="hidden" name="confirm" value="yes">
            <!-- 提交按鈕 -->
            <input type="submit" value="確認刪除">
        </form>

    <?php else: ?>
        <!-- 如果沒有查詢到資料，顯示查詢表單 -->
        <form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <label>輸入 ID: <input type="number" name="id" required></label><br>
            <input type="submit" value="查詢">
        </form>
    <?php endif; ?>

    <!-- 引入頁面底部資訊 -->
    <?php include 'topfooter.php'; ?>
</body>
</html>
