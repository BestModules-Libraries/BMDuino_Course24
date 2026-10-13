<?php
/*
 這段程式碼會顯示一個網頁，供使用者輸入並新增一筆「授權的 RFID 卡號」資料。
 - 頁面上提供表單，包含 RFID 卡號、啟用狀態、啟用與結束日期時間欄位。
 - 表單資料送出後會傳送至 rfidcardadd.php 做資料庫儲存。
*/

// 啟動 Session（雖然此頁面未使用，但良好習慣）
session_start();

// 載入通用函式庫（如時間格式處理）
include("../comlib.php");

// 取得系統當下的時間，格式可能為 yyyymmddhhmmss，用於預設「啟用時間」
$sysdt = getdataorder();

// 預設結束時間設定為遠未來，表示「永久有效」
$sysdt2 = '99991231235959';

// 取得目前用戶端的 IP 位址，可用於記錄來源
$ip = getenv("REMOTE_ADDR");
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <!-- 設定編碼方式為 UTF-8，支援多國語系 -->
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>RFID Card List (RFID卡號清單)</title>

    <!-- 引用外部 CSS 樣式表，定義表單外觀與格式 -->
    <link href="../webcss.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <!-- 載入網頁上方標題區塊（通常包含頁首或導覽列） -->
    <?php include("../toptitle.php"); ?>

    <hr>

    <!-- 表單區塊：新增 RFID 卡資訊 -->
    <div class="form-container">
        <h2>新增授權之RFID 卡號(Add Authorized RFID Card)</h2>

        <!-- 表單開始，使用 POST 方法，送出至 rfidcardadd.php 進行儲存 -->
        <form action="rfidcardadd.php" method="POST">
            
            <!-- 隱藏欄位：傳送用戶端 IP 位址 -->
            <input name="ip" type="hidden" id="ip" value="<?php echo $ip; ?>" /> 

            <!-- RFID 卡號欄位 -->
            <div class="form-group">
                <label for="rfidkey">RFID 卡號(RFID Key):</label>
                <!-- 輸入欄位限制為最多 12 位的數字 -->
                <input type="text" id="rfidkey" name="rfidkey" required maxlength="12" pattern="[0-9]{1,12}" title="請輸入1到12位數字">
            </div>

            <!-- 啟用狀態欄位 -->
            <div class="form-group">
                <label for="activate">狀態(0=未啟用 or 1=啟用):</label>
                <!-- 限制為 0 或 1 的數值輸入 -->
                <input type="number" id="activate" name="activate" required min="0" max="1" value="1">
            </div>

            <!-- 啟用時間欄位 -->
            <div class="form-group">
                <label for="startdatetime">啟用日期時間(Start DateTime)(YYYYMMDDHHMMSS):</label>
                <!-- 使用 PHP 預設值帶入當前系統時間，要求輸入 14 位數字 -->
                <input type="text" id="startdatetime" name="startdatetime" value="<?php echo $sysdt;?>" pattern="\d{14}" placeholder="e.g., 20250508123045">
            </div>

            <!-- 結束時間欄位 -->
            <div class="form-group">
                <label for="enddatetime">結束日期時間(End DateTime) (YYYYMMDDHHMMSS):</label>
                <!-- 預設值為 99991231235959，代表幾乎永久 -->
                <input type="text" id="enddatetime" name="enddatetime" pattern="\d{14}" value="<?php echo $sysdt2;?>"  placeholder="e.g., 20250508123045">
            </div>

            <!-- 表單控制按鈕 -->
            <input type="reset" value="清除資料(Clear)"> <!-- 重設表單 -->
            <input type="submit" value="新增資料(Add)s"> <!-- 送出表單 -->
        </form>
    </div>

    <hr>

    <!-- 載入網頁下方頁尾（例如版權宣告） -->
    <?php
    include("../topfooter.php"); 

    ?>
</body>
</html>
