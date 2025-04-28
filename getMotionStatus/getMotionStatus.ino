/*****************************************************************
File:         getMotionStatus.ino
Description:  1.Wire1 interface is used to communicate with BMS81M001.
              2.hardware Serial (BAUDRATE 125000) is used to communicate with Serial port monitor.
              3.when motion is detected printf"Motion detected!".
******************************************************************/

#include "BMS81M001.h"
BMS81M001 WakeOnShake(8);

uint8_t thr;        //show motion threshold setting
uint8_t dur;        //show motion duration setting
uint8_t halt_delay; //show idle mode delay setting

void setup()
{
    WakeOnShake.begin();   
    Serial.begin(115200);                      //Serial Monitor  
    delay(200);                            //Module initialization delay    
      if(WakeOnShake.getFWVer())        
      {
        Serial.print("Fermware version :");
        Serial.println(WakeOnShake.getFWVer(),HEX);    
      } 
    else
     {
         Serial.println("Fermware reading fail!");
     }
    
    if(!(WakeOnShake.getParameterSetting(thr,dur,halt_delay))) //Read the vibration settings and display them
    {
        Serial.print("Threshold=");
        Serial.print(thr);
        Serial.print(" Duration=");
        Serial.print(dur);
        Serial.print(" Delay=");
        Serial.println(halt_delay);
        Serial.println("Module is OK!");   
    }
    else
    {
        Serial.println(" Motion reading fail!");
    } 
    
 }                          
uint32_t counter=0;
void loop()
{ 
    if(WakeOnShake.getStatus() == 0)  
     {
        if(WakeOnShake.getShakeStatus())    //Read the state of shock
        {
            counter++;
            Serial.println("Motion detected!  "+ (String)counter);   //Tremors occur
        }       
     }

}
