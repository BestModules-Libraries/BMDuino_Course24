#include <BMD26M088.h> // 引入 BMD26M088 控制 LED 的函式庫
// BMD26M088 myBMD26M088(&Wire); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
BMD26M088 myBMD26M088(&Wire1); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
// BMD26M088 myBMD26M088(&Wire2); // 宣告使用 Wire2（I2C）通訊的 BMD26M088 物件
#define ADDRESS BMD26M088_I2C_ADDRESS_VDD // 定義 BMD26M088 的 I2C 地址
#define LEDLines 8
//  RGB Light Strength
uint8_t RGBBrightness = 0xff ;
// 定義一個 RGB 結構體，包含紅、綠、藍三個顏色的資料
typedef struct RGB_DATA
{
  uint8_t r; // 紅色值
  uint8_t g; // 綠色值
  uint8_t b; // 藍色值
} rgb_def;
int flow = 0; // 定義流動變數，控制 RGB 流動效果

//--------------------
rgb_def get_color(int param); // 函數宣告，用於根據參數計算並獲取 RGB 顏色值

void rgb_colorful(uint8_t div_num, uint8_t flow_ch, bool direction); // 用於控制 RGB 流動效果的函數宣告（未實現）
//-----------
#define CheckColorDelayTime 200
#define initDelayTime 2000
#define CommandDelay 100
// ------- 自定義函式宣告區 -----------
void initRGBLED();   //起始RGBLED模組
rgb_def get_color(int param);// 函數：根據參數計算並返回 RGB 顏色值               // 初始化整體系統
void Colorful(); //原廠測試
void setRGBMatrix(rgb_def colr); //設定RGBLED Matrix發光顏色
rgb_def RGB2Color(int r,int g,int b);  //傳入各自RGB顏色，回傳COLOR變數
void ChangeBulbColor(int r,int g,int b); //改變整個RGB Matrix顏色
boolean DecodeString(String INPStr, byte *r, byte *g , byte *b);
void CheckLed(); //測試燈泡

//-----------------------------------

//---------顏色變數
byte RedValue = 0, GreenValue = 0, BlueValue = 0;
String ReadStr = "      " ;
int delayval = 500; // delay for half a second
boolean readok = false ;
rgb_def RGBLight = {255,255,255} ;  //白光
rgb_def RGBBlank = {0,0,0} ;  //全暗
rgb_def RGBRED = {255,0,0} ;  //Red
rgb_def RGBGreen = {0,255,0} ;  //Green
rgb_def RGBBlue = {0,0,255} ;  //Blue
rgb_def CheckColor[] = {
                          {255 , 255,255} ,
                          {255 , 0,0} ,
                          {0 , 255,0} ,
                          {0 , 0,255} ,
                          {255 , 128,64} ,
                          {255 , 255,0} ,
                          {0 , 255,255} ,
                          {255 , 0,255} ,
                          {255 , 255,255} ,
                          {255 , 128,0} ,
                          {255 , 128,128} ,
                          {128 , 255,255} ,
                          {128 , 128,192} ,
                          {0 , 128,255} ,
                          {255 , 0,128} ,
                          {128 , 64,64} ,
                          {0 , 0,0} } ;
//------------------
void initRGBLED()   //起始RGBLED模組
{
    myBMD26M088.begin(ADDRESS);  // 初始化 BMD26M088 模組的 I2C 通訊
    myBMD26M088.setBrightness(ADDRESS, RGBBrightness); // 設置全域亮度，最大值為 0xff
    myBMD26M088.setCurrent(ADDRESS, BMD26M088_CCR_6MA); // 設定驅動電流為 6mA                 

}
// 函數：根據參數計算並返回 RGB 顏色值
rgb_def get_color(int param)// 函數：根據參數計算並返回 RGB 顏色值
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

rgb_def RGB2Color(int r,int g,int b)  //傳入各自RGB顏色，回傳COLOR變數
{
    rgb_def tmp;
    tmp.r = r;
    tmp.g = g ;
    tmp.b = b ;
    return tmp ;
}

void Colorful() //原廠測試
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


void setRGBMatrix(rgb_def colr) //設定RGBLED Matrix發光顏色
{
  for (int i=0; i <LEDLines;i++)
  {
    myBMD26M088.writeRow(ADDRESS, i, colr.r, colr.g, colr.b); // 更新 LED 的 RGB 顏色
  }
}
//-----------
void ChangeBulbColor(int r,int g,int b) //改變整個RGB Matrix顏色
{
  setRGBMatrix(RGB2Color(r,g,b)) ;
}

boolean DecodeString(String INPStr, byte *r, byte *g , byte *b) 
{
                    Serial.print("check string:(") ;
                    Serial.print(INPStr) ;
                         Serial.print(")\n") ;

      int i = 0 ;
      int strsize = INPStr.length();
      for(i = 0 ; i <strsize ;i++)
          {
                    Serial.print(i) ;
                    Serial.print(":(") ;
                         Serial.print(INPStr.substring(i,i+1)) ;
                    Serial.print(")\n") ;

              if (INPStr.substring(i,i+1) == "@")
                  {
                    Serial.print("find @ at :(") ;
                    Serial.print(i) ;
                         Serial.print("/") ;
                              Serial.print(strsize-i-1) ;
                         Serial.print("/") ;
                              Serial.print(INPStr.substring(i+1,strsize)) ;
                    Serial.print(")\n") ;
                      *r = byte(INPStr.substring(i+1,i+1+3).toInt()) ;
                      *g = byte(INPStr.substring(i+1+3,i+1+3+3).toInt() ) ;
                      *b = byte(INPStr.substring(i+1+3+3,i+1+3+3+3).toInt() ) ;
                       Serial.print("convert into :(") ;
                        Serial.print(*r) ;
                         Serial.print("/") ;
                        Serial.print(*g) ;
                         Serial.print("/") ;
                        Serial.print(*b) ;
                         Serial.print(")\n") ;
                            
                      return true ;
                  }
          }
    return false ;

}
void CheckLed() //測試燈泡
{
     for(int i = 0 ; i <16; i++)
        {
              setRGBMatrix(CheckColor[i]) ;
              delay(CheckColorDelayTime) ;
        }
}


//---------------
/*
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