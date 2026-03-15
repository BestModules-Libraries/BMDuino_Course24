#include <BMD26M088.h> // 引入 BMD26M088 控制 LED 的函式庫，這是一個 I2C 介面的 LED 驅動晶片

// BMD26M088 myBMD26M088(&Wire); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
BMD26M088 myBMD26M088(&Wire1); // 宣告使用 Wire1（I2C）通訊的 BMD26M088 物件
// BMD26M088 myBMD26M088(&Wire2); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件

#define ADDRESS BMD26M088_I2C_ADDRESS_VDD // 定義 BMD26M088 的 I2C 設備地址，VDD 表示使用 VDD 腳位設定地址
#define LEDLines 8  // 定義 LED 矩陣的行數，這裡設定為 8 行

//  RGB 燈光強度設定
uint8_t RGBBrightness = 0xff ;  // 設定 RGB LED 的整體亮度，0xff 表示最大亮度（255）

// 定義一個 RGB 結構體，包含紅、綠、藍三個顏色的資料
typedef struct RGB_DATA
{
  uint8_t r; // 紅色值（0-255）
  uint8_t g; // 綠色值（0-255）
  uint8_t b; // 藍色值（0-255）
} rgb_def;

int flow = 0; // 定義流動變數，用於控制 RGB 色彩循環流動效果的位置計數器

//--------------------
rgb_def get_color(int param); // 函數宣告：根據參數計算並獲取 RGB 顏色值

void rgb_colorful(uint8_t div_num, uint8_t flow_ch, bool direction); // 函數宣告：用於控制 RGB 流動效果的函數（此處未實作）

//-----------
#define CheckColorDelayTime 200   // 測試模式時，每個顏色顯示的延遲時間（毫秒）
#define initDelayTime 2000        // 初始化完成後的延遲時間（毫秒）
#define CommandDelay 100          // 指令執行間的延遲時間（毫秒）

// ------- 自定義函式宣告區 -----------
void initRGBLED();   // 初始化 RGB LED 模組
rgb_def get_color(int param);  // 根據參數計算並返回 RGB 顏色值
void Colorful(); // 原廠提供的色彩流動測試函數
void setRGBMatrix(rgb_def colr); // 設定整個 RGB LED 矩陣的發光顏色
rgb_def RGB2Color(int r,int g,int b);  // 將個別的 RGB 數值轉換為 RGB 結構變數
void ChangeBulbColor(int r,int g,int b); // 改變整個 RGB 矩陣的顏色
boolean DecodeString(String INPStr, byte *r, byte *g , byte *b); // 解析從序列埠接收的字串，提取 RGB 數值
void CheckLed(); // 測試燈泡功能，依序顯示預設的顏色

//-----------------------------------

//---------顏色變數定義區
byte RedValue = 0, GreenValue = 0, BlueValue = 0;  // 儲存解析後的 RGB 數值
String ReadStr = "      " ;  // 儲存從序列埠讀取的字串
int delayval = 500; // 延遲時間設定為 500 毫秒
boolean readok = false ;  // 標記是否成功讀取到有效指令

// 預先定義常用顏色
rgb_def RGBLight = {255,255,255} ;  // 白光（RGB 全亮）
rgb_def RGBBlank = {0,0,0} ;        // 全暗（關閉所有 LED）
rgb_def RGBRED = {255,0,0} ;        // 紅色
rgb_def RGBGreen = {0,255,0} ;      // 綠色
rgb_def RGBBlue = {0,0,255} ;       // 藍色

// 預設的顏色測試序列，包含多種顏色的組合
rgb_def CheckColor[] = {
                          {255 , 255,255} ,  // 白光
                          {255 , 0,0} ,      // 紅
                          {0 , 255,0} ,      // 綠
                          {0 , 0,255} ,      // 藍
                          {255 , 128,64} ,   // 橙紅色
                          {255 , 255,0} ,    // 黃
                          {0 , 255,255} ,    // 青
                          {255 , 0,255} ,    // 紫
                          {255 , 255,255} ,  // 白
                          {255 , 128,0} ,    // 橙
                          {255 , 128,128} ,  // 粉紅
                          {128 , 255,255} ,  // 淺青
                          {128 , 128,192} ,  // 淺紫
                          {0 , 128,255} ,    // 天藍
                          {255 , 0,128} ,    // 桃紅
                          {128 , 64,64} ,    // 棕
                          {0 , 0,0} } ;      // 黑（關閉）

//------------------
/**
 * 初始化 RGB LED 模組
 * 設定 I2C 通訊、整體亮度和驅動電流
 */
void initRGBLED()   //起始RGBLED模組
{
    myBMD26M088.begin(ADDRESS);  // 初始化 BMD26M088 模組的 I2C 通訊，使用指定的地址
    myBMD26M088.setBrightness(ADDRESS, RGBBrightness); // 設置全域亮度，最大值為 0xff（255）
    myBMD26M088.setCurrent(ADDRESS, BMD26M088_CCR_6MA); // 設定驅動電流為 6mA，可保護 LED 避免過電流                 
}

/**
 * 根據參數計算並返回 RGB 顏色值
 * 這是一個色彩循環函數，將 0-419 的參數映射到完整的色環上
 * @param param 色彩位置參數（0-419）
 * @return 對應的 RGB 顏色結構
 */
rgb_def get_color(int param)// 函數：根據參數計算並返回 RGB 顏色值
{
  rgb_def rgb; // 定義一個 RGB 結構變數
  
  if(param < 0) { param += 420; } // 如果 param 小於 0，將其加回 420（處理負數情況）
  if(param > 419) { param -= 420; } // 如果 param 大於 419，將其減去 420（確保在有效範圍內）

  // 根據不同的 param 值範圍計算 RGB 顏色值，形成連續的色彩循環
  if(param < 60)  // 步驟1: 紅色 -> 黃色（紅色漸強，綠色漸增）
  {
    rgb.r = 240;
    rgb.g = 4 * param;
    rgb.b = 0;
  }
  else if(param < 120) // 步驟2: 黃色 -> 綠色（紅色漸減，綠色保持）
  {
    rgb.r = 240 - (4 * (param - 60));
    rgb.g = 240;
    rgb.b = 0;
  }
  else if(param < 180) // 步驟3: 綠色 -> 青色（綠色保持，藍色漸增）
  {
    rgb.r = 0;
    rgb.g = 240;
    rgb.b = 4 * (param - 120);
  }
  else if(param < 240) // 步驟4: 青色接近藍色（綠色漸減，藍色保持）
  {
    rgb.r = 0;
    rgb.g = 240 - (3 * (param - 180));
    rgb.b = 240;
  }
  else if(param < 300) // 步驟5: 藍色 -> 紫色（紅色漸增，綠色漸減，藍色漸減）
  {
    rgb.r = (param - 240);
    rgb.g = 60 - (param - 240);
    rgb.b = 240 - (2 * (param - 240));
  }
  else if(param < 360) // 步驟6: 紫色 -> 粉紅色（紅色漸增，藍色保持）
  {
    rgb.r = 60 + (param - 300);
    rgb.g = 0;
    rgb.b = 120;
  }
  else if(param < 420) // 步驟7: 粉紅色 -> 紅色（紅色漸增，藍色漸減）
  {
    rgb.r = 120 + (2 * (param - 360));
    rgb.g = 0;
    rgb.b = 120 - (2 * (param - 360));
  }

  return rgb; // 返回計算出的 RGB 顏色值
}

/**
 * 將個別的 RGB 數值轉換為 RGB 結構變數
 * @param r 紅色數值（0-255）
 * @param g 綠色數值（0-255）
 * @param b 藍色數值（0-255）
 * @return 包含 RGB 數值的結構
 */
rgb_def RGB2Color(int r,int g,int b)  //傳入各自RGB顏色，回傳COLOR變數
{
    rgb_def tmp;
    tmp.r = r;
    tmp.g = g ;
    tmp.b = b ;
    return tmp ;
}

/**
 * 原廠提供的色彩流動測試函數
 * 會根據 flow 值產生漸變的彩虹效果，並顯示在 8 行 LED 上
 */
void Colorful() //原廠測試
{
      u8 i; // 宣告迴圈變數（無符號 8 位元整數）
    rgb_def rgb; // 宣告一個 RGB 結構變數
    
    for(i = 0; i < 8; i++) // 迴圈更新 8 行 LED 的顏色
    {
      // 計算每一行 LED 的顏色，根據 flow 值進行流動效果
      // 將 420 的色域範圍分成 16 份，讓每一行顯示不同的色相偏移
      rgb = get_color(flow + i * (420 / 16)); // 420 是色彩範圍，分成 16 份
      
      // 將計算得到的 RGB 值寫入第 i 行的 LED
      myBMD26M088.writeRow(ADDRESS, i, rgb.r, rgb.g, rgb.b); // 更新 LED 的 RGB 顏色
    }
  
    // 當 flow 超過 420 時，將其重置為 0，形成循環流動的效果
    if((flow++) > 420) {flow = 0;} // 控制 RGB 顏色的流動        
}

/**
 * 設定整個 RGB LED 矩陣為單一顏色
 * @param colr 要顯示的 RGB 顏色結構
 */
void setRGBMatrix(rgb_def colr) //設定RGBLED Matrix發光顏色
{
  for (int i=0; i <LEDLines;i++)  // 遍歷每一行 LED
  {
    myBMD26M088.writeRow(ADDRESS, i, colr.r, colr.g, colr.b); // 將該行設定為指定顏色
  }
}

//-----------
/**
 * 改變整個 RGB 矩陣的顏色（使用個別 RGB 數值）
 * @param r 紅色數值
 * @param g 綠色數值
 * @param b 藍色數值
 */
void ChangeBulbColor(int r,int g,int b) //改變整個RGB Matrix顏色
{
  setRGBMatrix(RGB2Color(r,g,b)) ;  // 先轉換為結構，再設定矩陣顏色
}

/**
 * 解析從序列埠接收的字串，提取 RGB 數值
 * 格式：@RRRGGGBBB，其中 RRR、GGG、BBB 為三位數的數字（如 @255128064）
 * @param INPStr 輸入字串
 * @param r 指向儲存紅色數值的指標
 * @param g 指向儲存綠色數值的指標
 * @param b 指向儲存藍色數值的指標
 * @return 解析成功返回 true，失敗返回 false
 */
boolean DecodeString(String INPStr, byte *r, byte *g , byte *b) 
{
    // 輸出除錯訊息，顯示正在檢查的字串
    Serial.print("check string:(") ;
    Serial.print(INPStr) ;
    Serial.print(")\n") ;

    int i = 0 ;
    int strsize = INPStr.length();  // 取得字串長度
    
    for(i = 0 ; i <strsize ;i++)
    {
        // 輸出每個字元的索引和內容（除錯用）
        Serial.print(i) ;
        Serial.print(":(") ;
        Serial.print(INPStr.substring(i,i+1)) ;
        Serial.print(")\n") ;

        // 尋找 '@' 符號作為指令起始標記
        if (INPStr.substring(i,i+1) == "@")
        {
            Serial.print("find @ at :(") ;
            Serial.print(i) ;
            Serial.print("/") ;
            Serial.print(strsize-i-1) ;
            Serial.print("/") ;
            Serial.print(INPStr.substring(i+1,strsize)) ;
            Serial.print(")\n") ;
            
            // 從 '@' 後面的字串中提取 RGB 數值
            // 格式為 @RRRGGGBBB，每個顏色佔 3 個字元
            *r = byte(INPStr.substring(i+1,i+1+3).toInt()) ;  // 提取紅色數值
            *g = byte(INPStr.substring(i+1+3,i+1+3+3).toInt()) ;  // 提取綠色數值
            *b = byte(INPStr.substring(i+1+3+3,i+1+3+3+3).toInt()) ;  // 提取藍色數值
            
            // 輸出解析結果（除錯用）
            Serial.print("convert into :(") ;
            Serial.print(*r) ;
            Serial.print("/") ;
            Serial.print(*g) ;
            Serial.print("/") ;
            Serial.print(*b) ;
            Serial.print(")\n") ;
                
            return true ;  // 解析成功
        }
    }
    return false ;  // 未找到有效的指令格式
}

/**
 * 測試燈泡功能
 * 依序顯示預設的顏色序列，每個顏色顯示一段時間
 */
void CheckLed() //測試燈泡
{
     for(int i = 0 ; i <16; i++)  // 遍歷預設的 16 個顏色
     {
          setRGBMatrix(CheckColor[i]) ;  // 設定矩陣顏色
          delay(CheckColorDelayTime) ;   // 等待一段時間，讓顏色可見
     }
}


//---------------
/*
  以下為被註解掉的主程式碼片段，原用途為從序列埠讀取指令並控制 LED
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