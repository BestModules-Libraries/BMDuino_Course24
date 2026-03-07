/******************************************************************
檔案名稱(File):       voiceUpdateAndPlayback.ino
功能說明(Description): 音源更新與語音播放控制範例
設計說明(Note):       使用 BMV31T001 模組進行語音檔更新與播放控制
******************************************************************/

#include "BMV31T001.h" 
#include "voice_cmd_list.h"    // 包含語音指令定義的標頭檔，內含 VOC_1 ~ VOC_10 等語音代碼

//==================================================================
// 建立物件與參數設定
//==================================================================
BMV31T001 myBMV31T001;         // 建立 BMV31T001 類別的物件，用來控制語音模組

#define VOICE_TOTAL_NUMBER 10  // 測試語音總數為 10 組
// 從 voice_cmd_list.h 取得的語音代碼表，對應 10 段語音
const uint8_t voice_table[VOICE_TOTAL_NUMBER] = { 
    VOC_1, VOC_2, VOC_3, VOC_4, VOC_5, VOC_6, VOC_7, VOC_8, VOC_9, VOC_10
};

//==================================================================
// 宣告變數
//==================================================================
uint8_t keyStatus;             // 當前按鍵狀態 (由模組掃描結果取得)
uint8_t lastKeyStatus;         // 上一次按鍵狀態，用來判斷是否為「新按下」事件
uint8_t playStatus;            // 播放狀態 (閒置或忙碌)
uint8_t playNum;               // 當前播放的語音序號 (0~9)

#define DEFAULT_VOLUME 6       // 預設音量值 (中等音量)
uint8_t volume = DEFAULT_VOLUME; // 當前音量
uint8_t keycode = 0;           // 紀錄目前被觸發的按鍵編號

//==================================================================
// 初始化區域
//==================================================================
void setup() {
    myBMV31T001.begin();                       // 初始化 BMV31T001 模組 (設定通訊介面)
    myBMV31T001.setPower(BMV31T001_POWER_ENABLE); // 開啟模組電源供應

    // 若需要更新音源內容，啟動音訊更新模式
    myBMV31T001.initAudioUpdate();

    delay(100);                                // 延遲 100ms，確保模組電源穩定
    myBMV31T001.setVolume(DEFAULT_VOLUME);     // 設定初始音量為預設值
}

//==================================================================
// 主程式循環 (Loop) - 不斷偵測按鍵與控制播放
//==================================================================
void loop() {
    //--------------------------------------------------------------
    // 1. 檢查是否進入音訊更新階段
    //--------------------------------------------------------------
    if (myBMV31T001.isUpdateBegin() == BMV31T001_UPDATA_BEGIN) {
        myBMV31T001.executeUpdate();           // 若有更新請求，執行音訊更新程序
    }

    //--------------------------------------------------------------
    // 2. 控制播放指示燈狀態
    //--------------------------------------------------------------
    if (myBMV31T001.isPlaying() == BMV31T001_BUSY) { 
        playStatus = BMV31T001_BUSY;           // 模組忙碌中 (正在播放)
        myBMV31T001.setLED(BMV31T001_LED_ON);  // 亮燈表示播放中
    } else {
        playStatus = BMV31T001_NOBUSY;         // 模組閒置中 (未播放)
        myBMV31T001.setLED(BMV31T001_LED_OFF); // 關燈表示停止
    }

    //--------------------------------------------------------------
    // 3. 掃描按鍵狀態
    //--------------------------------------------------------------
    myBMV31T001.scanKey();                     // 執行按鍵掃描

    //--------------------------------------------------------------
    // 4. 若有按鍵動作則進行對應處理
    //--------------------------------------------------------------
    if (myBMV31T001.isKeyAction() != BMV31T001_NO_KEY) { 
        keyStatus = myBMV31T001.readKeyValue(); // 讀取目前按鍵的值

        // 僅當上一次沒有按鍵動作時，才記錄新的按鍵 (避免重複觸發)
        if (lastKeyStatus == 0) { 
            if ((keyStatus & BMV31T001_KEY_MIDDLE) != 0) { 
                keycode = BMV31T001_KEY_MIDDLE; // 中間鍵
            } else if ((keyStatus & BMV31T001_KEY_UP) != 0) {
                keycode = BMV31T001_KEY_UP;     // 上鍵
            } else if ((keyStatus & BMV31T001_KEY_DOWN) != 0) {
                keycode = BMV31T001_KEY_DOWN;   // 下鍵
            } else if ((keyStatus & BMV31T001_KEY_LEFT) != 0) {
                keycode = BMV31T001_KEY_LEFT;   // 左鍵
            } else {
                keycode = BMV31T001_KEY_RIGHT;  // 右鍵
            }
        }

        lastKeyStatus = keyStatus; // 更新上一次按鍵狀態

        //----------------------------------------------------------
        // 5. 根據按鍵功能執行動作
        //----------------------------------------------------------
        switch (keycode) {
            //-------------------------------
            // (1) 中間鍵：播放 / 停止
            //-------------------------------
            case BMV31T001_KEY_MIDDLE:
                if (playStatus == BMV31T001_BUSY) { 
                    myBMV31T001.playStop();     // 若正在播放，則停止
                } else {
                    myBMV31T001.playVoice(voice_table[playNum]); // 否則播放目前語音
                }
                keycode = 0;                    // 重置按鍵代碼
                break;

            //-------------------------------
            // (2) 上鍵：音量增加
            //-------------------------------
            case BMV31T001_KEY_UP:
                if (volume < BMV31T001_VOLUME_MAX) { 
                    volume++;                   // 增加音量 (上限保護)
                    myBMV31T001.setVolume(volume); 
                }
                keycode = 0;                    // 重置按鍵代碼
                break;

            //-------------------------------
            // (3) 下鍵：音量減少
            //-------------------------------
            case BMV31T001_KEY_DOWN:
                if (volume > BMV31T001_VOLUME_MIN) { 
                    volume--;                   // 減少音量 (下限保護)
                    myBMV31T001.setVolume(volume); 
                }
                keycode = 0;                    // 重置按鍵代碼
                break;

            //-------------------------------
            // (4) 左鍵：切換上一個語音
            //-------------------------------
            case BMV31T001_KEY_LEFT:
                if (playNum > 0) { 
                    playNum--;                  // 語音編號 -1
                }
                myBMV31T001.playVoice(voice_table[playNum]); // 播放切換後語音
                keycode = 0;                    // 重置按鍵代碼
                break;

            //-------------------------------
            // (5) 右鍵：切換下一個語音
            //-------------------------------
            case BMV31T001_KEY_RIGHT:
                if (playNum < VOICE_TOTAL_NUMBER - 1) { 
                    playNum++;                  // 語音編號 +1
                }
                myBMV31T001.playVoice(voice_table[playNum]); // 播放切換後語音
                keycode = 0;                    // 重置按鍵代碼
                break;

            //-------------------------------
            // (6) 預設情況：無按鍵
            //-------------------------------
            default:
                keycode = 0;                    // 清除按鍵代碼
                break;     
        }  
    }       
}
