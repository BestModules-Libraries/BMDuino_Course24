<?php
// 引入資料庫連線設定檔 iotcnn.php
include("iotcnn.php");
// 建立資料庫連線
$link = Connection();

// 用來儲存查詢到的單筆資料
$row = null;

// ------------------- 如果是 GET 請求 (用於查詢資料) -------------------
if (isset($_GET['id'])) {  // 檢查網址是否有傳入 id 參數
    $id = (int)$_GET['id'];  // 將 id 轉為整數型態，避免 SQL Injection
    $query = "SELECT * FROM dhtdata WHERE id = ?";  // SQL 查詢語法 (使用 ? 預備參數)
    $stmt = $link->prepare($query);  // 預備 SQL 語句
    $stmt->bind_param("i", $id);  // 綁定 id 參數 (i = integer)
    $stmt->execute();  // 執行 SQL 查詢
    $result = $stmt->get_result();  // 取得查詢結果
    if ($result->num_rows > 0) {  // 如果有查到資料
        $row = $result->fetch_assoc();  // 取出單筆資料
    } else {
        echo "未找到記錄";  // 如果沒有找到，輸出提示訊息
    }
    $stmt->close();  // 關閉語句物件

// ------------------- 如果是 POST 請求 (用於更新資料) -------------------
} elseif ($_SERVER["REQUEST_METHOD"] === "POST") {
    // 從表單接收資料
    $id = (int)$_POST['id'];  // 轉為整數型態
    $mac = $_POST['mac'] ?? '';  // MAC 位址
    $ip = $_POST['ip'] ?? '';  // IP 位址 (可選)
    $temperature = $_POST['temperature'] ?? '';  // 溫度
    $humidity = $_POST['humidity'] ?? '';  // 濕度
    $systime = $_POST['systime'] ?? '';  // 系統時間

    // 檢查必填欄位是否都有填寫
    if (!empty($mac) && !empty($temperature) && !empty($humidity) && !empty($systime)) {
        // 更新 SQL 語法
        $query = "UPDATE dhtdata SET MAC = ?, IP = ?, temperature = ?, humidity = ?, systime = ? WHERE id = ?";
        $stmt = $link->prepare($query);  // 預備語句
        // 綁定參數 (s = string, d = double, i = integer)
        $stmt->bind_param("sssddi", $mac, $ip, $temperature, $humidity, $systime, $id);
        
        if ($stmt->execute()) {  // 執行更新
            echo "記錄已成功更新";
        } else {
            echo "錯誤: " . $link->error;  // 顯示錯誤訊息
        }
        $stmt->close();  // 關閉語句物件
    } else {
        echo "請填寫所有必填欄位";  // 有必填欄位未填
    }
}
?>

<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>修改資料</title>
</head>
<body>
    <?php include 'toptitle.php'; ?> <!-- 頁面頂部共用檔案 -->

    <h2>修改溫濕度資料</h2>

    <?php if ($row): ?> <!-- 如果有查詢到資料，顯示修改表單 -->
        <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <!-- 隱藏欄位，存放資料 ID -->
            <input type="hidden" name="id" value="<?php echo $row['id']; ?>">

            <!-- MAC 欄位 -->
            <label>MAC:
                <input type="text" name="mac"
                       value="<?php echo htmlspecialchars($row['MAC']); ?>"
                       maxlength="12" required>
            </label><br>

            <!-- IP 欄位 -->
            <label>IP:
                <input type="text" name="ip"
                       value="<?php echo htmlspecialchars($row['IP'] ?? ''); ?>"
                       maxlength="20">
            </label><br>

            <!-- 溫度欄位 -->
            <label>溫度:
                <input type="number" step="0.1" name="temperature"
                       value="<?php echo $row['temperature']; ?>" required>
            </label><br>

            <!-- 濕度欄位 -->
            <label>濕度:
                <input type="number" step="0.1" name="humidity"
                       value="<?php echo $row['humidity']; ?>" required>
            </label><br>

            <!-- 系統時間欄位 -->
            <label>系統時間:
                <input type="text" name="systime"
                       value="<?php echo htmlspecialchars($row['systime']); ?>"
                       maxlength="14" required>
            </label><br>

            <!-- 提交按鈕 -->
            <input type="submit" value="更新">
        </form>

    <?php else: ?> <!-- 如果還沒查詢資料，顯示查詢表單 -->
        <form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <label>輸入 ID:
                <input type="number" name="id" required>
            </label><br>
            <input type="submit" value="查詢">
        </form>
    <?php endif; ?>

    <?php include 'topfooter.php'; ?> <!-- 頁面底部共用檔案 -->
</body>
</html>
