<?php     
  // 範例呼叫方式 (用網址帶參數)：
  // http://localhost:8888/bigdata/dhtdata/dht2json.php?MAC=246F28248CE0&start=20200406&end=20200407
  // http://iot.arduino.org.tw/bigdata/dhtdata/dht2json.php?MAC=246F28248CE0&start=20200406&end=20200407

    //include("../comlib.php");		// (已註解) 可用來引入共用程式庫
   	include("../Connections/iotcnn.php");		// 引入資料庫連線程式
  	$link=Connection();		// 建立 MySQL 資料庫連線物件
	/*
	此程式輸出的 JSON 範例格式如下：
	{
		"Device":"3C71BFFD882C",
		"Temperature":[12,33,45,34,34],
		"Humidity":[12,33,45,34,34]
	}
	*/

    // 定義一個 class，存放輸出的主要資料結構
    class maindata{
        public $Device ;        // MAC 裝置編號 (例如感測器的唯一識別碼)
        public $Temperature ;   // 溫度值陣列
        public $Humidity ;      // 濕度值陣列
     }

	// 從 URL (GET) 參數取得查詢條件
	$sid=$_GET["MAC"];	  // 取得 MAC 地址 (裝置編號)
	$s1=$_GET["start"];	  // 取得開始時間 (格式: YYYYMMDD)
	$s2=$_GET["end"];	  // 取得結束時間 (格式: YYYYMMDD)
    // 說明：MAC = 裝置編號，start = 開始時間，end = 結束時間

	// SQL 查詢語法範例：
	// select * FROM big.dhtdata where mac = '246F28248CE0' and systime > "20200406" and systime < "20200407" order by systime asc 
	
	$maindata = new maindata() ;   // 建立 class maindata 的物件實體，用來存放回傳資料


	// SQL 查詢字串 (帶入三個參數: MAC, start, end)
	$qry1 = "select * from big.dhtdata where mac = '%s' and systime > '%s' and systime < '%s' order by systime asc " ;		
	$qrystr = sprintf($qry1 , $sid, $s1, $s2) ;	// 使用 sprintf 將參數填入 SQL 查詢字串

	//echo $qrystr."<br>" ;  // 偵錯用，可輸出完整 SQL 語法以確認

	// 執行 SQL 查詢
	$result= mysqli_query($link ,$qrystr);		

	$cnt= 1 ;	                               // (未實際使用) 計數器
	$count = mysqli_num_rows($result) ;        // 取得查詢到的筆數
	//echo $count."<br>";   // 偵錯用，可輸出查詢筆數

	
	// 如果有查詢到資料
	if ($count >0)
	{
        $d2 = array() ;     // 用來存放溫度值的陣列
        $d3 = array() ;     // 用來存放濕度值的陣列

	    // 逐筆讀取查詢結果
	    while($row = mysqli_fetch_array($result))   
		{
            // 取得溫度值，轉換為浮點數並四捨五入到小數點 1 位
            array_push($d2, round((float)$row["temperature"], 1));

            // 取得濕度值，轉換為浮點數並四捨五入到小數點 1 位
            array_push($d3, round((float)$row["humidity"], 1));

            // 偵錯用：可輸出溫度/濕度
            // echo round((float)$row["temperature"],1)."/".round((float)$row["humidity"],1)."<br>" ;
        }

        // 將查詢結果填入物件
        $maindata->Device = $sid ;    // 設定裝置編號 (MAC)
        $maindata->Temperature = $d2;	// 設定溫度值陣列
        $maindata->Humidity = $d3;	// 設定濕度值陣列

        // 輸出 JSON 格式的資料
        echo json_encode($maindata, JSON_UNESCAPED_UNICODE);
        // json_encode() 將 PHP 物件轉為 JSON 字串並輸出
	}
	// 釋放查詢結果資源
	mysqli_free_result($result);	 
 
	// 關閉資料庫連線
	mysqli_close($link);		
        /*
         {
            "Device": "246F28248CE0",
            "Temperature": [25.3, 25.5, 25.7],
            "Humidity": [60.1, 60.3, 60.4]
          }
         */
?> 
