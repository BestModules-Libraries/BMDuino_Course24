/*****************************************************************
BMK52T016 初始化：

使用 I2C Wire 介面，並將中斷引腳設置為 2 號腳位來監聽按鍵事件。
註解中的其他兩行可以根據實際情況來修改，用於不同的 I2C 介面，如 Wire1 或 Wire2，這些一般在特定的硬體平台（如 BMduino 開發板）上使用。
setup() 函數：

初始化 BMK52 按鍵模組，並開始序列通訊，波特率設置為 9600。
loop() 函數：

不斷檢查 BMK52 的中斷引腳，如果值為 0，表示按鍵被按下。
讀取按鍵的值並將其以二進制格式輸出到序列監控台，方便觀察按鍵狀態。
這段程式用於監測 BMK52 按鍵模組的按鍵狀態，並實時顯示按鍵的值到序列監控台。
******************************************************************/
#include "BMK52T016.h"  // 引入 BMK52T016 庫，該庫用於與 BMK52 按鍵模組通訊
BMK52T016 BMK52(2, &Wire); // 創建 BMK52T016 物件，並設置中斷引腳為 2，使用 I2C Wire 介面通訊
//BMK52T016 BMK52(22, &Wire1); // 如果使用 Wire1 介面，取消此行註解並使用 22 號引腳（用於 BMduino 開發板）
//BMK52T016 BMK52(25, &Wire2); // 如果使用 Wire2 介面，取消此行註解並使用 25 號引腳（用於 BMduino 開發板）

void setup() 
{
    // 初始化 BMK52 按鍵模組
    BMK52.begin();
    
    // 初始化序列通訊，設置波特率為 9600，供與電腦或其他裝置通訊
    Serial.begin(9600);
}

void loop() 
{  
    // 如果 BMK52 的中斷引腳為 0（表示有按鍵事件發生）
    if(BMK52.getINT() == 0)
    {
         // 輸出 "keyValue:" 作為提示文字
         Serial.print("keyValue:");
         
         // 讀取 BMK52 按鍵的值，並以二進制格式輸出到序列監控台
         Serial.println(BMK52.readKeyValue(), BIN);
    }  
}
