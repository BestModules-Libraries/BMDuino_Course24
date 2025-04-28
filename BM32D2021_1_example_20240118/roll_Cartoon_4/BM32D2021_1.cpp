/*****************************************************************
File:          BM32D2021_1.cpp
Author:        BESTMODULE
Description:   I2C communication with the BM32D2021_1 and control the RGB.
History：         
V1.0.1  -- initial version;2021-12-10;Arduino IDE : ≥v1.8.16
******************************************************************/
#include "BM32D2021_1.h"
/**********************************************************
Description: constructor 
parameter:
    Input:       *wirePort: Wire object if your board has more than one I2C interface              
    Output:         
Return:            
Others:      
**********************************************************/
BM32D2021_1::BM32D2021_1(TwoWire *wirePort)
{  
  _i2cPort = wirePort;
}
/**********************************************************
Description: Module Initial 
parameter:
    Input:                  
    Output:       
Return:            
Others:      
**********************************************************/
void BM32D2021_1::begin(uint8_t address,uint32_t i2c_Clock)
{
  _i2cPort->begin();
  _i2cPort->setClock(i2c_Clock);
    //static display image   
    writeCmd(address,HT16D33_CMD_DISPLAY_MODE,
                                    BM32D2021_1_DISPLAY_STATIC);         
    //display value mode,matrix type is 16x16
    writeCmd(address,HT16D33_CMD_CONFIG_MODE,
            BM32D2021_1_DISPLAY_VALUE|BM32D2021_1_MATRIX_16x16); 
    //Configure HT16D33B and set the constant flow rate to 6ma      
    writeCmd(address,HT16D33_CMD_SET_CURRENT,
                           BM32D2021_1_CCR_6MA);   
    //The global brightness,maximum 
    writeCmd(address,HT16D33_CMD_SET_BRIGHTNESS,0xff);   
    //Blank area voltage control,default 0x8f; 
    writeCmd(address,HT16D33_CMD_SET_V_BLANK,0x8f);    
    //set Internal crystal oscillator on          
    writeCmd(address,HT16D33_CMD_SYS_CONTROL,
                           BM32D2021_1_SYS_OSC_ON);
    //set mode for normaly display 
    writeCmd(address,HT16D33_CMD_SYS_CONTROL,
                            BM32D2021_1_SYS_DIS_ON);  
    //clear screen 
    clearAll(address);  
    //set led ram on                  
    setLedRAMEnable(address);     

    
}
/*************************************************
Description : check connection
parameter:
    Input:      
         address:I2C address 
Return:  
Others:        
*************************************************/
bool BM32D2021_1::isConnected(uint8_t address)
{
  uint8_t triesBeforeGiveup = 5;
  for (uint8_t x = 0; x < triesBeforeGiveup; x++)
  {
    _i2cPort->beginTransmission(address);
    if (_i2cPort->endTransmission() == 0)
    {
      return true;
    }
    delay(100);
  }
  return false;
}


/*************************************************
Description :clear all/one/raw/column
parameter:
    Input:      
         address:I2C address 
Return:  
Others:        
*************************************************/
void BM32D2021_1::clearAll(uint8_t address)
{
    writeAllRGB(address,0,0,0);
}
void BM32D2021_1::clearRGB(uint8_t address, uint8_t RGB_Number)
{
    writeRGB(address,RGB_Number,0,0,0);
}
void BM32D2021_1::clearRow(uint8_t address, uint8_t RowIndex)
{
    writeRow(address,RowIndex,0,0,0);
}
void BM32D2021_1::clearColumn(uint8_t address, uint8_t ColumnIndex)
{
    writeColumn(address,ColumnIndex,0,0,0);
}
/*************************************************
Description : write a rgb to display,the color data from color seting
parameter:
    Input:      
         address:I2C address 
         RGB_Number:RGB index    
        R/G/B:color tdata 
Return:  
Others:        
*************************************************/
void BM32D2021_1::writeRGB(uint8_t address,uint8_t RGB_Number, uint8_t R, uint8_t G, uint8_t B)
{
     const uint8_t *p=RAM_INDEX_TABLE[0];
  
  _i2cPort->beginTransmission(address);
    _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
    _i2cPort->write(p[RGB_Number]); 
  _i2cPort->write(B);
  if(R<170)
  {
    _i2cPort->write(uint8_t(R*1.5));  
  } 
  else
  {
    _i2cPort->write(R); 
  }   
    _i2cPort->write(G);                    
  _i2cPort->endTransmission();
}
/*************************************************
Description : write a column to display the same color from color table
parameter:
    Input:      
         address:I2C address     
        R/G/B:color data      
Return:  
Others:        
*************************************************/
void BM32D2021_1::writeColumn(uint8_t address, uint8_t ColumnIndex, uint8_t R, uint8_t G, uint8_t B)
{
            uint8_t ram_addr=RAM_INDEX_TABLE[ColumnIndex][0];
             uint8_t rgb_count=0;

            _i2cPort->beginTransmission(address);             //0 ~ 15 RGB
             _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
             _i2cPort->write(ram_addr); 
            
            while(1){//combine rgb data and some dumy data to liner address
                _i2cPort->write(R);                    
                _i2cPort->write(G);                    
                _i2cPort->write(B);  
                rgb_count++;//point to next rgb
                if(rgb_count>7) break;//is last rgb
                ram_addr+=3; //the address for next rgb;
                //if the ram_addr no equal to the next address  
                while(ram_addr<RAM_INDEX_TABLE[ColumnIndex][rgb_count]){
                    _i2cPort->write(0); //insert zero  
                    ram_addr++;    //ram_addr add 1
                } 
            }
            _i2cPort->endTransmission(); 
}

/*************************************************
Description : write a raw to display the same color from color table
parameter:
    Input:      
         address:I2C address     
        R/G/B:color data      
Return:  
Others:        
*************************************************/
void BM32D2021_1::writeRow(uint8_t address, uint8_t RawIndex, uint8_t R, uint8_t G, uint8_t B)
{
            for(uint8_t j = 0; j < 8; j++){

            _i2cPort->beginTransmission(address);             //0 ~ 15 RGB
             _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
             _i2cPort->write(RAM_INDEX_TABLE[RawIndex][j] ); 
             _i2cPort->write(R);                    
            _i2cPort->write(G);                   
            _i2cPort->write(B); 
            _i2cPort->endTransmission();
        }  
}
/*************************************************
Description : write all rgb to display the same color from seting.
parameter:
    Input:      
         address:I2C address     
        R/G/B:color data    
Return:  
Others:        
*************************************************/
void BM32D2021_1::writeAllRGB(uint8_t address, uint8_t R, uint8_t G, uint8_t B) 
{
    for(uint8_t i = 0; i < 8; i++)//write 8 column
    {
            uint8_t ram_addr=RAM_INDEX_TABLE[i][0];
             uint8_t rgb_count=0;

            _i2cPort->beginTransmission(address);             //0 ~ 15 RGB
             _i2cPort->write(HT16D33_CMD_WRITE_LED_DATA);
             _i2cPort->write(ram_addr); 
            
            while(1){//combine rgb data and some dumy data to liner address
                _i2cPort->write(R);                    
                _i2cPort->write(G);                   
                _i2cPort->write(B);  
                rgb_count++;//point to next rgb
                if(rgb_count>7) break;//is last rgb
                ram_addr+=3; //the address for next rgb;
                //if the ram_addr no equal to the next address  
                while(ram_addr<RAM_INDEX_TABLE[i][rgb_count]){
                    _i2cPort->write(0); //insert zero  
                    ram_addr++;    //ram_addr add 1
                } 
            }
            _i2cPort->endTransmission(); 
    }
}
/*************************************************
Description : set display a char from char table.
parameter:
    Input:      
         address:I2C address   
         R/G/B:color    
Return:  
Others:        
*************************************************/
void BM32D2021_1::DrawAsciiChar(uint8_t address, char Ch, uint8_t R, uint8_t G, uint8_t B)
{
  uint8_t Temp, Index;
  
  Index = Ch - 32;

  for(uint8_t i = 0; i < 8; i++)
  {
    Temp = ASCII_8X8[Index][i];
    for(uint8_t j = 0; j < 8; j++)
    {
      if(((Temp >> j)  & 0x01))
      {
      writeRGB(address, i * 8 + j, R,G,B);
      }
   }
  } 
}

/*************************************************
Description : enable led ram
parameter:
    Input:      
         address:I2C address             
Return:  
Others:        
*************************************************/
const uint8_t LED_ON_OFF_16_16[32] =//see HT16D33A datasheet page21~22
{
  0xF8, 0x7F,                                     //0x00,0x01
    0xF8, 0x7F,                                     //0x02,0x03
    0xF8, 0x7F,                                     //0x04,0x05
    0xF8, 0x7F,                                     //0x06,0x07
    0xCE, 0x7F,                                     //0x08,0x09
    0xCE, 0x7F,                                     //0x0a,0x0b
    0xCE, 0x7F,                                     //0x0c,0x0d
    0x7E, 0x7E,                                     //0x0e,0x0f
    0x7E, 0x7E,                                     //0x10,0x11
    0x7E, 0x7E,                                     //0x12,0x13
    0xFE, 0x73,                                     //0x14,0x15
    0xFE, 0x73,                                     //0x16,0x17
    0xFE, 0x73,                                     //0x18,0x19
    0xFE, 0x1F,                                     //0x1a,0x1b
    0xFE, 0x1F,                                     //0x1c,0x1d
    0xFE, 0x1F                                      //0x1e,0x1f  
};
void BM32D2021_1::setLedRAMEnable(uint8_t address) 
{
  _i2cPort->beginTransmission(address);
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(0x00);                
  for(uint8_t i = 0; i < 28; i++)
  {
    _i2cPort->write(LED_ON_OFF_16_16[i]); 
  } 
  _i2cPort->endTransmission();
  //Repeat transmission
    _i2cPort->beginTransmission(address);//beacuse arduino I2C Maximum transmission 32byte
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(28);
  for(uint8_t i = 28; i < 32; i++) 
    {
    _i2cPort->write(LED_ON_OFF_16_16[i]);      
    }
  _i2cPort->endTransmission();
}
/*************************************************
Description : disable led ram
parameter:
    Input:      
         address:I2C address             
Return:  
Others:        
*************************************************/
void BM32D2021_1::setLedRAMDisable(uint8_t address) 
{
  _i2cPort->beginTransmission(address);
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(0x00);                
  for(uint8_t i = 0; i < 28; i++)
  {
    _i2cPort->write(0); 
  } 
  _i2cPort->endTransmission();

    _i2cPort->beginTransmission(address);//beacuse arduino I2C Maximum transmission 32byte
    _i2cPort->write(HT16D33_CMD_SET_LED_ON_OFF);
    _i2cPort->write(0x1C);
  for(uint8_t i = 28; i < 32; i++) 
    {
    _i2cPort->write(0);      
    }
  _i2cPort->endTransmission();
}

/*************************************************
Description : rest module
parameter:
    Input:      
         address:I2C address             
Return:  
Others:        
*************************************************/
void BM32D2021_1::reset(uint8_t address)
{
  _i2cPort->beginTransmission(address);
    _i2cPort->write(RESET);
  _i2cPort->endTransmission();
}
/*************************************************
Description : rest module
parameter:
    Input:      
         address:I2C address             
Return:  
Others:        
*************************************************/
void BM32D2021_1::_writeRegister(uint8_t address, uint8_t registerAddress,uint8_t data)
{
  _i2cPort->beginTransmission(address);
    _i2cPort->write(registerAddress);

    _i2cPort->write(data);

  _i2cPort->endTransmission() ;

}
/*************************************************
Description : read Register
parameter:
    Input:      
         i2c_addr:I2C address   
         registerAddress:  register Address   

Return:  register data
Others:        
*************************************************/
uint8_t BM32D2021_1::_readRegister(uint8_t i2c_addr, uint8_t registerAddress)
{
    uint8_t result;
    _i2cPort->beginTransmission(i2c_addr);
    _i2cPort->write(registerAddress);
    _i2cPort->requestFrom(i2c_addr, 1);
    _i2cPort->endTransmission() ;
    if(_i2cPort->available()==1)
    {
        result= _i2cPort->read();
    }
    else
       result= 0; 
    return result;
}
/*************************************************
Description: Write data to the register
see the simHT16D33A_Bv132.pdf
parameter:
    Input:      
         address:I2C address   
         cmd: 
            HT16D33_CMD_DISPLAY_MODE 0X30
            HT16D33_CMD_CONFIG_MODE 0x31
            HT16D33_CMD_CONNECT_MODE 0x34
            HT16D33_CMD_SYS_CONTROL 0x35
            HT16D33_CMD_SET_CURRENT 0x36
            HT16D33_CMD_SET_BRIGHTNESS 0x37
            HT16D33_CMD_SET_V_BLANK 0x39
         data:data                       
Return:   
*************************************************/
void BM32D2021_1::writeCmd(uint8_t address,uint8_t cmd,uint8_t data)
{
    _writeRegister(address, cmd, data);
}
/*************************************************
Description: read data from the register
see the simHT16D33A_Bv132.pdf
parameter:
    Input:      
         i2c_addr:I2C address   
         cmd: 
            read CMD
         data:data                       
Return:  read data from the register
*************************************************/
uint8_t BM32D2021_1::readCmd(uint8_t i2c_addr,uint8_t cmd)
{
     return _readRegister(i2c_addr, cmd);
}
/*************************************************
Description: set all RGB brightness
parameter:
    Input:      
         address:I2C address   
         brightness:brightness,0~255,the max brightness is 255                     
Return:   
*************************************************/
void BM32D2021_1::setBrightness( uint8_t address,uint8_t brightness)
{
    _writeRegister(address,HT16D33_CMD_SET_BRIGHTNESS,brightness);
}
/*************************************************
Description: set current
parameter:
    Input:      
         address:I2C address   
         brightness:level current = 1+3*level(mA)                        
Return:   
*************************************************/
void BM32D2021_1::setCurrent( uint8_t address,uint8_t level)
{
    _writeRegister(address,HT16D33_CMD_SET_CURRENT,level);
}
/*************************************************
Description: set Over Temperture Protection funtion
parameter:
    Input:      
         i2c_addr:I2C address   
         status:disable or enable the   protection funtion  1:enable 0:disable  
         auto_control:if Overtemperature protection occurs, Automatic control or not ,to control the LED RAM on/off.         
Return:   
*************************************************/
#define MODE_CONROL_CMD 0x38 
void BM32D2021_1::setOverTemperatureProtect(uint8_t i2c_addr,uint8_t status,bool auto_control)
{
    uint8_t param=0;
    if(status==1)
    {
        param |= 0x80;//enable OverTempertureProtect
        if(auto_control)
        {
            param |= 0x40;//enable auto_control
        }
        else
            param &= ~0x40;//disable auto_control
    }
    else
        param &= 0x7f;

    writeCmd(i2c_addr,MODE_CONROL_CMD,param);
}
/*************************************************
Description: return Over Temperture Protection flag
            If over temperature protection and manual control are enabled
parameter:
    Input:      
         i2c_addr:I2C address        
Return:   Protection flag 
*************************************************/
#define FUNTION_FLAG_CMD 0x73
bool BM32D2021_1::getOverTemperatureFlag(uint8_t i2c_addr)
{
    uint8_t data;
    bool result;
    data = readCmd(i2c_addr,FUNTION_FLAG_CMD);
    if(data&0x01) result=true;
    else result = false;
    return result;
}


/*************************************************
Description: set  gradient 
parameter:
    Input:      
         i2c_addr:I2C address
         mode: set Gradient work mode is Gradient garma/linker or blink
         T1: the time of dark to light 
         T2: keep light time
         T3: the time of light to dark
         T4: keep dark time
Return:  void
*************************************************/
#define GROBAL_GRADIENT_CONTROL_CMD 0x32
#define SET_GROBAL_GRADIENT_TIME_CMD 0x33

void BM32D2021_1::setGradient(uint8_t i2c_addr,uint8_t mode,uint8_t T1,uint8_t T2,uint8_t T3,uint8_t T4)
{
    uint8_t param0 = 0;
    uint8_t param1 = 0;

  param0 = 0x80;//enable
 
  param0 |= 0x20;//Globe
  param0 |= T3<<4;//T3
  param0 |= T4<<2;//T4
  param0 |= T2;//T2

  if(mode==GFS_GRADIENT_GARMA)
  { 
      param0 |= 0x40;//garma
  }
  else if(mode==GFS_GRADIENT_LINEAR)
  {

  }
  else if(mode==GFS_BLINK)
  {
    param1 = 0x80;//blink
  }
  else
  {
    
  }
  param1 |= T1;//param1

  writeCmd(i2c_addr, GROBAL_GRADIENT_CONTROL_CMD, param0);
  writeCmd(i2c_addr, SET_GROBAL_GRADIENT_TIME_CMD, param1);
}
