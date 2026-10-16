/******************************************************************
File:             voiceUpdateAndPlayback.ino
Description:      Audio source update and playback
Note:             
******************************************************************/
#include "BMV31T001.h" 
#include "voice_cmd_list.h" //Contains a library of voice information

BMV31T001 myBMV31T001; //Create an object

#define VOICE_TOTAL_NUMBER 4//This example tests 10 voices
#define SENTENCE_TOTAL_NUMBER 4//This example tests 10 voices

//Voice_cmd_list. h is used to create a voice list
const uint8_t voice_table[VOICE_TOTAL_NUMBER]={ 
VOC_1,VOC_2,VOC_3,VOC_4};
const uint8_t sentence_table[SENTENCE_TOTAL_NUMBER]={ 
SEN_1,SEN_2,SEN_3, SEN_4};
uint8_t keyStatus; //status of keys
uint8_t lastKeyStatus;//status of last keys
uint8_t playStatus; //current Playing Status
uint8_t playNum; //number of the song being played
#define DEFAULT_VOLUME 1 //default volume
uint8_t volume = DEFAULT_VOLUME; //current volume
uint8_t keycode = 0;//Number of the key that was triggered

void setup() {

    myBMV31T001.begin();//Initialize the BMV31T001
    myBMV31T001.setPower(BMV31T001_POWER_ENABLE);//Power on the BMV31T001

    //=====================================================================
    //If you want to update the audio source, add the serial port initializer for the audio source update
    myBMV31T001.initAudioUpdate();
    //=====================================================================

    delay(100);//Delay until the expansion version is powered on
    myBMV31T001.setVolume(DEFAULT_VOLUME);//Initialize the default volume
}

 
void loop() {
    //=========================================================================
    //-----------------update audio source------------------------------
    //If you want to update your audio source, please add this program
    if(myBMV31T001.isUpdateBegin() == BMV31T001_UPDATA_BEGIN)
    {//detect update signal
        myBMV31T001.executeUpdate();//Execute audio source updates
    }
    //=========================================================================
    //-----------------Play indicator led control------------------------------
    if(myBMV31T001.isPlaying() == BMV31T001_BUSY)
    { //Gets the current playback status
        playStatus = BMV31T001_BUSY;
        myBMV31T001.setLED(BMV31T001_LED_ON);//LED on
    }
    else
    {
        playStatus = BMV31T001_NOBUSY;
        myBMV31T001.setLED(BMV31T001_LED_OFF);//LED off
    }
    //-----------------scan key-----------------------------
    myBMV31T001.scanKey();//polling key status

    //-----------------dispose key------------------------------
    if(myBMV31T001.isKeyAction() != BMV31T001_NO_KEY)//there is key pressed
    {
        
        keyStatus = myBMV31T001.readKeyValue();//Gets the current key level value
        if(lastKeyStatus == 0)
        {
            if((keyStatus & BMV31T001_KEY_MIDDLE) != 0){//middle key pressed;
                keycode = BMV31T001_KEY_MIDDLE;
            }
            else if((keyStatus & BMV31T001_KEY_UP) != 0){//up key pressed;
                keycode = BMV31T001_KEY_UP;
            }
            else if((keyStatus & BMV31T001_KEY_DOWN) != 0){//down key pressed;
                keycode = BMV31T001_KEY_DOWN;
            }
            else if((keyStatus & BMV31T001_KEY_LEFT) != 0){//left key pressed;
                keycode = BMV31T001_KEY_LEFT;
            }
            else {
                keycode = BMV31T001_KEY_RIGHT;//right key pressed;
            }
        }
        lastKeyStatus = keyStatus ;
        switch(keycode)//Judge and dispose key
        {
            case BMV31T001_KEY_MIDDLE://The middle key is pressed
                if(playStatus == BMV31T001_BUSY)//in the play
                {
                    myBMV31T001.playStop();//Stop playing if it is currently playing
                }
                else
                {
                    myBMV31T001.playSentence(sentence_table[playNum]);
                    //myBMV31T001.playVoice(voice_table[playNum]);//Play the current voice
                }
                keycode = 0;
                break;
            case BMV31T001_KEY_UP://The up key is pressed
                if(volume < BMV31T001_VOLUME_MAX)//If not the loudest
                {
                    volume++;//The volume increase
                    myBMV31T001.setVolume(volume);
                }
                keycode = 0;
                break;
            case BMV31T001_KEY_DOWN://The down key is pressed
                if(volume > BMV31T001_VOLUME_MIN)//If not the smallest volume
                {
                    volume--;//The volume reduction
                    myBMV31T001.setVolume(volume);
                }    
                keycode = 0;
                break;
            case BMV31T001_KEY_LEFT://The left key is pressed
                if(playNum > 0)
                {
                    playNum--;//last voice
                }
                myBMV31T001.playSentence(sentence_table[playNum]);
                //myBMV31T001.playVoice(voice_table[playNum]); 
                keycode = 0;
                break;
            case BMV31T001_KEY_RIGHT://The right key is pressed
                if(playNum < VOICE_TOTAL_NUMBER - 1)
                {
                    playNum++;//next voice
                }
                myBMV31T001.playSentence(sentence_table[playNum]);
                //myBMV31T001.playVoice(voice_table[playNum]);// play next voice
                keycode = 0;
                break;
            default:
                keycode = 0;
                break;     
        }  
    }       
}
