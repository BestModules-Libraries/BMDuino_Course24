/*****************************************************************
File:        BM7701-00-1.cpp
Author:      BEST MODULES CORP.
Description: UART communication with the BM7701_00_1 
             and control bc7701 to transmit data with mobile phone  
Version:     V1.0.3   -- 2025-01-21
******************************************************************/
#include <BM7701-00-1.h>

/**********************************************************
Description: Constructor
Parameters:  *theSerial：hardware serial 
             BMduino optional:serial(default) serial1/seria2/seria3/seria4
             UNO optional:serial(default)
Return:      none     
Others:     
**********************************************************/
BM7701_00_1::BM7701_00_1(HardwareSerial *theSerial)
{
     _softSerial = NULL;
     _serial = theSerial;
}


/**********************************************************
Description: Constructor
Parameters:  rxPin : Receiver pin of the UART
             txPin : Send signal pin of UART  
Return:      none    
Others:   
**********************************************************/
BM7701_00_1::BM7701_00_1(uint16_t rxPin,uint16_t txPin)
{
    _serial = NULL;
    _rxPin = rxPin;
    _txPin = txPin;
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
}


/**********************************************************
Description: Module Initial
Parameters:  br:Baud rate    
Return:      none      
Others:   BC7701 must delay 60ms after reset 
          If the hardware UART is initialized, the _softSerial 
          pointer is null, otherwise it is non-null       
**********************************************************/
void BM7701_00_1::begin(uint32_t baud)
{
   if(_softSerial != NULL)
    {
       _softSerial->begin(baud); 
    }
    else
    {
       _serial->begin(baud);
    }
    
    setBaudRate(baud);
}

/**********************************************************
Description:    Turn on broadcast
Parameters:     ENABLE:OPEN
                DISABLE:close       
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setAdvCtrl(bool ctrl)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t Adven[6]={0x77,0x04,0x00,0x07,0x00,ctrl};
   writeBytes(Adven,6);  

   previous_time=millis();     
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x07 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}
/**********************************************************
Description:    Disconnect
Parameters:                     
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::disconnect()
{
  bool flag = false;
  uint32_t current_time,previous_time;
  uint8_t Disconn[5]={0x77,0x03,0x00,0x02,0x00};
  writeBytes(Disconn,5);

  previous_time=millis();
  while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x02 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}
/**********************************************************
Description:    BC7701 writeData Data 
Parameters:     length: <128 byte 
                Data                         
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::writeData(uint8_t dataBuf[],uint8_t length)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i;
   uint8_t data[255]= {0x77,length,0x20,0xF1,0xFF,0xb2};
   for(i=0;i<length;i++)
   {
    data[6+i]=dataBuf[i];
   }
   data[1]=length+4;
   writeBytes(data,length+6);

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x20 && receive_data[1]==0xF1 && 
                                    receive_data[2]==0xFF)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}
/**********************************************************
Description:    readData
Parameters:     receiveBuff:Received custom data(not include:Header,Length)
                &length: Received data length                   
Return:         0: check ok
        1: check error
        2: timeout error
Others:        
**********************************************************/
bool BM7701_00_1::readData(uint8_t receiveBuff[],uint8_t &length)
{
     uint8_t receive[256]={0};
 if (_softSerial != NULL)
  {
    while (_softSerial->available() > 2)  
    {
      readBytes(receive,2,50); //Header,Length
      if(receive[0]!=0x78)
      {
        while (_softSerial->available() != 0) 
        {
          _softSerial->read();
        }
        return false;
      }
      length=receive[1];
      readBytes(receiveBuff,length,50);
      return true;
    }
    return false;
  }
  else
  {
    while (_serial->available() > 2)  
    {
      readBytes(receive,2,50);  //Header,Length
      if(receive[0]!=0x78)
      {
        while (_softSerial->available() != 0) 
        {
          _softSerial->read();
        }
        return false;
      }
      length=receive[1];
      readBytes(receiveBuff,length,50);
      return true;
    }
     return false;
  }
  return false;
}
/**********************************************************
Description:    BC7701 Send Data 
Parameters:     length: <128 byte 
                Data                         
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::sendData(uint8_t dataBuf[],uint8_t length)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i;
   uint8_t data[255]= {0x77,0,0x20,0xF1,0xFF};
   for(i=0;i<length;i++)
   {
    data[5+i]=dataBuf[i];
   }
   data[1]=length+3; //CmdFlag/EvtStatus,Type1,Type2,value(length Bytes)
   writeBytes(data,length+5);

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x20 && receive_data[1]==0xF1 && 
                                    receive_data[2]==0xFF)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}
/**********************************************************
Description:    receiveData
Parameters:     receiveBuff:Received custom data(not include:Header,Length,CmdFlag/EvtStatus,Type1,Type2)
                &length: Received data length
Return:         0x81: API_CONNECTED
                0x82: API_DISCONNECTED
                0x83: DATA_RECEIVED
                0x84: API_ERROR
Others:        
**********************************************************/
uint8_t BM7701_00_1::receiveData(uint8_t receiveBuff[],uint8_t &length)
{
  uint8_t receive[256]={0};
  uint8_t st = 0x00;
  uint8_t receiveLen = 0x00;
  if(readData(receive,receiveLen))
  {
    st = API_ERROR;
    switch (receive[1])
    {
      case 0x00:
        if (receive[0] == 0x00)
        {
          if ((receive[3] & 0x01) == 0x01)
          {
            st = API_CONNECTED;
          }
          else st = API_DISCONNECTED;

        }
        break;
      case 0xF2:
        if ((receive[0] == 0x00) && (receive[2] == 0xFF))
        {
          st = DATA_RECEIVED;
        }
        break;
      default: 
        break;
    }
    length = receiveLen - 3;
    for(uint8_t i=0;i<length;i++)
    {
      receiveBuff[i] = receive[3+i];
    }
    return st;
  }
  return st;

}

/**********************************************************
Description:    Change baud rate
Parameters:     br:1200~125000      
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setBaudRate(uint32_t baud)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t a[4];
   a[0] = (uint8_t)baud;
   a[1] = (uint8_t)(baud>>8);
   a[2] = (uint8_t)(baud>>16);
   a[3] = (uint8_t)(baud>>24);
   uint8_t CMD_BR[9]={0x77,0x07,0x00,0x26,0x00,a[0],a[1],a[2],a[3]};
   writeBytes(CMD_BR,9);  

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x26 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        if(_softSerial != NULL)
        {
            _softSerial->begin(baud);
        }
        else
        {
            _serial->begin(baud);
        }
        
        delay(2);
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set Advertise Interval
Parameters:     min
                max
                chmap   
Return:         true:reply true event
                false:reply false event 
Others:        
**********************************************************/
bool BM7701_00_1::setAdvIntv(uint16_t min,uint16_t max,uint8_t advMap)
{
   bool flag = false; 
   uint32_t current_time,previous_time; 
   uint8_t a[4];
   a[0] = (uint8_t)min;
   a[1] = (uint8_t)(min>>8);
   a[2] = (uint8_t)max;
   a[3] = (uint8_t)(max>>8);
   uint8_t advintv[]={0x77,0x08,0x00,0x08,0x00,a[0],a[1],a[2],a[3],advMap};
   writeBytes(advintv,10);  

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x08 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set Advertise Data
Parameters:     length
                Advdata[]        
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setAdvData(uint8_t appendname,uint8_t length,uint8_t Advdata[])
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i;
   if(length >31)length = 31;
   uint8_t advdata[128] = {0x77,length,appendname,0x09,0x00};
   for(i=0;i<length;i++)
   {
    advdata[5+i]=Advdata[i];
   }
   advdata[1]=length+3;
   writeBytes(advdata,length+5);

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x09 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set Scan Data
Parameters:     length
                Scandata[]           
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setScanData(uint8_t length,uint8_t Scandata[])
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i;
   if(length >31)length = 31;
   uint8_t scandata[128] = {0x77,length,0x00,0x0A,0x00};
   for(i=0;i<length;i++)
   {
    scandata[5+i]=Scandata[i];
   }
   scandata[1]=length+3;
   writeBytes(scandata,length+5);
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x0A && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set Connected Interval
Parameters:     interval           
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setConnIntv(uint16_t interval)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t a1,a0;
   a0 = (uint8_t)interval;
   a1 = (uint8_t)(interval>>8);
   uint8_t connintv[]={0x77,0x05,0x00,0x03,0x00,a0,a1};
   writeBytes(connintv,7);  
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x03 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set Connected Interval (more detail)
Parameters:     minIntv
                maxIntv
                latency
                timeout            
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setConnIntv(uint16_t minIntv,uint16_t maxIntv,uint16_t latency,uint16_t timeout)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t a[8];
   a[0] = (uint8_t)minIntv;
   a[1] = (uint8_t)(minIntv>>8);
   a[2] = (uint8_t)maxIntv;
   a[3] = (uint8_t)(maxIntv>>8);
   a[4] = (uint8_t)latency;
   a[5] = (uint8_t)(latency>>8);
   a[6] = (uint8_t)timeout;
   a[7] = (uint8_t)(timeout>>8);   
   uint8_t connintv1[128]={0x77,0x0B,0x00,0x04,0x00,a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]};
   writeBytes(connintv1,13);  

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x04 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}
/**********************************************************
Description:    Set BLE Name
Parameters:     length[]
                Name[]            
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setName(uint8_t length,uint8_t name[])
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i;
   if(length >31)length = 31;
   uint8_t setname[128] = {0x77,length,0x00,0x05,0x00};  
   for(i=0;i<length;i++)
   {
    setname[5+i]=name[i];
   }
   setname[1]=length+3;
   writeBytes(setname,length+5);
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x05 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set  MAC address
Parameters:     Address[]: 6byte only          
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setAddress(uint8_t address[])
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i;
   uint8_t setaddr[128] = {0x77,0x09,0x00,0x06,0x00};
   for(i=0;i<6;i++)
   {
    setaddr[5+i]=address[i];
   }
   writeBytes(setaddr,11);

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x06 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}



/**********************************************************
Description:    Set Feature
Parameters:     md
                fr     
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setFeature(uint8_t cmdFlag,uint32_t codeFeature)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t a3,a2,a1,a0;
   a0 = (uint8_t)codeFeature;
   a1 = (uint8_t)(codeFeature>>8);
   a2 = (uint8_t)(codeFeature>>16);
   a3 = (uint8_t)(codeFeature>>24);
   uint8_t FR[]={0x77,0x07,cmdFlag,0x10,0x00,a0,a1,a2,a3};   
   writeBytes(FR,9);
     
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x10 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Enter Sleep mode
Parameters:     ps                         
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setPowerSaving(uint8_t mode)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t PS[]={0x77,0x04,0x00,0x25,0x00,mode};
   writeBytes(PS,6);  

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x25 && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}



/**********************************************************
Description:    Set Whilelist Address
Parameters:     whilelistaddr[]
                mask[]                
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setWhiteList(bool erase,uint8_t whiteListaddr[],uint8_t mask[])
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t i,a[1];
   if(erase==ENABLE)
    a[0]=0x10;
   else
    a[0]=0x00;
   uint8_t WL[128]={0x77,0x0F,a[0],0x2A,0x00};
   for(i=0;i<6;i++)
   {
    WL[5+i]=whiteListaddr[i];
   } 
   for(i=0;i<6;i++)
   {
    WL[11+i]=mask[i];
   }
   writeBytes(WL,17);  
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x2A && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set TX power
Parameters:     0~15                          
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setTXpower(uint8_t power)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t setpower[]={0x77,0x04,0x00,0x0B,0x00,power};
   writeBytes(setpower,6);

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x0B && 
                                    receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Set Xtal Cload 
Parameters:     0~15                        
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::setCrystalOffset(uint8_t cload)
{
   bool flag = false;
   uint32_t current_time,previous_time;
   uint8_t setcload[]={0x77,0x04,0x00,0x0E,0x00,cload};
   writeBytes(setcload,6);
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x0E && receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    software Reset,not save configuration 
Parameters:                          
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::reset()
{
  bool flag = false;
  uint32_t current_time,previous_time;
  uint8_t Wdtreset[6]= {0x77,0x04,0x00,0x28,0x00,0x00};
  writeBytes(Wdtreset,6);
  
  previous_time=millis();
  while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }

     
     if(readData(receive_data,len))
     {    
      if(receive_data[0]==0x00 && receive_data[1]==0x28 &&  receive_data[2]==0x00)
      {
        flag = true;
        delay(60);     
        break;
      }
      }    
    }
   return flag;
}



/**********************************************************
Description:    software Reset,save configuration 
Parameters:                         
Return:         true:reply true event
                false:reply false event
Others:        
**********************************************************/
bool BM7701_00_1::restoreDefault()
{
  bool flag = false; 
  uint32_t current_time,previous_time;
  uint8_t swreset[13]= {0x77,0x0B,0x00,0x28,0x00,0x32,0x30,0x32,0x36,0x00,0x03,0x00,0x00};
  writeBytes(swreset,13);
  previous_time=millis(); 
  while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[0]==0x00 && receive_data[1]==0x28 && receive_data[2]==0x00)
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}
/**********************************************************
Description:    Dummy wake up from deepsleep/powerdown mode 
Parameters:                
Return:      
Others:        
**********************************************************/
void BM7701_00_1::wakeUp()
{
     uint8_t buff[2];
     buff[0]=0x00;
     buff[1]=0x00;
     writeBytes(buff,2);
     delay(30);
}
/**********************************************************
Description:    Send Command to BM7701_00_1 with waiting evt
Parameters:     type 
                cmd_flag 
                cmd_length 
                value  :Data sent to the module
Return:         true:reply true event
                false:reply false event   
Others:        
**********************************************************/
bool BM7701_00_1::sendCMD(uint16_t type,uint8_t cmd_flag,uint8_t cmd_length,uint8_t value[])
{
   uint8_t i;
   uint32_t current_time,previous_time;
   bool flag = false;
   uint8_t a[2];
   a[0] = (uint8_t)type;
   a[1] = (uint8_t)(type>>8);
   cmd_length=cmd_length+3;
   uint8_t cmd[128]= {0x77,cmd_length,cmd_flag,a[0],a[1]};
   for(i=0;i<cmd_length;i++)
   {
    cmd[5+i]=value[i];
   }
   writeBytes(cmd,cmd_length+5);
   
   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[1]==a[0] && receive_data[2]==a[1])
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}

/**********************************************************
Description:    Send Command to BM7701_00_1 with no waiting evt
Parameters:     type 
                cmd_flag 
                cmd_length 
                value :Data sent to the module   
Return:         none
Others:        
**********************************************************/
void BM7701_00_1::sendCMD_NoResponse(uint16_t type,uint8_t cmd_flag,uint8_t cmd_length,uint8_t value[])
{
   uint8_t i;
   uint8_t a[2];
   a[0] = (uint8_t)type;
   a[1] = (uint8_t)(type>>8);
   cmd_length=cmd_length+3;
   uint8_t cmd[128]= {0x77,cmd_length,cmd_flag,a[0],a[1]};
   for(i=0;i<cmd_length;i++)
   {
    cmd[5+i]=value[i];
   }
   writeBytes(cmd,cmd_length+5); 
}



/**********************************************************
Description:    Read Command to BM7701_00_1 with waiting evt
Parameters:     type 
                cmd_flag 
Return:         true:reply true event
                false:reply false event   
Others:        
**********************************************************/
bool BM7701_00_1::readCMD(uint16_t type,uint8_t cmd_flag)
{
   uint32_t current_time,previous_time;
   bool flag = false;
   uint8_t a[2];
   a[0] = (uint8_t)type;
   a[1] = (uint8_t)(type>>8);
   uint8_t cmd[5]= {0x77,0x03,cmd_flag,a[0],a[1]};
   writeBytes(cmd,5);

   previous_time=millis();
   while(1)
   {
     current_time=millis();
     if((current_time - previous_time)>=EVT_RETURN_TIME)
        return flag;
     else if (current_time - previous_time <= 0)                   
        {   
          previous_time = current_time;
        }
     if(readData(receive_data,len))
     {
      if(receive_data[1]==a[0] && receive_data[2]==a[1])
      {
        flag = true;
        break;
      }
      }    
    }
   return flag;
}


/**********************************************************
Description:    Read Command to BM7701_00_1 with waiting no evt
Parameters:     type 
                cmd_flag   
Return:         true:reply true event
                false:reply false event   
Others:        
**********************************************************/
void BM7701_00_1::readCMD_NoResponse(uint16_t type,uint8_t cmd_flag)
{
   uint8_t a[2];
   a[0] = (uint8_t)type;
   a[1] = (uint8_t)(type>>8);
   uint8_t cmd[5]= {0x77,0x03,cmd_flag,a[0],a[1]};
   writeBytes(cmd,5); 
}
/**********************************************************
Description: Write data through UART
Parameters: wbuf:The array for storing Data to be sent
            wlen:Length of data sent
Return: None
Others: None
**********************************************************/
void BM7701_00_1::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  /* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
    _softSerial->flush(); 
  }
  /* Select HardwareSerial Interface */
  else
  {
    while (_serial->available() > 0)
    {
      _serial->read();
    }
    _serial->write(wbuf, wlen);
    _serial->flush(); 
  }
}
/**********************************************************
Description: Read data through UART
Parameters: rbuf: Used to store received data
            rlen: Length of data to be read
Return: 0: check ok
        1: check error
        2: timeout error
Others: None
**********************************************************/
uint8_t BM7701_00_1::readBytes(uint8_t rbuf[], uint8_t rlen,uint16_t timeout)
{
  uint8_t i=0,delayCnt=0;
  bool fReceive = false;
/* Select SoftwareSerial Interface */
 if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return 0x02; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }
/* Select HardwareSerial Interface */
  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_serial->available() == 0)
      {
        if (delayCnt > timeout)
        {
          return 0x02; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _serial->read();
    }
  }

  if(rbuf[1]!=0)
         {
            fReceive = true;
         }
         else 
         {fReceive = false;} 
         
    return  fReceive;
}

 
