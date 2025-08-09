<?php
include("iotcnn.php");
$link = Connection();

$row = null;
if (isset($_GET['id'])) {
    $id = (int)$_GET['id'];
    $query = "SELECT * FROM dhtdata WHERE id = ?";
    $stmt = $link->prepare($query);
    $stmt->bind_param("i", $id);
    $stmt->execute();
    $result = $stmt->get_result();
    if ($result->num_rows > 0) {
        $row = $result->fetch_assoc();
    } else {
        echo "未找到記錄";
    }
    $stmt->close();
} elseif (isset($_POST['confirm']) && $_POST['confirm'] === 'yes') {
    $id = (int)$_POST['id'];
    $query = "DELETE FROM dhtdata WHERE id = ?";
    $stmt = $link->prepare($query);
    $stmt->bind_param("i", $id);
    
    if ($stmt->execute()) {
        echo "記錄已成功刪除";
    } else {
        echo "錯誤: " . $link->error;
    }
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
    <?php include 'toptitle.php'; ?>
    <h2>刪除溫濕度資料</h2>
    <?php if ($row): ?>
        <p>確定要刪除以下記錄嗎？</p>
        <p>ID: <?php echo $row['id']; ?>, MAC: <?php echo htmlspecialchars($row['MAC']); ?>, IP: <?php echo htmlspecialchars($row['IP'] ?? ''); ?>, 溫度: <?php echo $row['temperature']; ?>, 濕度: <?php echo $row['humidity']; ?>, 系統時間: <?php echo htmlspecialchars($row['systime']); ?></p>
        <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <input type="hidden" name="id" value="<?php echo $row['id']; ?>">
            <input type="hidden" name="confirm" value="yes">
            <input type="submit" value="確認刪除">
        </form>
    <?php else: ?>
        <form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <label>輸入 ID: <input type="number" name="id" required></label><br>
            <input type="submit" value="查詢">
        </form>
    <?php endif; ?>
    <?php include 'topfooter.php'; ?>
</body>
</html>