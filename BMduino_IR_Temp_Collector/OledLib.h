// OLED 顯示模組自訂函式庫
/*****************************************************************
  檔案名稱：display.ino
  功能說明：
  1. 使用 I2C 通訊介面與 BMD31M090 OLED 顯示模組通訊（頻率：400KHz）
  2. 使用硬體序列埠 Serial（115200 bps）與電腦序列監控器通訊
  硬體接腳說明：
    SCL 接腳：D19
    SDA 接腳：D18
******************************************************************/
/*****************************************************************
File:         display.ino
Description:  1.Wire interface (Clock Frequency: 400K) is used to communicate with BMD31M090.
              2.Hardware Serial (BAUDRATE 115200) is used to communicate with Serial Port Monitor.
connection method: sclPin:SCL(D19) sdaPin:SDA(D18)
******************************************************************/
//------感測器外部函式庫----------------
#include "BMD31M090.h"  // 引入 BMD31M090 OLED 顯示模組的函式庫
//#include  "BestModuleLogo.h"
#include "Bitmap.h"     // 引入位圖相關的函式庫

//------全域變數區--------------------------
// 定義 BMD31M090 顯示模組的寬度和高度（單位：像素）
#define BMD31M090_WIDTH   128        
#define BMD31M090_HEIGHT  64         

// 設定 BMD31M090 顯示模組的 I2C 地址，根據電路圖來設定 I2C 地址
#define BMD31M090_ADDRESS 0x3C       
// #define BMD31M090_ADDRESS 0x3D   // 可以配置的 I2C 地址：Addr0:0x3C 或 Addr1:0x3D

uint8_t t = ' ';  // 宣告變數 t，初始化為空格字元

//-----------------感測元件物件區-------------------------
// 創建 BMD31M090 顯示模組的物件，並使用 HW Wire 進行通訊
//BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire);
BMD31M090     BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire1); //Please uncomment out this line of code if you use HW Wire1 on BMduino
//BMD31M090     BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire2); //Please uncomment out this line of code if you use HW Wire1 on BMduino
//----------自定義函式區宣告--------------
void initOled() ;//初始化OLED12864，0.96吋OLED顯示模組 BMD31M090
void test_drawString_6x8();             // 顯示 6x8 字型字串
void test_drawString_8x16();            // 顯示 8x16 字型字串
void test_drawString_drawChar_drawNum();// 同時顯示字串、單字元與數字
void test_drawPixel();                  // 像素繪製測試（白→黑→反轉）
void test_drawFastHLine_drawFastVLine();// 繪製水平/垂直線條
void test_drawBitmap();                 // 顯示 LOGO 點陣圖
void test_variousScroll();              // 滾動顯示效果
void test_invertDisplay();              // 顯示反白與恢復
void  test_dim();                        // 顯示亮度切換（省電模式）
void drawPicture(int x,int y, const uint8_t *pp,int width, int height) ;
void clearScreen()  ;  //清除螢幕
void updateScreen() ;  //顯示當前緩衝區的內容
void setFont(const unsigned char* font) ;  //設定字形
    //   font：字體格式
    // FontTable_6X8：字體格式為 6×8
    // FontTable_8X16：字體格式為 8×16
    // FontTable_16X32：字體格式為 16×32
    // FontTable_32X64：字體格式為 32×64
void printText(int x,int y, uint8_t *str) ;  //在xcolumn,y row位置，印出文字
void printChar(int x,int y, char str) ;  //在xcolumn,y row位置，印出文字
void printNumber(int x,int y, int num);   //在xcolumn,y row位置，印出整數
void printFloat(int x,int y, float num);  //在xcolumn,y row位置，印出浮點數
void drawPoint(int x,int y,int pixelColor); //在xcolumn,y row位置，繪出一點
    //   x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
void drawLine(int x1,int y1,int x2,int y2,int pixelColor); //畫(x1,y1) - (x2,y2) 的一條直線
    // x_Start：起點 x 座標
    // y_Start：起點 y 座標
    // x_End：終點 x 座標
    // y_End：終點 y 座標
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
void drawfastVline(int x,int y, int width, int pixelColor);  //在xcolumn,y row位置，繪出width寬度的畫垂直線
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
void drawVline(int x,int y, int width, int pixelColor);  //在xcolumn,y row位置，繪出width寬度的畫垂直線
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
void drawVline(int x,int y, int width, int pixelColor) ; //在xcolumn,y row位置，繪出width寬度的畫垂直線
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉

//-------------向右捲動-----------------------
void scrollRight(int x, int y, int speed,int dir); //向右捲動
  // x:startRow：起始列位址，範圍 0~7
  // y:endRow：結束列位址，範圍 0~7
  // speed:scrollSpeed：滾動速度
  //   0x07 (SCROLL_2FRAMES)：滾動 2 幀
  //   0x04 (SCROLL_3FRAMES)：滾動 3 幀
  //   0x05 (SCROLL_4FRAMES)：滾動 4 幀
  //   0x00 (SCROLL_5FRAMES)：滾動 5 幀
  //   0x06 (SCROLL_25FRAMES)：滾動 25 幀
  //   0x01 (SCROLL_64FRAMES)：滾動 64 幀
  //   0x02 (SCROLL_128FRAMES)：滾動 128 幀
  //   0x03 (SCROLL_256FRAMES)：滾動 256 幀
  // dir:scrollVDirection：對角滾動方向
  //   0x00 (SCROLLV_NONE)：不滾動
  //   0x01 (SCROLLV_TOP)：向上滾動
  //   0x3F (SCROLLV_BOTTOM)：向下滾動
  
  //-------------向左捲動------------------------------------
void scrollLeft(int x, int y, int speed,int dir); //向左捲動
  // x:startRow：起始列位址，範圍 0~7
  // y:endRow：結束列位址，範圍 0~7
  // speed:scrollSpeed：滾動速度
  //   0x07 (SCROLL_2FRAMES)：滾動 2 幀
  //   0x04 (SCROLL_3FRAMES)：滾動 3 幀
  //   0x05 (SCROLL_4FRAMES)：滾動 4 幀
  //   0x00 (SCROLL_5FRAMES)：滾動 5 幀
  //   0x06 (SCROLL_25FRAMES)：滾動 25 幀
  //   0x01 (SCROLL_64FRAMES)：滾動 64 幀
  //   0x02 (SCROLL_128FRAMES)：滾動 128 幀
  //   0x03 (SCROLL_256FRAMES)：滾動 256 幀
  // dir:scrollVDirection：對角滾動方向
  //   0x00 (SCROLLV_NONE)：不滾動
  //   0x01 (SCROLLV_TOP)：向上滾動
  //   0x3F (SCROLLV_BOTTOM)：向下滾動
  
  
  //-----------------停止滾動-----------------------
void stopScroll(); //停止滾動
//-----------------------
void showDeviceonOled(String ss,int row); //列印Device ID於OLED上
void showTitleonOled(String ss,int row); //列印抬頭於OLED上
void showIPonOled(String ss,int row); //列印IP Address於OLED上
//-------列印ＭＡＣ　Ａｄｄｒｅｓｓ於OLED上---------------
void showMACeonOled(String ss,int row); //列印MAC Address於OLED上
//-------列印ＭＡＣ　Ａｄｄｒｅｓｓ於OLED上---------------
void showSSIDeonOled(String ss,int row); //列印SSID於OLED上
void showMsgonOled(String ss,int row); //列印Message於OLED上
//--------------物件初始化區---------------------
void initOled()  //OLED12863物件初始化區
{
  // 初始化 BMD31M090 顯示模組，使用 I2C 地址進行通訊
  BMD31.begin(BMD31M090_ADDRESS);
  delay(100);  // 建議的初始化延遲
    Serial.println("init OLED12864 OK") ;

}

//-------------設定字形----------------
void setFont(const unsigned char* font)   //設定字形
{
    //   font：字體格式
    // FontTable_6X8：字體格式為 6×8
    // FontTable_8X16：字體格式為 8×16
    // FontTable_16X32：字體格式為 16×32
    // FontTable_32X64：字體格式為 32×64
    BMD31.setFont(font);  // 設定字型為 6x8
}

//----------顯示當前緩衝區的內容------------------
void updateScreen()  //顯示當前緩衝區的內容   
{
  BMD31.display();  //顯示當前緩衝區的內容
}

//--------在xcolumn,y row位置，印出文字--------
void printText(int x,int y, String str)  //在xcolumn,y row位置，印出文字
{
  BMD31.drawString((uint8_t)x, (uint8_t)y, (uint8_t*)str.c_str());
}

//-----------在xcolumn,y row位置，印出字元文字------------
void printChar(int x,int y, char str)  //在xcolumn,y row位置，印出字元文字
{
  BMD31.drawChar((uint8_t)x, (uint8_t)y, (uint8_t)str);
}

//--------在xcolumn,y row位置，印出整數-----------
void printNumber(int x,int y, int num)  //在xcolumn,y row位置，印出整數
{
  BMD31.drawNum((uint8_t)x, (uint8_t)y, (uint32_t)num,(uint8_t)(String(num).length()) );
}

//-----------在xcolumn,y row位置，印出浮點數-----------
void printFloat(int x,int y, float num)  //在xcolumn,y row位置，印出浮點數
{
  printText(x, y, String(num) );  //印出字串文字
}

//--------在xcolumn,y row位置，繪出一點-----------
void drawPoint(int x,int y,int pixelColor) //在xcolumn,y row位置，繪出一點
{
    //   x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawPixel((uint8_t)x, (uint8_t)y, (uint8_t)pixelColor) ;
    BMD31.display();
}

//---------畫(x1,y1) - (x2,y2) 的一條直線------------
void drawLine(int x1,int y1,int x2,int y2,int pixelColor) //畫(x1,y1) - (x2,y2) 的一條直線
{
    // x_Start：起點 x 座標
    // y_Start：起點 y 座標
    // x_End：終點 x 座標
    // y_End：終點 y 座標
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.drawLine((uint8_t)x1,(uint8_t)y1,(uint8_t)x2,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x1,y1) - (x2,y2) 的一條直線
  BMD31.display();
}

//---------在xcolumn,y row位置，繪出width寬度的畫垂直線-------------
void drawfastVline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫垂直線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastVLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫垂直線
  BMD31.display();
}

 //---------在xcolumn,y row位置，繪出width寬度的畫水平線---------
void drawfastHline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫水平線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastHLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫水平線
  BMD31.display();
}

//---------在xcolumn,y row位置，繪出width寬度的畫垂直線-------
void drawVline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫垂直線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastVLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫垂直線
  BMD31.display();
}

//--------在xcolumn,y row位置，繪出width寬度的畫水平線----------
void drawHline(int x,int y, int width, int pixelColor)  //在xcolumn,y row位置，繪出width寬度的畫水平線
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // height：高度，height(h)≤64
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
    BMD31.drawFastHLine((uint8_t)x,(uint8_t)y,(uint8_t)width,(uint8_t)pixelColor) ;//在xcolumn,y row位置，繪出width寬度的畫水平線
  BMD31.display();
}

//-------畫(x1,y1) - (x2,y2) 的對角的繪出一個矩形-------
void drawBox(int x1,int y1,int x2,int y2,int pixelColor) //畫(x1,y1) - (x2,y2) 的對角的繪出一個矩形
{
    // x_Start：起點 x 座標
    // y_Start：起點 y 座標
    // x_End：終點 x 座標
    // y_End：終點 y 座標
    //   pixelColor：像素顏色
    //     0 (pixelColor_BLACK)：像素顏色為黑
    //     1 (pixelColor_WHITE)：像素顏色為白
    //     2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.drawLine((uint8_t)x1,(uint8_t)y1,(uint8_t)x2,(uint8_t)y1,(uint8_t)pixelColor) ;//畫(x1,y1) - (x2,y1) 的一條直線
  BMD31.drawLine((uint8_t)x2,(uint8_t)y1,(uint8_t)x2,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x2,y1) - (x2,y2) 的一條直線
  BMD31.drawLine((uint8_t)x1,(uint8_t)y1,(uint8_t)x1,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x1,y1) - (x1,y2) 的一條直線
  BMD31.drawLine((uint8_t)x1,(uint8_t)y2,(uint8_t)x2,(uint8_t)y2,(uint8_t)pixelColor) ;//畫(x1,y2) - (x2,y2) 的一條直線
  BMD31.display();
}

//--------清除螢幕-----------
void clearScreen()    //清除螢幕
{
  BMD31.clearDisplay(); //清除螢幕資訊
  BMD31.display();  //更新螢幕資訊到螢幕硬體
}

//-------使用黑色畫一張圖---------
void invdrawPicture(int x,int y, const uint8_t *pp,int width, int height) //使用黑色畫一張圖
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // *Bitmap：點陣圖名稱
    // w：點陣圖寬度
    // h：點陣圖高度
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.clearDisplay();
  BMD31.drawBitmap((uint8_t)x, (uint8_t)y, pp, (uint8_t)width, (uint8_t)height, pixelColor_BLACK);//使用黑色畫一張圖
  BMD31.display();
}

//-------------使用白色畫一張圖-------------
void drawPicture(int x,int y, const uint8_t *pp,int width, int height)  //使用白色畫一張圖
{
    // x：x 座標，範圍 0~127
    // y：y 座標，範圍 0~63
    // *Bitmap：點陣圖名稱
    // w：點陣圖寬度
    // h：點陣圖高度
    // pixelColor：像素顏色
    // 0 (pixelColor_BLACK)：像素顏色為黑
    // 1 (pixelColor_WHITE)：像素顏色為白
    // 2 (pixelColor_INVERSE)：像素顏色翻轉
  BMD31.clearDisplay();
  BMD31.drawBitmap((uint8_t)x, (uint8_t)y, pp, (uint8_t)width, (uint8_t)height, pixelColor_WHITE);  //使用白色畫一張圖
  BMD31.display();
}

//----------降低亮度（省電模式）-------------
void setsaveMode()  // 降低亮度（省電模式）
{
  // dim：亮度選擇
  //   true：暗
  //   false：正常亮度
  BMD31.dim(TRUE);   // 降低亮度（省電模式）
}

//--------回復正常亮度-----------
void setlightMode() // 回復正常亮度
{
    // dim：亮度選擇
  //   true：暗
  //   false：正常亮度
    BMD31.dim(FALSE);  // 回復正常亮度
}

//---------設定螢幕反白模式-----------
void setdisplayInverse()  //設定螢幕反白模式
{
  //  BMD31.invertDisplay(parameter)：是否反白
  // true：白底黑字 (black-on-white)
  // false：黑底白字 (white-on-black)
  BMD31.invertDisplay(TRUE); // invert Display Mode:black-on-white
}

//--------設定螢幕正常模式--------
void setdisplayNormal()  //設定螢幕正常模式
{
  //  BMD31.invertDisplay(parameter)：是否反白
  // true：白底黑字 (black-on-white)
  // false：黑底白字 (white-on-black)  
  BMD31.invertDisplay(FALSE); // invert Display Mode:black-on-white
}

//----------向右捲動------------------
void scrollRight(int x, int y, int speed,int dir) //向右捲動
{
  // x:startRow：起始列位址，範圍 0~7
  // y:endRow：結束列位址，範圍 0~7
  // speed:scrollSpeed：滾動速度
  //   0x07 (SCROLL_2FRAMES)：滾動 2 幀
  //   0x04 (SCROLL_3FRAMES)：滾動 3 幀
  //   0x05 (SCROLL_4FRAMES)：滾動 4 幀
  //   0x00 (SCROLL_5FRAMES)：滾動 5 幀
  //   0x06 (SCROLL_25FRAMES)：滾動 25 幀
  //   0x01 (SCROLL_64FRAMES)：滾動 64 幀
  //   0x02 (SCROLL_128FRAMES)：滾動 128 幀
  //   0x03 (SCROLL_256FRAMES)：滾動 256 幀
  // dir:scrollVDirection：對角滾動方向
  //   0x00 (SCROLLV_NONE)：不滾動
  //   0x01 (SCROLLV_TOP)：向上滾動
  //   0x3F (SCROLLV_BOTTOM)：向下滾動
BMD31.startScrollRight((uint8_t)x, (uint8_t)y, (uint8_t)speed,(uint8_t)dir) ;
}

//--------------向左捲動-----------------
void scrollLeft(int x, int y, int speed,int dir) //向左捲動
{
  // x:startRow：起始列位址，範圍 0~7
  // y:endRow：結束列位址，範圍 0~7
  // speed:scrollSpeed：滾動速度
  //   0x07 (SCROLL_2FRAMES)：滾動 2 幀
  //   0x04 (SCROLL_3FRAMES)：滾動 3 幀
  //   0x05 (SCROLL_4FRAMES)：滾動 4 幀
  //   0x00 (SCROLL_5FRAMES)：滾動 5 幀
  //   0x06 (SCROLL_25FRAMES)：滾動 25 幀
  //   0x01 (SCROLL_64FRAMES)：滾動 64 幀
  //   0x02 (SCROLL_128FRAMES)：滾動 128 幀
  //   0x03 (SCROLL_256FRAMES)：滾動 256 幀
  // dir:scrollVDirection：對角滾動方向
  //   0x00 (SCROLLV_NONE)：不滾動
  //   0x01 (SCROLLV_TOP)：向上滾動
  //   0x3F (SCROLLV_BOTTOM)：向下滾動
BMD31.startScrollLeft((uint8_t)x, (uint8_t)y, (uint8_t)speed,(uint8_t)dir) ;
}

//-----------------停止滾動-----------------------
void stopScroll() //停止滾動
{
  BMD31.stopScroll();   //停止滾動
}

//--------------------
void test_invertDisplay(void) // 顯示反白與恢復
{
  BMD31.invertDisplay(TRUE); // invert Display Mode:black-on-white
  delay(500);
  BMD31.invertDisplay(FALSE); // normal Display Mode:white-on-black
  delay(500);
  BMD31.invertDisplay(TRUE); // invert Display Mode:black-on-white
  delay(500);
  BMD31.invertDisplay(FALSE); // normal Display Mode:white-on-black
  delay(500);
}

void test_dim(void)   // 顯示亮度切換（省電模式）
{
  BMD31.dim(TRUE);   // 降低亮度（省電模式）
  delay(500);
  BMD31.dim(FALSE);  // 回復正常亮度
  delay(500);
  BMD31.dim(TRUE);
  delay(500);
  BMD31.dim(FALSE);
  delay(500);
}
void test_drawString_6x8(void)  //顯示 6x8 字型字串
{
  BMD31.clearDisplay();  // 清除畫面
  BMD31.display();       // 更新顯示

  BMD31.setFont(FontTable_6X8);  // 設定字型為 6x8
  uint8_t col = (128 - (6 * sizeof("Hello World!"))) / 2;  // 計算置中位置

  for (uint8_t row = 0; row < 8; row++)
    BMD31.drawString(col, row, (u8*)"Hello World!");

  delay(500);
}

void test_drawString_8x16(void) // 顯示 8x16 字型字串
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.setFont(FontTable_8X16);  // 設定字型為 8x16
  uint8_t col = (128 - (8 * sizeof("Hello World!"))) / 2;

  for (uint8_t row = 0; row < 8; row += 2)  // 每 2 行顯示一次
    BMD31.drawString(col, row, (u8*)"Hello World!");

  delay(500);
}
void test_drawString_drawChar_drawNum(void) // 同時顯示字串、單字元與數字
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.drawString(16, displayROW0, (u8*)"Best Modules");
  BMD31.drawString(8, displayROW2, (u8*)"BMD31M090 TEST");
  /* Sequentially display ASCII ' ' (0x20/32) to ' ~' (0x7E/126) values for Font 6x8 & 8x16 */
  do
  {
    BMD31.setFont(FontTable_6X8);
    BMD31.drawString(0, displayROW5, (u8*)"ASCII:");
    BMD31.drawString(63, displayROW5, (u8*)"CODE:");
    BMD31.drawChar(48, displayROW5, t);
    BMD31.drawNum(103, displayROW5, t, 3);

    BMD31.setFont(FontTable_8X16);
    BMD31.drawString(0, displayROW6, (u8*)"ASCII:");
    BMD31.drawString(63, displayROW6, (u8*)"CODE:");
    BMD31.drawChar(48, displayROW6, t);
    BMD31.drawNum(103, displayROW6, t, 3);
    delay(10);
  } while (++t < '~');
  t = ' ';
}
void test_drawPixel(void) // 像素繪製測試（白→黑→反轉）
{
  BMD31.clearDisplay();
  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 64; row++)
      BMD31.drawPixel(col, row, pixelColor_WHITE);
  BMD31.display();
  delay(500);

  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 32; row++)
      BMD31.drawPixel(col, row, pixelColor_BLACK);
  BMD31.display();
  delay(500);

  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 64; row++)
      BMD31.drawPixel(col, row, pixelColor_INVERSE);
  BMD31.display();
  delay(500);
}

void test_drawFastHLine_drawFastVLine(void) // 繪製水平/垂直線條
{
  int8_t col = 112;
  BMD31.clearDisplay();

  for (int8_t row = 0; row < 64; row += 8) {
    BMD31.drawFastHLine(0, row, col, pixelColor_WHITE);
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE);
    col -= 14;
  }

  BMD31.display();
  delay(500);

  BMD31.clearDisplay();
  col = 112;

  for (int8_t row = 56; row >= 0; row -= 8) {
    BMD31.drawFastHLine(col, row, 128 - col, pixelColor_WHITE);
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE);
    col -= 14;
  }

  BMD31.display();
  delay(500);
}
void test_drawBitmap()  // 顯示 LOGO 點陣圖
{
  /*==========================================================
    |Using Image2LCD Software conversion, setting as bellow    |
    |1. Output file type: C array (*.c)                        |
    |2. Scan mode: Horizon Scan                                |
    |3. Bits Pixel: monochrome                                 |
    |4. Max Width and Height: 128 (Width) / 64 (Height)        |
    |5. Select "MSB First"                                     |
    ----------------------------------------------------------
    |Using LCD Assistant Software conversion, setting as bellow|
    |1. Byte orientation: Horizontal                           |
    |2. Size: According your image size                        |
    |3. Size endianness: Little                                |
    |4. Pixels/byte: 8                                         |
    ==========================================================*/

  // 顯示 BestModule_LOGO
  BMD31.clearDisplay();
  BMD31.drawBitmap(0, 0, BestModule_LOGO, 128, 64, pixelColor_WHITE);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGO, 128, 64, pixelColor_BLACK);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGO, 128, 64, pixelColor_INVERSE);
  BMD31.display();
  delay(300);

  BMD31.clearDisplay();
  BMD31.drawBitmap(0, 0, BestModule_LOGOandName, 128, 64, pixelColor_WHITE);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGOandName, 128, 64, pixelColor_BLACK);
  BMD31.display();
  delay(300);
  BMD31.drawBitmap(0, 0, BestModule_LOGOandName, 128, 64, pixelColor_INVERSE);
  BMD31.display();
  delay(300);
}
void test_variousScroll(void) // 滾動顯示效果
{
  /* Scroll Function Test */
  uint8_t startrow = displayROW0;
  uint8_t endrow = displayROW7;

  BMD31.startScrollRight(startrow, endrow, SCROLL_2FRAMES);
  delay(500);
  BMD31.startScrollRight(startrow, endrow, SCROLL_2FRAMES, SCROLLV_TOP);
  delay(500);
  BMD31.startScrollRight(startrow, endrow, SCROLL_2FRAMES, SCROLLV_BOTTOM);
  delay(500);

  BMD31.startScrollLeft(startrow, endrow, SCROLL_2FRAMES);
  delay(500);
  BMD31.startScrollLeft(startrow, endrow, SCROLL_2FRAMES, SCROLLV_TOP);
  delay(500);
  BMD31.startScrollLeft(startrow, endrow, SCROLL_2FRAMES, SCROLLV_BOTTOM);
  delay(500);

  BMD31.stopScroll();
}
//-------列印抬頭於OLED上---------------
void showTitleonOled(String ss,int row) //列印抬頭於OLED上
{
      printText(0,row,"              ")  ;  //清空
    printText(0,row,ss)  ;  //轉換string字串到char array
    Serial.print("Title on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}
//-------列印IP Address於OLED上---------------
void showIPonOled(String ss,int row) //列印IP Address於OLED上
{
      printText(0,row,"              ")  ;  //清空
    printText(0,row,ss)  ;  //轉換string字串到char array
    Serial.print("IP Address on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}
//-------列印Device ID於OLED上---------------
void showDeviceonOled(String ss,int row) //列印Device ID於OLED上
{
      printText(0,row,"              ")  ;  //清空
    printText(0,row,ss)  ;  //轉換string字串到char array
    Serial.print("Title on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印ＭＡＣ　Ａｄｄｒｅｓｓ於OLED上---------------
void showMACeonOled(String ss,int row) //列印MAC Address於OLED上
{
      printText(0,row,"              ")  ;  //清空
    printText(0,row,ss)  ;  //轉換string字串到char array
    Serial.print("MAC on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印ＭＡＣ　Ａｄｄｒｅｓｓ於OLED上---------------
void showSSIDeonOled(String ss,int row) //列印SSID於OLED上
{
      printText(0,row,"              ")  ;  //清空
    printText(0,row,ss)  ;  //轉換string字串到char array
    Serial.print("SSID on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}

//-------列印Message於OLED上---------------
void showMsgonOled(String ss,int row) //列印Message於OLED上
{
      printText(0,row,"              ")  ;  //清空
    printText(0,row,ss)  ;  //轉換string字串到char array
    Serial.print("Message on OLED:(") ;
  Serial.print(ss) ;
  Serial.print(")\n") ;     
}