#include "BMB22M210.h"

//BMB22M210 mySerial(22,&Serial1);   //Please uncomment out this line of code if you use HW Serial1 on BMduino
BMB22M210 mySerial(25,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMB22M210 mySerial(3,&Serial3);  //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMB22M210 mySerial(3,&Serial4);  //Please uncomment out this line of code if you use HW Serial4 on BMduino



void initUARTEXT()
{
  mySerial.beginPort(1,115200);   //set uart port1 baud：9600
}
