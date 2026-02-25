<?php
//systemtitle()	輸出統一網站標題
//getqtyinfo()	設定數量輸入框的限制屬性
//makekey()	產生 MD5 加密字串
//cryptkey()	使用 crypt() 加密密碼
//comparekey()	密碼比對
//twdate2date()	民國轉西元日期格式
//getdataorder()	回傳數字時間碼（常作為唯一識別碼）
//rfidstatus()	解析 RFID 狀態數值對應的說明文字
//trandatetime()	轉換壓縮時間格式為標準輸出格式


//取得統一抬頭
// 設定網站的標題顯示
function systemtitle()
{
    $tt = "曹永忠老師的學習網站(New教學網站)" ; // 設定網站標題內容
    echo $tt ; // 將標題輸出到網頁
}



// 函數返回針對 HTML input 元素的屬性，用於設置最大和最小值
// 傳回 HTML input 元素的屬性字串，限制數值的最小與最大值
function getqtyinfo($dd) 
{
    $rr = "value='%d' min='%d' max='%d'"; // 定義格式字串
    if ($dd < 1) {
        return sprintf($rr, 0, 0, 0); // 若數量小於 1，預設為 0
    } else {
        return sprintf($rr, 1, 1, $dd); // 限定輸入值在 1 ~ $dd 之間
    }
}


// 函數返回日期的年份
function GetYear($dd) 
{
    $tmp = DateTime::createFromFormat('Y-m-d', $dd); // 建立日期物件
    return $tmp->format('Y'); // 回傳年份
}


// 函數返回日期的月份
function GetMonth($dd) 
{
    // 根據 'Y-m-d' 格式創建 DateTime 對象
    $tmp = DateTime::createFromFormat('Y-m-d', $dd);
    // 返回月份
    return $tmp->format('m');
}

// 函數返回日期的日
function GetDay($dd) 
{
    // 根據 'Y-m-d' 格式創建 DateTime 對象
    $tmp = DateTime::createFromFormat('Y-m-d', $dd);
    // 返回日
    return $tmp->format('d');
}

// 函數生成一個字串的 MD5 哈希
function makekey($cc) 
{
    return md5($cc); // 將輸入字串以 MD5 加密
}

// 函數加密一個字串
function cryptkey($cc) 
{
    return crypt($cc, rand(0, 99)); // 使用 PHP crypt 函數加密（簡易版）
}

// 函數比較兩個字串，如果相等返回 1
function comparekey($c1, $c2) 
{
    return $c1 == $c2 ? 1 : 0; // 比對兩字串是否相等
}

// 函數將台灣日期（民國紀元）轉換為公元日期，民國日期轉西元日期
function twdate2date($ddt) {
    $yyyy = (int)substr($ddt, 0, 3) + 1911; // 將民國年轉西元年
    $mm = (int)substr($ddt, 4, 2);
    $dd = (int)substr($ddt, 7, 2);
    return "$yyyy/$mm/$dd"; // 回傳格式 YYYY/MM/DD
}

function getshiftdataorder($d) 
        {
                $date = new DateTime();
                       $dt = getdate($date->getTimestamp() + ($d * 60*60)) ;
                       $yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
                       $mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
                       $dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);
                       $hh  =  str_pad($dt['hours'] ,2,"0",STR_PAD_LEFT);
                       $min  =  str_pad($dt['minutes'] ,2,"0",STR_PAD_LEFT);
                       $sec  =  str_pad($dt['seconds'] ,2,"0",STR_PAD_LEFT);

               return ($yyyy.$mm.$dd.$hh.$min.$sec)  ;
}	
function getshiftdayorder($d) 
        {
                $date = new DateTime();
                       $dt = getdate($date->getTimestamp() + ($d *24 * 60*60)) ;
                       $yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
                       $mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
                       $dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);

               return ($yyyy.$mm.$dd)  ;
}		 

// 函數返回當前日期和時間的格式化字串，生成日期時間代碼（格式為數字編碼）
function getdataorder() {
    $dt = getdate();
    $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
    $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
    $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
    $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
    $min = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);
    $sec = str_pad($dt['seconds'], 2, "0", STR_PAD_LEFT);
    return "$yyyy$mm$dd$hh$min$sec"; // 輸出格式：20250513150333
}
function getdayorder() 
        {
  $dt = getdate() ;
                       $yyyy =  str_pad($dt['year'],4,"0",STR_PAD_LEFT);
                       $mm  =  str_pad($dt['mon'] ,2,"0",STR_PAD_LEFT);
                       $dd  =  str_pad($dt['mday'] ,2,"0",STR_PAD_LEFT);

               return ($yyyy.$mm.$dd)  ;
}


// 其他類似的函數同樣返回不同形式的日期時間
function getdataorder2() {
    $dt = getdate();
    $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
    $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
    $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
    $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
    $min = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);

    return "$yyyy$mm$dd$hh$min";
}

function getdataorderten() {
    $dt = getdate();
    $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
    $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
    $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
    $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
    $min = str_pad((int)($dt['minutes'] / 10), 1, "0", STR_PAD_LEFT);

    return "$yyyy$mm$dd$hh$min";
}

// 獲取當前日期和時間，以 YYYY/MM/DD HH:MM:SS 的形式返回
function getdatetime() {
    $dt = getdate();
    $yyyy = str_pad($dt['year'], 4, "0", STR_PAD_LEFT);
    $mm = str_pad($dt['mon'], 2, "0", STR_PAD_LEFT);
    $dd = str_pad($dt['mday'], 2, "0", STR_PAD_LEFT);
    $hh = str_pad($dt['hours'], 2, "0", STR_PAD_LEFT);
    $min = str_pad($dt['minutes'], 2, "0", STR_PAD_LEFT);
    $sec = str_pad($dt['seconds'], 2, "0", STR_PAD_LEFT);

    return "$yyyy/$mm/$dd $hh:$min:$sec";
}

// 函數轉換日期格式，以標準格式轉換為壓縮形式
function trandatetime0($dt) {
    $yyyy = substr($dt, 0, 4);
    $mm = substr($dt, 5, 2);
    $dd = substr($dt, 8, 2);
    $hh = substr($dt, 11, 2);
    $min = substr($dt, 14, 2);
    $sec = substr($dt, 17, 2);

    return "$yyyy$mm$dd$hh$min$sec";
}

// 函數返回日期轉換後的格式，帶有 "/" 分隔
function trandatetime($dt) {
    $yyyy = substr($dt, 0, 4);
    $mm = substr($dt, 4, 2);
    $dd = substr($dt, 6, 2);
    $hh = substr($dt, 8, 2);
    $min = substr($dt, 10, 2);

    return "$yyyy/$mm/$dd $hh:$min";
}

// 函數返回日期轉換後的格式，只包含月份和日期
function trandatetime2($dt) {
    $yyyy = substr($dt, 0, 4);
    $mm = substr($dt, 4, 2);
    $dd = substr($dt, 6, 2);
    $hh = substr($dt, 8, 2);
    $min = substr($dt, 10, 2);

    return "$mm/$dd $hh:$min";
}

// 返回類似 trandatetime2，但帶有 HTML 換行符
function trandatetime3($dt) {
    $yyyy = substr($dt, 0, 4);
    $mm = substr($dt, 4, 2);
    $dd = substr($dt, 6, 2);
    $hh = substr($dt, 8, 2);
    $min = substr($dt, 10, 2);

    return "$mm/$dd <br>$hh:$min";
}

// 返回帶空格的日期時間格式
function trandatetime4($dt) {
    $yyyy = substr($dt, 0, 4);
    $mm = substr($dt, 4, 2);
    $dd = substr($dt, 6, 2);
    $hh = substr($dt, 8, 2);
    $min = substr($dt, 10, 2);

    return "$mm/$dd  $hh:$min";
}

// 將 Unix 時間戳轉換為標準日期和時間
function uniztime2datetime($udt) {
    return date("Y-m-d H:i:s", $udt);
}

// 在數字前面填充零
function strzero($no, $len) {
    $tmp = (string)$no;
    $l = strlen($tmp);
    if ($len > $l) {
        return str_repeat("0", $len - $l) . $tmp;
    } else {
        return $tmp;
    }
}

// 獲取當前日期
function getToday() {
    $today = getdate();
    $year = $today["year"];
    $month = str_pad($today["mon"], 2, "0", STR_PAD_LEFT);
    $day = str_pad($today["mday"], 2, "0", STR_PAD_LEFT);
    
    return "$year-$month-$day"; // 返回日期格式：YYYY-MM-DD
}

// 獲取當前日期（格式不同）
function getThisToday() {
    $today = getdate();
    $year = $today["year"];
    $month = str_pad($today["mon"], 2, "0", STR_PAD_LEFT);
    $day = str_pad($today["mday"], 2, "0", STR_PAD_LEFT);
    
    return "$year/$month/$day"; // 返回日期格式：YYYY/MM/DD
}

// 獲取當前年份
function getThisYear() {
    $today = getdate();
    return $today["year"];
}

function rfidstatus($parameterno) //RFID 狀態說明
{
    $statusMap = [
        0 => '未啟用',
        1 => '啟用中',
        2 => '失效',
        3 => '遺失',
        4 => '禁用',
        9 => '註銷'
    ];
    return isset($statusMap[$parameterno]) ? $statusMap[$parameterno] : '有問題';
}

// JavaScript 函數用於瀏覽器返回或前進
?>
<script>
function goBack() {
  window.history.back(); // 返回上一頁
}
function goForward() {
  window.history.forward(); // 前進下一頁
}
</script>