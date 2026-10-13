
dataadd.php
加入卡號  

    //http://iot.arduino.org.tw:8888/bigdata/rfid/dataadd.php?KEY=1234567890&ACT=1
    //http://localhost:8888/bigdata/rfid/dataadd.php?KEY=1234567890&ACT=1
    /*
        這支 PHP 程式檔主要用來處理來自網址參數（GET）所傳來的 RFID 卡片資訊，
       並根據卡號 (KEY) 與啟用狀態 (ACT) 查詢或更新資料庫中的 RFID 卡片記錄，
       最後將使用者導回卡片清單頁面 cardlistmgt.php。
        使用者透過 GET 請求送來參數 KEY（RFID卡號）與 ACT（啟用狀態）。
        程式驗證參數是否正確、存在。
        查詢資料庫是否已存在此卡號與對應啟用狀態的資料。
        若已存在 → 直接導回卡片列表頁面。
        若不存在 → 根據 ACT 值進行卡號啟用或停用狀態更新，並記錄使用者 IP 與系統時間。
      */

-----------------------
act.php
    // 範例網址（用於測試啟用 RFID 卡片的功能）：
    // http://iot.arduino.org.tw:8888/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC
    // http://localhost:8888/bigdata/rfid/act.php?KEY=0219390674&MAC=B8D61A68DAFC
    如果卡號 存在rfidlist and activate=0，則讓該卡號的 activate=1
    如果卡號不存在rfidlist則讓該卡號的加入rfidlist and 設定期卡號 activate=1



-----------------------
cardlistmgt.php
   卡號管理程式(有新增/修改/刪除)功能的

-----------------------
cardlist.php
   卡號管理程式

-----------------------
checkpass.php
    RFID Reader 讀RFID Card，判斷是否可以進入，
    不管可進入或不可進入，lockpass資料表都會存入一筆資料

-----------------------
devicelist.php
    RFID list表格顯示功能
    以activate 分類

-----------------------
reglockdevice.php
註冊 卡號到lockdevice資料表，
這個lockdevice資料表 主要用來建立locklist資料表時，
有哪些門禁裝置可以選擇
建立rfidlist->locklist(locklist->mid foreign to rfidlist->id)


-----------------------
removecard.php
註冊 卡號到rfidlist資料表，
代表此卡號 的activate = 9， 
代表此卡號 已經作廢

-----------------------
rfidrecordadd.php
AI產生 rfidlist資料表一筆資料新增功能

-----------------------
rfidpass.php
http://localhost:8888/bigdata/rfid/rfidpass.php?KEY=3367216813
檢查卡號是否存在rfidlist的 rfidkey的資料內
{"Device":"3367216813","Result":"Find"}
{"Device":"336721622","Result":"notFind"}
   找到==>"Result":"Find"
   找不到==>"Result":"notFind"




























































































