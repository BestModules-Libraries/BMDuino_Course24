/*
 * ==================================================
 * 程式名稱：LedButtonLib.h - LED 按鈕模組自訂函式庫
 * 適用硬體：BMK22M131 LED 按鈕模組（支援串接多顆按鈕）
 * 通訊介面：I2C (可選擇 Wire、Wire1、Wire2)
 * 
 * 程式功能說明：
 *   本程式為 LED 按鈕模組的自訂函式庫，提供多顆串接按鈕的狀態偵測功能。
 *   每個按鈕內建 LED 指示燈，可透過 I2C 介面讀取按鈕狀態（短按/長按），
 *   並支援外部中斷通知機制，讓主程式能即時響應按鈕事件。
 * 
 * 主要功能：
 *   1. 模組初始化 (intLedButton)：
 *      - 設定外部中斷腳位，偵測按鈕事件
 *      - 初始化 I2C 通訊與 LED 按鈕模組
 *      - 自動偵測串接的按鈕數量
 *      - 設定每顆按鈕的長按時間閥值
 * 
 *   2. 按鈕狀態讀取：
 *      - getAllButton()：取得所有串接按鈕的狀態，回傳字串格式
 *      - getButton(btn) / getButtonStatus(btn)：取得指定編號按鈕的狀態
 *      - 狀態碼定義：0 = 無動作，1 = 短按，2 = 長按
 * 
 *   3. 中斷機制：
 *      - 使用外部中斷 (FALLING edge) 偵測按鈕事件
 *      - 設定中斷旗標 (int_flag) 通知主程式處理
 *      - 避免在 ISR 中執行過多程式，確保系統響應速度
 * 
 * 應用場景：
 *   - 智慧家電控制面板
 *   - 工業控制設備操作介面
 *   - 互動式裝置按鍵輸入
 *   - 需要多按鈕輸入的嵌入式系統
 * 
 * 作者/版本：Best Module / LED 按鈕模組函式庫
 * 建立日期：2025.03.27
 * 修改紀錄：
 *   - 2025.03.27：加入詳細繁體中文註解與整體說明
 *   - 修正 getButton 與 getButtonStatus 函式的邏輯錯誤
 * ==================================================
 */

//----------外部引用函式區----------------
//------------------------------------------------------
#include <Wire.h>        // 引入 I2C（TWI）通訊的標準函式庫，支援多設備傳輸
#include <BMK22M131.h>   // 引入 BMK22M131 按鈕模組的函式庫，用於 LED+按鈕模組控制
//------------------------------------------------------

//------------全域變數區----------------
#define longpressed 500   // 定義「長按」的時間長度 (單位：500毫秒)，超過此值視為長按
#define intPin 22         // 設定外部中斷輸入腳位，使用 BMCOM1 (腳位 D22)
//#define intPin 25       // 如果使用 BMCOM2，則改為腳位 D25

//----------感測物件建立區----------------
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
String getAllButtonStatus();        // 取得所有按鈕的即時狀態（原程式宣告但未實作）
String getAllButton();              // 取得所有按鈕的狀態字串
uint8_t getButton(uint8_t btn);     // 回傳第 btn 按鈕的狀態碼
uint8_t getButtonStatus(uint8_t btn); // 回傳第 btn 按鈕的狀態碼（功能與 getButton 相同）

//================================================//
// 函式區
//================================================//

//-------------------------
// 外部中斷服務函數（ISR）
// 當指定腳位偵測到下降沿 (FALLING edge) 時被呼叫
// 注意：ISR 中應盡量保持簡短，只設定旗標，不做複雜運算
//-------------------------
void ButtonInt() 
{
  int_flag = 1;  // 將中斷旗標設為 1，通知主程式有按鈕事件需要處理
}

//-------------------------
// 初始化 LED 按鈕模組
// 功能：
//   1. 啟用外部中斷偵測（下降沿觸發）
//   2. 初始化 I2C 與 BMK22M131 模組
//   3. 開啟 LED 按鈕模式（按鈕按壓時 LED 會亮起）
//   4. 自動偵測串接的按鈕數量
//   5. 設定每顆按鈕的「長按」判斷時間
// 
// 使用方式：在 setup() 中呼叫此函式進行初始化
//-------------------------
void intLedButton()   
{
  // 設定外部中斷，偵測到「下降沿」時執行 ButtonInt()
  // 當按鈕被按下時，模組會送出中斷訊號觸發此中斷
  attachInterrupt(digitalPinToInterrupt(intPin), ButtonInt, FALLING);

  // 初始化 BMK22M131 按鈕模組（I2C 設定與通訊建立）
  myButton.begin();

  // 啟用 LED 按鈕模式（參數 1 表示開啟 LED 功能）
  // 啟用後，按壓按鈕時對應的 LED 會亮起
  myButton.ledButtonMode(1);

  // 取得目前連接的 LED 按鈕模組數量
  // 支援多顆按鈕串接，系統會自動計算總數量
  MaxButton = myButton.getNumber();

  // 確認模組是否成功連線，並輸出連線資訊到序列埠
  Serial.println("Check whether the module is connected, waiting...");
  if (myButton.isConnected() == true) 
  {
    Serial.println("The module is connecting");  
    Serial.print("Total Amount of LEDButton is Connected:(");  
    Serial.print(MaxButton);  
    Serial.print(")\n");  
  } 

  // 設定每一顆按鈕的「長按時間」閥值
  // 當按壓時間超過此設定值時，會被識別為長按事件
  for (int i=0 ; i < MaxButton ; i++)  
  {
      myButton.setButtonLongOnTime((uint8_t)i, longpressed);  
  }
}

//-------------------------
// 取得所有聯集（串接）按鈕的狀態
// 回傳一個字串，內容為每個按鈕的狀態碼（從第 1 顆到第 MaxButton 顆）
// 狀態碼定義：
//   '0'：無動作（按鈕未被按下）
//   '1'：短按（按壓時間小於長按閥值）
//   '2'：長按（按壓時間大於或等於長按閥值）
//
// 使用時機：主迴圈中定期呼叫，檢查是否有按鈕事件發生
//-------------------------
String getAllButton() 
{
  String tmp = "";   // 用來累加所有按鈕狀態的字串

  // 當中斷旗標被設定為 1（表示有按鈕事件發生）
  if (int_flag) 
  {
    int_flag = 0;  // 重設旗標，避免重複觸發（一次事件只處理一次）

    // 從模組讀取所有按鈕的狀態，存入 BTstatus 陣列
    // BTstatus[i] 代表第 i 顆按鈕的狀態（i 從 0 開始）
    // 注意：陣列索引 0 可能對應第 1 顆按鈕，實際使用時需確認
    buttonStatus = myButton.getButtonStatus(BTstatus);

    // 將所有按鈕狀態轉成字串並累加
    // 從索引 1 開始（跳過第 0 顆），因為第 0 顆可能對應第一顆按鈕
    for(int i = 1 ; i <= MaxButton; i++)  
    {
      // 將數值轉換為字元後加入字串
      // 例如：BTstatus[1]=1 時，會加入 '1'
      tmp.concat(BTstatus[i]);  
    }
  }  

  // 將按鈕狀態輸出到序列埠監控視窗（方便除錯）
  // 若無按鈕事件，tmp 為空字串，不輸出
  if (tmp.length() > 0)
  {
    Serial.print("All Button:(") ;
    Serial.print(tmp) ;
    Serial.print(")\n") ;
  }

  return tmp;   // 回傳所有按鈕狀態的字串
}

//-------------------------
// 取得指定編號按鈕的狀態碼
// 參數 btn：按鈕編號（從 1 開始）
// 回傳值：
//   0x00：未按下
//   0x01：短按
//   0x02：長按
//   若按鈕編號超出範圍，回傳 0x00
// 
// 使用時機：需要讀取特定按鈕狀態時呼叫
//-------------------------
uint8_t getButtonStatus(uint8_t btn)  // 回傳第 btn 按鈕的狀態碼
{
  // 檢查按鈕編號是否超出所有按鈕數量
  if (btn > MaxButton) 
  {
      return 0x00;  // 超出範圍，回傳「未按下」狀態
  }
  else 
  {
    // 當中斷旗標被設定為 1（表示有按鈕事件發生）
    if (int_flag)   // 表示有按鈕事件發生
    {
      int_flag = 0;  // 重設旗標，避免重複觸發
      
      // 從模組讀取指定按鈕的狀態並回傳
      // 狀態可能為：0=無動作，1=短按，2=長按
      return myButton.getButtonStatus(btn);
    }
    else
    {
      // 沒有按鈕事件發生時，回傳「無動作」狀態
      return 0x00;
    }
  }
}

//-------------------------
// 取得指定編號按鈕的狀態碼（功能與 getButtonStatus 相同）
// 參數 btn：按鈕編號（從 1 開始）
// 回傳值：
//   0x00：未按下
//   0x01：短按
//   0x02：長按
//   若按鈕編號超出範圍，回傳 0x00
// 
// 此函式為 getButtonStatus 的別名，提供相容性
//-------------------------
uint8_t getButton(uint8_t btn)  // 回傳第 btn 按鈕的狀態碼
{
  // 檢查按鈕編號是否超出所有按鈕數量
  if (btn > MaxButton) 
  {
      return 0x00;  // 超出範圍，回傳「未按下」狀態
  }
  else 
  {
    // 當中斷旗標被設定為 1（表示有按鈕事件發生）
    if (int_flag)   // 表示有按鈕事件發生
    {
      int_flag = 0;  // 重設旗標，避免重複觸發
      
      // 從模組讀取指定按鈕的狀態並回傳
      // 狀態可能為：0=無動作，1=短按，2=長按
      return myButton.getButtonStatus(btn);
    }
    else
    {
      // 沒有按鈕事件發生時，回傳「無動作」狀態
      return 0x00;
    }
  }
}