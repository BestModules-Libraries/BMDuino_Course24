/*************************************************
File:             breath.ino
Description:      The display lights up all blue, and the lights turn on and off
Note:               
**************************************************/
#include <BM32D2021_1.h>
#define ADDRESS BM32D2021_1_I2C_ADDRESS_SCL    

BM32D2021_1 BM32D2021_1(&Wire2);//BMCOM2 is use I2C2

void setup() 
{                                  
 BM32D2021_1.begin(ADDRESS);  //I2C init            
 BM32D2021_1.writeAllRGB(ADDRESS, 0xff,0,0);  //set red  
 BM32D2021_1.setBrightness(ADDRESS,0xff);//set global brightness,max
 BM32D2021_1.setCurrent(ADDRESS,BM32D2021_1_CCR_6MA);//set drive current 6mA            
}
uint8_t fall_rise_flag = 0;
uint8_t breath_time = 20;
uint8_t breath_count = 0;

void loop()
{  
  if(fall_rise_flag==0)
  {
    if(breath_count<255){
      breath_count++;
    }
    else{
      fall_rise_flag=1;
    }
     BM32D2021_1.setBrightness(ADDRESS, breath_count);  
  }
  else
  { 
    if(breath_count>0){
      breath_count--;
    }
    else{
      fall_rise_flag=0;
    }
    BM32D2021_1.setBrightness(ADDRESS, breath_count); 
  }
  delay(breath_time);
}
