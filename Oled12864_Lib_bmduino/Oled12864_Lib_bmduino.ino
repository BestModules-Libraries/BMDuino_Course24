/*****************************************************************
  檔案名稱：display.ino
  功能說明：
  1. 使用 I2C 通訊介面與 BMD31M090 OLED 顯示模組通訊（頻率：400KHz）
  2. 使用硬體序列埠 Serial（115200 bps）與電腦序列監控器通訊
  硬體接腳說明：
    SCL 接腳：D19
    SDA 接腳：D18
******************************************************************/
#include "OledLib.h"  // OLED 顯示模組自訂函式庫

//-------自定義函式宣告區-----------
void initSensor();   // 初始化所有感測模組
void initAll() ;  // 初始化整體系統
//-------自定義函式宣告區-----------


void setup()
{
    initAll() ;
    printText(0,0,"Best Module")  ;  // 顯示 LOGO 點陣圖
    printText(0,2,"BMduino UNO")  ;  // 顯示 LOGO 點陣圖
    printText(0,4,"ESP12F")  ;  // 顯示 LOGO 點陣圖
    delay(1000);
    scrollRight(0,7,0x07,0x01) ;
    delay(2000);
    scrollLeft(0,7,0x07,0x01) ;
    delay(2000); 
    stopScroll() ;   
    //updateScreen() ;  //顯示當前緩衝區的內容
  // 以下為測試各種顯示功能的函式呼叫
  // test_drawString_6x8();             // 顯示 6x8 字型字串
  // test_drawString_8x16();            // 顯示 8x16 字型字串
  // test_drawString_drawChar_drawNum();// 同時顯示字串、單字元與數字
  // test_drawPixel();                  // 像素繪製測試（白→黑→反轉）
  // test_drawFastHLine_drawFastVLine();// 繪製水平/垂直線條
  // test_drawBitmap();                 // 顯示 LOGO 點陣圖
  // test_variousScroll();              // 滾動顯示效果
  // test_invertDisplay();              // 顯示反白與恢復
  // test_dim();                        // 顯示亮度切換（省電模式）
  //clearScreen();
}

void loop()
{
  // 主程式不重複執行，測試功能已於 setup() 完成
}

//----------初始化所有感測模組------------
void initSensor()   // 初始化所有感測模組
{
  initOled(); //初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
}

//---------初始化整體系統------
void initAll()   // 初始化整體系統
{
  Serial.begin(9600); // 啟動序列埠，設定速率為 9600 bps（監控輸出與除錯使用）
  initSensor();       // 初始化所有感測器，目前僅包含按鍵矩陣模組
}



