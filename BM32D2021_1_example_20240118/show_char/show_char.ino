/*************************************************
File:             show_char.ino
Description:  show_char on matrix.It will auto diplay after set ASCII char 
Note:               
**************************************************/
#include <BM32D2021_1.h>
                                    
#define ADDRESS  BM32D2021_1_I2C_ADDRESS_SCL  //Set the I2C address of the module and turn on the SCL pin with the dial switch.

BM32D2021_1 BM32D2021_1(&Wire2);//BMCOM2 is use I2C2

void setup() 
{                                     
  BM32D2021_1.begin(ADDRESS);  //I2C init 
  BM32D2021_1.setBrightness(ADDRESS,0xff);//set global brightness,max
  BM32D2021_1.setCurrent(ADDRESS,BM32D2021_1_CCR_6MA);//set drive current 6mA     
}

uint8_t show_index = 0;
uint8_t color_index = 0;//red
const char char_table[]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r',
's','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I'
,'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
void loop()
{
    BM32D2021_1.clearAll(ADDRESS);
    BM32D2021_1.DrawAsciiChar(ADDRESS,char_table[show_index],0xff,0x00,0x00);  
    show_index++;
    if(show_index>=sizeof(char_table))show_index=0;
    delay(1000);
}
