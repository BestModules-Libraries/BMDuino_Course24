//----------外部引用函式區---------------
#include "PIRLib.h" //PIR偵測模組(BMCOM) BMA46M422 自訂函式區

// ------- 自定義函式宣告區 -----------
void initSensor();                 // 初始化所有感測模組
void initAll();                    // 初始化整體系統
// ------- 自定義函式宣告區 -----------

void setup() 
{
  initAll();       // 初始化整體系統（啟動序列埠、初始化 OLED、Relay）
  delay(200);      // 延遲 200ms，確保硬體模組穩定

}

void loop() {

  /***************************************
   * 當感測器輸出 HIGH（偵測到物體）
   * 且 flag 尚未進入警報狀態（!= 1）
   * → 則觸發警報事件
   ***************************************/
  if (readPIR() == HIGH && flag != 1) { 
    Serial.println("Alarm! an object passes by");  // 印出警報訊息
    flag = 1;                                      // 設定為警報狀態
  }

  /***************************************
   * 當感測器恢復 LOW（無物體）
   * 且 flag 不為 0（表示剛解除警報）
   * → 模組恢復正常
   ***************************************/
  if (flag != 0 && readPIR() == LOW) {
    flag = 0;                                      // 重設旗標
    Serial.println("Module normal; no alarm");     // 印出正常訊息
  }

  /***************************************
   * 根據 flag 狀態控制 LED 行為
   * case 0 → 正常模式：每 1 秒閃爍一次
   * case 1 → 警報模式：LED 長亮（每秒亮一次）
   ***************************************/
  switch (flag) {

    case 0:                                // 正常狀態（無偵測）
      digitalWrite(13, HIGH);              // LED 快速亮一下
      delay(100);                          // 亮 100ms
      digitalWrite(13, LOW);               // 熄滅
      delay(900);                          // 每秒一次週期
      break;

    case 1:                                // 警報狀態（偵測到物體）
      digitalWrite(13, HIGH);              // LED 持續亮
      delay(1000);                         // 每秒亮一次（等同長亮）
      break;
  }
}


//---------------  自訂函式區 ------------------
// ------------------ 系統初始化區 ------------------
// 初始化所有感測模組
void initSensor()	// 初始化所有感測模組
{
  initPIR();  //初始化PIR偵測模組(BMCOM) BMA46M422
  delay(2000);   // 延遲 2 秒，等待顯示模組穩定
}

// 初始化整體系統
void initAll()	// 初始化整體系統
{
  Serial.begin(9600);  // 啟動序列埠，速率 9600 bps
  Serial.println("System Start.....") ; //印出"System Start....." 
  initSensor(); 
}










