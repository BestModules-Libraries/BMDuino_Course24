<?php
//session_start();
function Connection() 
{

        $server="127.0.0.1";		///因為apache server and db server(mysql) 安裝在同一台機器，所以用本機端
        //如果apache server and db server(mysql) 安裝在不同一台機器，請改為DB SERVER那台機器的IP ADDRESS
        $db="big";
        //系統使用的資料庫名稱，與 db="big"; 變數值相同，可能有重複定義的嫌疑。
        //註解：System using this database name。

        $user="big";			// DB 伺服器的使用者帳號，與 MySQL 的使用者認證相關。註解：DB SERVER user account。
 
        $pass="12345678";		// DB 伺服器的使用者密碼，編碼在程式碼中 $db="big";

        $dbport = 3306 ;		// DB 伺服器使用的通訊埠，MySQL 預設為 3306。註解：DB SERVER installed and used port

        //echo "cnn is ok 01"."<br>" ;	   	
        $connection = new mysqli($server,$user,$pass,$db , $dbport);
//    使用 mysqli 類別的建構子建立連線，參數包括伺服器、主機、密碼、資料庫和通訊埠。
//    這是物件導向方式的連線，較程序式方式（如 mysqli_connect）更現代化且功能更強。
//    註解：使用提供的伺服器、使用者、密碼、資料庫和通訊埠建立新的 MySQL 連線（物件導向方式）。
   	
        //透過上述資訊，建立mysql的資料連線，並把連線回傳到$connection 變數

        if ($connection -> connect_errno) 	//檢查 connect_errno 是否有值，若有則表示連線失敗。
            {
              echo "Failed to connect to MySQL: " . $mysqli -> connect_error;
            //  輸出連線失敗的錯誤訊息。
            //注意：原始碼中寫為 $mysqli -> connect_error，這是一個拼寫錯誤，
            //應為 $connection -> connect_error，
            //因為連線物件名稱為 $connection。
            //修正後的註解：輸出連線失敗的錯誤訊息，
            // 注意：原始碼中 $mysqli -> connect_error 應為 $connection -> connect_error（有拼寫錯誤）。
              exit();		//終止系統
            //連線失敗時直接終止程式執行。
            //註解：Terminate the system。
            //潛在問題：使用 exit() 會立即終止腳本，
            //可能不適合穩健應用，
            //建議在生產環境中考慮返回 false 或拋出例外（如 throw new Exception("Database connection failed");）
            //以便上層程式處理。
              
            }

        //echo "cnn is ok 03"."<br>" ;	   	
        mysqli_select_db($connection,$db); 	//切換資料庫到系統變數$db所指的資料庫
        //切換到系統變數 $db 指定的資料庫。
        //註解：Switch database to the one specified by the system variable $db。
        //潛在問題：因為在 mysqli 建構子中已指定資料庫（$db），
        //這一步可能多餘。根據 PHP 文件，mysqli 建構子若提供資料庫名稱，
        // 會自動連接到該資料庫，因此 select_db 可能是不必要的重複操作。
        //建議：考慮移除這行，除非有特殊需求。
        $connection -> query("SET NAMES UTF8");		//設定連線資料庫使用語系(unicode UTF8)
        //設定連線資料庫使用語系(unicode UTF8)
        //執行 SQL 命令設定連線的字符集為 UTF-8，以確保正確處理特殊字符（如中文）。
        //註解：Set the database connection to use UTF8 charset
   	

        return $connection  ;			//回傳連線
        //函數結束，返回建立的連線物件 $connection，供其他程式碼使用。
        //註解：Return the connection。

}

?>