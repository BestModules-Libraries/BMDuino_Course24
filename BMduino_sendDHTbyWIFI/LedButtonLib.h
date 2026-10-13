#include <BMK22M131.h>   // 引入 BMK22M131 按鈕模組的函式庫，負責模組初始化、按鈕狀態讀取、LED 控制等功能
#include <Wire.h>        // 引入 I2C（TWI）通訊的標準函式庫，負責主控板與模組之間的 I2C 傳輸

#define intPin 22         // 定義外部中斷腳位為 D22，用來偵測 BMK22M131 按鈕模組的中斷訊號

// 建立一個 BMK22M131 類別的物件 myButton，並指定使用 I2C 傳輸介面
// BMK22M131 myButton(intPin, &Wire);   // 若使用 Wire (預設 I2C)
// BMK22M131 myButton(intPin, &Wire2);  // 若使用 Wire2 (另一組 I2C)
BMK22M131 myButton(intPin, &Wire1);     // 在此範例中，使用 Wire1 這組 I2C 介面

// 宣告一個中斷旗標 (flag)，用來標記是否發生中斷事件（按鈕被觸發）
// 初始值為 0，代表尚未偵測到中斷
uint8_t int_flag = 0;

// 宣告變數 buttonStatus，用來儲存按鈕的狀態值
// （1 = 短按，2 = 長按）
uint8_t buttonStatus;

//------------------
// 函式前置宣告 (function prototype)，讓主程式可以先知道有哪些函式存在
void ButtonInt();     // 外部中斷服務函式（ISR）
void initButton();    // 初始化 BMK22M131 按鈕模組的函式

//------------------
/*
 * 函式名稱：initButton()
 * 功能：初始化 BMK22M131 按鈕模組
 * 步驟：
 *   1. 設定中斷觸發方式
 *   2. 啟動模組
 *   3. 啟用 LED 顯示模式
 *   4. 確認模組連線狀態
 */
void initButton()
{
  /*
      initButton()
    設定外部中斷 → 偵測按鈕下降沿
    初始化模組 → I2C 通訊
    啟動 LED 按鈕模式
    確認模組是否連線
  */
  Serial.println("now init Led Button....") ;  // 顯示初始化訊息

  // 將腳位 D22 設為外部中斷輸入腳
  // 當偵測到「下降沿」（FALLING）時，會自動呼叫 ButtonInt() 函式
  attachInterrupt(digitalPinToInterrupt(intPin), ButtonInt, FALLING);

  // 啟動按鈕模組，初始化 I2C 傳輸
  myButton.begin();

  // 啟用模組上的 LED 模式（參數 1 表示啟用 LED 顯示功能）
  myButton.ledButtonMode(1);

  // 確認模組是否有成功連線（透過 I2C 確認）
  Serial.println("Check whether the module is connected, waiting...");
  if (myButton.isConnected() == true) 
  {
    Serial.println("The module is connecting");  // 顯示模組連線成功
  } 
  // 若 isConnected() == false，則代表模組未正常連線，使用者需檢查 I2C 線路
}

//------------------
/*
 * 函式名稱：ButtonInt()
 * 功能：外部中斷服務函數
 * 說明：
 *   - 當按鈕輸出訊號觸發「下降沿」時，會進入此函式
 *   - 此函式僅設定 int_flag = 1，不做其他耗時的動作
 */
void ButtonInt() 
{
  int_flag = 1;  // 將中斷旗標設為 1，通知主程式有按鈕事件
}

//------------------
/*
 * 函式名稱：getButtonStatus()
 * 功能：
 *   1. 檢查是否發生中斷（int_flag 是否為 1）
 *   2. 若有，則讀取按鈕的狀態（短按或長按）
 *   3. 回傳對應的按鈕狀態值
 * 回傳值：
 *   0 = 沒有按鈕事件
 *   1 = 按鈕短按
 *   2 = 按鈕長按
 */
int getButtonStatus()
{
  /*
      getButtonStatus()
    檢查是否有中斷事件
    讀取按鈕狀態（短按 / 長按）
    印出對應訊息並回傳狀態值
  */
  if (int_flag)  // 若 int_flag = 1，表示有中斷事件
  {
    int_flag = 0;  // 將旗標重設為 0，避免重複觸發

    // 讀取按鈕狀態
    // 參數 1 表示模組的按鈕 ID（若模組支援多個按鈕，可指定不同 ID）
    buttonStatus = myButton.getButtonStatus(1);

    // 判斷按鈕狀態
    if (buttonStatus == 1) 
    {
      Serial.println("The button is short pressed.");  // 顯示「短按」
      return buttonStatus;  // 回傳 1
    } 
    else 
    {
      Serial.println("The button is long pressed.");   // 顯示「長按」
      return buttonStatus;  // 回傳 2
    }
  }
  
  return 0;  // 若沒有按鈕事件，則回傳 0
}
