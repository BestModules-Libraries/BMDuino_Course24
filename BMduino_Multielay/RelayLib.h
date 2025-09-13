#include <BMP75M131.h> // 引入 BMP75M131 繼電器模組的函式庫，支援 I2C 介面控制

// BMP75M131 myRelay(&Wire);  // 若使用 Wire (默認 I2C 通道) 作為通訊接口，請取消註解此行
BMP75M131 myRelay(&Wire1);   // 使用 Wire1 (I2C 第二通道) 作為通訊接口（BMduino 上通常使用 Wire1）
 // BMP75M131 myRelay(&Wire2); // 若使用 Wire2 作為通訊接口，請取消註解此行
//-------------------------------------------------------------
#define RelayON   1   // 定義繼電器開啟狀態為 1
#define RelayOFF  0   // 定義繼電器關閉狀態為 0

uint8_t MaxRelay = 0;        // 儲存目前偵測到的繼電器模組數量
uint8_t relayStatus;         // 儲存單一繼電器的狀態（ON / OFF）
uint8_t Relaystatus[9];      // 儲存所有繼電器的狀態（陣列最多支援 9 個繼電器，依模組支援可達 16 顆）
//-------------------------------------------------------------
// 函式前置宣告 (Function Prototype)
void initRealy();             // 初始化繼電器模組
void TurnonRelay(int rno);    // 開啟第 rno 個繼電器
void TurnoffRelay(int rno);   // 關閉第 rno 個繼電器
void TurnonAllRelay();        // 開啟所有繼電器
void TurnoffAllRelay();       // 關閉所有繼電器
void GetAllRelayStatus();     // 取得所有繼電器狀態
int RelayStatus(int nn);      // 取得第 nn 個繼電器的狀態
//-------------------------------------------------------------

//-------------------------------------------------------------
// 初始化繼電器模組
//-------------------------------------------------------------
void initRealy()
{
  myRelay.begin();  // 初始化 BMP75M131 繼電器模組，啟動 I2C 通訊
  MaxRelay = myRelay.getNumber(); // 取得目前連接的繼電器數量
  Serial.print("Total Relay Amount is :(");
  Serial.print(MaxRelay);
  Serial.print(")\n"); // 輸出偵測到的繼電器數量
}

//-------------------------------------------------------------
// 開啟第 n 個繼電器
//-------------------------------------------------------------
void TurnonRelay(int rno)
{
  myRelay.setRelaySwitch(rno, RelayON); // 控制第 rno 個繼電器打開
  Serial.print("Relay(");
  Serial.print(rno);
  Serial.print("): is on\n"); // 輸出提示資訊
}

//-------------------------------------------------------------
// 關閉第 n 個繼電器
//-------------------------------------------------------------
void TurnoffRelay(int rno)
{
  myRelay.setRelaySwitch(rno, RelayOFF); // 控制第 rno 個繼電器關閉
  Serial.print("Relay(");
  Serial.print(rno);
  Serial.print("): is off\n"); // 輸出提示資訊
}

//-------------------------------------------------------------
// 開啟所有繼電器
//-------------------------------------------------------------
void TurnonAllRelay()
{
  myRelay.setAllRelay(RelayON); // 將所有繼電器設定為開啟
}

//-------------------------------------------------------------
// 關閉所有繼電器
//-------------------------------------------------------------
void TurnoffAllRelay()
{
  myRelay.setAllRelay(RelayOFF); // 將所有繼電器設定為關閉
}

//-------------------------------------------------------------
// 取得所有繼電器狀態
//-------------------------------------------------------------
void GetAllRelayStatus()
{
  myRelay.getAllRelayStatus(Relaystatus); // 將所有繼電器狀態讀取到 Relaystatus 陣列中
}

//-------------------------------------------------------------
// 取得第 nn 個繼電器狀態
//-------------------------------------------------------------
int RelayStatus(int nn)
{
  return (int)myRelay.getRelayStatus((uint8_t)nn); // 回傳第 nn 個繼電器狀態 (ON / OFF)
}
