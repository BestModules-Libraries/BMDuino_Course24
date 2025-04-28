/*****************************************************************
File:         display.ino
Description:  1.Wire interface (Clock Frequency: 400K) is used to communicate with BMD31M090.
              2.Hardware Serial (BAUDRATE 115200) is used to communicate with Serial Port Monitor.
connection method: sclPin:SCL(D19) sdaPin:SDA(D18)
******************************************************************/
#include "BMD31M090.h"  // 引入 BMD31M090 OLED 顯示模組的函式庫
#include "Bitmap.h"     // 引入位圖相關的函式庫

// 定義 BMD31M090 顯示模組的寬度和高度（單位：像素）
#define BMD31M090_WIDTH   128        
#define BMD31M090_HEIGHT  64         

// 設定 BMD31M090 顯示模組的 I2C 地址，根據電路圖來設定 I2C 地址
#define BMD31M090_ADDRESS 0x3C       
// #define BMD31M090_ADDRESS 0x3D   // 可以配置的 I2C 地址：Addr0:0x3C 或 Addr1:0x3D

uint8_t t = ' ';  // 宣告變數 t，初始化為空格字元

// 創建 BMD31M090 顯示模組的物件，並使用 HW Wire 進行通訊
BMD31M090 BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire);
//BMD31M090     BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire1); //Please uncomment out this line of code if you use HW Wire1 on BMduino
//BMD31M090     BMD31(BMD31M090_WIDTH, BMD31M090_HEIGHT, &Wire2); //Please uncomment out this line of code if you use HW Wire1 on BMduino

void setup()
{
  // 初始化序列通訊，波特率設定為 115200，方便與電腦或其他設備通訊
  Serial.begin(115200);
  Serial.println("BMD31M090 0.96\" OLED 模組範例程式");

  // 初始化 BMD31M090 顯示模組，使用 I2C 地址進行通訊
  BMD31.begin(BMD31M090_ADDRESS);
  delay(100);  // 建議的初始化延遲

  // 測試函式：顯示字串 "Hello World!" (字體大小 6x8)
  test_drawString_6x8();

  // 測試函式：顯示字串 "Hello World!" (字體大小 8x16)
  test_drawString_8x16();

  // 測試函式：顯示字串、字元和數字
  test_drawString_drawChar_drawNum();

  // 測試函式：繪製單個像素
  test_drawPixel();

  // 測試函式：繪製快速水平線和垂直線
  test_drawFastHLine_drawFastVLine();

  // 測試函式：繪製位圖（顯示 LOGO）
  test_drawBitmap();

  // 測試函式：顯示滾動功能，顯示不同方向的滾動效果
  test_variousScroll();

  // 測試函式：顯示翻轉顯示效果
  test_invertDisplay();

  // 測試函式：顯示調暗效果
  test_dim();
}

void loop()
{
  // 主循環中不執行任何操作，所有功能都在 setup() 中完成
}

void test_drawString_6x8(void)
{
  BMD31.clearDisplay();  // 清空顯示畫面
  BMD31.display();       // 更新顯示

  uint8_t col, row;

  // 設置字體類型為 6x8
  BMD31.setFont(FontTable_6X8);

  // 計算字串 "Hello World!" 水平居中的起始列
  col = (128 - (6 * sizeof("Hello World!"))) / 2;

  // 在每一行上顯示字串
  for (row = 0; row < 8; row++)
  {
    BMD31.drawString(col, row, (u8*)"Hello World!");
  }
  delay(500);  // 延遲 500 毫秒
}

void test_drawString_8x16(void)
{
  BMD31.clearDisplay();  // 清空顯示畫面
  BMD31.display();       // 更新顯示

  uint8_t col, row;

  // 設置字體類型為 8x16
  BMD31.setFont(FontTable_8X16);

  // 計算字串 "Hello World!" 水平居中的起始列
  col = (128 - (8 * sizeof("Hello World!"))) / 2;

  // 在每一行上顯示字串
  for (row = 0; row < 8; row += 2)
  {
    BMD31.drawString(col, row, (u8*)"Hello World!");
  }
  delay(500);  // 延遲 500 毫秒
}

void test_drawString_drawChar_drawNum(void)
{
  BMD31.clearDisplay();  // 清空顯示畫面
  BMD31.display();       // 更新顯示

  // 顯示兩行字串 "Best Modules" 和 "BMD31M090 TEST"
  BMD31.drawString(16, displayROW0, (u8*)"Best Modules");
  BMD31.drawString(8, displayROW2, (u8*)"BMD31M090 TEST");

  // 循環顯示 ASCII 字元和對應的代碼，從空格（0x20）到波浪號（0x7E）
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
  t = ' ';  // 重置 t 為空格
}

void test_drawPixel(void)
{
  BMD31.clearDisplay();  // 清空顯示畫面

  // 繪製所有像素為白色
  for (uint8_t col = 0; col < 128; col++)
  {
    for (uint8_t row = 0; row < 64; row++)
    {
      BMD31.drawPixel(col, row, pixelColor_WHITE);
    }
  }
  BMD31.display();  // 更新顯示
  delay(500);       // 延遲 500 毫秒

  // 將上半部分像素設為黑色
  for (uint8_t col = 0; col < 128; col++)
  {
    for (uint8_t row = 0; row < 32; row++)
    {
      BMD31.drawPixel(col, row, pixelColor_BLACK);
    }
  }
  BMD31.display();
  delay(500);

  // 將所有像素設為反相顏色
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

void test_drawFastHLine_drawFastVLine(void)
{
  int8_t col, row;

  BMD31.clearDisplay();  // 清空顯示畫面

  // 從右上角向下繪製水平線和垂直線
  col = 112;
  for (row = 0; row < 64; row += 8)
  {
    BMD31.drawFastHLine(0, row, col, pixelColor_WHITE);
    BMD31.drawFastVLine(col, row, 64 - row, pixelColor_WHITE);
    col -= 14;
  }
  BMD31.display();
  delay(500);

  // 反向繪製，從左下角開始
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

void test_drawBitmap(void)
{
  // 使用預設位圖 LOGO 繪製，顯示不同顏色
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
}

void test_variousScroll(void)
{
  // 測試滾動效果，包括不同方向和不同滾動速度
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

  BMD31.stopScroll();  // 停止滾動
}

void test_invertDisplay(void)
{
  // 測試顯示翻轉效果，顯示不同的黑白反轉
  BMD31.invertDisplay(TRUE);  // 黑底白字模式
  delay(500);
  BMD31.invertDisplay(FALSE);  // 白底黑字模式
  delay(500);
}

void test_dim(void)
{
  // 測試調暗顯示，分別進行調暗和恢復
  BMD31.dim(TRUE);  // 顯示調暗模式
  delay(500);
  BMD31.dim(FALSE);  // 恢復亮度
  delay(500);
}
