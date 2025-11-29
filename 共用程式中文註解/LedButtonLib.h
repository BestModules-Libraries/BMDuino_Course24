//----------外部引用函式區----------------
//------------------------------------------------------
#include <Wire.h>        // 引入 I2C（TWI）通訊的標準函式庫，支援多設備傳輸
#include <BMK22M131.h>   // 引入 BMK22M131 按鈕模組的函式庫，用於 LED+按鈕模組控制
//------------------------------------------------------
//------------全域變數區----------------
#define longpressed 500   // 定義「長按」的時間長度 (單位：500毫秒)，超過此值視為長按
#define intPin 22         // 設定外部中斷輸入腳位，使用 BMCOM1 (腳位 D22)
//#define intPin 25       // 如果使用 BMCOM2，則改為腳位 D25

// 建立 BMK22M131 類別物件 myButton，並指定 I2C 通道
// 可依實際使用的 Wire, Wire1, Wire2 來決定使用哪組 I2C 腳位
//BMK22M131 myButton(intPin, &Wire);
BMK22M131 myButton(intPin, &Wire1);
//BMK22M131 myButton(intPin, &Wire2);

//------------------------------------------------------
//------------全域變數區----------------
// 宣告中斷旗標，預設為 0
// 當發生中斷時會設為 1，提醒主程式需要處理
uint8_t int_flag = 0;

uint8_t MaxButton = 0;   // 用來儲存目前偵測到的 LED 按鈕模組數量
uint8_t buttonStatus;    // 用來儲存單一按鈕的狀態（短按 / 長按）
uint8_t BTstatus[17];    // 用來存放所有聯集（串接）按鈕的狀態，最多支援 16 顆按鈕

//------------------ 函式宣告區 ------------------//
void ButtonInt();                   // 外部中斷觸發後執行的函數
void intLedButton();                // 初始化 LED 按鈕模組
String getAllButtonStatus();        // 取得所有按鈕的即時狀態
uint8_t getButton(uint8_t btn);  //回傳第btn按鈕的狀態碼
uint8_t getButtonStatus(uint8_t btn);   //回傳第btn按鈕的狀態碼

//================================================//
// 函式區
//================================================//

//-------------------------
// 外部中斷服務函數（ISR）
// 當指定腳位偵測到下降沿 (FALLING edge) 時被呼叫
//-------------------------
void ButtonInt() 
{
  int_flag = 1;  // 將中斷旗標設為 1，通知主程式有按鈕事件需要處理
}

//-------------------------
// 初始化 LED 按鈕模組
// 功能：
//   1. 啟用外部中斷偵測
//   2. 初始化 I2C 與模組
//   3. 開啟 LED 模式
//   4. 設定「長按」的判斷時間
//-------------------------
void intLedButton()   
{
  // 設定外部中斷，偵測到「下降沿」時執行 ButtonInt()
  attachInterrupt(digitalPinToInterrupt(intPin), ButtonInt, FALLING);

  // 初始化 BMK22M131 按鈕模組（I2C 設定）
  myButton.begin();

  // 啟用 LED 按鈕模式（參數 1 表示開啟 LED）
  myButton.ledButtonMode(1);

  // 取得目前連接的 LED 按鈕模組數量
  MaxButton = myButton.getNumber();

  // 確認模組是否成功連線
  Serial.println("Check whether the module is connected, waiting...");
  if (myButton.isConnected() == true) 
  {
    Serial.println("The module is connecting");  
    Serial.print("Total Amount of LEDButton is Connected:(");  
    Serial.print(MaxButton);  
    Serial.print(")\n");  
  } 

  // 設定每一顆按鈕的「長按時間」
  for (int i=0 ; i <MaxButton ; i++)  
  {
      myButton.setButtonLongOnTime((uint8_t)i, longpressed);  
  }
}

//-------------------------
// 取得所有聯集（串接）按鈕的狀態
// 回傳一個字串，內容為每個按鈕的狀態碼
//-------------------------
String getAllButton() 
{
  String tmp="";   // 用來累加所有按鈕狀態的字串

  // 當中斷旗標被設定為 1（表示有按鈕事件發生）
  if (int_flag) 
  {
    int_flag = 0;  // 重設旗標，避免重複觸發

    // 從模組讀取所有按鈕的狀態，存入 BTstatus 陣列
    // BTstatus[i] 代表第 i 顆按鈕的狀態
    // 狀態可能為：0=無動作，1=短按，2=長按
    buttonStatus = myButton.getButtonStatus(BTstatus);

    // 將所有按鈕狀態轉成字串並累加
    for(int i=1 ; i <= MaxButton; i++)  
    {
      tmp.concat(BTstatus[i]);  
    }
  }  

  // 將按鈕狀態輸出到序列埠監控視窗
  Serial.print("All Button:(") ;
  Serial.print(tmp) ;
  Serial.print(")\n") ;

  return tmp;   // 回傳所有按鈕狀態的字串
}


uint8_t getButtonStatus(uint8_t btn)  //回傳第btn按鈕的狀態碼
{
  if (btn >MaxButton) //檢查超出所有按鈕數目
  {
      return (0x00) ; //0x00：未按下
  }
  else 
 {
   // 當中斷旗標被設定為 1（表示有按鈕事件發生）
  if (int_flag)   //表示有按鈕事件發生
  {
    int_flag = 0;  // 重設旗標，避免重複觸發

    // 從模組讀取getButtonStatus(btn)按鈕的狀態，回傳資料
    // getButtonStatus(btn)按鈕的狀態
    // 狀態可能為：0=無動作，1=短按，2=長按
    return myButton.getButtonStatus(btn);
    }  
  }
}

uint8_t getButton(uint8_t btn)  //回傳第btn按鈕的狀態碼
{
  if (btn >MaxButton) //檢查超出所有按鈕數目
  {
      return (0x00) ; //0x00：未按下
  }
  else 
 {
   // 當中斷旗標被設定為 1（表示有按鈕事件發生）
  if (int_flag)   //表示有按鈕事件發生
  {
    int_flag = 0;  // 重設旗標，避免重複觸發

    // 從模組讀取getButtonStatus(btn)按鈕的狀態，回傳資料
    // getButtonStatus(btn)按鈕的狀態
    // 狀態可能為：0=無動作，1=短按，2=長按
    return myButton.getButtonStatus(btn);
    }  
  }
}
