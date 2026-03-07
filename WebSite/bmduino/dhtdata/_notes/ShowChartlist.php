<?php
    // 包含共用的資料庫和連線設置
    include("../comlib.php");  // 引入通用函數庫
    include("../Connections/iotcnn.php");  // 引入資料庫連線配置

    // 建立資料庫連線
    $link = Connection();  // 建立 MySQL 連線物件

    // 獲取 MAC 地址，從 GET 或 POST 請求中
    if (!isset($_GET["MAC"])) {  // 如果 GET 請求中沒有 MAC
        $mid = $_POST["MAC"];  // 從 POST 請求中獲取 MAC
    } else {
        $mid = $_GET["MAC"];  // 從 GET 請求中獲取 MAC
    }

    // 設定查詢的起始日期
    if (!isset($_POST["dt1"])) {  // 如果 POST 請求中沒有開始日期
        $sd1 = getshiftdataorder(-3 * 24);  // 設定預設開始日期（3 天前）
        $dd1 = $sd1;  // 開始日期
    } else {
        $dd1 = $_POST["dt1"];  // 從 POST 請求中獲取開始日期
        $sd1 = $_POST["dt1"];  // 開始日期
    }
    
    // 設定查詢的結束日期
    if (!isset($_POST["dt2"])) {  // 如果 POST 請求中沒有結束日期
        $sd2 = getdataorder();  // 設定預設結束日期（現在）
        $dd2 = $sd2;  // 結束日期
    } else {
        $dd2 = $_POST["dt2"];  // 從 POST 請求中獲取結束日期
        $sd2 = $_POST["dt2"];  // 結束日期
    }

    // 構建 SQL 查詢，用於獲取特定 MAC 地址在指定日期範圍內的資料
    $qry = "SELECT * FROM big.dhtdata WHERE MAC = '%s' AND systime >= '%s' AND systime <= '%s' ORDER BY systime ASC";
    //SELECT * FROM big.dhtdata WHERE MAC = '246F28248CE0' AND systime >= '20160601112210' AND systime <= '20240604112210' ORDER BY systime ASC

    $qrystr = sprintf($qry, $mid, $dd1, $dd2);  // 使用 sprintf 格式化查詢語句

    // 初始化空陣列，用於存儲查詢結果
    $d00 = array();  // 儲存時間戳記（格式化後）
    $d00a = array();  // 儲存原始時間戳記
    $d01 = array();  // 儲存溫度
    $d02 = array();  // 儲存濕度
    $d03 = array();  // 儲存 MAC 地址

    // 執行 SQL 查詢
    $result = mysqli_query($link, $qrystr);  // 執行查詢
    if ($result !== FALSE) 
    {  // 如果查詢成功
        // 遍歷查詢結果，並將資料存入對應的陣列中
        while ($row = mysqli_fetch_array($result)) 
        {
            array_push($d00, trandatetime3($row["systime"]));  // 轉換後的時間戳記
            array_push($d00a, $row["systime"]);  // 原始時間戳記
            array_push($d01, (double)sprintf("%8.2f", (double)$row["temperature"]));  // 溫度
            array_push($d02, (double)sprintf("%8.2f", (double)$row["humidity"]));  // 濕度
            array_push($d03, $row["MAC"]);  // MAC 地址
        }
    }

    // 釋放查詢結果並關閉資料庫連線
    mysqli_free_result($result);  // 釋放結果集資源
    mysqli_close($link);  // 關閉資料庫連線

    // 建立 CSV 檔案，用於儲存查詢結果
    $myfile = fopen("../tmp/dhtdata.csv", "w");  // 打開 CSV 檔案以進行寫入

    //設定CSV輸入資料格式化字串
    $datah = "'%s', '%s', '%s'\n";  // CSV 標題
    $datar = "'%s', %10.3f, %5.2f\n";  // CSV 資料格式

    //利用資料格式化字串產生標題真正輸出內容
    $datarow = sprintf($datah, "DateTime", "Temperature", "Humidity");  // 輸出真正輸出內容

    // 將標題真正輸出內容，寫入檔案
    fwrite($myfile, $datarow);  // 寫入標題

    // 利用迴圈，將查詢資料列結果寫入 CSV 檔案
    for ($i = 0; $i < count($d00a); $i++) {  
        //依迴圈產生資料列寫入CSV
        $datarow = sprintf($datar, $d00a[$i], $d01[$i], $d02[$i]);  // 格式化資料
        fwrite($myfile, $datarow);  // 寫入 CSV
    }

    // 關閉 CSV 檔案
    fclose($myfile);  // 關閉檔案
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">  <!-- XHTML 文件類型宣告 -->
<html xmlns="http://www.w3.org/1999/xhtml">  <!-- XHTML 命名空間 -->
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />  <!-- 設定字符編碼 -->
    <title>Display Temperature and Humidity Curve Chart by MAC</title>  <!-- 頁面標題 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />  <!-- 連結外部樣式表 -->

    <!-- 引入 Highcharts 的 JavaScript 模組，用於繪製圖表 -->
    <script src="../code/highcharts.js"></script>
    <script src="../code/highcharts-more.js"></script>
    <script src="../code/modules/exporting.js"></script>
    <script src="../code/modules/export-data.js"></script>
    <script src="../code/modules/accessibility.js"></script>  
</head>

<body>
    <?php
        // 包含網頁頂部的共用內容
        include("../toptitle.php");  // 引入網頁頂部標題
    ?>


    <!-- 表單:用於設置查詢條件 -->
    <form id="form1" name="form1" method="post" action="">  <!-- 表單開頭 -->
        <table width="100%" border="1">  <!-- 表格開頭 -->
<tr>
    <td colspan='5'><div align='center'>Temperature & Humidity Sensor(溫濕度感測裝置)</div></td>
</tr>
<tr>  <!-- 表格內的表單項目 -->
    <td width="220">開始日期時間(YYYYMMDDHHMMSS)</td>  <!-- 開始日期時間 -->
    <td width="200">
        <input name="MAC" type="hidden" id="MAC" value="<?php echo $mid; ?>" />  <!-- 隱藏的 MAC 值 -->
        <input type="text" name="dt1" id="dt1" size="14" maxlength="14" value="<?php echo $sd1; ?>" />  <!-- 開始日期的輸入框 -->
    </td>
    <td width="220">結束日期時間(YYYYMMDDHHMMSS)</td>  <!-- 結束日期時間 -->
    <td width="200">
        <input type="text" name="dt2" id="dt2" size="14" maxlength="14" value="<?php echo $sd2; ?>" />  <!-- 結束日期的輸入框 -->
    </td>
    <td width="200">
        <input type="submit" name="button" id="button" value="送出" />  <!-- 送出按鈕 -->
        <a href="/tmp/dhtdata.csv">下載</a>  <!-- CSV 檔案的下載連結 -->
    </td>
</tr>
        </table>
    </form>  <!-- 表單結束 -->

    <!-- 容器，用於顯示 Highcharts 的溫度曲線圖 -->
    <div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>

    <!-- 容器，用於顯示 Highcharts 的濕度曲線圖 -->
    <div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>

    <!-- JavaScript，用於繪製圖表 -->
    <script type="text/javascript">
        // 溫度曲線圖的設定
        Highcharts.chart('container1', {
            chart: {
                zoomType: 'x'  // 允許 x 軸縮放
            },
            title: {
                text: 'Temperature °C by MAC:<?php echo $mid ?>'  // 圖表標題
            },
            xAxis: {
                categories: <?php echo json_encode($d00, JSON_UNESCAPED_UNICODE); ?>  // x 軸標籤
            },
            yAxis: {
                title: {
                    text: '°C'  // y 軸標籤
                }
            },
            legend: {
                enabled: false  // 不顯示圖例
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 0,
                            x2: 0,
                            y2: 1
                        },
                    },
                    marker: {
                        radius: 2  // 標記點的半徑
                    },
                    lineWidth: 0.1,  // 線條寬度
                    states: {
                        hover: {
                            lineWidth: 1  // 當滑鼠懸停時，增加線條寬度
                        }
                    },
                    threshold: null  // 沒有閾值
                }
            },
            series: [{
                name: 'Temperature',  // 數據系列名稱
                data: <?php echo json_encode($d01, JSON_UNESCAPED_UNICODE); ?>  // 溫度數據
            }]
        });

        // 濕度曲線圖的設定
        Highcharts.chart('container2', {
            chart: {
                zoomType: 'x'  // 允許 x 軸縮放
            },
            title: {
                text: 'Humidity Curve Chart by MAC:<?php echo $mid ?>'  // 圖表標題
            },
            xAxis: {
                categories: <?php echo json_encode($d00, JSON_UNESCAPED_UNICODE); ?>  // x 軸標籤
            },
            yAxis: {
                title: {
                    text: 'Percent(%)'  // y 軸標籤
                }
            },
            legend: {
                enabled: false  // 不顯示圖例
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 0,
                            x2: 0,
                            y2: 1
                        },
                    },
                    marker: {
                        radius: 2  // 標記點的半徑
                    },
                    lineWidth: 0.1,  // 線條寬度
                    states: {
                        hover: {
                            lineWidth: 1  // 當滑鼠懸停時，增加線條寬度
                        }
                    },
                    threshold: null  // 沒有閾值
                }
            },
            series: [{
                name: 'Percent(%)',  // 數據系列名稱
                data: <?php echo json_encode($d02, JSON_UNESCAPED_UNICODE); ?>  // 濕度數據
            }]
        });
    </script>

    <?php
        // 包含頁面底部的共用內容
        include("../topfooter.php");  // 引入頁面底部
    ?> 
</body>
</html>
