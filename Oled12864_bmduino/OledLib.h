/*****************************************************************
File:         display.ino
Description:  1.Wire interface (Clock Frequency: 400K) is used to communicate with BMD31M090.
              2.Hardware Serial (BAUDRATE 115200) is used to communicate with Serial Port Monitor.
connection method: sclPin:SCL(D19) sdaPin:SDA(D18)
******************************************************************/
#include "BMD31M090.h"  // 引入 BMD31M090 OLED 顯示模組的函式庫
#include "Bitmap.h"     // 引入位圖相關的函式庫

//------全域變數區--------------------------
// 定義 BMD31M090 顯示模組的寬度和高度（單位：像素）
#define BMD31M090_WIDTH   128        
#define BMD31M090_HEIGHT  64         

// 設定 BMD31M090 顯示模組的 I2C 地址，根據電路圖來設定 I2C 地址
#define BMD31M090_ADDRESS 0x3C       
// #define BMD31M090_ADDRESS 0x3D   // 可以配置的 I2C 地址：Addr0:0x3C 或 Addr1:0x3D

uint8_t t = ' ';  // 宣告變數 t，初始化為空格字元

