// ================================================================
// =============== 全域變數宣告區 (Global Variables) ===============
// ================================================================

// 宣告三個全域變數，用於儲存 WiFi 模組相關資訊
String MacData;   // 儲存目前裝置的 WiFi MAC Address（網路卡的唯一識別碼）
String SSIDData;  // 儲存目前連線的 WiFi 熱點名稱 (SSID)
String IPData;    // 儲存 WiFi 連線後由路由器分配的 IP 位址

//--------將所有感測模組自訂函式引用進來-------------------

#include <String.h>    // Arduino 內建字串處理函式庫
#include "commlib.h"   // 通訊相關的共用函式庫（可能包含封包處理、緩衝區管理等）
#include "CO2Lib.h"  //二氧化碳偵測模組(BMCOM) BME58M332 獨立模組
#include "TCP.h"       // TCP 通訊函式庫（包含 WiFi 初始化、MAC 取得等函式）
#include "clouding.h"       // REST Ful API 基本模組

/*
 * CO2Lib.h：
 *   - 此為自訂的 CO2 感測器函式庫（你自行撰寫或封裝的版本）
 *   - 內含：
 *       initCO2()      → 初始化 CO2 模組
 *       readCO2()      → 讀取 CO2 濃度（ppm）
 *       CO2Value       → 全域變數，儲存 CO2 數值
 *   - 程式主體只需要 include 本檔即可使用所有功能
 */


//--------自訂函式宣告區-------------------
void initSensor();	// 初始化所有感測模組（目前包含 CO2 模組）
void initAll();		// 初始化整體系統（序列埠、感測器…等）




/*************************************************
 * setup()：系統初始化（Arduino 開機後只會執行一次）
 *************************************************/
void setup()
{
   initAll();  // 初始化整體系統（啟動序列埠、感測模組初始化）
  delay(200);      // 延遲 200ms，確保硬體模組穩定
  INITtWIFI();     // 初始化 WiFi 網路，並取得 SSID、IP 與 MAC 資料
   Serial.println("Enter Loop()");  
   // 顯示訊息：系統初始化完成，準備進入主迴圈 loop()
}




/*************************************************
 * loop()：主迴圈（Arduino 不斷重複執行的區域）
 *************************************************/
void loop()
{
  CO2Value = readCO2();  // 讀取二氧化碳量測值（ppm）

  Serial.print("CO2: ");
  Serial.print(CO2Value);  // 顯示 CO2 濃度（ppm）
  Serial.println(" ppm");
   SendtoClouding() ;    //傳送感測資料到雲端

  delay(30000);  
  /*
   * 延遲 30 秒：
   * - 避免量測過於頻繁導致 UART 資料壅塞
   * - CO2 感測模組本身更新速率約 1 秒，因此 30 秒間隔足夠
   */
}




//------------------------------------------------------------------
//--------自訂函式定義區（功能與邏輯實作區域）----------------------
//------------------------------------------------------------------

// 初始化所有感測模組
void initSensor()
{
  initCO2();  
  /*
   * initCO2():
   * ---------------------------------------------------
   * - 初始化 CO2 感測模組 UART
   * - 執行預熱 countdown（約 60 秒）
   * - 設定量測上限（常用 2000 / 5000 ppm）
   * ---------------------------------------------------
   */
}



// 初始化整體系統
void initAll()
{
  Serial.begin(9600);  
  /*
   * 初始化序列埠（Serial Monitor）：
   * - 波特率 9600 bps
   * - 用於顯示感測資料與除錯訊息
   */

  Serial.println("System Start.....");
  // 印出系統啟動提示文字

  initSensor();  
  /*
   * 初始化所有感測器模組：
   * - 目前只有 CO2（initCO2）
   * - 若未來加入溫濕度、PM2.5、VOC 等模組，可統一放在這裡
   */
}
// 初始化 WiFi
void INITtWIFI()
{
  initWiFi();   // 初始化 WiFi 功能
  ShowWiFiInformation() ; //顯示wifi的基本參數

}


void ShowWiFiInformation()  //顯示wifi的基本參數
{
  //顯示wifi的基本參數
  MacData = GetMAC();     // 取得裝置的 MAC 位址
  Serial.println("---MAC Address----");
  Serial.println(MacData);

  Serial.println("");
  Serial.println("---wifi access point----");

  SSIDData = GetSSID();   // 取得連線的 WiFi 熱點名稱
  Serial.println(SSIDData);

  Serial.println("---Show IP Address----");
  IPData = GetIP();       // 取得裝置分配到的 IP 位址
  Serial.println(IPData);


}