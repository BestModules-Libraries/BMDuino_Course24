
/*************************************************
File:            colorful.ino
Description:     This example display the colorful flowing effect.
Note:               
**************************************************/
#include <BM32D2021_1.h>
BM32D2021_1 BM32D2021_1(&Wire2);
#define ADDRESS BM32D2021_1_I2C_ADDRESS_SCL

typedef struct RGB_DATA
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}rgb_def;

rgb_def get_color(int param);

void rgb_colorful(uint8_t div_num, uint8_t flow_ch, bool direction);

void setup() 
{                                  
    BM32D2021_1.begin(ADDRESS);  //I2C init 
    BM32D2021_1.setBrightness(ADDRESS,0xff);//set global brightness,max
    BM32D2021_1.setCurrent(ADDRESS,BM32D2021_1_CCR_6MA);//set drive current 6mA                 
}
int flow = 0;
void loop()
{
    u8 i;
    rgb_def rgb;
 
    
    for(i = 0; i < 8; i++)
    {

      rgb = get_color(flow + i * (420 / 16));//get color to update RGB
      
      BM32D2021_1.writeRow(ADDRESS, i, rgb.r, rgb.g, rgb.b);//update a row RGB
    }
  
      if((flow++) > 420) {flow = 0;}//flowing        
}

rgb_def get_color(int param)//Change color through calculation
{
  rgb_def rgb;
  
  if(param<0){param+=420;}
  if(param>419){param-=420;}

  if(param<60)  //step 1:red->yellow
  {
    rgb.r=240;
    rgb.g=4 * param;
    rgb.b=0;
  }
  else if(param<120)//step 2:yellow->green
  {
    rgb.r=240-(4*(param-60));
    rgb.g=240;
    rgb.b=0;
  }
  else if(param<180)//step 3:green->cyan
  {
    rgb.r=0;
    rgb.g=240;
    rgb.b=4*(param-120);
  }
  else if(param<240)//step 4:cyan close to blue
  {
    rgb.r=0;
    rgb.g=240-(3*(param-180));
    rgb.b=240;
  }
  else if(param<300)//step 5:blue->purple
  {
    rgb.r=(param-240);
    rgb.g=60-(param-240);
    rgb.b=240-(2*(param-240));
  }
  else if(param<360)//step6:purple->pink
  {
    rgb.r=60+(param-300);
    rgb.g=0;
    rgb.b=120;
  }
  else if(param<420)//step7:pink->red
  {
    rgb.r=120+(2*(param-360));
    rgb.g=0;
    rgb.b=120-(2*(param-360));
  }

  return rgb;   
}
