<?php
include("iotcnn.php");
$link = Connection();

$query = "SELECT crtdatetime, temperature, humidity FROM dhtdata ORDER BY crtdatetime";
$result = $link->query($query);
$data = [];
while ($row = $result->fetch_assoc()) {
    $data[] = $row;
}
?>

<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>資料圖表</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <?php include 'toptitle.php'; ?>
    <h2>溫濕度資料圖表</h2>
    <canvas id="myChart" width="800" height="400"></canvas>
    <script>
        const ctx = document.getElementById('myChart').getContext('2d');
        const labels = <?php echo json_encode(array_column($data, 'crtdatetime')); ?>;
        const temperatureData = <?php echo json_encode(array_column($data, 'temperature')); ?>;
        const humidityData = <?php echo json_encode(array_column($data, 'humidity')); ?>;

        const myChart = new Chart(ctx, {
            type: 'line',
            data: {
                labels: labels,
                datasets: [
                    {
                        label: '溫度 (°C)',
                        data: temperatureData,
                        borderColor: 'red',
                        fill: false
                    },
                    {
                        label: '濕度 (%)',
                        data: humidityData,
                        borderColor: 'blue',
                        fill: false
                    }
                ]
            },
            options: {
                scales: {
                    x: {
                        type: 'time',
                        time: {
                            unit: 'minute'
                        },
                        title: {
                            display: true,
                            text: '時間'
                        }
                    },
                    y: {
                        title: {
                            display: true,
                            text: '數值'
                        }
                    }
                }
            }
        });
    </script>
    <?php include 'topfooter.php'; ?>
</body>
</html>