/*****************************************************************
File:         readAccelerationAndGyroscopeAndTemperature.ino
Description:  repeatedly obtain the 3 axis Acceleration(unit: g),
              3 axis Gyroscope(unit: °/s) and Temperature(unit: ℃) 
              through IIC and display the value in the serial port.
******************************************************************/
#include "BMS56M605.h" // 引入 BMS56M605 的庫
BMS56M605 Mpu(8); // 設定腳位 8 為中斷腳位（intpin）

void setup() {
  Mpu.begin(); // 初始化 BMS56M605 感測器
  Serial.begin(9600); // 設定序列通訊，鮑率為 9600
}

void loop() {
  Mpu.getEvent(); // 取得感測器的最新數據
  Serial.print("Temp = "); // 顯示溫度
  Serial.print(Mpu.temperature); // 顯示溫度值
  Serial.println(" ℃"); // 顯示溫度單位

  // 顯示加速度數據
  Serial.print("ax = ");
  Serial.print(Mpu.accX); // X 軸加速度
  Serial.print("  ay = ");
  Serial.print(Mpu.accY); // Y 軸加速度
  Serial.print("  az = ");
  Serial.print(Mpu.accZ); // Z 軸加速度
  Serial.println("   g"); // 加速度單位

  // 顯示陀螺儀數據
  Serial.print("gx = ");
  Serial.print(Mpu.gyroX); // X 軸角速度
  Serial.print("  gy = ");
  Serial.print(Mpu.gyroY); // Y 軸角速度
  Serial.print("  gz = ");
  Serial.print(Mpu.gyroZ); // Z 軸角速度
  Serial.println(" °/s"); // 角速度單位

  Serial.println(); // 空行分隔
  delay(1000); // 暫停 1 秒鐘
}
