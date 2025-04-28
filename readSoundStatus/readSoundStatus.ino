/*****************************************************************
File:        readSoundStatus.ino 
Description:  1.  The Sound Detector is a small board that combines a microphone and some processing circuitry.
                  It provides not only an audio output, but also a binary indication of the presence of sound,
                  an analog representation of it's amplitude and I2C interface.  
              2.hardware Serial (BAUDRATE 115200) is used to communicate with Serial port monitor.When there is a sound, 
              read the sensor data displayed on the serial port monitor.
              3.This project introduces the use of I2C interface.
          
note:connection method(Sound Detector:Arduino pin):GND:GND VDD:3V3 SDA:SDA1 SCL:SCL1 STA:STATUS1
******************************************************************/
#include <BMV23M001.h>

// BMV23M001 soundDetector(2,&Wire); // 如果你不使用 Wire，請註解掉這行程式碼
BMV23M001 soundDetector(22,&Wire1); // 如果你在 BMduino 上使用 Wire1，請取消註解這行程式碼
// BMV23M001 soundDetector(25,&Wire2); // 如果你在 BMduino 上使用 Wire2，請取消註解這行程式碼

void setup() 
{
  soundDetector.begin();   // IIC 傳輸速率為 100kHz
  Serial.begin(115200);    // 開啟序列埠以進行除錯
}

void loop()
{
     if(soundDetector.readSoundStatus() == StatusFAIL)  // 讀取聲音狀態
     {
       Serial.println("通訊失敗，請檢查連線！"); // 顯示通訊失敗訊息
     }
     else
     {
       Serial.print("聲音狀態: ");
        if(soundDetector.readSoundStatus())
        {
          Serial.println("有聲音！"); // 當檢測到聲音時顯示
        }
        else
        { 
          Serial.println("安靜！"); // 當沒有檢測到聲音時顯示
        }
     }
    // 暫停 10 毫秒
}
