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
} elseif ($_SERVER["REQUEST_METHOD"] === "POST") {
    $id = (int)$_POST['id'];
    $mac = $_POST['mac'] ?? '';
    $ip = $_POST['ip'] ?? '';
    $temperature = $_POST['temperature'] ?? '';
    $humidity = $_POST['humidity'] ?? '';
    $systime = $_POST['systime'] ?? '';

    if (!empty($mac) && !empty($temperature) && !empty($humidity) && !empty($systime)) {
        $query = "UPDATE dhtdata SET MAC = ?, IP = ?, temperature = ?, humidity = ?, systime = ? WHERE id = ?";
        $stmt = $link->prepare($query);
        $stmt->bind_param("sssddi", $mac, $ip, $temperature, $humidity, $systime, $id);
        
        if ($stmt->execute()) {
            echo "記錄已成功更新";
        } else {
            echo "錯誤: " . $link->error;
        }
        $stmt->close();
    } else {
        echo "請填寫所有必填欄位";
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
    <?php include 'toptitle.php'; ?>
    <h2>修改溫濕度資料</h2>
    <?php if ($row): ?>
        <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
            <input type="hidden" name="id" value="<?php echo $row['id']; ?>">
            <label>MAC: <input type="text" name="mac" value="<?php echo htmlspecialchars($row['MAC']); ?>" maxlength="12" required></label><br>
            <label>IP: <input type="text" name="ip" value="<?php echo htmlspecialchars($row['IP'] ?? ''); ?>" maxlength="20"></label><br>
            <label>溫度: <input type="number" step="0.1" name="temperature" value="<?php echo $row['temperature']; ?>" required></label><br>
            <label>濕度: <input type="number" step="0.1" name="humidity" value="<?php echo $row['humidity']; ?>" required></label><br>
            <label>系統時間: <input type="text" name="systime" value="<?php echo htmlspecialchars($row['systime']); ?>" maxlength="14" required></label><br>
            <input type="submit" value="更新">
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