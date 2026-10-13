<?php
// 引入 iotcnn.php 檔案，通常用來建立與資料庫的連線
include("iotcnn.php");

// 呼叫 Connection() 函式取得資料庫連線物件（$link 代表一個 MySQLi 連線）
$link = Connection();

// SQL 查詢語法：從 dhtdata 資料表中，選擇 crtdatetime（建立時間）、temperature（溫度）、humidity（濕度）三個欄位
// 並依照 crtdatetime 欄位進行排序（ORDER BY 預設為遞增，由舊到新）
$query = "SELECT crtdatetime,systime, temperature, humidity FROM dhtdata ORDER BY systime asc  limit 0,50;";

// 使用資料庫連線物件執行 SQL 查詢
$result = $link->query($query);
//echo $query."<br>" ;
// 建立一個空陣列 $data，用來儲存查詢結果
$data = [];

// 使用 while 迴圈逐筆讀取查詢結果（fetch_assoc() 會回傳關聯式陣列，欄位名稱作為索引）
while ($row = $result->fetch_assoc()) {
    $data[] = $row; // 將每筆資料加入 $data 陣列
	//echo $row["systime"]."/".$row["temperature"]."/".$row["humidity"]."<br>";
}
?>


<!DOCTYPE html>
<html lang="zh-TW">
<head>
    <meta charset="UTF-8">
    <title>資料圖表</title>
    <!-- 載入 Chart.js 函式庫（用來繪製統計圖表） -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
    <!-- 引入頁面標題與導覽列（toptitle.php 通常包含網站標題、LOGO 或選單） -->
    <?php include 'toptitle.php'; ?>

    <h2>溫濕度資料圖表</h2>

    <!-- canvas 是 HTML5 提供的畫布標籤，Chart.js 會在這個元素上繪圖 -->
    <canvas id="myChart" width="800" height="400"></canvas>

    <script>
        // 取得 canvas 元素的 2D 繪圖環境（ctx 用來傳給 Chart.js 建立圖表）
        const ctx = document.getElementById('myChart').getContext('2d');

        // 將 PHP 陣列 $data 中的 crtdatetime 欄位值取出成一維陣列，並轉成 JSON 格式給 JavaScript
        const labels = <?php echo json_encode(array_column($data, 'crtdatetime')); ?>;

        // 將 $data 陣列中的 temperature 欄位取出成一維陣列（溫度資料）
        const temperatureData = <?php echo json_encode(array_column($data, 'temperature')); ?>;

        // 將 $data 陣列中的 humidity 欄位取出成一維陣列（濕度資料）
        const humidityData = <?php echo json_encode(array_column($data, 'humidity')); ?>;

        // 使用 Chart.js 建立折線圖（line chart）
        const myChart = new Chart(ctx, {
            type: 'line', // 圖表類型：折線圖
            data: {
                labels: labels, // X 軸的標籤（時間）
                datasets: [
                    {
                        label: '溫度 (°C)',  // 資料集名稱（顯示在圖例 legend 上）
                        data: temperatureData, // 對應的資料（溫度）
                        borderColor: 'red',    // 折線顏色：紅色
                        fill: false            // 不填滿線下方的區域
                    },
                    {
                        label: '濕度 (%)',     // 資料集名稱（顯示在圖例 legend 上）
                        data: humidityData,    // 對應的資料（濕度）
                        borderColor: 'blue',   // 折線顏色：藍色
                        fill: false            // 不填滿線下方的區域
                    }
                ]
            },
            options: {
                // 設定座標軸
                scales: {
                    // X 軸設定
                    x: {
                        //type: 'time', // X 軸為時間軸（需搭配時間格式的資料）
                        time: {
                            unit: 'hour' // 時間單位為分鐘，可依需求改為 'hour', 'day'
                        },
                        title: {
                            display: true, // 顯示 X 軸標題
                            text: '時間'   // 標題文字
                        }
                    },
                    // Y 軸設定
                    y: {
                        title: {
                            display: true, // 顯示 Y 軸標題
                            text: '數值'   // 標題文字
                        }
                    }
                }
            }
        });
    </script>

    <!-- 引入頁面底部（topfooter.php 通常包含版權、聯絡資訊等） -->
    <?php include 'topfooter.php'; ?>
</body>
</html>
