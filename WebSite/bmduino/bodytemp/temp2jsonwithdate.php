<?php     
// 範例網址（這支 PHP 會根據 MAC、起始日期與結束日期來查詢血氧感測資料，並回傳 JSON 格式資料）
// http://iot.arduino.org.tw:8888/bigdata/bodytemp/temp2jsonwithdate.php?MAC=AABBCCDDEEFF=20200406&end=20251231


// ✅ 功能摘要：
// 1. 讀取網址參數：MAC（裝置代號）、start（查詢起始時間）、end（查詢結束時間）。
// 2. 根據這些參數，查詢 MySQL 資料庫中的 bloodoxygen 資料表。
// 3. 把查詢結果轉換成 JSON 格式（包含 MAC、時間區間、血氧值、心跳值）。
// 4. 回傳給前端（例如網頁或 APP）使用，常見應用是用 Chart.js 畫感測歷史資料曲線。

// ---------------------------------------------------
// 載入資料庫連線函式庫，這個檔案內含 Connection() 函式，
// 用來建立與 MySQL 的連線。
// ---------------------------------------------------
include("../Connections/iotcnn.php");

// 呼叫自訂的 Connection() 函式，建立資料庫連線，回傳的連線物件存放在 $link
$link = Connection();

/*
輸出 JSON 格式：
{
  "Device": "246F28248CE0",
  "Interval": "20200406~20251231",
  "DateTime": ["092318564","092401485","092402242"],
  "bodytempature": [97.4,97.4,97.4]
}
*/

// ---------------------------------------------------
// 定義一個類別 maindata，用來儲存輸出的資料結構
// ---------------------------------------------------
class maindata {
  public $Device;       // 裝置的 MAC 位址
  public $Interval;     // 查詢的時間區間（start~end）
  public $DateTime;     // 紀錄時間的陣列
  public $bodytemperature;       // 體溫值陣列
}

// 建立一個 maindata 物件實體，後面查詢結果會放進來
$maindata = new maindata();

// ---------------------------------------------------
// 基本防呆：檢查 GET 參數是否存在
// ---------------------------------------------------
if (!isset($_GET["MAC"])) {
    echo "no MAC Data <br>";   // 沒有傳入 MAC 參數 → 顯示錯誤
}

if (!isset($_GET["start"])) {
    echo "no Start Date <br>"; // 沒有傳入開始日期
}

if (!isset($_GET["end"])) {
    echo "no End Date <br>";   // 沒有傳入結束日期
}

// ---------------------------------------------------
// 從網址參數取出使用者輸入的查詢條件
// ---------------------------------------------------
$sid = $_GET["MAC"];     // 裝置的 MAC 位址
$s1 = $_GET["start"];    // 查詢起始日期 (格式 YYYYMMDD)
$s2 = $_GET["end"];      // 查詢結束日期 (格式 YYYYMMDD)

// ---------------------------------------------------
// SQL 查詢樣板：
// 1. 從 bloodoxygen 資料表查詢指定 MAC 的資料
// 2. systime 欄位介於 start 與 end 之間
// 3. 按 systime 升冪排序
// ---------------------------------------------------
$qry1 = "select * from bodytemp where MAC = '%s' and systime >= '%s' and systime <= '%s' order by systime asc";

// 使用 sprintf 將變數帶入 SQL 字串
$qrystr = sprintf($qry1, $sid, $s1, $s2);

// 執行 SQL 查詢，結果存到 $result
$result = mysqli_query($link, $qrystr);

// 計算查詢到幾筆資料
$count = mysqli_num_rows($result);

// ---------------------------------------------------
// 如果有查到資料才處理，否則不輸出
// ---------------------------------------------------
if ($count > 0) {
  $d1 = array();  // 儲存血氧值 (blood)
  $d2 = array();  // 儲存時間 (systime 的部分字串)

  // 逐筆讀取資料
  while($row = mysqli_fetch_array($result)) 
  {
     // 轉型後丟到對應陣列
     array_push($d1, (float)$row["bodytemperature"]);          // 血氧值轉 float
     array_push($d2, $row["systime"]);   // systime 從第5字元開始取9位數 → 縮短時間字串
  }

  // 將查詢結果整理到物件裡 
  $maindata->Device = $sid;
  $maindata->Interval = $s1."~".$s2;
  $maindata->DateTime = $d2;
  $maindata->bodytemperature = $d1;

  // 輸出 JSON 字串
  // JSON_UNESCAPED_UNICODE：避免中文被轉成 \uXXXX
  echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
}

// ---------------------------------------------------
// 善後：釋放查詢資源並關閉資料庫連線
// ---------------------------------------------------
mysqli_free_result($result);
mysqli_close($link);
?>
