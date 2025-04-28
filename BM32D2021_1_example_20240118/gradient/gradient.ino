
/*************************************************
File: gradient.ino
Description:set globe gradient control
Note:               
**************************************************/
#include <BM32D2021_1.h>
BM32D2021_1 BM32D2021_1(&Wire2);
#define ADDRESS BM32D2021_1_I2C_ADDRESS_SCL

void setup() 
{
  BM32D2021_1.begin(ADDRESS);  //I2C init 
  BM32D2021_1.setBrightness(ADDRESS,0xff);//set global brightness,max
  BM32D2021_1.setCurrent(ADDRESS,BM32D2021_1_CCR_6MA);//set drive current 6mA 

  //gradient control
  BM32D2021_1.writeAllRGB(ADDRESS, 0xff, 0, 0);//set all RGB red gradient
  //myBMD26M088.writeRGB(ADDRESS, 1,0xff, 0, 0);//set one RGB red gradient
  //set globe gradient control
  BM32D2021_1.setGradient(ADDRESS,GFS_GRADIENT_GARMA,GFT_T1_1024_FRAME);
  
}   
void loop()
{
 
}
