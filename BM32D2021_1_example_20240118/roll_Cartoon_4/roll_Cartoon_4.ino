/*************************************************
File:             roll_Cartoon_4.ino
Description:   A set of pictures scrolls horizontally
Note:               
**************************************************/
#include <BM32D2021_1.h>
                                    
#define i2c_addr_boardcast BM32D2021_1_I2C_ADDRESS_BOARDCAST
#define i2c_addr_board1 BM32D2021_1_I2C_ADDRESS_VDD
#define i2c_addr_board2 BM32D2021_1_I2C_ADDRESS_GND
#define i2c_addr_board3 BM32D2021_1_I2C_ADDRESS_SCL
#define i2c_addr_board4 BM32D2021_1_I2C_ADDRESS_SDA
BM32D2021_1 BM32D2021_1(&Wire2);//BMCOM2 is use I2C2
void array1(uint8_t address,uint8_t address1,uint8_t address2,uint8_t address3)
{ 
    for(uint8_t k = 0;k <32; k++)//The total length of the picture is 4x8=32 RGB LEDs
  {
    //第一张图
    for(uint8_t j = 0;(j<16); j++)//The number of lights in 4 RGB LED
    {
    for(uint8_t i = 0; i < 16; i++)//Divide the picture into four quadrants
    {
      if((j <= k)&(j+8>k))
     {
      if((i<8))//The first quadrant is displayed
      {
        //Horizontal displacement of 7 RGB LEDs and line scan 7 displacements
        //Displays the image data stored in the array 
        BM32D2021_1.writeRGB(i2c_addr_board2, (7-i)*8+j+7-k, ASCII_8X8_4[3*(16*i+j)+2],ASCII_8X8_4[3*(16*i+j)+1],ASCII_8X8_4[3*(16*i+j)]); 
      }
      if((i>=8))//The third quadrant is displayed
      {
       //Horizontal displacement of 7 RGB LEDs and line scan 15 displacements
       //Displays the image data stored in the array
       BM32D2021_1.writeRGB(i2c_addr_board1, (15-i) * 8 + j+7-k, ASCII_8X8_4[3*(16*i+j)+2],ASCII_8X8_4[3*(16*i+j)+1],ASCII_8X8_4[3*(16*i+j)]);
      }
     }
     if((k>8)&(j+8<=k)&(j+16>k))
     {
      if((i<8))//The second quadrant is displayed
      {
        //Horizontal displacement of 7 RGB LEDs and line scan 7 displacements
        //Displays the image data stored in the array
        BM32D2021_1.writeRGB(i2c_addr_board4, (8-i)*8+j+7-k, ASCII_8X8_4[3*(16*i+j)+2],ASCII_8X8_4[3*(16*i+j)+1],ASCII_8X8_4[3*(16*i+j)]); 
      }
     
      if((i>=8))//The fourth quadrant is displayed
      {
       //Horizontal displacement of 7 RGB LEDs and line scan 15 displacements
       //Displays the image data stored in the array
       BM32D2021_1.writeRGB(i2c_addr_board3, (16-i) * 8 + j+7-k, ASCII_8X8_4[3*(16*i+j)+2],ASCII_8X8_4[3*(16*i+j)+1],ASCII_8X8_4[3*(16*i+j)]);
      }
     }
      
     }
    }
    delay(100);
  }
  //第二张图
  for(uint8_t k = 0;k <32; k++)
  {
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      if((j <= k)&(j+8>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (7-i)*8+j+7-k, ASCII_8X8_7[3*(16*i+j)+2],ASCII_8X8_7[3*(16*i+j)+1],ASCII_8X8_7[3*(16*i+j)]); 
      }
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (15-i) * 8 + j+7-k, ASCII_8X8_7[3*(16*i+j)+2],ASCII_8X8_7[3*(16*i+j)+1],ASCII_8X8_7[3*(16*i+j)]);
      }
     }
     if((k>8)&(j+8<=k)&(j+16>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (8-i)*8+j+7-k, ASCII_8X8_7[3*(16*i+j)+2],ASCII_8X8_7[3*(16*i+j)+1],ASCII_8X8_7[3*(16*i+j)]); 
      }
     
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (16-i) * 8 + j+7-k, ASCII_8X8_7[3*(16*i+j)+2],ASCII_8X8_7[3*(16*i+j)+1],ASCII_8X8_7[3*(16*i+j)]);
      }
     }
      
     }
    }
    delay(100);  
  }
  //第三张图
    for(uint8_t k = 0;k <32; k++)
  {
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      if((j <= k)&(j+8>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (7-i)*8+j+7-k, ASCII_8X8_8[3*(16*i+j)+2],ASCII_8X8_8[3*(16*i+j)+1],ASCII_8X8_8[3*(16*i+j)]); 
      }
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (15-i) * 8 + j+7-k, ASCII_8X8_8[3*(16*i+j)+2],ASCII_8X8_8[3*(16*i+j)+1],ASCII_8X8_8[3*(16*i+j)]);
      }
     }
     if((k>8)&(j+8<=k)&(j+16>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (8-i)*8+j+7-k, ASCII_8X8_8[3*(16*i+j)+2],ASCII_8X8_8[3*(16*i+j)+1],ASCII_8X8_8[3*(16*i+j)]); 
      }
     
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (16-i) * 8 + j+7-k, ASCII_8X8_8[3*(16*i+j)+2],ASCII_8X8_8[3*(16*i+j)+1],ASCII_8X8_8[3*(16*i+j)]);
      }
     }
      
     }
    }
    delay(100);  
  }
  //第四张图
    for(uint8_t k = 0;k <32; k++)
  {
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      if((j <= k)&(j+8>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (7-i)*8+j+7-k, ASCII_8X8_9[3*(16*i+j)+2],ASCII_8X8_9[3*(16*i+j)+1],ASCII_8X8_9[3*(16*i+j)]); 
      }
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (15-i) * 8 + j+7-k, ASCII_8X8_9[3*(16*i+j)+2],ASCII_8X8_9[3*(16*i+j)+1],ASCII_8X8_9[3*(16*i+j)]);
      }
     }
     if((k>8)&(j+8<=k)&(j+16>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (8-i)*8+j+7-k, ASCII_8X8_9[3*(16*i+j)+2],ASCII_8X8_9[3*(16*i+j)+1],ASCII_8X8_9[3*(16*i+j)]); 
      }
     
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (16-i) * 8 + j+7-k, ASCII_8X8_9[3*(16*i+j)+2],ASCII_8X8_9[3*(16*i+j)+1],ASCII_8X8_9[3*(16*i+j)]);
      }
     }
      
     }
    }
    delay(100);  
  }
  //第五张图
    for(uint8_t k = 0;k <32; k++)
  {
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      if((j <= k)&(j+8>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (7-i)*8+j+7-k, ASCII_8X8_10[3*(16*i+j)+2],ASCII_8X8_10[3*(16*i+j)+1],ASCII_8X8_10[3*(16*i+j)]); 
      }
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (15-i) * 8 + j+7-k, ASCII_8X8_10[3*(16*i+j)+2],ASCII_8X8_10[3*(16*i+j)+1],ASCII_8X8_10[3*(16*i+j)]);
      }
     }
     if((k>8)&(j+8<=k)&(j+16>k))
     {
      if((i<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (8-i)*8+j+7-k, ASCII_8X8_10[3*(16*i+j)+2],ASCII_8X8_10[3*(16*i+j)+1],ASCII_8X8_10[3*(16*i+j)]); 
      }
     
      if((i>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (16-i) * 8 + j+7-k, ASCII_8X8_10[3*(16*i+j)+2],ASCII_8X8_10[3*(16*i+j)+1],ASCII_8X8_10[3*(16*i+j)]);
      }
     }
      
     }
    }
    delay(100);  
  }
}



void setup() 
{                                     
  BM32D2021_1.begin(i2c_addr_boardcast);  //I2C init 
  BM32D2021_1.setBrightness(i2c_addr_boardcast,0xff);//set global brightness,max
  BM32D2021_1.setCurrent(i2c_addr_boardcast,BM32D2021_1_CCR_6MA);//set drive current 6mA     
  
}

void loop()
{
  array1(i2c_addr_board3,i2c_addr_board4,i2c_addr_board1,i2c_addr_board2);
}
