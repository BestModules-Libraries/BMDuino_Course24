<?php
include("iotcnn.php");
$link = Connection();

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $mac = $_POST['mac'] ?? '';
    $ip = $_POST['ip'] ?? '';
    $temperature = $_POST['temperature'] ?? '';
    $humidity = $_POST['humidity'] ?? '';
    $systime = $_POST['systime'] ?? '';

    // Basic validation
    if (!empty($mac) && !empty($temperature) && !empty($humidity) && !empty($systime)) {
        $query = "INSERT INTO dhtdata (MAC, IP, temperature, humidity, systime) VALUES (?, ?, ?, ?, ?)";
        $stmt = $link->prepare($query);
        $stmt->bind_param("sssdd", $mac, $ip, $temperature, $humidity, $systime);
        
        if ($stmt->execute()) {
            echo "新紀錄已成功創建";
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
    <title>新增資料</title>
</head>
<body>
    <?php include 'toptitle.php'; ?>
    <h2>新增溫濕度資料</h2>
    <form method="post" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <label>MAC: <input type="text" name="mac" maxlength="12" required></label><br>
        <label>IP: <input type="text" name="ip" maxlength="20"></label><br>
        <label>溫度: <input type="number" step="0.1" name="temperature" required></label><br>
        <label>濕度: <input type="number" step="0.1" name="humidity" required></label><br>
        <label>系統時間: <input type="text" name="systime" maxlength="14" required></label><br>
        <input type="submit" value="提交">
    </form>
    <?php include 'topfooter.php'; ?>
</body>
</html>