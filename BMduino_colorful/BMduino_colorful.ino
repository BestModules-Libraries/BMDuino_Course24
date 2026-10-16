
/*
這段程式會使 LED 燈以流動的方式顯示不同的 RGB 顏色，
並根據 flow 變數改變色彩效果，
達到持續變換的視覺效果。
*/
#include <BMD26M088.h> // 引入 BMD26M088 控制 LED 的函式庫
BMD26M088 myBMD26M088(&Wire2); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
#define ADDRESS BMD26M088_I2C_ADDRESS_VDD // 定義 BMD26M088 的 I2C 地址

// 定義一個 RGB 結構體，包含紅、綠、藍三個顏色的資料
typedef struct RGB_DATA
{
  uint8_t r; // 紅色值
  uint8_t g; // 綠色值
  uint8_t b; // 藍色值
} rgb_def;

rgb_def get_color(int param); // 函數宣告，用於根據參數計算並獲取 RGB 顏色值

void rgb_colorful(uint8_t div_num, uint8_t flow_ch, bool direction); // 用於控制 RGB 流動效果的函數宣告（未實現）

void setup() 
{                                  
    myBMD26M088.begin(ADDRESS);  // 初始化 BMD26M088 模組的 I2C 通訊
    myBMD26M088.setBrightness(ADDRESS, 0xff); // 設置全域亮度，最大值為 0xff
    myBMD26M088.setCurrent(ADDRESS, BMD26M088_CCR_6MA); // 設定驅動電流為 6mA                 
}

int flow = 0; // 定義流動變數，控制 RGB 流動效果

void loop()
{
    u8 i; // 宣告迴圈變數
    rgb_def rgb; // 宣告一個 RGB 結構變數
    
    for(i = 0; i < 8; i++) // 迴圈更新 8 行 LED 的顏色
    {
      // 計算每一行 LED 的顏色，根據 flow 值進行流動效果
      rgb = get_color(flow + i * (420 / 16)); // 420 是色彩範圍，分成 16 份
      
      // 將計算得到的 RGB 值寫入第 i 行的 LED
      myBMD26M088.writeRow(ADDRESS, i, rgb.r, rgb.g, rgb.b); // 更新 LED 的 RGB 顏色
    }
  
    // 當 flow 超過 420 時，將其重置為 0，形成循環流動的效果
    if((flow++) > 420) {flow = 0;} // 控制 RGB 顏色的流動        
}

// 函數：根據參數計算並返回 RGB 顏色值
rgb_def get_color(int param)
{
  rgb_def rgb; // 定義一個 RGB 結構變數
  
  if(param < 0) { param += 420; } // 如果 param 小於 0，將其加回 420
  if(param > 419) { param -= 420; } // 如果 param 大於 419，將其減去 420

  // 根據不同的 param 值範圍計算 RGB 顏色值
  if(param < 60)  // 步驟1: 紅色 -> 黃色
  {
    rgb.r = 240;
    rgb.g = 4 * param;
    rgb.b = 0;
  }
  else if(param < 120) // 步驟2: 黃色 -> 綠色
  {
    rgb.r = 240 - (4 * (param - 60));
    rgb.g = 240;
    rgb.b = 0;
  }
  else if(param < 180) // 步驟3: 綠色 -> 青色
  {
    rgb.r = 0;
    rgb.g = 240;
    rgb.b = 4 * (param - 120);
  }
  else if(param < 240) // 步驟4: 青色接近藍色
  {
    rgb.r = 0;
    rgb.g = 240 - (3 * (param - 180));
    rgb.b = 240;
  }
  else if(param < 300) // 步驟5: 藍色 -> 紫色
  {
    rgb.r = (param - 240);
    rgb.g = 60 - (param - 240);
    rgb.b = 240 - (2 * (param - 240));
  }
  else if(param < 360) // 步驟6: 紫色 -> 粉紅色
  {
    rgb.r = 60 + (param - 300);
    rgb.g = 0;
    rgb.b = 120;
  }
  else if(param < 420) // 步驟7: 粉紅色 -> 紅色
  {
    rgb.r = 120 + (2 * (param - 360));
    rgb.g = 0;
    rgb.b = 120 - (2 * (param - 360));
  }

  return rgb; // 返回計算出的 RGB 顏色值
}
