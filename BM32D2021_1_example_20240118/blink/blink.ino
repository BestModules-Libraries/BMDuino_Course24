
/*************************************************
File:             blink.ino
Description:     The display lights up all blue
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
}
void loop()
{     
  //set init color from color table by index.
    
    BM32D2021_1.writeAllRGB(ADDRESS, 0xff,0,0);  
    delay(200);//delay 200ms
    BM32D2021_1.clearAll(ADDRESS);//switch off all light
    delay(200);
   
   
  /* 
   BM32D2021_1.writeRGB(ADDRESS, 3,0xff,0xff,0xff);
   delay(200);//delay 200ms
   BM32D2021_1.clearRGB(ADDRESS,3);
   delay(200);//delay 200ms 
  */

  /*
   BM32D2021_1.writeColumn(ADDRESS, 3,0xff,0,0);
   delay(200);//delay 200ms
   BM32D2021_1.clearColumn(ADDRESS,3);
   delay(200);//delay 200ms
  */
  /*
   BM32D2021_1.writeRow(ADDRESS, 4,0xff,0,0);
   delay(200);//delay 200ms
   BM32D2021_1.clearRow(ADDRESS,4);
   delay(200);//delay 200ms 
  */  
}
