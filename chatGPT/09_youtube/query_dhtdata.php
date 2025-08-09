<?php
include("iotcnn.php");
$link = Connection();

$results = [];
if (isset($_GET['mac'])) {
    $mac = $_GET['mac'];
    $query = "SELECT * FROM dhtdata WHERE MAC = ? ORDER BY crtdatetime";
    $stmt = $link->prepare($query);
    $stmt->bind_param("s", $mac);
    $stmt->execute();
    $result = $stmt->get_result();
    while ($row = $result->fetch_assoc()) {
        $results[] = $row;
    }
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
    <?php include 'toptitle.php'; ?>
    <h2>查詢溫濕度資料</h2>
    <form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>">
        <label>輸入 MAC: <input type="text" name="mac" maxlength="12" required></label><br>
        <input type="submit" value="查詢">
    </form>
    <?php if (!empty($results)): ?>
        <h3>查詢結果:</h3>
        <table border="1">
            <tr>
                <th>ID</th><th>MAC</th><th>IP</th><th>溫度</th><th>濕度</th><th>系統時間</th><th>建立時間</th>
            </tr>
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
        <p>沒有找到記錄</p>
    <?php endif; ?>
    <?php include 'topfooter.php'; ?>
</body>
</html>