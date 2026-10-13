#include <BMD26M088.h> // 引入 BMD26M088 控制 LED 的函式庫

// BMD26M088 myBMD26M088(&Wire); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
BMD26M088 myBMD26M088(&Wire1); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
// BMD26M088 myBMD26M088(&Wire2); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件

#define ADDRESS BMD26M088_I2C_ADDRESS_VDD // 定義 BMD26M088 的 I2C 地址
#define LEDLines 8 // LED 矩陣的行數（共 8 行）

// RGB 燈光強度（亮度），最大值為 0xFF（255）
uint8_t RGBBrightness = 0xff ;

// 定義一個 RGB 結構體，包含紅、綠、藍三個顏色的資料
typedef struct RGB_DATA
{
  uint8_t r; // 紅色值（0-255）
  uint8_t g; // 綠色值（0-255）
  uint8_t b; // 藍色值（0-255）
} rgb_def;

int flow = 0; // 定義流動變數，控制 RGB 流動效果的相位

//--------------------
// 函數宣告：根據參數計算並獲取 RGB 顏色值
rgb_def get_color(int param);

// 函數宣告：用於控制 RGB 流動效果（未實作）
void rgb_colorful(uint8_t div_num, uint8_t flow_ch, bool direction);

//-----------
// 常數定義
#define CheckColorDelayTime 200   // 測試顏色時的延遲時間（毫秒）
#define initDelayTime 2000        // 初始化延遲時間（毫秒）
#define CommandDelay 100          // 命令間隔延遲（毫秒）

// ------- 自定義函式宣告區 -----------
void initRGBLED();                // 初始化 RGB LED 模組
rgb_def get_color(int param);      // 根據參數計算並返回 RGB 顏色值
void Colorful();                   // 原廠測試：彩色流動效果
void setRGBMatrix(rgb_def colr);   // 設定整個 RGB LED 矩陣的顏色
rgb_def RGB2Color(int r,int g,int b); // 將三個顏色值組合成 RGB 結構
void ChangeBulbColor(int r,int g,int b); // 改變整個 RGB 矩陣的顏色
boolean DecodeString(String INPStr, byte *r, byte *g , byte *b); // 解析字串命令
void CheckLed();                   // 測試燈泡：依序顯示多種顏色

//-----------------------------------

//---------顏色變數
byte RedValue = 0, GreenValue = 0, BlueValue = 0; // 儲存解析出的 RGB 值
String ReadStr = "      " ; // 儲存從序列埠讀取的字串
int delayval = 500; // 延遲時間（目前未使用）
boolean readok = false ; // 讀取成功旗標（目前未使用）

// 預定義的顏色變數
rgb_def RGBLight = {255,255,255} ;  // 白光
rgb_def RGBBlank = {0,0,0} ;        // 全暗
rgb_def RGBRED = {255,0,0} ;        // 紅色
rgb_def RGBGreen = {0,255,0} ;      // 綠色
rgb_def RGBBlue = {0,0,255} ;       // 藍色

// 測試顏色陣列（用於 CheckLed 函式）
rgb_def CheckColor[] = {
                          {255 , 255,255} , // 白光
                          {255 , 0,0} ,     // 紅
                          {0 , 255,0} ,     // 綠
                          {0 , 0,255} ,     // 藍
                          {255 , 128,64} ,  // 橘
                          {255 , 255,0} ,   // 黃
                          {0 , 255,255} ,   // 青
                          {255 , 0,255} ,   // 紫
                          {255 , 255,255} , // 白
                          {255 , 128,0} ,   // 橙
                          {255 , 128,128} , // 粉紅
                          {128 , 255,255} , // 淺青
                          {128 , 128,192} , // 淺紫
                          {0 , 128,255} ,   // 天藍
                          {255 , 0,128} ,   // 洋紅
                          {128 , 64,64} ,   // 棕
                          {0 , 0,0} } ;     // 黑

//------------------
// 初始化 RGB LED 模組
void initRGBLED()
{
    // 初始化 BMD26M088 模組的 I2C 通訊，使用指定的 I2C 地址
    myBMD26M088.begin(ADDRESS);
    
    // 設置全域亮度，最大值為 0xff（255）
    myBMD26M088.setBrightness(ADDRESS, RGBBrightness);
    
    // 設定驅動電流為 6mA
    myBMD26M088.setCurrent(ADDRESS, BMD26M088_CCR_6MA);
}

// 函數：根據參數計算並返回 RGB 顏色值
// 參數 param 範圍：0~419，對應色環上的不同顏色
rgb_def get_color(int param)
{
  rgb_def rgb; // 定義一個 RGB 結構變數
  
  // 確保參數在 0~419 範圍內（循環處理）
  if(param < 0) { param += 420; } // 如果 param 小於 0，將其加回 420
  if(param > 419) { param -= 420; } // 如果 param 大於 419，將其減去 420

  // 根據不同的 param 值範圍計算 RGB 顏色值
  if(param < 60)  // 步驟1: 紅色 -> 黃色
  {
    rgb.r = 240;          // 紅色固定 240
    rgb.g = 4 * param;    // 綠色從 0 逐漸增加到 240
    rgb.b = 0;            // 藍色為 0
  }
  else if(param < 120) // 步驟2: 黃色 -> 綠色
  {
    rgb.r = 240 - (4 * (param - 60)); // 紅色從 240 逐漸減少到 0
    rgb.g = 240;                       // 綠色固定 240
    rgb.b = 0;                         // 藍色為 0
  }
  else if(param < 180) // 步驟3: 綠色 -> 青色
  {
    rgb.r = 0;                          // 紅色為 0
    rgb.g = 240;                         // 綠色固定 240
    rgb.b = 4 * (param - 120);           // 藍色從 0 逐漸增加到 240
  }
  else if(param < 240) // 步驟4: 青色接近藍色
  {
    rgb.r = 0;                          // 紅色為 0
    rgb.g = 240 - (3 * (param - 180));   // 綠色從 240 逐漸減少
    rgb.b = 240;                         // 藍色固定 240
  }
  else if(param < 300) // 步驟5: 藍色 -> 紫色
  {
    rgb.r = (param - 240);                // 紅色逐漸增加
    rgb.g = 60 - (param - 240);            // 綠色逐漸減少
    rgb.b = 240 - (2 * (param - 240));     // 藍色逐漸減少
  }
  else if(param < 360) // 步驟6: 紫色 -> 粉紅色
  {
    rgb.r = 60 + (param - 300);            // 紅色逐漸增加
    rgb.g = 0;                             // 綠色為 0
    rgb.b = 120;                           // 藍色固定 120
  }
  else if(param < 420) // 步驟7: 粉紅色 -> 紅色
  {
    rgb.r = 120 + (2 * (param - 360));     // 紅色逐漸增加
    rgb.g = 0;                             // 綠色為 0
    rgb.b = 120 - (2 * (param - 360));     // 藍色逐漸減少
  }

  return rgb; // 返回計算出的 RGB 顏色值
}

// 將三個獨立的 RGB 顏色值組合成 rgb_def 結構
rgb_def RGB2Color(int r,int g,int b)
{
    rgb_def tmp;
    tmp.r = r;
    tmp.g = g ;
    tmp.b = b ;
    return tmp ;
}

// 原廠測試函式：產生彩色流動效果
void Colorful()
{
    u8 i; // 宣告迴圈變數（應為 uint8_t）
    rgb_def rgb; // 宣告一個 RGB 結構變數
    
    for(i = 0; i < 8; i++) // 迴圈更新 8 行 LED 的顏色
    {
      // 計算每一行 LED 的顏色，根據 flow 值進行流動效果
      // 420 是色彩範圍，分成 16 份（8 行 × 2）
      rgb = get_color(flow + i * (420 / 16));
      
      // 將計算得到的 RGB 值寫入第 i 行的 LED
      myBMD26M088.writeRow(ADDRESS, i, rgb.r, rgb.g, rgb.b);
    }
  
    // 當 flow 超過 420 時，將其重置為 0，形成循環流動的效果
    if((flow++) > 420) {flow = 0;}
}

// 設定整個 RGB LED 矩陣為指定的顏色
void setRGBMatrix(rgb_def colr)
{
  for (int i=0; i < LEDLines; i++) // 遍歷每一行
  {
    // 將顏色寫入第 i 行的 LED
    myBMD26M088.writeRow(ADDRESS, i, colr.r, colr.g, colr.b);
  }
}

//-----------
// 改變整個 RGB 矩陣的顏色（參數為三個獨立的顏色值）
void ChangeBulbColor(int r,int g,int b)
{
  setRGBMatrix(RGB2Color(r,g,b)) ;
}

// 解析從序列埠收到的字串命令
// 字串格式：@RRRGGGBBB（例如 @255000000 表示紅色全亮）
// 回傳值：true 表示解析成功，false 表示失敗
boolean DecodeString(String INPStr, byte *r, byte *g , byte *b) 
{
    // 輸出原始字串以供偵錯
    Serial.print("check string:(") ;
    Serial.print(INPStr) ;
    Serial.print(")\n") ;

    int i = 0 ;
    int strsize = INPStr.length(); // 取得字串長度
    
    // 逐字元檢查
    for(i = 0 ; i < strsize ; i++)
    {
        // 輸出每個字元的索引和內容（供偵錯）
        Serial.print(i) ;
        Serial.print(":(") ;
        Serial.print(INPStr.substring(i,i+1)) ;
        Serial.print(")\n") ;

        // 尋找 '@' 符號作為命令起始標記
        if (INPStr.substring(i,i+1) == "@")
        {
            Serial.print("find @ at :(") ;
            Serial.print(i) ;
            Serial.print("/") ;
            Serial.print(strsize-i-1) ;
            Serial.print("/") ;
            Serial.print(INPStr.substring(i+1,strsize)) ;
            Serial.print(")\n") ;
            
            // 解析 @ 後面的數字：前三個字元為紅色，接著三個為綠色，最後三個為藍色
            *r = byte(INPStr.substring(i+1, i+1+3).toInt()) ;
            *g = byte(INPStr.substring(i+1+3, i+1+3+3).toInt()) ;
            *b = byte(INPStr.substring(i+1+3+3, i+1+3+3+3).toInt()) ;
            
            // 輸出解析結果（供偵錯）
            Serial.print("convert into :(") ;
            Serial.print(*r) ;
            Serial.print("/") ;
            Serial.print(*g) ;
            Serial.print("/") ;
            Serial.print(*b) ;
            Serial.print(")\n") ;
            
            return true ; // 解析成功
        }
    }
    return false ; // 未找到 '@'，解析失敗
}

// 測試燈泡：依序顯示預定義顏色陣列中的每一種顏色
void CheckLed()
{
    for(int i = 0 ; i < 16; i++) // 遍歷顏色陣列（共 16 種）
    {
        setRGBMatrix(CheckColor[i]) ; // 設定顏色
        delay(CheckColorDelayTime) ;  // 延遲一段時間
    }
}

//---------------
/*  註解掉的範例主程式碼
  // put your main code here, to run repeatedly:
  if (Serial.available() >0)
  {
    ReadStr = Serial.readStringUntil(0x23) ;    // read char # 
     //  Serial.read() ;
      DebugMsg("ReadString is :(") ;
       DebugMsg(ReadStr) ;
       DebugMsg(")\n") ;
        if (DecodeString(ReadStr,&RedValue,&GreenValue,&BlueValue) )
           {
            DebugMsgln("Change RGB Led Color") ;
            ChangeBulbColor(RedValue,GreenValue,BlueValue) ;
           }
  }
*/