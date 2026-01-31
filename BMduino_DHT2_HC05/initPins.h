// 引用自訂的通訊函式庫，其中包含 GPIO 控制相關函式
#include "commlib.h"

// 定義板載 LED 燈腳位：BMduino-UNO 的腳位 13 為板載 LED
#define LEDPin 13

// ================== 自訂函式：開啟板載 LED ==================
// 功能：將 LEDPin（腳位 13）設定為高電位，點亮 LED
void TurnOnLed13()
{
  // 呼叫 GPIOControl 函式，將指定腳位設為高電位（IOon）
  GPIOControl(LEDPin, IOon);
}

// ================== 自訂函式：關閉板載 LED ==================
// 功能：將 LEDPin（腳位 13）設定為低電位，熄滅 LED
void TurnOffLed13()
{
  // 呼叫 GPIOControl 函式，將指定腳位設為低電位（IOoff）
  GPIOControl(LEDPin, IOoff);
}