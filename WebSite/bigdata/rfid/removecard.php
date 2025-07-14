<?php
// 範例網址（用於測試作廢 RFID 卡片的功能）：
// http://iot.arduino.org.tw:8888/bigdata/rfid/removecard.php?KEY=0219390674&MAC=B8D61A68DAFC
// http://localhost:8888/bigdata/rfid/removecard.php?KEY=0219390674&MAC=B8D61A68DAFC

    include("../Connections/iotcnn.php"); // 引入資料庫連線檔案，通常內含 Connection() 函式

    $link = Connection(); // 呼叫資料庫連線函式，取得連線物件

    // 定義資料結構類別，用來儲存回傳給前端的結果
    class maindata
    {
        public $Device;  // MAC：裝置的實體卡片位址
        public $Card;    // KEY：RFID 卡號
        public $Result;  // 結果訊息，例如 "Card Removed"
    }

    $maindata = new maindata();  // 建立 maindata 類別的實體

    // 取得用戶端 IP 位址，紀錄來源
    $ip = getenv("REMOTE_ADDR");

    // 呼叫自訂函式，取得目前系統的日期與時間（格式如：20250528143000）
    $sysdt = getdatetime();

    // 檢查網址參數中是否有提供 MAC（裝置編號）
    if (!isset($_GET["MAC"])) {
        echo "Device MAC lost <br>";  // 若未提供則顯示錯誤
        die(); // 終止執行
    }

    // 檢查網址參數中是否有提供 RFID KEY（卡號）
    if (!isset($_GET["KEY"])) {
        echo "RFID KEY lost <br>";  // 若未提供則顯示錯誤
        die(); // 終止執行
    }

    // 將網址中的 MAC 與 KEY 參數儲存到變數中
    $temp0 = $_GET["MAC"]; // 裝置 MAC
    $temp1 = $_GET["KEY"]; // RFID 卡號

    // 確認 MAC 不為空值
    if (trim($temp0) == "") {
        echo "MAC is empty string <br>";
        die(); // 終止程式
    }

    // 確認 RFID 卡號不為空值
    if (trim($temp1) == "") {
        echo "Card number is empty string <br>";
        die(); // 終止程式
    }

    // SQL 查詢語句：查找 rfidlist 資料表中，符合卡號且 activate 為 1 的紀錄（1 表示啟用中）
    $qry = "select * from rfidlist where rfidkey = '%s' and activate = 1 ";
    $qrystr = sprintf($qry, $temp1); // 將卡號代入查詢語句

    // 執行 SQL 查詢
    $result = mysqli_query($link, $qrystr);

    // 取得符合條件的資料筆數
    $num = mysqli_num_rows($result);

    // 若沒有找到任何資料
    if ($num == 0) {
        $passok = "Card Not Existed";  // 卡片不存在或已非啟用狀態
    }

    // 若找到了至少一筆資料（通常只有一筆）
    if ($num >= 1) {
        $row = mysqli_fetch_array($result); // 取出資料
        $id = $row["id"];  // 取得該筆資料的 ID

        // 如果該卡片的 activate 欄位為 1（表示啟用中）
        if ((int)$row["activate"] == 1) {
            // 將該卡片的 activate 狀態更新為 9（表示作廢），並更新時間與 IP 位址
            $qry = "update rfidlist set activate = 9, systime = '%s' , IP = '%s' where id = %s";
            $qrystr = sprintf($qry, $sysdt, $ip, $id); // 格式化 SQL 語句

            $result = mysqli_query($link, $qrystr); // 執行更新語句

            $passok = "Card Removed"; // 成功標記為已作廢
        }
    }

    // 將處理結果填入物件中
    $maindata->Device = $temp0;   // 設定裝置 MAC
    $maindata->Card = $temp1;     // 設定 RFID 卡號
    $maindata->Result = $passok;  // 設定處理結果

    // 若有查詢結果集，釋放記憶體資源
    if ($result && $result instanceof mysqli_result) {
        mysqli_free_result($result);
    }

    // 關閉資料庫連線
    mysqli_close($link);

    // 將結果轉為 JSON 格式並輸出，JSON_UNESCAPED_UNICODE 避免中文轉碼為 Unicode
    echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
?>


<?php
    // 將日期時間轉換為指定格式（yyyymmddhhmmss）
    function getdataorder($dt) {
        $splitTimeStamp = explode(" ", $dt);
        $vdate = explode('-', $splitTimeStamp[0]);
        $vtime = explode(':', $splitTimeStamp[1]);
        $yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
        $min = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
        $sec = str_pad($vtime[2], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min.$sec);
    }

    // 同上，但不含秒數
    function getdataorder2($dt) {
        $splitTimeStamp = explode(" ", $dt);
        $vdate = explode('-', $splitTimeStamp[0]);
        $vtime = explode(':', $splitTimeStamp[1]);
        $yyyy = str_pad($vdate[0], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($vdate[1], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($vdate[2], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($vtime[0], 2, "0", STR_PAD_LEFT);
        $min = str_pad($vtime[1], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min);
    }

    // 取得目前系統時間（yyyymmddhhmmss）
    function getdatetime() {
        $dt = getdate();
        $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
        $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
        $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
        $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
        $min = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);
        $sec = str_pad($dt['seconds'], 2, "0", STR_PAD_LEFT);
        return ($yyyy.$mm.$dd.$hh.$min.$sec);
    }

    // 將 yyyymmddhhmmss 格式轉回為可讀取的時間格式 yyyy/mm/dd hh:mm:ss
    function trandatetime1($dt) {
        $yyyy = substr($dt, 0, 4);
        $mm = substr($dt, 4, 2);
        $dd = substr($dt, 6, 2);
        $hh = substr($dt, 8, 2);
        $min = substr($dt, 10, 2);
        $sec = substr($dt, 12, 2);
        return ($yyyy . "/" . $mm . "/" . $dd . " " . $hh . ":" . $min . ":" . $sec);
    }
?>
