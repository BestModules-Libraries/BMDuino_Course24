/*********************************************************************************************
File:       	  BMV31T001.cpp
Author:           YANG, BESTMODULE
Description:      single wire communicates with BMV31T001 and controls audio playback
History：		  
	V1.0.1	 -- initial version； 2022-11-01； Arduino IDE : v1.8.19

**********************************************************************************************/

#include "BMV31T001.h"
#include "SPI.h"


#define KEY_UP  	A1
#define KEY_LEFT  	A2
#define KEY_DOWN  	A3
#define KEY_RIGHT  	A4
#define KEY_MIDDLE  A5

#define POWER_PIN	A0
#define LED_PIN		8
#define STATUS_PIN	9
#define DATA  		12//Data line

#define PAUSE_PLAY    	0XF1	//Pause playing the current voice and sentence command
#define CONTINUE_PLAY   0XF2	//Continue playing the paused voice and sentence command
#define LOOP_PLAY    	0XF4	//Loop playback for the current voice and sentence command
#define STOP_PLAY     	0XF8	//Stop playing the current voice and sentence command


#define SPI_FLASH_PAGESIZE 256

#define CE         0x60  // Chip Erase instruction 
#define PP         0x02  // Page Program instruction 
#define READ       0x03  // Read from Memory instruction  
#define WREN       0x06  // Write enable instruction 
#define RDSR       0x05  // Read Status Register instruction 
#define	SFDP	   0x5a	 // Read SFDP.

#define WIP_FLAG   0x01  // Write In Progress (WIP) flag 
#define WEL_FLAG   0x02 // Write Enable Latch

#define DUMMY_BYTE 0xff

/************SPI PIN**************/
#define SEL 10  //cs


#define ICPCK 13
#define ICPDA 11

/*CRC8：x8+x5+x4+1，MSB*/
static const uint8_t crc_table[] =
{
    0x00, 0x31, 0x62, 0x53, 0xc4, 0xf5, 0xa6, 0x97, 0xb9, 0x88, 0xdb, 0xea, 0x7d, 0x4c, 0x1f, 0x2e,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xb6, 0xe5, 0xd4, 0xfa, 0xcb, 0x98, 0xa9, 0x3e, 0x0f, 0x5c, 0x6d,
    0x86, 0xb7, 0xe4, 0xd5, 0x42, 0x73, 0x20, 0x11, 0x3f, 0x0e, 0x5d, 0x6c, 0xfb, 0xca, 0x99, 0xa8,
    0xc5, 0xf4, 0xa7, 0x96, 0x01, 0x30, 0x63, 0x52, 0x7c, 0x4d, 0x1e, 0x2f, 0xb8, 0x89, 0xda, 0xeb,
    0x3d, 0x0c, 0x5f, 0x6e, 0xf9, 0xc8, 0x9b, 0xaa, 0x84, 0xb5, 0xe6, 0xd7, 0x40, 0x71, 0x22, 0x13,
    0x7e, 0x4f, 0x1c, 0x2d, 0xba, 0x8b, 0xd8, 0xe9, 0xc7, 0xf6, 0xa5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xbb, 0x8a, 0xd9, 0xe8, 0x7f, 0x4e, 0x1d, 0x2c, 0x02, 0x33, 0x60, 0x51, 0xc6, 0xf7, 0xa4, 0x95,
    0xf8, 0xc9, 0x9a, 0xab, 0x3c, 0x0d, 0x5e, 0x6f, 0x41, 0x70, 0x23, 0x12, 0x85, 0xb4, 0xe7, 0xd6,
    0x7a, 0x4b, 0x18, 0x29, 0xbe, 0x8f, 0xdc, 0xed, 0xc3, 0xf2, 0xa1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5b, 0x6a, 0xfd, 0xcc, 0x9f, 0xae, 0x80, 0xb1, 0xe2, 0xd3, 0x44, 0x75, 0x26, 0x17,
    0xfc, 0xcd, 0x9e, 0xaf, 0x38, 0x09, 0x5a, 0x6b, 0x45, 0x74, 0x27, 0x16, 0x81, 0xb0, 0xe3, 0xd2,
    0xbf, 0x8e, 0xdd, 0xec, 0x7b, 0x4a, 0x19, 0x28, 0x06, 0x37, 0x64, 0x55, 0xc2, 0xf3, 0xa0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xb2, 0xe1, 0xd0, 0xfe, 0xcf, 0x9c, 0xad, 0x3a, 0x0b, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xc0, 0xf1, 0xa2, 0x93, 0xbd, 0x8c, 0xdf, 0xee, 0x79, 0x48, 0x1b, 0x2a,
    0xc1, 0xf0, 0xa3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1a, 0x2b, 0xbc, 0x8d, 0xde, 0xef,
    0x82, 0xb3, 0xe0, 0xd1, 0x46, 0x77, 0x24, 0x15, 0x3b, 0x0a, 0x59, 0x68, 0xff, 0xce, 0x9d, 0xac
};



/************************************************************************* 
Description:    Constructor
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
BMV31T001::BMV31T001()
{
	_lastMillis = 0;
	_keyValue = 0;
	_isKey = 0;
	_flashAddr = 0;
}
/************************************************************************* 
Description:    Initialize communication between development board and BMV31T001
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::begin(void)
{
    reset();
    pinMode(POWER_PIN, OUTPUT);
    digitalWrite(POWER_PIN, LOW);	
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
    pinMode(DATA, OUTPUT);//DATA
	digitalWrite(DATA, HIGH);
    pinMode(ICPCK, INPUT);
    pinMode(STATUS_PIN, INPUT);
    //Key port
	pinMode(KEY_UP, INPUT_PULLUP);
	pinMode(KEY_LEFT, INPUT_PULLUP);
	pinMode(KEY_DOWN, INPUT_PULLUP);
	pinMode(KEY_RIGHT, INPUT_PULLUP);
	pinMode(KEY_MIDDLE, INPUT_PULLUP);

	delay(1000);//There's a delay here to get the BMV31T001 ready
}
/************************************************************************* 
Description:    Reset BMV31T001
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::reset(void)
{
    digitalWrite(POWER_PIN, LOW);
    delay(500);
    digitalWrite(POWER_PIN, HIGH);
}
/************************************************************************* 
Description:    Sends playback control commands
parameter:
    Input:          cmd：playback control commands
                    data : 0x00~0x7f is select the voice 0~127 to play if cmd is 0xfa
                           0x00~0x7f is select the voice 128~255 to play if cmd is 0xfb
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::writeCmd(uint8_t cmd, uint8_t data)
{
	delayMicroseconds(5000);
	uint8_t i, temp;
	temp = 0x01;
    
    if(0xff != data)
    {
        //start signal
        digitalWrite(DATA, LOW);
        delay(5);

        for (i = 0; i < 8; i ++)
        {
            if (1 == (cmd & temp))
            {
                    // out bit high
                    digitalWrite(DATA, HIGH);
                    delayMicroseconds(1200);
                    digitalWrite(DATA, LOW);
                    delayMicroseconds(400);
            }
            else
            {
                // out bit low
                digitalWrite(DATA, HIGH);
                delayMicroseconds(400);
                digitalWrite(DATA, LOW);
                delayMicroseconds(1200);
            }
            cmd >>= 1;
        }
        digitalWrite(DATA, HIGH);
        delay(5);
        //start signal
        digitalWrite(DATA, LOW);
        delay(5);

        for (i = 0; i < 8; i ++)
        {
            if (1 == (data & temp))
            {
                    // out bit high
                    digitalWrite(DATA, HIGH);
                    delayMicroseconds(1200);
                    digitalWrite(DATA, LOW);
                    delayMicroseconds(400);
            }
            else
            {
                // out bit low
                digitalWrite(DATA, HIGH);
                delayMicroseconds(400);
                digitalWrite(DATA, LOW);
                delayMicroseconds(1200);
            }
            data >>= 1;
        }
        digitalWrite(DATA, HIGH);
        delay(5);
    }
    else
    {
        //start signal
        digitalWrite(DATA, LOW);
        delay(5);

        for (i = 0; i < 8; i ++)
        {
            if (1 == (cmd & temp))
            {
                    // out bit high
                    digitalWrite(DATA, HIGH);
                    delayMicroseconds(1200);
                    digitalWrite(DATA, LOW);
                    delayMicroseconds(400);
            }
            else
            {
                // out bit low
                digitalWrite(DATA, HIGH);
                delayMicroseconds(400);
                digitalWrite(DATA, LOW);
                delayMicroseconds(1200);
            }
            cmd >>= 1;
        }
        digitalWrite(DATA, HIGH);
        delay(5);        
    }

}
/************************************************************************* 
Description:    Set the volume
parameter:
    Input:          volume：0~11(0:minimum volume（mute）;11:maximum volume)
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::setVolume(uint8_t volume)
{
	writeCmd(0xe1 + volume);
}
/************************************************************************* 
Description:    Play voice.
parameter:
    Input:			num：VOC_01~VOC_256(Enter the number according to the number of the voice 
                    generated by the PC tool.The maximum number is VOC_256. )
                    loop：default 0.(1：Loops the current voice，0：Play it only once) 
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::playVoice(uint8_t num, uint8_t loop)
{
    if(num < 128)
    {
        writeCmd(0xfa, num);
    }
    else
    {
        writeCmd(0xfb, num % 128);
    }
	
	if(loop)
	{
		writeCmd(0xf4);
	}
}
/************************************************************************* 
Description:    Play sentence.
parameter:
    Input:			num：SEN_01~SEN_96(Enter the number according to the number of the sentence 
                    generated by the PC tool.The maximum number is SEN_96. )
                    loop：default 0.(1：Loops the current sentence，0：Play it only once)          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::playSentence(uint8_t num, uint8_t loop)
{
	writeCmd(num);
	if(loop)
	{
		writeCmd(0xf4);
	}
}
/************************************************************************* 
Description:    Stop playing the current voice and sentence.
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::playStop(void)
{
	writeCmd(STOP_PLAY);
}
/************************************************************************* 
Description:    Pause playing the current voice and sentence.
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::playPause(void)
{
	writeCmd(PAUSE_PLAY);
}
/************************************************************************* 
Description:    Continue playing the paused voice and sentence.
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::playContinue(void)
{
	writeCmd(CONTINUE_PLAY);
}
/************************************************************************* 
Description:    Loop playback the current voice/sentence
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::playRepeat(void)
{
    writeCmd(LOOP_PLAY);
}
/************************************************************************* 
Description:    Get the play status
parameter:
    Input:          
    Output:         
Return:         0:Not in the play
				1:In the play
Others:         
*************************************************************************/
bool BMV31T001::isPlaying(void)
{
	if (0 == digitalRead(STATUS_PIN))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/************************************************************************* 
Description:    Scanning key
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::scanKey(void)
{
    static uint8_t step = 0;
    static uint8_t currentKey = 0;
    static uint8_t lastKey = 0;
    if((millis() - _lastMillis) > 10)
    {
        _lastMillis = millis();
        currentKey = 0;
        if(digitalRead(KEY_MIDDLE) == LOW)
        {
            currentKey |= 0X01;
        }
        if(digitalRead(KEY_UP) == LOW)
        {
            currentKey |= 0X02;
        }
        if(digitalRead(KEY_DOWN) == LOW)
        {
            currentKey |= 0X04;
        }
        if(digitalRead(KEY_LEFT) == LOW)
        {
            currentKey |= 0X08;
        }
        if(digitalRead(KEY_RIGHT) == LOW)
        {
            currentKey |= 0X10;
        }
        switch(step)
        {
            case 0:
                if(currentKey != lastKey)
                {
                    step = 1;
                }
                return;
            case 1:
                if(currentKey != lastKey)
                {
                    _keyValue = currentKey;
                    _isKey = 1;
                }
                step = 0;
                break;
        }
        lastKey = currentKey;
    }
}
/************************************************************************* 
Description:    Determine if any keys are pressed
parameter:
    Input: 
    Output:     
Return:         0: No keys were pressed; 1: there are keys that are pressed
Others:         
*************************************************************************/
bool BMV31T001::isKeyAction(void)
{
    if(_isKey)
    {
        _isKey = 0;
        return 1;     
    }
	else 
    {
        return 0;
    }
}
/************************************************************************* 
Description:    Read the key level value
parameter:
    Input:          
    Output:         
Return:         0: pressing; 1: no pressing
Others:         
*************************************************************************/
uint8_t BMV31T001::readKeyValue(void)
{
	return _keyValue;
}
/************************************************************************* 
Description:    Set the power up or down of the BMV31T001
parameter:
    Input:          status: 0:power down; 1:power up
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::setPower(uint8_t status)
{
	digitalWrite(POWER_PIN, status);
}
/************************************************************************* 
Description:    Set the onboard LED on or off
parameter:
    Input:          status: 0:LED off; 1:LED on
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::setLED(uint8_t status)
{
	digitalWrite(LED_PIN, !status);
}
/************************************************************************* 
Description:    Update your audio source with Ardunio
parameter:
    Input:          boardType: 0: BMduino UNO; 1: Arduino UNO
                    baudrate：Updated baud rate
    Output:         
Return:         
Others:         
*************************************************************************/
#if defined(__AVR_ATmega328P__)
void BMV31T001::initAudioUpdate(unsigned long baudrate)
{
    pinMode(DATA, OUTPUT);
    digitalWrite(DATA, HIGH);
    Serial.begin(baudrate);
}
#elif defined(ARDUINO_HT32_USB)
void BMV31T001::initAudioUpdate(unsigned long baudrate)
{
    pinMode(DATA, OUTPUT);
    digitalWrite(DATA, HIGH);
	SerialUSB.begin(baudrate);
}
#endif
/************************************************************************* 
Description:    Get the update sound source signal
parameter:
    Input:         		
    Output:         
Return:         1：execute update; 0：not execute update
Others:         
*************************************************************************/
#if defined(__AVR_ATmega328P__)
bool BMV31T001::isUpdateBegin(void)
{
    if (Serial.available())
    {
        return 1;
    }
    else
    {
        return 0;
    }        
}
#elif defined(ARDUINO_HT32_USB)
bool BMV31T001::isUpdateBegin(void)
{
    if (SerialUSB.available())
    {
        return 1;
    }
    else
    {
        return 0;
    }        
}
#endif

/************************************************************************* 
Description:    Update the audio source
parameter:
    Input:         		
    Output:         
Return:         1:Update completed; 0:Update failed 
Others:         
*************************************************************************/
#if defined(__AVR_ATmega328P__)
bool BMV31T001::executeUpdate(void)
{
    static int8_t dataLength = 0;
    uint32_t delayCount = 0;
    while(1)
    {
        if (Serial.available())
        {
            delayCount = 0;
            Serial.readBytes(rxBuffer, 3);   
            if ((0xAA == rxBuffer[0]) && (0x23 == rxBuffer[1]))
            {
                dataLength = rxBuffer[2];
                Serial.readBytes(rxBuffer + 3, dataLength + 2);  
                if (rxBuffer[dataLength + 3] == checkCRC8(rxBuffer + 2, dataLength + 1))
                {
                    if (6 == dataLength)
                    {
                        if ((rxBuffer[3] == 'C') && (rxBuffer[4] == 'O') && (rxBuffer[5] == 'M')
                        && (rxBuffer[6] == 'S') && (rxBuffer[7] == 'P') && (rxBuffer[8] == 'I'))
                        {
                            if (false == switchSPIMode())
                            {
                            
                                Serial.write(0xe3);
                                digitalWrite(POWER_PIN, LOW);
                                delay(500);
                                digitalWrite(POWER_PIN, HIGH);    

                                _flashAddr = 0;
                                pinMode(DATA, OUTPUT);
                                digitalWrite(DATA, HIGH);
                                pinMode(STATUS_PIN, INPUT);
                                pinMode(ICPDA, OUTPUT);
                                digitalWrite(ICPDA, HIGH);
                                pinMode(ICPCK, INPUT);
                            }
                            else
                            {
                                Serial.write(0x3e);//ACK
                            }
                            
                        }
                        else if ((rxBuffer[3] == 'C') && (rxBuffer[4] == 'O') && (rxBuffer[5] == 'M')
                        && (rxBuffer[6] == 'O') && (rxBuffer[7] == 'R') && (rxBuffer[8] == 'D'))
                        {
                            Serial.write(0x3e);//ACK

                            digitalWrite(POWER_PIN, LOW);
                            delay(500);
                            digitalWrite(POWER_PIN, HIGH);                
                            _flashAddr = 0;
                            SPI.end();
                            pinMode(DATA, OUTPUT);
                            digitalWrite(DATA, HIGH);
                            pinMode(STATUS_PIN, INPUT);
                            pinMode(ICPDA, OUTPUT);
                            digitalWrite(ICPDA, HIGH);
                            pinMode(ICPCK, INPUT);
                            delay(10);
                            return 1;
                        }
                    }
                    else if (5 == dataLength)
                    {
                        if ((rxBuffer[3] == 'C') && (rxBuffer[4] == 'O') && (rxBuffer[5] == 'M')
                        && (rxBuffer[6] == 'C') && (rxBuffer[7] == 'E'))
                        {
                            SPIFlashChipErase();
                            Serial.write(0x3e);//ACK
                        }
                    }       
                }
                else
                {
                    Serial.write(0xe3);//NACK
                }
            }
            else
            {
                recAudioData();
            }             
        }
        delayCount++;
        delayMicroseconds(50);//waiting for receive data 
        if(delayCount>=2000)
        {
            return 0;//timeout is 50us*200=100ms,nothing for receive
        }
    }
}
#elif defined(ARDUINO_HT32_USB)
bool BMV31T001::executeUpdate(void)
{
    static int8_t dataLength = 0;
    uint32_t delayCount = 0;
    while(1)
    {
        if(SerialUSB.available())
        {
            delayCount = 0;
            SerialUSB.readBytes(rxBuffer, 3);   
            if ((0xAA == rxBuffer[0]) && (0x23 == rxBuffer[1]))
            {
                dataLength = rxBuffer[2];
                SerialUSB.readBytes(rxBuffer + 3, dataLength + 2);                 
                if (rxBuffer[dataLength + 3] == checkCRC8(rxBuffer + 2, dataLength + 1))
                {
                    if (6 == dataLength)
                    {
                        if ((rxBuffer[3] == 'C') && (rxBuffer[4] == 'O') && (rxBuffer[5] == 'M')
                        && (rxBuffer[6] == 'S') && (rxBuffer[7] == 'P') && (rxBuffer[8] == 'I'))
                        {
                            if (false == switchSPIMode())
                            {
                            
                                SerialUSB.write(0xe3);
                                digitalWrite(POWER_PIN, LOW);
                                delay(500);
                                digitalWrite(POWER_PIN, HIGH);    

                                _flashAddr = 0;
                                pinMode(DATA, OUTPUT);
                                digitalWrite(DATA, HIGH);
                                pinMode(STATUS_PIN, INPUT);
                                pinMode(ICPDA, OUTPUT);
                                digitalWrite(ICPDA, HIGH);
                                pinMode(ICPCK, INPUT);
                            }
                            else
                            {
                                SerialUSB.write(0x3e);//ACK
                            }
                            
                        }
                        else if ((rxBuffer[3] == 'C') && (rxBuffer[4] == 'O') && (rxBuffer[5] == 'M')
                        && (rxBuffer[6] == 'O') && (rxBuffer[7] == 'R') && (rxBuffer[8] == 'D'))
                        {
                            SerialUSB.write(0x3e);//ACK

                            digitalWrite(POWER_PIN, LOW);
                            delay(500);
                            digitalWrite(POWER_PIN, HIGH);                
                            _flashAddr = 0;
                            SPI.end();
                            pinMode(DATA, OUTPUT);
                            digitalWrite(DATA, HIGH);
                            pinMode(STATUS_PIN, INPUT);
                            pinMode(ICPDA, OUTPUT);
                            digitalWrite(ICPDA, HIGH);
                            pinMode(ICPCK, INPUT);
                            delay(10);
                            return 1;
                        }
                    }
                    else if (5 == dataLength)
                    {
                        if ((rxBuffer[3] == 'C') && (rxBuffer[4] == 'O') && (rxBuffer[5] == 'M')
                        && (rxBuffer[6] == 'C') && (rxBuffer[7] == 'E'))
                        {
                            SPIFlashChipErase();
                            SerialUSB.write(0x3e);//ACK
                        }
                    }       
                }
                else
                {
                    SerialUSB.write(0xe3);//NACK
                }

            }
            else
            {
                recAudioData();
            }
        }
        delayCount++;
        delayMicroseconds(50);//waiting for receive data 
        if(delayCount>=2000)
        {
            return 0;//timeout is 50us*2000=100ms,nothing for receive
        }
    }
}
#endif
/************************************************************************* 
Description:    Receive audio data update from upper computer into BMV31T001
parameter:
    Input:          
    Output:         
Return:         1:Correct reception; 0:Error of reception
Others:         
*************************************************************************/
#if defined(__AVR_ATmega328P__)
void BMV31T001::recAudioData(void)
{
    static int8_t dataLength = 0;
    static uint8_t remainder = 0;
    static uint32_t sumDataCnt = 0;
    if ((0x55 == rxBuffer[0]) && (0x23 == rxBuffer[1]))
    {
        dataLength = rxBuffer[2];
        Serial.readBytes(rxBuffer + 3, dataLength + 2);  
        if (rxBuffer[dataLength + 3] == checkCRC8(rxBuffer + 2, dataLength + 1))
        {
            sumDataCnt += dataLength;
            remainder = sumDataCnt % 64;
            if (remainder <= 59)
            {
                SPIFlashPageWrite(rxBuffer + 3, _flashAddr, dataLength - remainder);
                SPIFlashPageWrite(rxBuffer + 3 + dataLength - remainder, _flashAddr + dataLength - remainder, remainder);
                
            }
            else
            {
                SPIFlashPageWrite(rxBuffer + 3, _flashAddr, dataLength);
            }
                
            _flashAddr += dataLength;
            Serial.write(0x3e);//ACK
        }
        else
        {
            Serial.write(0xe3);//NACK
            return;
        }
    }       
}
#elif defined(ARDUINO_HT32_USB)
void BMV31T001::recAudioData(void)
{
    static int8_t dataLength = 0;
    static uint8_t remainder = 0;
    static uint32_t sumDataCnt = 0;
    if ((0x55 == rxBuffer[0]) && (0x23 == rxBuffer[1]))
    {
        dataLength = rxBuffer[2];
        SerialUSB.readBytes(rxBuffer + 3, dataLength + 2);  
        if (rxBuffer[dataLength + 3] == checkCRC8(rxBuffer + 2, dataLength + 1))
        {
            sumDataCnt += dataLength;
            remainder = sumDataCnt % 64;
            if (remainder <= 59)
            {
                SPIFlashPageWrite(rxBuffer + 3, _flashAddr, dataLength - remainder);
                SPIFlashPageWrite(rxBuffer + 3 + dataLength - remainder, _flashAddr + dataLength - remainder, remainder);
                
            }
            else
            {
                SPIFlashPageWrite(rxBuffer + 3, _flashAddr, dataLength);
            }
                
            _flashAddr += dataLength;
            SerialUSB.write(0x3e);//ACK
        }
        else
        {
            SerialUSB.write(0xe3);//NACK
            return;
        }
    }
}
#endif

/************************************************************************* 
Description:    Enter update mode
parameter:
    Input:          
    Output:         
Return:         1:Enter mode successfully
                0:Failed to enter mode
Others:         
*************************************************************************/
bool BMV31T001::programEntry(uint16_t mode)
{
    static uint8_t retransmissionTimes = 0;
	
    digitalWrite(POWER_PIN, LOW);
    pinMode(STATUS_PIN, OUTPUT);
    digitalWrite(STATUS_PIN, LOW);
    pinMode(DATA, OUTPUT);
    digitalWrite(DATA, LOW);
    pinMode(SEL, OUTPUT);
    digitalWrite(SEL, LOW);
    pinMode(ICPCK, OUTPUT);
    digitalWrite(ICPCK, LOW);
    pinMode(ICPDA, OUTPUT);
    digitalWrite(ICPDA, LOW);
    
    delay(10);
    pinMode(STATUS_PIN, OUTPUT);
    digitalWrite(STATUS_PIN, LOW);
    pinMode(ICPCK, OUTPUT);
    digitalWrite(ICPCK, LOW);
    pinMode(ICPDA, OUTPUT);
    digitalWrite(ICPDA, LOW);
    delay(5);
    digitalWrite(ICPCK, LOW);
    pinMode(STATUS_PIN, INPUT);
    delay(1);
    digitalWrite(POWER_PIN, HIGH);
    digitalWrite(ICPCK, HIGH);
    delay(2);
    digitalWrite(ICPDA, HIGH);
    do{
        /*READY*/
        digitalWrite(ICPCK, LOW);
        delayMicroseconds(160);//tready:150us~

        /*MATCH*/
        digitalWrite(ICPCK, HIGH);
        delayMicroseconds(84);//tmatch:60us~
        /*Match Pattern and set mode:0100 1010 1xxx*/
        matchPattern(mode);
        retransmissionTimes++;
        if(5 == retransmissionTimes)
        {
            retransmissionTimes = 0;
            return false;
        }
    }while(mode != ack());
    dummyClocks();
    retransmissionTimes = 0;
    return true;
}
/************************************************************************* 
Description:    ack of mode
parameter:
    Input:          
    Output:         
Return:         mode data
Others:         
*************************************************************************/
uint16_t BMV31T001::ack(void)
{
    /*MSB*/
    static uint8_t i;
    uint16_t ackData = 0;
    pinMode(ICPDA, INPUT);
    digitalWrite(ICPCK, LOW);
    for (i = 0; i < 3; i++)
    {
        digitalWrite(ICPCK, HIGH);
        digitalWrite(ICPCK, LOW);
        if (HIGH == digitalRead(ICPDA))
        {
             ackData |= (0x04 >> i);
        }
        else
        {
            ackData &= ~(0x04 >> i);
        }
        delayMicroseconds(5);
    }
    
    digitalWrite(ICPCK, HIGH);
    pinMode(ICPDA, OUTPUT);
    return ackData;
}
/************************************************************************* 
Description:    Send the dummy Clocks
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::dummyClocks(void)
{
    static uint16_t i;
    for (i = 0; i < 512; i++)
    {
        digitalWrite(ICPCK, LOW);
        delayMicroseconds(1);
        digitalWrite(ICPCK, HIGH);
        delayMicroseconds(1);    
    }
}
/************************************************************************* 
Description:    Send data bit in high
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::programDataOut1(void)
{
    digitalWrite(ICPDA, HIGH);
    delayMicroseconds(1);
    digitalWrite(ICPCK, LOW);  
    delayMicroseconds(1);//tckl:1~15us
    digitalWrite(ICPCK, HIGH);

}
/************************************************************************* 
Description:    Send data bit in low
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::programDataOut0(void)
{
    digitalWrite(ICPDA, LOW);
    delayMicroseconds(1);
    digitalWrite(ICPCK, LOW);
    delayMicroseconds(1);//tckl:1~15us
    digitalWrite(ICPCK, HIGH);
}
/************************************************************************* 
Description:    Send address bit in high
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::programAddrOut1(void)
{
    /*at entry mode :tckl+tckh < 15us*/
    digitalWrite(ICPDA, HIGH);
    digitalWrite(ICPCK, LOW);  
    delayMicroseconds(1);//tckl:1~15us
    digitalWrite(ICPCK, HIGH);
    delayMicroseconds(4);//tckh:1~15us
}
/************************************************************************* 
Description:    Send address bit in low
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::programAddrOut0(void)
{
    digitalWrite(ICPDA, LOW);
    digitalWrite(ICPCK, LOW);
    delayMicroseconds(1);//tckl:1~15us
    digitalWrite(ICPCK, HIGH);
    delayMicroseconds(4);//tckh:1~15us
}
/************************************************************************* 
Description:    Pattern(mode) matching
parameter:
    Input:          mode:0x02
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::matchPattern(uint16_t mode)
{
    uint16_t i, temp, pattern, mData;
    pattern = 0x4A8;//0100 1010 1000:low 3 bits are mode; high 9 bits are fixed
    mData = (pattern | mode) << 4;
	temp = 0x8000;//MSB

	for (i = 0; i < 12; i++)
	{
		if(mData&temp)
			programDataOut1();
		else
			programDataOut0();
			
		mData <<= 1;
		
	}
    digitalWrite(ICPDA, HIGH);
}
/************************************************************************* 
Description:    Send the address
parameter:
    Input:          addr:The address to which data is written
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::sendAddr(uint16_t addr)
{
    pinMode(ICPDA, OUTPUT);
    digitalWrite(ICPDA, HIGH);
    /*LSB*/
	uint16_t i, temp;
	temp = 0x0001;//LSB
	
	for (i = 0; i < 12; i++)
	{
		if (addr & temp)
			programAddrOut1();
		else
			programAddrOut0();
			
		addr >>= 1;
		
	}
}
/************************************************************************* 
Description:    Send the data
parameter:
    Input:          data:Data sent to the BMV31T001 at a fixed address
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::sendData(uint16_t data)
{
    pinMode(ICPDA, OUTPUT);
	uint16_t i, temp;
	temp = 0x0001;//LSB

	for (i = 0; i < 14; i++)
	{
		if (data & temp)
			programDataOut1();
		else
			programDataOut0();
			
		data >>= 1;		
	}
    delayMicroseconds(1);
    digitalWrite(ICPCK, LOW);
    delayMicroseconds(1);
    digitalWrite(ICPCK, HIGH);
    delayMicroseconds(2000);
	digitalWrite(ICPCK, LOW);
    delayMicroseconds(1);
    digitalWrite(ICPCK, HIGH);
    delayMicroseconds(5);
}
/************************************************************************* 
Description:    Send the data
parameter:
    Input:          data:Data read from the BMV31T001 at a fixed address
    Output:         
Return:         
Others:         
*************************************************************************/
uint16_t BMV31T001::readData(void)
{
    /*LSB*/
	uint8_t i;
    uint16_t rxData = 0;
    pinMode(ICPDA, INPUT);
    digitalWrite(ICPCK, LOW);    	
    for (i = 0; i < 14; i++)
    {
        digitalWrite(ICPCK, LOW);
        if (HIGH == digitalRead(ICPDA))
        {
            rxData |= (0x01 << i);
        }
        else
        {
            rxData &= ~(0x01 << i);
        }
        digitalWrite(ICPCK, HIGH);
        delayMicroseconds(2);
    }
    digitalWrite(ICPCK, HIGH);//15th
    delayMicroseconds(2);
    digitalWrite(ICPCK, LOW);
    delayMicroseconds(1);
    digitalWrite(ICPCK, HIGH);//16th
    delayMicroseconds(2000);
    digitalWrite(ICPCK, LOW);
    delayMicroseconds(1);
    digitalWrite(ICPCK, HIGH);
    return rxData;
}
/************************************************************************* 
Description:    Switch SPI Mode
parameter:
    Input:          
    Output:         
Return:         1:Switch successfully
                1:Fail to switch
Others:         
*************************************************************************/
bool BMV31T001::switchSPIMode(void)
{
    static uint8_t correctFlag = 0;
    static uint8_t retransmissionTimes = 0;
    if (false == programEntry(0x02))
    {
        return false;
    }
    sendAddr(0x0020);
    sendData(0x0000);
    sendData(0x0000);
    sendData(0x0007);
    sendData(0x0000);    



    SPI.begin();
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    delay(10);
    do{     
		SPIFlashReadSFDP(deviceSFDPBuf,0,4);
 
        if ((0x53 == deviceSFDPBuf[0]) && (0x46 == deviceSFDPBuf[1]) 
			&& (0x44 == deviceSFDPBuf[2]) && (0x50 == deviceSFDPBuf[3]))      
        {
            correctFlag = 1;
        }
        retransmissionTimes++;
        if (3 == retransmissionTimes)
        {
            retransmissionTimes = 0;
            return false;
        }
    }while(0 == correctFlag);
	correctFlag = 0;
    retransmissionTimes = 0;
    return true;
}
/************************************************************************* 
Description:    Data check
parameter:
    Input:          *ptr:The array to check
                    len:Length of data to be check
    Output:         
Return:         1:correct
                0:error
Others:         
*************************************************************************/
uint8_t BMV31T001::checkCRC8(uint8_t *ptr, uint8_t len) 
{
    uint8_t  crc = 0x00;

    while (len--)
    {
        crc = crc_table[crc ^ *ptr++];
    }
    return (crc);
}
/************************************************************************* 
Description:    Enables the write access to the FLASH.
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::SPIFlashWriteEnable(void)
{
      /* Select the FLASH: Chip Select low */
      digitalWrite(SEL, LOW);

      /* Send instruction */
      SPI.transfer(WREN);

      /* Deselect the FLASH: Chip Select high */
      digitalWrite(SEL, HIGH);
}
/************************************************************************* 
Description:    Polls the status of the Write In Progress (WIP) flag in 
                the FLASH's status register and loop until write  opertaion has completed.
parameter:                
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::SPIFlashWaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;

    /* Select the FLASH: Chip Select low */
    digitalWrite(SEL, LOW);	

    /* Send "Read Status Register" instruction */
    SPI.transfer(RDSR);
    /* Loop as long as the memory is busy with a write cycle */
    do
    {
    /* Send a dummy byte to generate the clock needed by the FLASH 
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI.transfer(DUMMY_BYTE);

    } while((FLASH_Status & WIP_FLAG) == 1); /* Write in progress */
    /* Deselect the FLASH: Chip Select high */
    digitalWrite(SEL, HIGH);	
}
/************************************************************************* 
Description:    Erases the entire FLASH.
parameter:
    Input:          
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::SPIFlashChipErase(void)
{
  /* Send write enable instruction */
  SPIFlashWriteEnable();

  /* Bulk Erase */ 
  /* Select the FLASH: Chip Select low */
  digitalWrite(SEL, LOW);
  /* Send Chip Erase instruction  */
  SPI.transfer(CE);
  /* Deselect the FLASH: Chip Select high */
  digitalWrite(SEL, HIGH);	

  /* Wait the end of Flash writing */
  SPIFlashWaitForWriteEnd();
}
/************************************************************************* 
Description:    Writes more than one byte to the FLASH with a single WRITE cycle(Page WRITE sequence). 
                The number of byte can't exceed the FLASH page size.
parameter:
    Input:          pBuffer : pointer to the buffer  containing the data to be written to the FLASH.
                    writeAddr : FLASH's internal address to write to.
                    numByteToWrite : number of bytes to write to the FLASH, must be equal or less 
                                    than "SPI_FLASH_PAGESIZE" value.
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::SPIFlashPageWrite(uint8_t* pBuffer, uint32_t writeAddr, uint16_t numByteToWrite)
{
  /* Enable the write access to the FLA
  SH */
  SPIFlashWriteEnable();
  /* Select the FLASH: Chip Select low */
  digitalWrite(SEL, LOW);
  /* Send "Write to Memory " instruction */
  SPI.transfer(PP);
  /* Send writeAddr high nibble address byte to write to */
  SPI.transfer((writeAddr & 0xFF0000) >> 16);
  /* Send writeAddr medium nibble address byte to write to */
  SPI.transfer((writeAddr & 0xFF00) >> 8);  
  /* Send writeAddr low nibble address byte to write to */
  SPI.transfer(writeAddr & 0xFF);
  
  /* while there is data to be written on the FLASH */
  while(numByteToWrite--) 
  {
    /* Send the current byte */
    SPI.transfer(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++; 
  }
  
  /* Deselect the FLASH: Chip Select high */
  digitalWrite(SEL, HIGH);	
  /* Wait the end of Flash writing */
  SPIFlashWaitForWriteEnd();
}
/************************************************************************* 
Description:    Read SFDP.
parameter:
    Input:          pBuffer : pointer to the buffer that receives the data read from the FLASH.
                    ReadAddr : FLASH's internal address to read from.
                    NumByteToRead : number of bytes to read from the FLASH.
    Output:         
Return:         
Others:         
*************************************************************************/
void BMV31T001::SPIFlashReadSFDP(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /* Select the FLASH: Chip Select low */
    digitalWrite(SEL,LOW);	

    /* Send "Read from Memory " instruction */
    SPI.transfer(SFDP);

    /* Send ReadAddr high nibble address byte to read from */
    SPI.transfer((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    SPI.transfer((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    SPI.transfer(ReadAddr & 0xFF);
	/* Send 1 byte dummy clock */
	SPI.transfer(DUMMY_BYTE);

    //SPI_FIFOReset(SPIx, SPI_FIFO_RX);

    while(NumByteToRead--) /* while there is data to be read */
    {
		/* Read a byte from the FLASH */
		*pBuffer = SPI.transfer(DUMMY_BYTE);
		/* Point to the next location where the byte read will be saved */
		pBuffer++;
    }

    /* Deselect the FLASH: Chip Select high */
    digitalWrite(SEL,HIGH);	
}



