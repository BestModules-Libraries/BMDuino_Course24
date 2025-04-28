/*************************************************
File:             Example1_dcMotorRun.ino
Description:      The DC motor cycles through the following actions
                  M1 and M2 rotates forward for 5 seconds at a speed of 80% and 60% duty cycle
                  and reverses for 5 seconds at a speed of 60 and 80 duty cycle
Note:               
**************************************************/
#include <BMP73T104.h> // 引入 BMP73T104 函式庫
BMP73T104 myM1(2, DCMotor); // 建立 DC 馬達物件 myM1，使用 DCMotor 模式
// BMP73T104 myM2(2, DCMotor); // 可以建立第二個 DC 馬達物件 myM2（目前未使用）

// 構造 DC 馬達物件
// BMP73T104(物件) 用來選擇馬達的類型。物件選擇 DCMotor 或 StepMotor
void setup() {
    myM1.begin(M3); // 初始化 myM1 作為 DCMotor1，使用 M3 端口
    // myM2.begin(M2); // 初始化 myM2 作為 DCMotor2（目前未使用）
    // M1 = 0x01; M2 = 0x02; M3 = 0x04; M4 = 0x08; // 馬達端口編碼
}

void loop() {
    myM1.dcMotorRun(80); // 啟動 myM1 馬達，設定速度為 80
    // myM2.dcMotorRun(60); // 可以啟動 myM2 馬達，設定速度為 60（目前未使用）
    delay(5000); // 等待 5000 毫秒（5 秒）
    
    myM1.dcMotorRun(-60); // 啟動 myM1 馬達，設定反向速度為 -60
    // myM2.dcMotorRun(-80); // 可以啟動 myM2 馬達，設定反向速度為 -80（目前未使用）
    delay(5000); // 等待 5000 毫秒（5 秒）
}
