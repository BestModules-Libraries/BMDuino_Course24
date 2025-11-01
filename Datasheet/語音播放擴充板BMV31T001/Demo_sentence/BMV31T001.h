/*************************************************************************
File:       	  BMV31T001.h
Author:           YANG, BESTMODULE
Description:      Define classes and required variables
History：		  -
	V1.0.1	 -- initial version； 2022-11-01； Arduino IDE : v1.8.13

**************************************************************************/
#ifndef _BMV31T001_H
#define _BMV31T001_H

#include "Arduino.h"
#include <stdio.h>
#include <math.h>
/*************************playback control command***************************************************************************************
 * Play voice                                00H~7FH ——> when the 0xfa command is used,00H:is voice 0； from 0 to 127;
                                                         when the 0xfa cammand is used ,00H:is voice 128;from 128 to 255.
 * Play sentence                             80H~DFH ——> 80H:is sentence 0；from 0 to 95, there are 96 sentences.
 * Volume selection                          E1H~ECH ——> E1H:is the minimum volume（mute）；There are 12 levels of volume adjustment.
 * Pause voice/sentence                      F1H     ——> Pause playing the current voice and sentence.
 * Play after pause                          F2H      ——> Continue playing the paused voice and sentence.
 * Loop playback the current voice/sentence  F4H      ——> Loop playback for the current voice and sentence.
 * Stop playing the current voice/sentence   F8H      ——> Stop playing the current voice and sentence.
**************************************************************************************************************************************/

#define BMV31T001_KEY_MIDDLE 	0X01
#define BMV31T001_KEY_UP 	 	0X02
#define BMV31T001_KEY_DOWN	 	0X04
#define BMV31T001_KEY_LEFT	 	0X08
#define BMV31T001_KEY_RIGHT  	0X10

#define BMV31T001_LED_ON	 	1
#define BMV31T001_LED_OFF    	0

#define BMV31T001_BUSY	 	 	1
#define BMV31T001_NOBUSY     	0

#define BMV31T001_POWER_ENABLE	1	 	 
#define BMV31T001_POWER_DISABLE 0

#define BMV31T001_UPDATA_BEGIN  1
#define BMV31T001_NO_KEY		0

#define BMV31T001_VOLUME_MAX     11
#define BMV31T001_VOLUME_MIN	 0



class BMV31T001
{
public:
	BMV31T001();
	void begin(void);
    void reset(void);
	//play funtion
	void setVolume(uint8_t volume);
	void playVoice(uint8_t num, uint8_t loop = 0);
	void playSentence(uint8_t num, uint8_t loop = 0);
	void playStop(void);
	void playPause(void);
	void playContinue(void);
	void playRepeat(void);
	bool isPlaying(void);
	//key funtion
	void scanKey(void);
	bool isKeyAction(void);
	uint8_t readKeyValue(void);
	//Power control
	void setPower(uint8_t status);
	//led control
	void setLED(uint8_t status);
	//voice source update function
	void initAudioUpdate(unsigned long baudrate = 256000);
	bool isUpdateBegin(void);
	bool executeUpdate(void);

private:
	void writeCmd(uint8_t cmd, uint8_t data = 0xff);


	uint32_t _lastMillis;
	uint8_t _keyValue;
	uint8_t _isKey;
	//--------------------program voice source--------------------------
    bool programEntry(uint16_t mode);
    void programDataOut1(void);
    void programDataOut0(void);
    void programAddrOut1(void);
    void programAddrOut0(void);
    void sendAddr(uint16_t addr);
    void sendData(uint16_t data);
    void matchPattern(uint16_t mode);
    uint16_t ack(void);
    void dummyClocks(void);
    uint16_t readData(void);
    bool switchSPIMode(void);
    uint8_t checkCRC8(uint8_t *ptr, uint8_t len); 
    void recAudioData(void);
    void SPIFlashWriteEnable(void);
    void SPIFlashWaitForWriteEnd(void);
    void SPIFlashChipErase(void);
    void SPIFlashPageWrite(uint8_t* pBuffer, uint32_t writeAddr, uint16_t numByteToWrite);
	void SPIFlashReadSFDP(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);

    uint8_t deviceSFDPBuf[3];
    uint8_t rxBuffer[64];
    uint32_t _flashAddr;

};

#endif
