/*****************************************************************
  檔案名稱: display.ino
  描述:
    1. 使用 I2C (Wire 介面，時脈頻率: 400KHz) 與 BMD31M090 顯示模組通訊。
    2. 使用硬體序列埠 (波特率 115200) 與序列埠監控器通訊。
  接線方式: sclPin -> D19、sdaPin -> D18
******************************************************************/
#include "BMD31M090.h"  // 引入 BMD31M090 OLED 模組控制函式庫
#include "Bitmap.h"     // 引入圖片 Bitmap 資料

#define BMD31M090_WIDTH   128        // 顯示模組寬度 (像素)
#define BMD31M090_HEIGHT  64         // 顯示模組高度 (像素)
#define BMD31M090_ADDRESS 0x3C       // 預設 I2C 位址為 0x3C，亦可改為 0x3D

uint8_t t = ' '; // 用來顯示 ASCII 字元的初始值

// 初始化 BMD31M090 物件，設定顯示解析度與使用哪一個 I2C 通道 (Wire1)
// 可根據實際使用的 Wire/Wire1/Wire2 來開啟對應的註解
// BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire);
BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire1);
// BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire2);

void setup()
{
  Serial.begin(115200); // 初始化序列埠，設為 115200 波特率
  Serial.println("BMD31M090 0.96\" OLED Module Sketch");

  BMD31.begin(BMD31M090_ADDRESS); // 初始化 OLED 模組，使用指定的 I2C 位址
  delay(100); // 建議初始化後稍作延遲

  // 執行各種顯示測試函式
  test_drawString_6x8();
  test_drawString_8x16();
  test_drawString_drawChar_drawNum();
  test_drawPixel();
  test_drawFastHLine_drawFastVLine();
  test_drawBitmap();
  test_variousScroll();
  test_invertDisplay();
  test_dim();
}

void loop()
{
  // 空的 loop，因為所有測試在 setup() 中完成
}

// 測試顯示 6x8 字體的字串
void test_drawString_6x8(void)
{
  BMD31.clearDisplay();  // 清除螢幕畫面
  BMD31.display();       // 將畫面更新

  uint8_t col, row;

  BMD31.setFont(FontTable_6X8); // 設定字型為 6x8 大小

  // 計算讓字串置中的列起始位置
  col = (128 - (6 * sizeof("Hello World!"))) / 2;

  // 逐列印出 "Hello World!"
  for (row = 0; row < 8; row++)
  {
    BMD31.drawString(col, row, (u8*)"Hello World!");
  }
  delay(500); // 暫停 0.5 秒
}

// 測試顯示 8x16 字體的字串
void test_drawString_8x16(void)
{
  BMD31.clearDisplay();
  BMD31.display();

  uint8_t col, row;

  BMD31.setFont(FontTable_8X16); // 設定字型為 8x16

  col = (128 - (8 * sizeof("Hello World!"))) / 2;

  // 每隔兩行顯示一次
  for (row = 0; row < 8; row += 2)
  {
    BMD31.drawString(col, row, (u8*)"Hello World!");
  }
  delay(500);
}

// 測試字串、單一字元與數字的顯示
void test_drawString_drawChar_drawNum(void)
{
  BMD31.clearDisplay();
  BMD31.display();

  BMD31.drawString(16, displayROW0, (u8*)"Best Modules");
  BMD31.drawString(8, displayROW2, (u8*)"BMD31M090 TEST");

  // 循環顯示 ASCII 字元 ' ' ~ '~'
  do
  {
    // 6x8 字體顯示
    BMD31.setFont(FontTable_6X8);
    BMD31.drawString(0, displayROW5, (u8*)"ASCII:");
    BMD31.drawString(63, displayROW5, (u8*)"CODE:");
    BMD31.drawChar(48, displayROW5, t); // 顯示單一字元
    BMD31.drawNum(103, displayROW5, t, 3); // 顯示字元對應的數值

    // 8x16 字體顯示
    BMD31.setFont(FontTable_8X16);
    BMD31.drawString(0, displayROW6, (u8*)"ASCII:");
    BMD31.drawString(63, displayROW6, (u8*)"CODE:");
    BMD31.drawChar(48, displayROW6, t);
    BMD31.drawNum(103, displayROW6, t, 3);
    delay(10);
  } while (++t < '~'); // 循環到 '~' 為止
  t = ' '; // 重設回空白字元
}

// 測試像素繪圖功能
void test_drawPixel(void)
{
  BMD31.clearDisplay();

  // 將整個螢幕以白色像素塗滿
  for (uint8_t col = 0; col < 128; col++)
  {
    for (uint8_t row = 0; row < 64; row++)
    {
      BMD31.drawPixel(col, row, pixelColor_WHITE);
    }
  }
  BMD31.display();
  delay(500);

  // 將上半部塗黑
  for (uint8_t col = 0; col < 128; col++)
  {
    for (uint8_t row = 0; row < 32; row++)
    {
      BMD31.drawPixel(col, row, pixelColor_BLACK);
    }
  }
  BMD31.display();
  delay(500);

  // 將整個畫面反相
  for (uint8_t col = 0; col < 128; col++)
  {
    for (uint8_t row = 0; row < 64; row++)
    {
      BMD31.drawPixel(col, row, pixelColor_INVERSE);
    }
  }
  BMD31.display();
  delay(500);
}

// 測試快速畫水平線與垂直線
void test_drawFastHLine_drawFastVLine(void)
{
  int8_t col, row;

  BMD31.clearDisplay();
  col = 112;
  for (row = 0; row < 64; row += 8)
  {
    BMD31.drawFastHLine(0, row, col, pixelColor_WHITE); // 水平線
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE); // 垂直線
    col -= 14;
  }
  BMD31.display();
  delay(500);

  BMD31.clearDisplay();
  col = 112;
  for (row = 56; row >= 0; row -= 8)
  {
    BMD31.drawFastHLine(col, row, 128 - col, pixelColor_WHITE);
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE);
    col -= 14;
  }
  BMD31.display();
  delay(500);
}

// 測試繪製圖片 Bitmap（需搭配 Bitmap.h 中的圖檔）
void test_drawBitmap(void)
{
  // 載入 LOGO 圖片並依序使用白、黑、反相方式顯示
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

  // 顯示 LOGO + 公司名稱
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

// 測試各種方向的捲動顯示功能
void test_variousScroll(void)
{
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

  BMD31.stopScroll(); // 停止捲動
}

// 測試畫面反相顯示
void test_invertDisplay(void)
{
  BMD31.invertDisplay(TRUE); // 黑底白字
  delay(500);
  BMD31.invertDisplay(FALSE); // 白底黑字
  delay(500);
  BMD31.invertDisplay(TRUE);
  delay(500);
  BMD31.invertDisplay(FALSE);
  delay(500);
}

// 測試畫面亮度調暗與回復
void test_dim(void)
{
  BMD31.dim(TRUE); // 進入低亮度模式
  delay(500);
  BMD31.dim(FALSE); // 回復正常亮度
  delay(500);
  BMD31.dim(TRUE);
  delay(500);
  BMD31.dim(FALSE);
  delay(500);
}
