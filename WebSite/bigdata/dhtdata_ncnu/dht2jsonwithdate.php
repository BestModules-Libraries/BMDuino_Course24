<?php

include("../Connections/iotcnn.php"); // 包含資料庫連接檔案
$link = Connection(); // 建立 MySQL 連接物件

/*
{
    "Device": "3C71BFFD882C",
    "Datalist": [
        {
            "DateTime": "20220101",
            "Temperature": "23",
            "Humidity": "23"
        }
    ]
}
*/
// 預期的 JSON 輸出格式

class maindata {
    public $Device; // MAC 設備編號
    public $Datalist; // 資料列表
}
class subdata {
    public $DateTime; // 日期和時間
    public $Temperature; // 溫度值
    public $Humidity; // 濕度值
}

$maindata = new maindata(); // 建立 maindata 類別的實例

if (!isset($_GET["MAC"])) {
    echo "no MAC Data <br>"; // 如果 MAC 參數未設定，輸出錯誤訊息
}

if (!isset($_GET["dt1"])) {
    echo "no Start Date <br>"; // 如果開始日期參數未設定，輸出錯誤訊息
}

if (!isset($_GET["dt2"])) {
    echo "no End Date <br>"; // 如果結束日期參數未設定，輸出錯誤訊息
}

$sid = $_GET["MAC"]; // 獲取 MAC 設備編號
$s1 = $_GET["dt1"]; // 獲取開始日期
$s2 = $_GET["dt2"]; // 獲取結束日期

$qry1 = "select * from ncnuiot.dhtData where mac = '%s' and systime >= '%s' and systime <= '%s' order by systime asc"; // SQL 查詢模板
$qrystr = sprintf($qry1, $sid, $s1, $s2); // 將參數填入 SQL 查詢

//echo $qrystr."<br>"; // 除錯：列印 SQL 查詢

$result = mysqli_query($link, $qrystr); // 執行 SQL 查詢
$cnt = 1; // 這個變數設定但未使用
$count = mysqli_num_rows($result); // 獲取返回的行數

//echo $qrystr."<br>"; // 除錯：列印 SQL 查詢

if ($count > 0) {
    $dd = array(); // 建立陣列用於儲存 subdata 物件

    while ($row = mysqli_fetch_array($result)) {
        $tmp1 = sprintf("%10.2f", (float)($row['temperature'])); // 將溫度格式化為兩位小數
        $tmp2 = sprintf("%10.2f", (float)($row['humidity'])); // 將濕度格式化為兩位小數
        $subdata = new subdata(); // 建立新的 subdata 物件
        $subdata->DateTime = $row['systime']; // 設定日期和時間
        $subdata->Humidity = $tmp1; // 設定濕度（但 tmp1 是溫度，存在賦值錯誤）
        $subdata->Temperature = $tmp2; // 設定溫度（但 tmp2 是濕度，存在賦值錯誤）
        array_push($dd, $subdata); // 將 subdata 添加到陣列
    }
    $maindata->Device = $sid; // 設定設備 MAC 地址
    $maindata->Datalist = $dd; // 設定資料列表
   echo json_encode($maindata, JSON_UNESCAPED_UNICODE); // 輸出 JSON 資料

}

mysqli_free_result($result); // 釋放結果集
mysqli_close($link); // 關閉資料庫連接
?>