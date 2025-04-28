/*
程式說明：
setup() 函數：

初始化序列通訊，波特率設定為 9600，方便與外部設備進行資料傳輸。
初始化 BMK54 感測器模組，並且設置其所有的感測門檻值為 2，這可能影響感測器的靈敏度或觸發條件。
loop() 函數：

不斷檢查 BMK54 的中斷引腳，當其值為 0 時，表示有感測事件發生。
讀取 BMK54 的滑動值並將其顯示在序列監控台上（可能是一個電容觸控條的數據）。
讀取 BMK54 的按鍵值並將其顯示在序列監控台上（可能是按鍵的輸入值）。
這段程式用於與 BMK54 感測器模組通訊，監測滑動條或按鍵的數據，並將結果即時顯示在序列監控台上。
*/
#include "BMK54T004.h"
BMK54T004     BMK54(2,&Wire);//intPin,Please comment out this line of code if you don't use Wire
//BMK54T004     BMK54(22,&Wire1);//Please uncomment out this line of code if you use Wire1 on BMduino
//BMK54T004     BMK54(25,&Wire2);//Please uncomment out this line of code if you use Wire2 on BMduino

void setup() 
{
  // 初始化序列通訊，設定波特率為 9600，方便與電腦或其他設備通訊
  Serial.begin(9600);
  
  // 初始化 BMK54 感測器模組
  BMK54.begin();
  
  // 設置 BMK54 的所有門檻值為 2，假設這個設定控制感測靈敏度
  BMK54.setAllThresholdLevel(2);
}

void loop() 
{
    // 如果 BMK54 的中斷引腳（表示有事件觸發）為 0
    if(BMK54.getINT() == 0)
    {          
        // 讀取 BMK54 的滑動值，並輸出到序列監控台
        Serial.print("SlideValue:");
        Serial.println(BMK54.readSlideValue());             

        // 讀取 BMK54 的按鍵值，並輸出到序列監控台
        Serial.print("KeyValue:");
        Serial.println(BMK54.readKeyValue());      
    }  
}

