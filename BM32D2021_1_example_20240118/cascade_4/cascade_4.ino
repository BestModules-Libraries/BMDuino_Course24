
/*************************************************
File:             BM32D2021_1_Cascade4_example.ino
Description:    Cascade 4 module on bus  
Note:               
**************************************************/
#include "BM32D2021_1.h"


BM32D2021_1 BM32D2021_1(&Wire2); //BMCOM2 is use I2C2


#define i2c_addr_boardcast BM32D2021_1_I2C_ADDRESS_BOARDCAST
#define i2c_addr_board1 BM32D2021_1_I2C_ADDRESS_VDD
#define i2c_addr_board2 BM32D2021_1_I2C_ADDRESS_GND
#define i2c_addr_board3 BM32D2021_1_I2C_ADDRESS_SCL
#define i2c_addr_board4 BM32D2021_1_I2C_ADDRESS_SDA

void setup() 
{                                           
    BM32D2021_1.begin(i2c_addr_boardcast);  //I2C init    
    BM32D2021_1.setBrightness(i2c_addr_boardcast,0xff);//set global brightness,max
    BM32D2021_1.setCurrent(i2c_addr_boardcast,BM32D2021_1_CCR_6MA);//set drive current 6mA   

    BM32D2021_1.writeAllRGB(i2c_addr_board1, 0xff,0,0);     
    BM32D2021_1.writeAllRGB(i2c_addr_board2, 0,0xff,0); 
    BM32D2021_1.writeAllRGB(i2c_addr_board3, 0,0,0xff);       
    BM32D2021_1.writeAllRGB(i2c_addr_board4, 0xff,0xff,0xff);                    
}
void loop()
{
                                        
}
