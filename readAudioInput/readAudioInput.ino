/*****************************************************************
File:        readAudioInput.ino 
Description:  1.  The Sound Detector is a small board that combines a microphone and some processing circuitry.
                  It provides not only an audio output, but also a binary indication of the presence of sound,
                  an analog representation of it's amplitude and I2C interface.  
              2.This project introduces the use of an audio output, a binary indication and an analog representation.
note:connection method(Sound Detector:Arduino pin):GND:GND VDD:3V3 Gate:D2 Envelope:A0 Audio:A1
******************************************************************/
//Define hardware connections
#define PIN_AUDIO_IN    (A1)

uint16_t audioADC    = 0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);  // start serial for Debug
}

void loop() 
{
  // put your main code here, to run repeatedly:
  //Check audio input
  analogReadResolution(12);  //change the resolution to 12 bits
  audioADC = analogRead(PIN_AUDIO_IN);
  Serial.println(audioADC);
}
