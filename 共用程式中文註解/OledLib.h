/*
 * ==================================================
 * 程式名稱：OledLib.h - OLED 顯示模組自訂函式庫
 * 適用硬體：BMD31M090 0.96 吋 OLED 顯示模組 (128x64)
 * 通訊介面：I2C (預設位址 0x3C)
 * 
 * 程式功能說明：
 *   本程式為一 OLED 顯示模組的自訂函式庫，提供多種顯示功能，
 *   包括初始化 OLED、設定字型、顯示文字/數字/浮點數、繪製點/線/矩形、
 *   顯示點陣圖、捲動效果、亮度調整、反白模式等。
 *   使用者可透過呼叫本函式庫提供的函式，輕鬆控制 OLED 顯示器，
 *   適用於嵌入式系統或 Arduino / BMduino 開發板。
 * 
 * 主要功能：
 *   1. OLED 初始化與基本控制 (清屏、更新顯示)
 *   2. 字型設定 (6x8、8x16、16x32、32x64)
 *   3. 文字、字元、整數、浮點數顯示
 *   4. 繪圖功能 (點、線、水平/垂直線、矩形)
 *   5. 點陣圖顯示 (Logo 或自訂圖片)
 *   6. 螢幕滾動效果 (向左/向右，可搭配垂直方向)
 *   7. 亮度調整 (省電模式/正常模式)
 *   8. 顯示反白模式切換
 *   9. 測試函式 (展示各項功能)
 * 
 * 作者/版本：Best Module / 自訂函式庫
 * 建立日期：2025.03.27
 * 修改紀錄：
 *   - 2025.03.27：加入詳細繁體中文註解與整體說明
 * ==================================================
 */

//-------- OLED 顯示模組自訂函式庫-----------

//------全域變數區--------------------------
// 定義 BMD31M090 顯示模組的寬度和高度（單位：像素）
#define BMD31M090_WIDTH   128        
#define BMD31M090_HEIGHT  64         

// 設定 BMD31M090 顯示模組的 I2C 地址，根據電路圖來設定 I2C 地址
#define BMD31M090_ADDRESS 0x3C       
// #define BMD31M090_ADDRESS 0x3D   // 可以配置的 I2C 地址：Addr0:0x3C 或 Addr1:0x3D

uint8_t t = ' ';  // 宣告變數 t，初始化為空格字元，用於測試時顯示 ASCII 字元

//----------外部引用函式區----------------
//------感測器外部函式庫----------------
#include "BMD31M090.h"      // 引入 BMD31M090 OLED 顯示模組的函式庫
#include "BestModuleLogo.h" // 引入 BestModule 的 Logo 點陣圖資料
//#include "Bitmap.h"       // 引入位圖相關的函式庫（可選）

//-----------------感測元件物件區-------------------------
// 創建 BMD31M090 顯示模組的物件，並使用 HW Wire 進行通訊
//BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire);
BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire1); // 使用硬體 Wire1 介面 (BMduino 適用)
//BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire2); // 使用硬體 Wire2 介面

//----------自定義函式區宣告--------------
void initOled();                                         // 初始化 OLED12864，0.96 吋 OLED 顯示模組 BMD31M090
void drawPicture(int x, int y, const uint8_t *pp, int width, int height); // 在指定位置繪製點陣圖（白色）
void clearScreen();                                      // 清除螢幕
void updateScreen();                                     // 顯示當前緩衝區的內容
void setFont(const unsigned char* font);                 // 設定字形
//   font：字體格式
//   FontTable_6X8：字體格式為 6×8
//   FontTable_8X16：字體格式為 8×16
//   FontTable_16X32：字體格式為 16×32
//   FontTable_32X64：字體格式為 32×64
void printText(int x, int y, uint8_t *str);              // 在指定位置 (x, y) 印出文字
void printChar(int x, int y, char str);                  // 在指定位置 (x, y) 印出單一字元
void printNumber(int x, int y, int num);                 // 在指定位置 (x, y) 印出整數
void printFloat(int x, int y, float num);                // 在指定位置 (x, y) 印出浮點數
void drawPoint(int x, int y, int pixelColor);            // 在指定位置繪製一個像素點
//   x：x 座標，範圍 0~127
//   y：y 座標，範圍 0~63
//   pixelColor：像素顏色
//     0 (pixelColor_BLACK)：黑色
//     1 (pixelColor_WHITE)：白色
//     2 (pixelColor_INVERSE)：顏色翻轉
void drawLine(int x1, int y1, int x2, int y2, int pixelColor); // 畫出從 (x1,y1) 到 (x2,y2) 的直線
void drawfastVline(int x, int y, int width, int pixelColor);    // 從 (x,y) 開始繪製垂直線（指定高度）
void drawVline(int x, int y, int width, int pixelColor);        // 同 drawfastVline（相容性）
void drawfastHline(int x, int y, int width, int pixelColor);    // 從 (x,y) 開始繪製水平線（指定寬度）
void drawHline(int x, int y, int width, int pixelColor);        // 同 drawfastHline（相容性）
void drawBox(int x1, int y1, int x2, int y2, int pixelColor);   // 繪製矩形（對角線兩點）
void setsaveMode();                                              // 設定省電模式（降低亮度）
void setlightMode();                                             // 設定正常模式（恢復亮度）
void setdisplayInverse();                                        // 設定螢幕反白模式（白底黑字）
void setdisplayNormal();                                         // 設定螢幕正常模式（黑底白字）

//-------------向右捲動-----------------------
void scrollRight(int x, int y, int speed, int dir); // 向右捲動
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
void scrollLeft(int x, int y, int speed, int dir);  // 向左捲動
// 參數說明同上

//-----------------停止滾動-----------------------
void stopScroll(); // 停止滾動

//-------顯示資訊於 OLED 上的輔助函式---------------
void showDeviceonOled(String ss, int row);  // 顯示 Device ID 於 OLED 上
void showTitleonOled(String ss, int row);   // 顯示標題於 OLED 上
void showIPonOled(String ss, int row);      // 顯示 IP Address 於 OLED 上
void showMACeonOled(String ss, int row);    // 顯示 MAC Address 於 OLED 上
void showSSIDeonOled(String ss, int row);   // 顯示 SSID 於 OLED 上
void showMsgonOled(String ss, int row);     // 顯示訊息於 OLED 上

//---------測試 OLED 顯示與展示----------------
void test_drawString_6x8();              // 測試 6x8 字型顯示字串
void test_drawString_8x16();             // 測試 8x16 字型顯示字串
void test_drawString_drawChar_drawNum(); // 同時測試字串、字元、數字顯示
void test_drawPixel();                   // 測試像素點繪製（白→黑→反轉）
void test_drawFastHLine_drawFastVLine(); // 測試水平與垂直線條繪製
void test_drawBitmap();                  // 測試顯示 LOGO 點陣圖
void test_variousScroll();               // 測試各種滾動效果
void test_invertDisplay();               // 測試顯示反白與恢復
void test_dim();                         // 測試亮度切換（省電模式）

//--------自定義函式區程式本體-----------

//--------初始化 OLED 顯示模組，啟動 I2C 通訊-----------
void initOled()  // OLED 物件初始化區
{
  // 初始化 BMD31M090 顯示模組，使用指定的 I2C 地址進行通訊
  BMD31.begin(BMD31M090_ADDRESS);
  delay(100);  // 建議的初始化延遲，確保模組穩定
  Serial.println("init OLED12864 OK");
}

//-------------設定字形----------------
void setFont(const unsigned char* font)   // 設定字形
{
  // font：字體格式選擇
  // FontTable_6X8、FontTable_8X16、FontTable_16X32、FontTable_32X64
  BMD31.setFont(font);
}

//----------顯示當前緩衝區的內容------------------
void updateScreen()  // 將緩衝區內容更新到螢幕硬體
{
  BMD31.display();
}

//--------指定位置印出文字函式--------
void printText(int x, int y, String str)  // 在 (x, y) 位置印出字串
{
  // 將 String 轉換為 C 字串後傳入底層繪製函式
  BMD31.drawString((uint8_t)x, (uint8_t)y, (uint8_t*)str.c_str());
}

//-----------指定位置印出字元函式------------
void printChar(int x, int y, char str)  // 在 (x, y) 位置印出單一字元
{
  BMD31.drawChar((uint8_t)x, (uint8_t)y, (uint8_t)str);
}

//--------指定位置印出整數-----------
void printNumber(int x, int y, int num)  // 在 (x, y) 位置印出整數
{
  // 利用 String 長度決定數字位數，傳入繪製函式
  BMD31.drawNum((uint8_t)x, (uint8_t)y, (uint32_t)num, (uint8_t)(String(num).length()));
}

//-----------指定位置印出浮點數-----------
void printFloat(int x, int y, float num)  // 在 (x, y) 位置印出浮點數
{
  printText(x, y, String(num));  // 轉為字串後呼叫 printText
}

//--------指定位置繪出一點-----------
void drawPoint(int x, int y, int pixelColor) // 在 (x, y) 位置繪製一個像素點
{
  BMD31.drawPixel((uint8_t)x, (uint8_t)y, (uint8_t)pixelColor);
  BMD31.display();  // 立即更新顯示
}

//---------在兩點畫一條直線------------
void drawLine(int x1, int y1, int x2, int y2, int pixelColor) // 繪製從 (x1,y1) 到 (x2,y2) 的直線
{
  BMD31.drawLine((uint8_t)x1, (uint8_t)y1, (uint8_t)x2, (uint8_t)y2, (uint8_t)pixelColor);
  BMD31.display();
}

//-----在一點畫一條垂直線（指定高度）-------
void drawfastVline(int x, int y, int width, int pixelColor)  // 從 (x, y) 向下繪製垂直線，長度 width
{
  BMD31.drawFastVLine((uint8_t)x, (uint8_t)y, (uint8_t)width, (uint8_t)pixelColor);
  BMD31.display();
}

//---------在一點畫一條水平線（指定寬度）---------
void drawfastHline(int x, int y, int width, int pixelColor)  // 從 (x, y) 向右繪製水平線，長度 width
{
  BMD31.drawFastHLine((uint8_t)x, (uint8_t)y, (uint8_t)width, (uint8_t)pixelColor);
  BMD31.display();
}

//---------垂直線相容性函式（同 drawfastVline）-------
void drawVline(int x, int y, int width, int pixelColor)
{
  drawfastVline(x, y, width, pixelColor);
}

//---------水平線相容性函式（同 drawfastHline）-------
void drawHline(int x, int y, int width, int pixelColor)
{
  drawfastHline(x, y, width, pixelColor);
}

//-------以兩點為對角繪製一個矩形-------
void drawBox(int x1, int y1, int x2, int y2, int pixelColor) // 繪製矩形邊框
{
  // 上邊線
  BMD31.drawLine((uint8_t)x1, (uint8_t)y1, (uint8_t)x2, (uint8_t)y1, (uint8_t)pixelColor);
  // 右邊線
  BMD31.drawLine((uint8_t)x2, (uint8_t)y1, (uint8_t)x2, (uint8_t)y2, (uint8_t)pixelColor);
  // 左邊線
  BMD31.drawLine((uint8_t)x1, (uint8_t)y1, (uint8_t)x1, (uint8_t)y2, (uint8_t)pixelColor);
  // 下邊線
  BMD31.drawLine((uint8_t)x1, (uint8_t)y2, (uint8_t)x2, (uint8_t)y2, (uint8_t)pixelColor);
  BMD31.display();
}

//--------清除螢幕-----------
void clearScreen()    // 清除螢幕所有內容
{
  BMD31.clearDisplay(); // 清除記憶體緩衝區
  BMD31.display();      // 將清除結果更新到螢幕
}

//-------在指定位置以黑色繪製點陣圖（反白效果）---------
void invdrawPicture(int x, int y, const uint8_t *pp, int width, int height)
{
  BMD31.clearDisplay();
  BMD31.drawBitmap((uint8_t)x, (uint8_t)y, pp, (uint8_t)width, (uint8_t)height, pixelColor_BLACK);
  BMD31.display();
}

//-------------在指定位置以白色繪製點陣圖-------------
void drawPicture(int x, int y, const uint8_t *pp, int width, int height)  // 使用白色繪製點陣圖
{
  BMD31.clearDisplay();
  BMD31.drawBitmap((uint8_t)x, (uint8_t)y, pp, (uint8_t)width, (uint8_t)height, pixelColor_WHITE);
  BMD31.display();
}

//----------設定省電模式（降低亮度）-------------
void setsaveMode()  // 降低亮度以節省電量
{
  BMD31.dim(TRUE);   // 啟用省電模式
}

//--------設定正常模式（恢復正常亮度）-----------
void setlightMode() // 恢復正常亮度
{
  BMD31.dim(FALSE);  // 關閉省電模式
}

//---------設定螢幕反白模式（白底黑字）-----------
void setdisplayInverse()
{
  BMD31.invertDisplay(TRUE); // 啟用反白模式
}

//--------設定螢幕正常模式（黑底白字）--------
void setdisplayNormal()
{
  BMD31.invertDisplay(FALSE); // 恢復正常模式
}

//----------向右捲動------------------
void scrollRight(int x, int y, int speed, int dir) // 啟動向右捲動效果
{
  BMD31.startScrollRight((uint8_t)x, (uint8_t)y, (uint8_t)speed, (uint8_t)dir);
}

//--------------向左捲動-----------------
void scrollLeft(int x, int y, int speed, int dir) // 啟動向左捲動效果
{
  BMD31.startScrollLeft((uint8_t)x, (uint8_t)y, (uint8_t)speed, (uint8_t)dir);
}

//---------停止滾動----------
void stopScroll() // 停止所有滾動效果
{
  BMD31.stopScroll();
}

//-------列印標題於 OLED 上---------------
void showTitleonOled(String ss, int row) // 在指定列顯示標題文字
{
  printText(0, row, "              ");  // 先清除該行
  printText(0, row, ss);                // 顯示新標題
  Serial.print("Title on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//----列印 IP Address 於 OLED 上------------
void showIPonOled(String ss, int row) // 顯示 IP 位址
{
  printText(0, row, "              ");
  printText(0, row, ss);
  Serial.print("IP Address on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//-------列印 Device ID 於 OLED 上-------
void showDeviceonOled(String ss, int row) // 顯示設備 ID
{
  printText(0, row, "              ");
  printText(0, row, ss);
  Serial.print("Device on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//-------列印 MAC Address 於 OLED 上---------------
void showMACeonOled(String ss, int row) // 顯示 MAC 位址
{
  printText(0, row, "              ");
  printText(0, row, ss);
  Serial.print("MAC on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//-------列印 SSID 於 OLED 上---------------
void showSSIDeonOled(String ss, int row) // 顯示 Wi-Fi SSID
{
  printText(0, row, "              ");
  printText(0, row, ss);
  Serial.print("SSID on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//-------列印訊息於 OLED 上---------------
void showMsgonOled(String ss, int row) // 顯示自訂訊息
{
  printText(0, row, "              ");
  printText(0, row, ss);
  Serial.print("Message on OLED:(");
  Serial.print(ss);
  Serial.print(")\n");
}

//---------測試 OLED 顯示與展示----------------

//--------顯示反白與恢復-------
void test_invertDisplay(void) // 測試反白模式切換
{
  BMD31.invertDisplay(TRUE);  // 白底黑字
  delay(500);
  BMD31.invertDisplay(FALSE); // 黑底白字
  delay(500);
  BMD31.invertDisplay(TRUE);
  delay(500);
  BMD31.invertDisplay(FALSE);
  delay(500);
}

//-------顯示亮度切換測試-----------
void test_dim(void)   // 測試亮度切換（省電模式）
{
  BMD31.dim(TRUE);    // 降低亮度
  delay(500);
  BMD31.dim(FALSE);   // 恢復亮度
  delay(500);
  BMD31.dim(TRUE);
  delay(500);
  BMD31.dim(FALSE);
  delay(500);
}

//--------顯示 6x8 字型字串測試----------
void test_drawString_6x8(void)  // 測試 6x8 字型顯示字串
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.setFont(FontTable_6X8);  // 設定 6x8 字型
  uint8_t col = (128 - (6 * sizeof("Hello World!"))) / 2;  // 計算水平置中位置

  for (uint8_t row = 0; row < 8; row++)
    BMD31.drawString(col, row, (u8*)"Hello World!");

  delay(500);
}

//--------顯示 8x16 字型字串測試----------
void test_drawString_8x16(void) // 測試 8x16 字型顯示字串
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.setFont(FontTable_8X16);  // 設定 8x16 字型
  uint8_t col = (128 - (8 * sizeof("Hello World!"))) / 2;  // 水平置中

  for (uint8_t row = 0; row < 8; row += 2)  // 每兩行顯示一次（因為字型高度為 16）
    BMD31.drawString(col, row, (u8*)"Hello World!");

  delay(500);
}

//-------同時顯示字串、字元與數字----------
void test_drawString_drawChar_drawNum(void) // 同時測試字串、字元、數字顯示
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.drawString(16, displayROW0, (u8*)"Best Modules");
  BMD31.drawString(8, displayROW2, (u8*)"BMD31M090 TEST");

  // 依序顯示 ASCII 從 32 (空白) 到 126 (~) 的字元
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
  t = ' ';  // 重置為空格
}

//-------像素繪製測試（白→黑→反轉）----
void test_drawPixel(void) // 測試繪製像素點
{
  BMD31.clearDisplay();
  // 全部繪製白色
  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 64; row++)
      BMD31.drawPixel(col, row, pixelColor_WHITE);
  BMD31.display();
  delay(500);

  // 上半部繪製黑色
  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 32; row++)
      BMD31.drawPixel(col, row, pixelColor_BLACK);
  BMD31.display();
  delay(500);

  // 全部反轉
  for (uint8_t col = 0; col < 128; col++)
    for (uint8_t row = 0; row < 64; row++)
      BMD31.drawPixel(col, row, pixelColor_INVERSE);
  BMD31.display();
  delay(500);
}

//--------繪製水平與垂直線條測試-------------
void test_drawFastHLine_drawFastVLine(void) // 測試水平與垂直線條繪製
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

//--------顯示 LOGO 點陣圖測試-----------
void test_drawBitmap()  // 測試點陣圖顯示（BestModule Logo）
{
  /* 使用 Image2LCD 或 LCD Assistant 轉換點陣圖資料
     設定為單色、水平掃描、MSB First 等參數 */
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

//----------滾動顯示效果測試----------
void test_variousScroll(void) // 測試各種滾動效果
{
  uint8_t startrow = displayROW0;  // 起始列
  uint8_t endrow = displayROW7;    // 結束列

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