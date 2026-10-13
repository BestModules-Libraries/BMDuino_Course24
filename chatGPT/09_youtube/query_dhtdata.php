<?php
// 引入 iotcnn.php 檔案（該檔案內應該定義了資料庫連線函式 Connection()）
include("iotcnn.php");

// 建立資料庫連線
$link = Connection();

// 用來存放查詢結果的陣列
$results = [];

// 如果 URL 有傳入 GET 參數 "mac"，代表使用者要查詢指定 MAC 的資料
if (isset($_GET['mac'])) {
    // 從 GET 參數取得 MAC 位址
    $mac = $_GET['mac'];

    // SQL 查詢語法，使用參數化查詢防止 SQL Injection
    // 依照 MAC 查詢 dhtdata 資料表，並依照建立時間 (crtdatetime) 排序
    $query = "SELECT * FROM dhtdata WHERE MAC = ? ORDER BY crtdatetime";

    // 使用 prepare 預先編譯 SQL
    $stmt = $link->prepare($query);

    // 綁定參數（s 代表字串型態）
    $stmt->bind_param("s", $mac);

    // 執行查詢
    $stmt->execute();

    // 取得查詢結果
    $result = $stmt->get_result();

    // 將每一筆資料轉成關聯陣列並存到 $results
    while ($row = $result->fetch_assoc()) {
        $results[] = $row;
    }

    // 關閉 Statement
    $stmt->close();
}
?>


<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>查詢資料</title>
</head>
<body>
    <!-- 引入頁面頂部樣板（例如標題列、導覽列） -->
    <?php include 'toptitle.php'; ?>

    <h2>查詢溫濕度資料</h2>

    <!-- 查詢表單 -->
    <!-- method="get"：使用 GET 方法傳送資料（會顯示在網址列） -->
    <!-- action 設為目前頁面 ($_SERVER["PHP_SELF"]) 並用 htmlspecialchars 避免 XSS 攻擊 -->
    <form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <!-- MAC 輸入欄位，最多 12 字元，必填 -->
        <label>輸入 MAC: <input type="text" name="mac" maxlength="12" required></label><br>
        <!-- 提交按鈕 -->
        <input type="submit" value="查詢">
    </form>

    <?php if (!empty($results)): ?>
        <!-- 如果查詢結果不為空，顯示結果表格 -->
        <h3>查詢結果:</h3>
        <table border="1">
            <tr>
                <th>ID</th>
                <th>MAC</th>
                <th>IP</th>
                <th>溫度</th>
                <th>濕度</th>
                <th>系統時間</th>
                <th>建立時間</th>
            </tr>

            <!-- 使用 PHP foreach 迴圈將每一筆資料顯示在表格中 -->
            <?php foreach ($results as $row): ?>
                <tr>
                    <td><?php echo $row['id']; ?></td>
                    <td><?php echo htmlspecialchars($row['MAC']); ?></td>
                    <td><?php echo htmlspecialchars($row['IP'] ?? ''); ?></td>
                    <td><?php echo $row['temperature']; ?></td>
                    <td><?php echo $row['humidity']; ?></td>
                    <td><?php echo htmlspecialchars($row['systime']); ?></td>
                    <td><?php echo $row['crtdatetime']; ?></td>
                </tr>
            <?php endforeach; ?>
        </table>

    <?php elseif (isset($_GET['mac'])): ?>
        <!-- 如果有輸入 MAC 但沒有查到任何資料，顯示提示訊息 -->
        <p>沒有找到記錄</p>
    <?php endif; ?>

    <!-- 引入頁面底部樣板（例如版權資訊、聯絡資訊） -->
    <?php include 'topfooter.php'; ?>
</body>
</html>
