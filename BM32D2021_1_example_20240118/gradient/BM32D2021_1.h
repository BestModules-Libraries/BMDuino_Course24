/*************************************************
File:             BM32D2021_1.h
Author:             BESTMODULE
Description:        Define classes and required variables
History：      
V1.0.1   -- initial version；2022-05-20；Arduino IDE : ≥v1.8.16
**************************************************/
#ifndef _BM32D2021_1_H
#define _BM32D2021_1_H

#include <Arduino.h>
#include <Wire.h>
#include <FONT1.h>


const uint8_t RAM_INDEX_TABLE[8][8] = //8x8RGB RAM address index
{//you can set data to this address to set led on or off 
//such as set number RGB1,write R G B value to 0x03 0x04 0x05.
  {0x03,0x06,0x09,0x0C,0x13,0x16,0x19,0x1C},  //column 1
  {0x23,0x26,0x29,0x2C,0x33,0x36,0x39,0x3C},  //column 2
  {0x41,0x46,0x49,0X4C,0x51,0x56,0x59,0X5C},  //column 3
  {0x61,0x66,0x69,0X6C,0x71,0x74,0x79,0X7C},  //column 4
  {0x81,0x84,0x89,0X8C,0x91,0x94,0x99,0X9C},  //column 5
  {0xA1,0xA4,0xA7,0XAC,0xB1,0xB4,0xB7,0XBC},  //column 6
  {0xC1,0xC4,0xC7,0XCC,0xD1,0xD4,0xD7,0XDA},  //column 7
  {0xE1,0xE4,0xE7,0XEA,0xF1,0xF4,0xF7,0XFA} //column 8
};



#define BM32D2021_1_CCR_3MA 0
#define BM32D2021_1_CCR_6MA 1
#define BM32D2021_1_CCR_9MA 2
#define BM32D2021_1_CCR_12MA 3
#define BM32D2021_1_CCR_15MA 4
#define BM32D2021_1_CCR_18MA 5
#define BM32D2021_1_CCR_21MA 6
#define BM32D2021_1_CCR_24MA 7
#define BM32D2021_1_CCR_27MA 8
#define BM32D2021_1_CCR_30MA 9
#define BM32D2021_1_CCR_33MA 10
#define BM32D2021_1_CCR_36MA 11
#define BM32D2021_1_CCR_39MA 12
#define BM32D2021_1_CCR_42MA 13
#define BM32D2021_1_CCR_45MA 14
#define BM32D2021_1_CCR_48MA 15

#define BM32D2021_1_DISPLAY_STATIC 0 
#define BM32D2021_1_DISPLAY_DYNAMICS 1 
#define BM32D2021_1_DISPLAY_VALUE 0x00
#define BM32D2021_1_DISPLAY_BIN 0x80
#define BM32D2021_1_MATRIX_9x10 1
#define BM32D2021_1_MATRIX_12x12 2
#define BM32D2021_1_MATRIX_16x16 3

#define BM32D2021_1_SYS_STANDBY 0
#define BM32D2021_1_SYS_OSC_ON 2
#define BM32D2021_1_SYS_DIS_ON 3

#define BM32D2021_1_I2C_ADDRESS_VDD 0X67
#define BM32D2021_1_I2C_ADDRESS_GND 0x64
#define BM32D2021_1_I2C_ADDRESS_SCL 0X65
#define BM32D2021_1_I2C_ADDRESS_SDA 0X66
#define BM32D2021_1_I2C_ADDRESS_BOARDCAST 0X2E

#define HT16D33_CMD_DISPLAY_MODE 0X30
#define HT16D33_CMD_CONFIG_MODE 0x31
#define HT16D33_CMD_CONNECT_MODE 0x34
#define HT16D33_CMD_SYS_CONTROL 0x35
#define HT16D33_CMD_SET_CURRENT 0x36
#define HT16D33_CMD_SET_BRIGHTNESS 0x37
#define HT16D33_CMD_SET_V_BLANK 0x39
#define HT16D33_CMD_WRITE_LED_DATA 0x80
#define HT16D33_CMD_SET_LED_ON_OFF 0x84
#define HT16D33_CMD_RESET 0xcc

#define GFS_GRADIENT_GARMA     0
#define GFS_GRADIENT_LINEAR    1
#define GFS_BLINK        2
#define GFT_OFF          0
#define GFT_T1_256_FRAME    1
#define GFT_T1_512_FRAME    2
#define GFT_T1_1024_FRAME   3
#define GFT_T1_1536_FRAME   4
#define GFT_T1_2048_FRAME   5
#define GFT_T1_2560_FRAME   6
#define GFT_T1_3072_FRAME   7

#define T3_IS_T1       0
#define T3_IS_T1x2     1
#define T4_IS_T1x1_4   0
#define T4_IS_T1x1_2   1
#define T4_IS_T1       2
#define T4_IS_T1x2     3
#define T2_IS_T1x1_4   0
#define T2_IS_T1x1_2   1
#define T2_IS_T1       2
#define T2_IS_T1x2     3

class BM32D2021_1
{
  public: 
      BM32D2021_1(TwoWire *wirePort = &Wire);
    void begin(uint8_t address=0x2e,uint32_t i2c_Clock=400000);
    void reset(uint8_t  address=0x2E);
    bool isConnected(uint8_t address);
  
    void writeCmd(uint8_t address,uint8_t cmd,uint8_t data);
    uint8_t readCmd(uint8_t i2c_addr,uint8_t cmd);

    void setOverTemperatureProtect(uint8_t i2c_addr,uint8_t status=1,bool auto_control=true);
    bool getOverTemperatureFlag(uint8_t i2c_addr);
    
    void setLedRAMEnable(uint8_t address);
    void setLedRAMDisable(uint8_t address);  
    void setBrightness( uint8_t address,uint8_t brightness);
    void setCurrent(uint8_t address, uint8_t level);
    
        void clearAll(uint8_t address);
      void clearRGB(uint8_t address, uint8_t RGB_Number);
    void clearRow(uint8_t address, uint8_t RowIndex);
    void clearColumn(uint8_t address, uint8_t ColumnIndex);
    

    void writeRGB(uint8_t address,uint8_t RGB_Number, uint8_t R, uint8_t G, uint8_t B);
        void writeAllRGB(uint8_t address, uint8_t R, uint8_t G, uint8_t B);
    void writeColumn(uint8_t address, uint8_t ColumnIndex, uint8_t R, uint8_t G, uint8_t B);
    void writeRow(uint8_t address, uint8_t RawIndex, uint8_t R, uint8_t G, uint8_t B);
    void DrawAsciiChar(uint8_t address, char Ch, uint8_t R, uint8_t G, uint8_t B);
    void DrawAsciiChar1(uint8_t address, char Ch, uint8_t R, uint8_t G, uint8_t B);
    
    void setGradient(uint8_t i2c_addr,uint8_t mode,uint8_t T1,uint8_t T2=T2_IS_T1,uint8_t T3=T3_IS_T1,uint8_t T4=T4_IS_T1);

  private:
    TwoWire *_i2cPort = NULL;
        void _writeRegister(uint8_t address, uint8_t registerAddress,uint8_t data);
    uint8_t _readRegister(uint8_t i2c_addr, uint8_t registerAddress);

};
#endif
