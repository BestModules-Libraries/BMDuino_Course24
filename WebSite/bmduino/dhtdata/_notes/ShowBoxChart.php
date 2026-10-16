<?php
    // http://ncnu.arduino.org.tw:9999/dhtdata/ShowChartlistday.php?MAC=CC50E3B5BB20
    // 這個網址用來傳遞參數，用於顯示溫濕度數據的曲線圖

    // 包含資料庫的呼叫程式
    include("../comlib.php");  // 共用函數庫
    include("../Connections/iotcnn.php");  // 資料庫連線配置

    // 建立MySQL資料庫連線
    $link = Connection();  // 建立MySQL連線物件

    // 確定MAC地址是從GET還是POST取得
    if (!isset($_GET["MAC"])) {  // 如果GET參數中沒有MAC
        $mid = $_POST["MAC"];  // 從POST取得MAC地址
    } else {
        $mid = $_GET["MAC"];  // 從GET取得MAC地址
    }

    // 設定開始和結束日期
    if (!isset($_POST["dt1"])) {  // 如果POST參數中沒有開始日期
        $sd1 = getshiftdayorder(-7);  // 預設開始日期為7天前
        $dd1 = $sd1;  // 設定預設開始日期
    } else {
        $dd1 = $_POST["dt1"];  // 從POST取得開始日期
        $sd1 = $_POST["dt1"];  // 設定開始日期
    }

    if (!isset($_POST["dt2"])) {  // 如果POST參數中沒有結束日期
        $sd2 = getdayorder();  // 預設結束日期為當天
        $dd2 = $sd2;  // 設定預設結束日期
    } else {
        $dd2 = $_POST["dt2"];  // 從POST取得結束日期
        $sd2 = $_POST["dt2"];  // 設定結束日期
    }

    // SQL查詢語句，用於獲取指定MAC地址在指定時間範圍內的數據
    $qry = "SELECT left(systime, 8) AS systime FROM big.dhtdata WHERE MAC = '%s' AND systime >= '%s' AND systime <= '%s' GROUP BY left(systime, 8) ORDER BY systime ASC";
    $qrystr = sprintf($qry, $mid, $dd1, $dd2);  // 格式化查詢語句
    
    $qry2 = "SELECT temperature, humidity FROM big.dhtdata WHERE MAC = '%s' AND left(systime, 8) = '%s' ORDER BY systime DESC";
    $qrystr2 = sprintf($qry2, $mid, $dd1, $dd2);  // 格式化第二個查詢語句

    // 初始化空陣列，用於存儲查詢結果
    $d00 = array();  // 儲存日期
    $d01 = array();  // 儲存溫度
    $d02 = array();  // 儲存濕度
    $d03 = array();  // 儲存MAC地址

    // 執行查詢
    $result = mysqli_query($link, $qrystr);  // 執行查詢
    $cn = 0;  // 計數器
    if ($result !== FALSE) {  // 如果查詢成功
        while ($row = mysqli_fetch_array($result)) {  // 遍歷結果
            array_push($d00, $row["systime"]);  // 將日期加入陣列
        }
    }

    // 釋放結果資源並關閉連線
    mysqli_free_result($result);  // 釋放結果集
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">  <!-- XHTML 文件類型 -->
<html xmlns="http://www.w3.org/1999/xhtml">  <!-- XHTML 命名空間 -->
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />  <!-- 設定字符編碼 -->
    <title>Display Temperature and Humidity Data as Curve Chart by MAC</title>  <!-- 頁面標題 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />  <!-- 連結樣式表 -->

    <!-- 引入Highcharts JavaScript模組，用於繪製圖表 -->
    <script src="../code/highcharts.js"></script>
    <script src="../code/highcharts-more.js"></script>
    <script src="../code/modules/exporting.js"></script>
    <script src="../code/modules/export-data.js"></script>
    <script src="../code/modules/accessibility.js"></script>
</head>

<body>
    <?php
        include '../toptitle.php';  // 包含標題
    ?>

    <!-- 返回上一頁的按鈕 -->
    <input type="button" onclick="history.back()" value="BACK(回到上一頁)">
    </input>

    <!-- 表單，用於設定查詢條件 -->
    <form id="form1" name="form1" method="post" action="">
        <table width="100%" border="1">
            <tr>
                <td width="220">開始日期(YYYYMMDD)</td>
                <td width="200">
                    <input name="MAC" type="hidden" id="MAC" value="<?php echo $mid; ?>" />  <!-- 傳遞MAC地址 -->
                    <input type="text" name="dt1" id="dt1" size="8" maxlength="8" value="<?php echo $sd1; ?>" />
                </td>
                <td width="220">結束日期(YYYYMMDD)</td>
                <td width="200">
                    <input type="text" name="dt2" id="dt2" size="8" maxlength="8" value="<?php echo $sd2; ?>" />
                </td>
                <td width="200">
                    <input type="submit" name="button" id="button" value="送出" />  <!-- 送出按鈕 -->
                </td>
            </tr>
        </table>
    </form>

    <!-- 容器，用於繪製Highcharts的圖表 -->
    <div id="container1" style="min-width: 95%; height: 600px; margin: 0 auto"></div>
    <div id="container2" style="min-width: 95%; height: 600px; margin: 0 auto"></div>

    <!-- JavaScript，用於初始化Highcharts並繪製圖表 -->
    <script type="text/javascript">
        // 繪製溫度的箱形圖
        Highcharts.chart('container1', {
            chart: {
                type: 'boxplot'  // 使用箱形圖
            },
            title: {
                text: 'Temperature °C by MAC:<?php echo $mid?>',  // 圖表標題
                fontsize: 30
            },
            xAxis: {
                categories: [
                    <?php
                    $cn = 1;
                    for ($i = 0; $i < count($d00); $i++) {
                        echo "'".$d00[$i]."'";  // 加入日期
                        if ($cn < count($d00)) {
                            echo ",\n";  // 添加換行
                        }
                        $cn++;
                    }
                    ?>
                ]
            },
            yAxis: {
                title: {
                    text: '°C'  // y軸標題
                }
            },
            series: [{
                name: 'Temperature',
                data: [
                    <?php
                    $cn = 1;
                    for ($i = 0; $i < count($d00); $i++) {
                        $qrystr2 = sprintf($qry2, $mid, $d00[$i]);  // 獲取當日溫度數據
                        $result2 = mysqli_query($link, $qrystr2);  // 執行查詢
                        if (mysqli_num_rows($result2) > 0) {  // 如果有結果
                            echo "[";  // 開始箱形圖的數據
                            $cn2 = 1;
                            while ($row2 = mysqli_fetch_array($result2)) {  // 遍歷結果
                                echo $row2["temperature"];  // 加入溫度
                                if ($cn2 < mysqli_num_rows($result2)) {
                                    echo ",";  // 添加逗號分隔
                                }
                                $cn2++;
                            }
                            echo "]";  // 結束箱形圖的數據
                        }
                        if ($cn < count($d00)) {
                            echo ",\n";  // 添加換行
                        }
                        $cn++;
                    }
                    ?>
                ]
            }]
        });

        // 繪製濕度的箱形圖
        Highcharts.chart('container2', {
            chart: {
                type: 'boxplot'  // 使用箱形圖
            },
            title: {
                text: 'Humidity % by MAC:<?php echo $mid?>',  // 圖表標題
                fontsize: 30
            },
            xAxis: {
                categories: [
                    <?php
                    $cn = 1;
                    for ($i = 0; $i < count($d00); $i++) {
                        echo "'".$d00[$i]."'";  // 加入日期
                        if ($cn < count($d00)) {
                            echo ",\n";  // 添加換行
                        }
                        $cn++;
                    }
                    ?>
                ]
            },
            yAxis: {
                title: {
                    text: '%'  // y軸標題
                }
            },
            series: [{
                name: 'Humidity',
                data: [
                    <?php
                    $cn = 1;
                    for ($i = 0; $i < count($d00); $i++) {
                        $qrystr2 = sprintf($qry2, $mid, $d00[$i]);  // 獲取當日濕度數據
                        $result2 = mysqli_query($link, $qrystr2);  // 執行查詢
                        if (mysqli_num_rows($result2) > 0) {  // 如果有結果
                            echo "[";  // 開始箱形圖的數據
                            $cn2 = 1;
                            while ($row2 = mysqli_fetch_array($result2)) {  // 遍歷結果
                                echo $row2["humidity"];  // 加入濕度
                                if ($cn2 < mysqli_num_rows($result2)) {
                                    echo ",";  // 添加逗號
                                }
                                $cn2++;
                            }
                            echo "]";  // 結束箱形圖的數據
                        }
                        if ($cn < count($d00)) {
                            echo ",\n";  // 添加換行
                        }
                        $cn++;
                    }
                    ?>
                ]
            }]
        });
    </script>

    <?php
        include '../topfooter.php';  // 包含頁尾
        mysqli_close($link);  // 關閉資料庫連線
    ?>   
</body>
</html>
