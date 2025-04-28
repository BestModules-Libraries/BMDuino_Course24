/*************************************************
File:             show_char.ino
Description:  show_char on matrix.It will auto diplay after set ASCII char 
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
    //BM32D2021_1.writeRGB(address1, 0, 0XFF,0,0);
     
    
  
  for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)//Divide the picture into four quadrants
    {
      // line scan 6 displacements of 7 RGB LEDs
      //Displays the image data stored in the array
      if((i<8)&&(j>=8))//The first quadrant is displayed
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_90[3*(16*i+j)+2],ASCII_8X8_90[3*(16*i+j)+1],ASCII_8X8_90[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))//The third quadrant is displayed
      {
       // line scan 14 displacements of 7 RGB LEDs
       //Displays the image data stored in the array
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_90[3*(16*i+j)+2],ASCII_8X8_90[3*(16*i+j)+1],ASCII_8X8_90[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))//The second quadrant is displayed
      {
        // line scan 7 displacements of 7 RGB LEDs
        //Displays the image data stored in the array
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_90[3*(16*i+j)+2],ASCII_8X8_90[3*(16*i+j)+1],ASCII_8X8_90[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))//The fourth quadrant is displayed
      {
       // line scan 15 displacements of 7 RGB LEDs
       //Displays the image data stored in the array 
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_90[3*(16*i+j)+2],ASCII_8X8_90[3*(16*i+j)+1],ASCII_8X8_90[3*(16*i+j)]);
      }
     }
    }
    delay(50);
  //第二张图
  for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_91[3*(16*i+j)+2],ASCII_8X8_91[3*(16*i+j)+1],ASCII_8X8_91[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_91[3*(16*i+j)+2],ASCII_8X8_91[3*(16*i+j)+1],ASCII_8X8_91[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_91[3*(16*i+j)+2],ASCII_8X8_91[3*(16*i+j)+1],ASCII_8X8_91[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_91[3*(16*i+j)+2],ASCII_8X8_91[3*(16*i+j)+1],ASCII_8X8_91[3*(16*i+j)]);
      }
     }
    }
    delay(50);
  //第三张图
  for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_92[3*(16*i+j)+2],ASCII_8X8_92[3*(16*i+j)+1],ASCII_8X8_92[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_92[3*(16*i+j)+2],ASCII_8X8_92[3*(16*i+j)+1],ASCII_8X8_92[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_92[3*(16*i+j)+2],ASCII_8X8_92[3*(16*i+j)+1],ASCII_8X8_92[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_92[3*(16*i+j)+2],ASCII_8X8_92[3*(16*i+j)+1],ASCII_8X8_92[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第四张图
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_93[3*(16*i+j)+2],ASCII_8X8_93[3*(16*i+j)+1],ASCII_8X8_93[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_93[3*(16*i+j)+2],ASCII_8X8_93[3*(16*i+j)+1],ASCII_8X8_93[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_93[3*(16*i+j)+2],ASCII_8X8_93[3*(16*i+j)+1],ASCII_8X8_93[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_93[3*(16*i+j)+2],ASCII_8X8_93[3*(16*i+j)+1],ASCII_8X8_93[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第五张图
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_94[3*(16*i+j)+2],ASCII_8X8_94[3*(16*i+j)+1],ASCII_8X8_94[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_94[3*(16*i+j)+2],ASCII_8X8_94[3*(16*i+j)+1],ASCII_8X8_94[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_94[3*(16*i+j)+2],ASCII_8X8_94[3*(16*i+j)+1],ASCII_8X8_94[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_94[3*(16*i+j)+2],ASCII_8X8_94[3*(16*i+j)+1],ASCII_8X8_94[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第六张图
     for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_95[3*(16*i+j)+2],ASCII_8X8_95[3*(16*i+j)+1],ASCII_8X8_95[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_95[3*(16*i+j)+2],ASCII_8X8_95[3*(16*i+j)+1],ASCII_8X8_95[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_95[3*(16*i+j)+2],ASCII_8X8_95[3*(16*i+j)+1],ASCII_8X8_95[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_95[3*(16*i+j)+2],ASCII_8X8_95[3*(16*i+j)+1],ASCII_8X8_95[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第七张图
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_96[3*(16*i+j)+2],ASCII_8X8_96[3*(16*i+j)+1],ASCII_8X8_96[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_96[3*(16*i+j)+2],ASCII_8X8_96[3*(16*i+j)+1],ASCII_8X8_96[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_96[3*(16*i+j)+2],ASCII_8X8_96[3*(16*i+j)+1],ASCII_8X8_96[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_96[3*(16*i+j)+2],ASCII_8X8_96[3*(16*i+j)+1],ASCII_8X8_96[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第八张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_97[3*(16*i+j)+2],ASCII_8X8_97[3*(16*i+j)+1],ASCII_8X8_97[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_97[3*(16*i+j)+2],ASCII_8X8_97[3*(16*i+j)+1],ASCII_8X8_97[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_97[3*(16*i+j)+2],ASCII_8X8_97[3*(16*i+j)+1],ASCII_8X8_97[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_97[3*(16*i+j)+2],ASCII_8X8_97[3*(16*i+j)+1],ASCII_8X8_97[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第九张图
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_98[3*(16*i+j)+2],ASCII_8X8_98[3*(16*i+j)+1],ASCII_8X8_98[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_98[3*(16*i+j)+2],ASCII_8X8_98[3*(16*i+j)+1],ASCII_8X8_98[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_98[3*(16*i+j)+2],ASCII_8X8_98[3*(16*i+j)+1],ASCII_8X8_98[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_98[3*(16*i+j)+2],ASCII_8X8_98[3*(16*i+j)+1],ASCII_8X8_98[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十张图
      for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_99[3*(16*i+j)+2],ASCII_8X8_99[3*(16*i+j)+1],ASCII_8X8_99[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_99[3*(16*i+j)+2],ASCII_8X8_99[3*(16*i+j)+1],ASCII_8X8_99[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_99[3*(16*i+j)+2],ASCII_8X8_99[3*(16*i+j)+1],ASCII_8X8_99[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_99[3*(16*i+j)+2],ASCII_8X8_99[3*(16*i+j)+1],ASCII_8X8_99[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十一张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_100[3*(16*i+j)+2],ASCII_8X8_100[3*(16*i+j)+1],ASCII_8X8_100[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_100[3*(16*i+j)+2],ASCII_8X8_100[3*(16*i+j)+1],ASCII_8X8_100[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_100[3*(16*i+j)+2],ASCII_8X8_100[3*(16*i+j)+1],ASCII_8X8_100[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_100[3*(16*i+j)+2],ASCII_8X8_100[3*(16*i+j)+1],ASCII_8X8_100[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十二张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_101[3*(16*i+j)+2],ASCII_8X8_101[3*(16*i+j)+1],ASCII_8X8_101[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_101[3*(16*i+j)+2],ASCII_8X8_101[3*(16*i+j)+1],ASCII_8X8_101[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_101[3*(16*i+j)+2],ASCII_8X8_101[3*(16*i+j)+1],ASCII_8X8_101[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_101[3*(16*i+j)+2],ASCII_8X8_101[3*(16*i+j)+1],ASCII_8X8_101[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十三张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_102[3*(16*i+j)+2],ASCII_8X8_102[3*(16*i+j)+1],ASCII_8X8_102[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_102[3*(16*i+j)+2],ASCII_8X8_102[3*(16*i+j)+1],ASCII_8X8_102[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_102[3*(16*i+j)+2],ASCII_8X8_102[3*(16*i+j)+1],ASCII_8X8_102[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_102[3*(16*i+j)+2],ASCII_8X8_102[3*(16*i+j)+1],ASCII_8X8_102[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十四张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_103[3*(16*i+j)+2],ASCII_8X8_103[3*(16*i+j)+1],ASCII_8X8_103[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_103[3*(16*i+j)+2],ASCII_8X8_103[3*(16*i+j)+1],ASCII_8X8_103[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_103[3*(16*i+j)+2],ASCII_8X8_103[3*(16*i+j)+1],ASCII_8X8_103[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_103[3*(16*i+j)+2],ASCII_8X8_103[3*(16*i+j)+1],ASCII_8X8_103[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十五张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_104[3*(16*i+j)+2],ASCII_8X8_104[3*(16*i+j)+1],ASCII_8X8_104[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_104[3*(16*i+j)+2],ASCII_8X8_104[3*(16*i+j)+1],ASCII_8X8_104[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_104[3*(16*i+j)+2],ASCII_8X8_104[3*(16*i+j)+1],ASCII_8X8_104[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_104[3*(16*i+j)+2],ASCII_8X8_104[3*(16*i+j)+1],ASCII_8X8_104[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十六张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_105[3*(16*i+j)+2],ASCII_8X8_105[3*(16*i+j)+1],ASCII_8X8_105[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_105[3*(16*i+j)+2],ASCII_8X8_105[3*(16*i+j)+1],ASCII_8X8_105[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_105[3*(16*i+j)+2],ASCII_8X8_105[3*(16*i+j)+1],ASCII_8X8_105[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_105[3*(16*i+j)+2],ASCII_8X8_105[3*(16*i+j)+1],ASCII_8X8_105[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十七张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_106[3*(16*i+j)+2],ASCII_8X8_106[3*(16*i+j)+1],ASCII_8X8_106[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_106[3*(16*i+j)+2],ASCII_8X8_106[3*(16*i+j)+1],ASCII_8X8_106[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_106[3*(16*i+j)+2],ASCII_8X8_106[3*(16*i+j)+1],ASCII_8X8_106[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_106[3*(16*i+j)+2],ASCII_8X8_106[3*(16*i+j)+1],ASCII_8X8_106[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十八张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_107[3*(16*i+j)+2],ASCII_8X8_107[3*(16*i+j)+1],ASCII_8X8_107[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_107[3*(16*i+j)+2],ASCII_8X8_107[3*(16*i+j)+1],ASCII_8X8_107[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_107[3*(16*i+j)+2],ASCII_8X8_107[3*(16*i+j)+1],ASCII_8X8_107[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_107[3*(16*i+j)+2],ASCII_8X8_107[3*(16*i+j)+1],ASCII_8X8_107[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第十九张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_108[3*(16*i+j)+2],ASCII_8X8_108[3*(16*i+j)+1],ASCII_8X8_108[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_108[3*(16*i+j)+2],ASCII_8X8_108[3*(16*i+j)+1],ASCII_8X8_108[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_108[3*(16*i+j)+2],ASCII_8X8_108[3*(16*i+j)+1],ASCII_8X8_108[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_108[3*(16*i+j)+2],ASCII_8X8_108[3*(16*i+j)+1],ASCII_8X8_108[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_109[3*(16*i+j)+2],ASCII_8X8_109[3*(16*i+j)+1],ASCII_8X8_109[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_109[3*(16*i+j)+2],ASCII_8X8_109[3*(16*i+j)+1],ASCII_8X8_109[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_109[3*(16*i+j)+2],ASCII_8X8_109[3*(16*i+j)+1],ASCII_8X8_109[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_109[3*(16*i+j)+2],ASCII_8X8_109[3*(16*i+j)+1],ASCII_8X8_109[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十一张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_110[3*(16*i+j)+2],ASCII_8X8_110[3*(16*i+j)+1],ASCII_8X8_110[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_110[3*(16*i+j)+2],ASCII_8X8_110[3*(16*i+j)+1],ASCII_8X8_110[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_110[3*(16*i+j)+2],ASCII_8X8_110[3*(16*i+j)+1],ASCII_8X8_110[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_110[3*(16*i+j)+2],ASCII_8X8_110[3*(16*i+j)+1],ASCII_8X8_110[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十二张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_111[3*(16*i+j)+2],ASCII_8X8_111[3*(16*i+j)+1],ASCII_8X8_111[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_111[3*(16*i+j)+2],ASCII_8X8_111[3*(16*i+j)+1],ASCII_8X8_111[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_111[3*(16*i+j)+2],ASCII_8X8_111[3*(16*i+j)+1],ASCII_8X8_111[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_111[3*(16*i+j)+2],ASCII_8X8_111[3*(16*i+j)+1],ASCII_8X8_111[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十三张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_112[3*(16*i+j)+2],ASCII_8X8_112[3*(16*i+j)+1],ASCII_8X8_112[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_112[3*(16*i+j)+2],ASCII_8X8_112[3*(16*i+j)+1],ASCII_8X8_112[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_112[3*(16*i+j)+2],ASCII_8X8_112[3*(16*i+j)+1],ASCII_8X8_112[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_112[3*(16*i+j)+2],ASCII_8X8_112[3*(16*i+j)+1],ASCII_8X8_112[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十四张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_113[3*(16*i+j)+2],ASCII_8X8_113[3*(16*i+j)+1],ASCII_8X8_113[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_113[3*(16*i+j)+2],ASCII_8X8_113[3*(16*i+j)+1],ASCII_8X8_113[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_113[3*(16*i+j)+2],ASCII_8X8_113[3*(16*i+j)+1],ASCII_8X8_113[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_113[3*(16*i+j)+2],ASCII_8X8_113[3*(16*i+j)+1],ASCII_8X8_113[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十五张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_114[3*(16*i+j)+2],ASCII_8X8_114[3*(16*i+j)+1],ASCII_8X8_114[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_114[3*(16*i+j)+2],ASCII_8X8_114[3*(16*i+j)+1],ASCII_8X8_114[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_114[3*(16*i+j)+2],ASCII_8X8_114[3*(16*i+j)+1],ASCII_8X8_114[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_114[3*(16*i+j)+2],ASCII_8X8_114[3*(16*i+j)+1],ASCII_8X8_114[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十六张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_115[3*(16*i+j)+2],ASCII_8X8_115[3*(16*i+j)+1],ASCII_8X8_115[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_115[3*(16*i+j)+2],ASCII_8X8_115[3*(16*i+j)+1],ASCII_8X8_115[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_115[3*(16*i+j)+2],ASCII_8X8_115[3*(16*i+j)+1],ASCII_8X8_115[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_115[3*(16*i+j)+2],ASCII_8X8_115[3*(16*i+j)+1],ASCII_8X8_115[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十七张图
        for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_116[3*(16*i+j)+2],ASCII_8X8_116[3*(16*i+j)+1],ASCII_8X8_116[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_116[3*(16*i+j)+2],ASCII_8X8_116[3*(16*i+j)+1],ASCII_8X8_116[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_116[3*(16*i+j)+2],ASCII_8X8_116[3*(16*i+j)+1],ASCII_8X8_116[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_116[3*(16*i+j)+2],ASCII_8X8_116[3*(16*i+j)+1],ASCII_8X8_116[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十八张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_117[3*(16*i+j)+2],ASCII_8X8_117[3*(16*i+j)+1],ASCII_8X8_117[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_117[3*(16*i+j)+2],ASCII_8X8_117[3*(16*i+j)+1],ASCII_8X8_117[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_117[3*(16*i+j)+2],ASCII_8X8_117[3*(16*i+j)+1],ASCII_8X8_117[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_117[3*(16*i+j)+2],ASCII_8X8_117[3*(16*i+j)+1],ASCII_8X8_117[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第二十九张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_118[3*(16*i+j)+2],ASCII_8X8_118[3*(16*i+j)+1],ASCII_8X8_118[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_118[3*(16*i+j)+2],ASCII_8X8_118[3*(16*i+j)+1],ASCII_8X8_118[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_118[3*(16*i+j)+2],ASCII_8X8_118[3*(16*i+j)+1],ASCII_8X8_118[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_118[3*(16*i+j)+2],ASCII_8X8_118[3*(16*i+j)+1],ASCII_8X8_118[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_119[3*(16*i+j)+2],ASCII_8X8_119[3*(16*i+j)+1],ASCII_8X8_119[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_119[3*(16*i+j)+2],ASCII_8X8_119[3*(16*i+j)+1],ASCII_8X8_119[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_119[3*(16*i+j)+2],ASCII_8X8_119[3*(16*i+j)+1],ASCII_8X8_119[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_119[3*(16*i+j)+2],ASCII_8X8_119[3*(16*i+j)+1],ASCII_8X8_119[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十一张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_120[3*(16*i+j)+2],ASCII_8X8_120[3*(16*i+j)+1],ASCII_8X8_120[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_120[3*(16*i+j)+2],ASCII_8X8_120[3*(16*i+j)+1],ASCII_8X8_120[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_120[3*(16*i+j)+2],ASCII_8X8_120[3*(16*i+j)+1],ASCII_8X8_120[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_120[3*(16*i+j)+2],ASCII_8X8_120[3*(16*i+j)+1],ASCII_8X8_120[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十二张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_121[3*(16*i+j)+2],ASCII_8X8_121[3*(16*i+j)+1],ASCII_8X8_121[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_121[3*(16*i+j)+2],ASCII_8X8_121[3*(16*i+j)+1],ASCII_8X8_121[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_121[3*(16*i+j)+2],ASCII_8X8_121[3*(16*i+j)+1],ASCII_8X8_121[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_121[3*(16*i+j)+2],ASCII_8X8_121[3*(16*i+j)+1],ASCII_8X8_121[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十三张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_122[3*(16*i+j)+2],ASCII_8X8_122[3*(16*i+j)+1],ASCII_8X8_122[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_122[3*(16*i+j)+2],ASCII_8X8_122[3*(16*i+j)+1],ASCII_8X8_122[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_122[3*(16*i+j)+2],ASCII_8X8_122[3*(16*i+j)+1],ASCII_8X8_122[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_122[3*(16*i+j)+2],ASCII_8X8_122[3*(16*i+j)+1],ASCII_8X8_122[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十四张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_123[3*(16*i+j)+2],ASCII_8X8_123[3*(16*i+j)+1],ASCII_8X8_123[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_123[3*(16*i+j)+2],ASCII_8X8_123[3*(16*i+j)+1],ASCII_8X8_123[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_123[3*(16*i+j)+2],ASCII_8X8_123[3*(16*i+j)+1],ASCII_8X8_123[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_123[3*(16*i+j)+2],ASCII_8X8_123[3*(16*i+j)+1],ASCII_8X8_123[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十五张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_124[3*(16*i+j)+2],ASCII_8X8_124[3*(16*i+j)+1],ASCII_8X8_124[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_124[3*(16*i+j)+2],ASCII_8X8_124[3*(16*i+j)+1],ASCII_8X8_124[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_124[3*(16*i+j)+2],ASCII_8X8_124[3*(16*i+j)+1],ASCII_8X8_124[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_124[3*(16*i+j)+2],ASCII_8X8_124[3*(16*i+j)+1],ASCII_8X8_124[3*(16*i+j)]);
      }
     }
    }
    delay(50);
    //第三十六张图
    for(uint8_t j = 0;(j<16); j++)
    {
    for(uint8_t i = 0; i < 16; i++)
    {
      
      if((i<8)&&(j>=8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board2, (6-i)*8+j, ASCII_8X8_125[3*(16*i+j)+2],ASCII_8X8_125[3*(16*i+j)+1],ASCII_8X8_125[3*(16*i+j)]); 
      }
      if((i>=8)&&(j>=8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board1, (14-i) * 8 + j, ASCII_8X8_125[3*(16*i+j)+2],ASCII_8X8_125[3*(16*i+j)+1],ASCII_8X8_125[3*(16*i+j)]);
      }
     
     
      if((i<8)&&(j<8))
      {
        BM32D2021_1.writeRGB(i2c_addr_board4, (7-i)*8+j, ASCII_8X8_125[3*(16*i+j)+2],ASCII_8X8_125[3*(16*i+j)+1],ASCII_8X8_125[3*(16*i+j)]); 
      }
     
      if((i>=8)&&(j<8))
      {
       // BM32D2021_1.writeRGB(address1, (i-8) * 8 + j+7-k, R,G,B);
       BM32D2021_1.writeRGB(i2c_addr_board3, (15-i) * 8 + j, ASCII_8X8_125[3*(16*i+j)+2],ASCII_8X8_125[3*(16*i+j)+1],ASCII_8X8_125[3*(16*i+j)]);
      }
     }
    }
    delay(50);
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
  //led_left(i2c_addr_board3,i2c_addr_board4,i2c_addr_board1,i2c_addr_board2,0xff,0,0);
}
