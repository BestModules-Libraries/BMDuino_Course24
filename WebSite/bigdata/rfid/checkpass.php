<?php
    // 範例網址（用於測試啟用 RFID 卡片的功能）：
    // http://localhost:8888/bigdata/rfid/checkpass.php?KEY=0243348066&MAC=B8D61A68DAFC
    // http://iot.arduino.org.tw:8888/bigdata/rfid/checkpass.php?KEY=0243348066&MAC=B8D61A68DAFC

    include("../Connections/iotcnn.php"); // 引入資料庫連線設定與函式
    // 此檔案內通常包含 Connection() 函式，用來建立與 MySQL 資料庫的連線
    $link = Connection(); // 與資料庫建立連線
    
    // 建立一個 class 類別，用來儲存傳回結果
    class maindata
    {
        public $Device ; // 儲存裝置的 MAC 位址
        public $Card ;   // 儲存感應到的 RFID 卡片號碼
        public $Result ; // 儲存檢查結果："Find" 代表允許進入，"notFind" 代表無效卡
    }
    $maindata = new maindata(); // 建立 maindata 類別的實體物件

    // 取得使用者 IP 位址，方便日後追蹤與記錄
    $ip = getenv("REMOTE_ADDR");

    // 取得系統目前時間（格式：yyyymmddhhmmss），用於紀錄與查詢
    $sysdt = getdatetime();



    // 檢查是否有傳入 GET 參數 "MAC"，此為裝置 MAC 位址
    if (!isset($_GET["MAC"])) {
        echo "Device MAC lost <br>"; // 若沒有傳入則顯示錯誤並結束執行
        die();
    }

    // 檢查是否有傳入 GET 參數 "KEY"，此為 RFID 卡號
    if (!isset($_GET["KEY"])) {
        echo "RFID KEY lost <br>"; // 若沒有傳入則顯示錯誤並結束執行
        die();
    }    
    
    // 從 GET 參數中取得 RFID 卡號與裝置 MAC
    $temp0 = $_GET["KEY"]; // RFID 卡號
    $temp1 = $_GET["MAC"]; // 裝置 MAC

// 檢查 RFID 卡號是否為空字串
if (trim($temp0) == "") {
    echo "RFID KEY is empty string <br>";
    die();
}

// 檢查 MAC 是否為空字串
if (trim($temp1) == "") {
    echo "Device MAC is empty string <br>";
    die();
}

    // SQL 語句樣板：查詢啟用中（activate = 1）且符合 RFID 卡號的資料
    $cardqry = "select * from rfidlist where rfidkey = '%s' and activate = 1";

    // SQL 語句樣板：將檢查記錄寫入 lockpass 紀錄表中
    $passqry = "insert into lockpass (systime, IP, MAC, cardnumber, status) values ('%s', '%s', '%s', '%s', %d);";

    
    
    // 依 RFID 卡號組合查詢語句，用來查找此卡是否存在於啟用名單中
    $qrystr = sprintf($cardqry, $temp0);

    // 執行查詢語句
    $result = mysqli_query($link, $qrystr);

    // 取得符合條件的筆數（即已啟用的卡片）
    $num = mysqli_num_rows($result);

    if ($num >= 1) 
    {
        // 若找到符合條件的卡片，表示允許進入，並記錄此筆成功通行資料
        $qrystr = sprintf($passqry, $sysdt, $ip, $temp1, $temp0, 1); // status=1 表示通行成功
        $result1 = mysqli_query($link, $qrystr); // 執行 insert 紀錄
        $passok = "Find"; // 記錄結果為 Find（允許進入）
    }
    else 
    {
        // 若查無卡片資料，記錄一筆通行失敗資料
        $qrystr = sprintf($passqry, $sysdt, $ip, $temp1, $temp0, 0); // status=0 表示通行失敗
        $result1 = mysqli_query($link, $qrystr); // 執行 insert 紀錄
        $passok = "notFind"; // 記錄結果為 notFind（不允許進入）
    }

    // 將裝置 MAC 與卡號寫入結果物件中，並設定通行結果
    $maindata->Device = $temp1;
    $maindata->Card = $temp0;
    $maindata->Result = $passok;

    // 若查詢結果為資料集物件，釋放其記憶體資源
    if ($result && $result instanceof mysqli_result) {
        mysqli_free_result($result);
    }

    // 若 insert 結果為資料集物件，也釋放其記憶體資源（實際上通常 insert 不會有資料集）
    if ($result1 && $result1 instanceof mysqli_result) {
        mysqli_free_result($result1);
    }

    // 關閉資料庫連線
    mysqli_close($link);

    // 將結果物件轉為 JSON 格式回傳，並保持中文不被轉義
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
