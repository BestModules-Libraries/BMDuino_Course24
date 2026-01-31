/*****************************************************************
File:        readTemperatureAndPressureWithIIC
Description: Power on to obtain the Device_ID once,and then 
             repeatedly obtain the Temperature and Pressure value
             through IIC and display the value in the theSerial port.
******************************************************************/
#include "BM62S2201-1.h"
BM62S2201_1 Press(&Wire1);
uint8_t devIDbuff[8];

void setup()
{  
   Serial.begin(9600);
   Press.begin();

   uint8_t chk = Press.getDevID(devIDbuff);
   if(chk != READ_OK)
   {
        switch(chk)
        {
          case READ_ERROR:Serial.println("Read sensor: CheckSum error");break;
          case NO_ACK    :Serial.println("Read sensor: NO_ACK");        break;
          default        :Serial.println("Read sensor: Unknown error"); break;
        } 
        Serial.println("Please check connection...");
   }
   else
   {
       Serial.println("Read sensor: OK"); 
       Serial.print("DeviceID : ");
       for(int i = 0; i<8 ; i++)
       {
          Serial.print(devIDbuff[i],HEX);
          Serial.print(" ");
       }
       Serial.println();
   } 
   delay(1000); 
}

void loop()
{
      Serial.print("Press:");
      Serial.print(Press.readPressure());
      Serial.print(" mpsi   ");
      Serial.print("Temp:");
      Serial.print(Press.readTemperature());
      Serial.println(" ℃");   
      delay(1000);
}
