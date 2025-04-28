/******************************************************************
File:             voiceUpdateAndPlayback.ino
Description:      Audio source update and playback
Note:             
******************************************************************/
#include "BMV31T001.h" 
#include "voice_cmd_list.h" // 包含語音資訊的函式庫

BMV31T001 myBMV31T001; // 建立 BMV31T001 的物件

#define VOICE_TOTAL_NUMBER 10 // 本範例測試 10 種語音指令
// 從 voice_cmd_list.h 中取得語音指令清單
const uint8_t voice_table[VOICE_TOTAL_NUMBER] = { 
    VOC_1, VOC_2, VOC_3, VOC_4, VOC_5, VOC_6, VOC_7, VOC_8, VOC_9, VOC_10
};

uint8_t keyStatus;          // 按鍵的當前狀態
uint8_t lastKeyStatus;      // 上一次按鍵的狀態
uint8_t playStatus;         // 當前播放狀態
uint8_t playNum;            // 正在播放的語音指令編號
#define DEFAULT_VOLUME 6    // 預設音量
uint8_t volume = DEFAULT_VOLUME; // 當前音量
uint8_t keycode = 0;        // 被觸發的按鍵編號

void setup() {
    myBMV31T001.begin(); // 初始化 BMV31T001
    myBMV31T001.setPower(BMV31T001_POWER_ENABLE); // 開啟 BMV31T001 的電源

    // 如果需要更新音訊來源，初始化音訊更新
    myBMV31T001.initAudioUpdate();

    delay(100); // 等待設備供電穩定
    myBMV31T001.setVolume(DEFAULT_VOLUME); // 設定預設音量
}

void loop() {
    // 檢測是否有音訊更新請求
    if (myBMV31T001.isUpdateBegin() == BMV31T001_UPDATA_BEGIN) {
        myBMV31T001.executeUpdate(); // 執行音訊更新
    }

    // 控制播放指示燈
    if (myBMV31T001.isPlaying() == BMV31T001_BUSY) { 
        playStatus = BMV31T001_BUSY; // 更新播放狀態為忙碌
        myBMV31T001.setLED(BMV31T001_LED_ON); // 開啟指示燈
    } else {
        playStatus = BMV31T001_NOBUSY; // 更新播放狀態為閒置
        myBMV31T001.setLED(BMV31T001_LED_OFF); // 關閉指示燈
    }

    // 掃描按鍵狀態
    myBMV31T001.scanKey(); 

    // 處理按鍵事件
    if (myBMV31T001.isKeyAction() != BMV31T001_NO_KEY) { 
        keyStatus = myBMV31T001.readKeyValue(); // 讀取當前按鍵狀態
        if (lastKeyStatus == 0) { // 若上一次按鍵狀態為未按下
            if ((keyStatus & BMV31T001_KEY_MIDDLE) != 0) { 
                keycode = BMV31T001_KEY_MIDDLE; // 中間鍵被按下
            } else if ((keyStatus & BMV31T001_KEY_UP) != 0) {
                keycode = BMV31T001_KEY_UP; // 上鍵被按下
            } else if ((keyStatus & BMV31T001_KEY_DOWN) != 0) {
                keycode = BMV31T001_KEY_DOWN; // 下鍵被按下
            } else if ((keyStatus & BMV31T001_KEY_LEFT) != 0) {
                keycode = BMV31T001_KEY_LEFT; // 左鍵被按下
            } else {
                keycode = BMV31T001_KEY_RIGHT; // 右鍵被按下
            }
        }

        lastKeyStatus = keyStatus; // 更新上一個按鍵狀態

        // 判斷按鍵功能
        switch (keycode) {
            case BMV31T001_KEY_MIDDLE: // 中間鍵
                if (playStatus == BMV31T001_BUSY) { 
                    myBMV31T001.playStop(); // 停止播放
                } else {
                    myBMV31T001.playVoice(voice_table[playNum]); // 播放當前語音
                }
                keycode = 0; // 重置按鍵狀態
                break;
            case BMV31T001_KEY_UP: // 上鍵
                if (volume < BMV31T001_VOLUME_MAX) { 
                    volume++; // 增加音量
                    myBMV31T001.setVolume(volume); 
                }
                keycode = 0; // 重置按鍵狀態
                break;
            case BMV31T001_KEY_DOWN: // 下鍵
                if (volume > BMV31T001_VOLUME_MIN) { 
                    volume--; // 減少音量
                    myBMV31T001.setVolume(volume); 
                }
                keycode = 0; // 重置按鍵狀態
                break;
            case BMV31T001_KEY_LEFT: // 左鍵
                if (playNum > 0) { 
                    playNum--; // 切換到上一個語音
                }
                myBMV31T001.playVoice(voice_table[playNum]); 
                keycode = 0; // 重置按鍵狀態
                break;
            case BMV31T001_KEY_RIGHT: // 右鍵
                if (playNum < VOICE_TOTAL_NUMBER - 1) { 
                    playNum++; // 切換到下一個語音
                }
                myBMV31T001.playVoice(voice_table[playNum]); 
                keycode = 0; // 重置按鍵狀態
                break;
            default:
                keycode = 0; // 重置按鍵狀態
                break;     
        }  
    }       
}
