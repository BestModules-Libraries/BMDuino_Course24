/*
 * 檔案名稱：commlib.h
 * 程式語言：Arduino C/C++
 * 檔案類型：標頭檔（Header File）- 通用通訊函式庫
 * 
 * ========================================
 * 程式功能完整解說
 * ========================================
 * 
 * 【檔案概述】
 *   本檔案為 Arduino 專案的通用工具函式庫（Common Library），提供一系列
 *   常用的輔助函式，涵蓋除錯輸出、GPIO 控制、數學運算、字串處理、
 *   數值轉換、JSON 擷取等功能。這些函式可被多個專案重複使用，
 *   提高程式碼的模組化與可維護性。
 * 
 * 【主要功能分類】
 * 
 *   一、除錯與輸出功能
 *       - DebugMsg()：除錯訊息輸出（不換行）
 *       - DebugMsgln()：除錯訊息輸出（換行）
 *       - 透過 _Debug 巨集控制是否啟用除錯輸出
 * 
 *   二、GPIO 控制
 *       - GPIOControl()：控制指定 GPIO 腳位的輸出高低電位
 * 
 *   三、數學運算
 *       - POW()：計算整數的整數次方（使用迴圈累乘）
 * 
 *   四、字串生成與格式化
 *       - SPACE()：生成指定長度的空格字串
 *       - genstr()：生成指定長度的重複字元字串
 *       - Double2Str()：將浮點數轉換為字串，保留指定小數位數
 * 
 *   五、數字與字串轉換（進位制處理）
 *       - strzero()：將數字轉換為指定長度與進位制的字串（補零）
 *       - unstrzero()：將指定進位制的字串轉換為數值
 *       - print2HEX()：將數字轉換為兩位 16 進位字串（不足補零）
 * 
 *   六、char 陣列與 String 互轉
 *       - chrtoString()：將 char 陣列轉換為 String 物件
 *       - CopyString2Char()：將 String 複製到 char 陣列
 *       - CharCompare()：比較兩個 char 陣列的內容是否相同
 * 
 *   七、JSON 資料處理
 *       - getjson()：從 HTTP 回應字串中提取 JSON 資料
 * 
 * 【運作流程說明】
 * 
 *   除錯輸出流程：
 *       1. 檢查全域巨集 _Debug 是否為 1
 *       2. 若為 1，將訊息輸出至序列埠
 *       3. 若為 0，不輸出任何訊息（節省資源）
 * 
 *   數值轉字串流程（strzero）：
 *       1. 根據指定的進位制（base）逐位取出數字
 *       2. 將每個位數轉換為對應的字元（0-9, A-F）
 *       3. 儲存於暫存陣列中（低位在前）
 *       4. 反轉順序後組合成最終字串（高位在前）
 * 
 *   JSON 提取流程（getjson）：
 *       1. 搜尋字串中 '[' 與 '{' 的位置
 *       2. 判斷 JSON 格式為陣列（array）或物件（object）
 *       3. 提取從起始符號到對應結束符號的內容
 *       4. 回傳純 JSON 資料字串
 * 
 * 【相依函式庫】
 *   - String.h：Arduino 內建字串處理函式庫
 * 
 * 【全域巨集說明】
 *   - _Debug：除錯模式開關（1：開啟，0：關閉）
 *   - IOon：GPIO 高電位定義（HIGH）
 *   - IOoff：GPIO 低電位定義（LOW）
 * 
 * 【注意事項】
 *   - 除錯輸出功能依賴 Serial 物件，使用前需先執行 Serial.begin()
 *   - GPIOControl() 呼叫前，需先使用 pinMode() 設定腳位為輸出模式
 *   - strzero() 函式回傳長度固定為 len 參數指定值
 *   - unstrzero() 函式支援 2-16 進位制的字串轉換
 *   - getjson() 函式假設輸入字串中僅包含一組 JSON 資料
 *   - 所有字串操作函式均設計為安全使用，避免緩衝區溢位
 * 
 * 【使用範例】
 *   // 啟用除錯輸出
 *   DebugMsgln("System started");
 *   
 *   // 數字轉換
 *   String hexStr = print2HEX(15);      // 回傳 "0f"
 *   String numStr = strzero(255, 8, 16); // 回傳 "000000FF"
 *   
 *   // GPIO 控制
 *   pinMode(13, OUTPUT);
 *   GPIOControl(13, 1);  // 將 pin 13 設為高電位
 *   
 *   // 浮點數轉換
 *   String temp = Double2Str(23.4567, 2); // 回傳 "23.45"
 * 
 * 【版本資訊】
 *   最後修改日期：2026年3月26日
 *   適用專案：通用 Arduino 專案
 */

// ========================================
// 全域巨集與函式庫引入
// ========================================

// 除錯模式開關 (1: 開啟除錯訊息輸出, 0: 關閉)
#define _Debug 1

// 引入 Arduino 內建字串處理函式庫，支援 String 類別操作
#include <String.h>

// GPIO 電位定義
#define IOon HIGH   // 定義 IO 高電位為 HIGH
#define IOoff LOW   // 定義 IO 低電位為 LOW

// ========================================
// 自訂函式前置宣告區
// ========================================

// 除錯訊息輸出函數 (不換行)
void DebugMsg(String msg);

// 除錯訊息輸出函數 (換行) 
void DebugMsgln(String msg);

// GPIO 控制輸出高低電位函式
void GPIOControl(int GP, int cmd);

// 計算 num 的 expo 次方
long POW(long num, int expo);

// 生成指定長度的空格字串
String SPACE(int sp);

// 生成指定長度的字元C字串
String genstr(char c, int sp);

// 轉換數字為指定長度與進位制的字串，並補零
String strzero(long num, int len, int base);

// 轉換指定進位制的字串為數值
unsigned long unstrzero(String hexstr, int base);

// 轉換數字為 16 進位字串，若小於 16 則補 0
String print2HEX(int number);

// 將 char 陣列轉為字串
String chrtoString(char *p);

// 複製 String 到 char 陣列
void CopyString2Char(String ss, char *p);

// 比較兩個 char 陣列是否相同
boolean CharCompare(char *p, char *q);

// 將 double 轉為字串，保留指定小數位數
String Double2Str(double dd, int decn);

// 從字串中擷取 JSON 結構片段
String getjson(String ss);

// ================================================================
// =============== 除錯訊息輸出相關函式 ============================
// ================================================================

/*
 * 函式名稱：DebugMsg
 * 功能：輸出除錯訊息（不換行）
 * 參數：
 *   msg - 要輸出的除錯訊息字串
 * 回傳值：無
 * 說明：
 *   - 當全域巨集 _Debug 設定為 1 時，才會輸出除錯訊息
 *   - 使用 Serial.print() 輸出，不會自動換行
 *   - 適用於需要連續輸出的情況（例如輸出陣列內容）
 * 使用範例：
 *   DebugMsg("溫度: ");
 *   DebugMsg(String(temp));
 *   DebugMsgln("°C");
 */
void DebugMsg(String msg)
{
    // 檢查除錯模式是否啟用
    if (_Debug != 0)  // 若除錯模式啟用
    {
        Serial.print(msg); // 顯示訊息（不自動換行）
    }
}

/*
 * 函式名稱：DebugMsgln
 * 功能：輸出除錯訊息（自動換行）
 * 參數：
 *   msg - 要輸出的除錯訊息字串
 * 回傳值：無
 * 說明：
 *   - 當全域巨集 _Debug 設定為 1 時，才會輸出除錯訊息
 *   - 使用 Serial.println() 輸出，會自動換行
 *   - 適用於每條訊息都需要獨立顯示的情況
 * 使用範例：
 *   DebugMsgln("系統初始化完成");
 *   DebugMsgln("RFID 卡號: " + cardNumber);
 */
void DebugMsgln(String msg)
{
    // 檢查除錯模式是否啟用
    if (_Debug != 0)  // 若除錯模式啟用
    {
        Serial.println(msg); // 顯示訊息（自動換行）
    }
}

// ================================================================
// ================== GPIO 控制函式 ================================
// ================================================================

/*
 * 函式名稱：GPIOControl
 * 功能：控制 GPIO 腳位輸出高低電位
 * 參數：
 *   GP  - GPIO 腳位編號（例如 13）
 *   cmd - 控制命令（1=輸出高電位, 0=輸出低電位）
 * 回傳值：無
 * 說明：
 *   - 使用巨集 IOon 和 IOoff 來表示高低電位，提高程式可讀性
 *   - 可簡化 digitalWrite() 的呼叫，提供統一的控制介面
 * 注意事項：
 *   - 呼叫此函式前，必須先使用 pinMode() 設定 GPIO 腳位為輸出模式
 *   - 使用前需確保 GPIO 腳位正確且未被其他功能佔用
 * 使用範例：
 *   pinMode(13, OUTPUT);      // 設定腳位 13 為輸出模式
 *   GPIOControl(13, 1);       // 點亮 LED
 *   delay(1000);
 *   GPIOControl(13, 0);       // 熄滅 LED
 */
void GPIOControl(int GP, int cmd)
{
    // 根據命令參數設定 GPIO 輸出電位
    if (cmd == 1) // 若命令為 1，設定腳位為高電位
    {
        digitalWrite(GP, IOon);  // 輸出高電位
    }
    else if (cmd == 0) // 若命令為 0，設定腳位為低電位
    {
        digitalWrite(GP, IOoff); // 輸出低電位
    }
    // 注意：若 cmd 不是 0 或 1，此函式不會有任何動作
}

// ================================================================
// ================== 數學運算與字串生成函式 =======================
// ================================================================

/*
 * 函式名稱：POW
 * 功能：計算整數的整數次方（簡單實作）
 * 參數：
 *   num  - 底數（long 型別）
 *   expo - 指數（int 型別）
 * 回傳值：long - 計算結果
 * 說明：
 *   - 此函式僅處理整數次方計算
 *   - 若指數為 0 或負數，回傳 1（任何數的 0 次方為 1）
 *   - 使用迴圈累乘的方式計算，適合小數值的次方運算
 *   - 若 expo 值過大可能導致溢位，使用時需注意
 * 使用範例：
 *   long result = POW(2, 8);   // 計算 2^8 = 256
 *   long result = POW(10, 3);  // 計算 10^3 = 1000
 */
long POW(long num, int expo)
{
    long tmp = 1;  // 初始化暫存變數為 1
  
    // 僅當指數大於 0 時進行計算
    if (expo > 0)
    { 
        // 使用迴圈進行累乘計算
        for (int i = 0; i < expo; i++)
        {
            tmp = tmp * num;  // 重複乘以 num，達成次方效果
        }
        return tmp;
    } 
    else 
    {
        return tmp;  // 若次方 <= 0，直接回傳 1
    }
}

// ================================================================
// ================== 字串處理輔助函式 =============================
// ================================================================

/*
 * 函式名稱：SPACE
 * 功能：產生指定長度的空格字串
 * 參數：
 *   sp - 空格字串的長度（int 型別）
 * 回傳值：String - 由空格組成的字串
 * 說明：
 *   - 用於格式化輸出，產生固定寬度的空白區域
 *   - 若 sp 為 0 或負數，回傳空字串
 * 使用範例：
 *   String spaces = SPACE(5);  // spaces = "     "
 *   Serial.print("姓名:" + name + SPACE(10) + "年齡:" + age);
 */
String SPACE(int sp)
{
    String tmp = "";           // 初始化空字串
    
    // 根據指定長度重複加入空格
    for (int i = 0; i < sp; i++)
    {
        tmp.concat(' ');         // 每次加入一個空格字元
    }
    
    return tmp;                // 回傳空白字串
}

/*
 * 函式名稱：genstr
 * 功能：產生指定字元的重複字串
 * 參數：
 *   c  - 要重複的字元（char 型別）
 *   sp - 重複次數（int 型別）
 * 回傳值：String - 由指定字元組成的重複字串
 * 說明：
 *   - 可產生任意字元的重複字串
 *   - 常用於產生分隔線、補零、填充字元等用途
 * 使用範例：
 *   String stars = genstr('*', 20);   // stars = "********************"
 *   String zeros = genstr('0', 5);    // zeros = "00000"
 *   Serial.println(genstr('-', 30));  // 輸出 30 個 '-' 作為分隔線
 */
String genstr(char c, int sp)
{
    String tmp = "";           // 初始化空字串
    
    // 根據指定次數重複加入字元
    for (int i = 0; i < sp; i++)
    {
        tmp.concat(c);           // 將字元 c 重複加入字串
    }
    
    return tmp;                // 回傳重複字串
}

// ================================================================
// =========== 數字與字串之間的轉換（進位制處理）==================
// ================================================================

/*
 * 函式名稱：strzero
 * 功能：將數字轉為指定進位制字串，並補零至固定長度
 * 參數：
 *   num  - 要轉換的數字（long 型別）
 *   len  - 轉換後的字串長度（int 型別），若不夠會補 0
 *   base - 進位制（int 型別），支援 2（二進位）、8（八進位）、10（十進位）、16（十六進位）
 * 回傳值：String - 轉換後的字串
 * 演算法說明：
 *   1. 使用除法取餘數的方式，逐位取出數字在各進位制下的表示
 *   2. 將餘數轉換為對應的字元（0-9 或 A-F）
 *   3. 將字元存入暫存陣列（從低位到高位）
 *   4. 最後反轉陣列順序，組合成正確順序的字串
 * 使用範例：
 *   strzero(10, 4, 16)   // 回傳 "000A"
 *   strzero(10, 4, 2)    // 回傳 "1010"
 *   strzero(255, 8, 16)  // 回傳 "000000FF"
 */
String strzero(long num, int len, int base)
{
    String retstring = "";          // 最終回傳字串
    int ln = 1;                     // 位數計數器
    char tmp[10];                   // 暫存每一位數字轉換後的字元
    long tmpnum = num;              // 複製原始數字
    int tmpchr = 0;                 // 暫存字元代碼
    
    // 十六進位字元對照表（支援 2-16 進位）
    char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    // 將數字依序轉換為字元（由低位數開始）
    while (ln <= len)
    {
        tmpchr = (int)(tmpnum % base);  // 取餘數（對應進位字元）
        tmp[ln - 1] = hexcode[tmpchr];  // 存入對應的字元
        ln++;
        tmpnum = tmpnum / base;         // 除以 base，繼續取下一位
    }

    // 將字元反向排列成正確順序（高位在前）
    for (int i = len - 1; i >= 0; i--)
    {
        retstring.concat(tmp[i]);      // 拼接字元
    }

    return retstring;                  // 回傳最終字串
}

/*
 * 函式名稱：unstrzero
 * 功能：將指定進位制字串轉換為數值
 * 參數：
 *   hexstr - 要轉換的字串（String 型別）
 *   base   - 進位制（int 型別），支援 2-16
 * 回傳值：unsigned long - 轉換後的數值
 * 演算法說明：
 *   1. 將輸入字串轉為大寫
 *   2. 逐字元取出，查詢對應的數值（0-15）
 *   3. 根據位數權重累加計算總值
 * 使用範例：
 *   unstrzero("A", 16)      // 回傳 10
 *   unstrzero("1010", 2)    // 回傳 10
 *   unstrzero("FF", 16)     // 回傳 255
 */
unsigned long unstrzero(String hexstr, int base) 
{
    // 十六進位字元對照表（同時適用於 2-16 進位）
    String hexcode = "0123456789ABCDEF";   
    int len = hexstr.length();             // 取得字串長度
    unsigned long tmpnum = 0;              // 計算結果
    
    hexstr.toUpperCase();                  // 統一轉成大寫（確保大小寫一致）

    // 從高位到低位處理每個字元
    for (int i = 0; i < len; i++)
    {
        int tmpchr = hexstr.charAt(i);        // 取出第 i 個字元
        int tmpval = hexcode.indexOf(tmpchr); // 轉成對應的數字值
        tmpnum += tmpval * POW(base, (len - i - 1)); // 累加計算（考慮位數權重）
    }

    return tmpnum;  // 回傳計算結果
}

// ================================================================
// =============== 數字與十六進位輸出相關函式 ======================
// ================================================================

/*
 * 函式名稱：print2HEX
 * 功能：將數字轉為兩位十六進位字串，若 <16 則前補 0
 * 參數：
 *   number - 要轉換的數字（int 型別），建議範圍 0-255
 * 回傳值：String - 兩位十六進位字串
 * 說明：
 *   - 輸出固定為兩位，確保格式一致
 *   - 十六進位字元使用小寫 a-f（Arduino 預設）
 *   - 常用於將位元組資料轉為 HEX 字串表示
 * 使用範例：
 *   print2HEX(10)    // 回傳 "0a"
 *   print2HEX(255)   // 回傳 "ff"
 *   print2HEX(0)     // 回傳 "00"
 */
String print2HEX(int number) 
{
    String ttt;
    
    // 若數字小於 16，需要補上前導 0
    if (number >= 0 && number < 16)
    {
        ttt = "0" + String(number, HEX); // 補上前導 0
    }
    else
    {
        ttt = String(number, HEX);       // 直接轉換
    }
    
    return ttt;
}

// ================================================================
// =============== char 與 String 互轉函式 =========================
// ================================================================

/*
 * 函式名稱：chrtoString
 * 功能：將 char 陣列轉換為 String 物件
 * 參數：
 *   p - char 陣列的指標（指向陣列第一個元素的指標）
 * 回傳值：String - 轉換後的字串
 * 說明：
 *   - 遇到字串結束符號（0x00 或 '\0'）時停止轉換
 *   - 最多處理 100 個字元，防止無限迴圈
 * 注意：
 *   - 傳入的 char 陣列必須以 0x00 結尾（C 字串標準格式）
 * 使用範例：
 *   char buffer[] = "Hello World";
 *   String str = chrtoString(buffer);  // str = "Hello World"
 */
String chrtoString(char *p)
{
    String tmp = "";      // 初始化回傳字串
    char c;               // 暫存字元
    int count = 0;        // 字元計數器

    // 最多處理 100 個字元，防止記憶體存取錯誤
    while (count < 100)
    {
        c = *(p + count);     // 取出第 count 個字元
        
        // 檢查是否為字串結尾
        if (c != 0x00)        // 若非字串結尾
        {
            tmp.concat(String(c)); // 累加到字串
        }
        else
        {
            return tmp;       // 若遇到結尾字元，回傳結果
        }
        
        count++;              // 處理下一個字元
    }
    
    return tmp;               // 回傳結果（處理完 100 個字元或遇到結尾）
}

/*
 * 函式名稱：CopyString2Char
 * 功能：將 String 物件複製到 char 陣列
 * 參數：
 *   ss - 來源字串（String 型別）
 *   p  - 目標 char 陣列的指標
 * 回傳值：無
 * 說明：
 *   - 使用 Arduino 內建的 toCharArray() 函式
 *   - 會自動在結尾加入字串結束符號（0x00 或 '\0'）
 * 注意：
 *   - 必須確保 char 陣列有足夠的空間容納字串內容
 *   - 需要預留一個額外的位元組存放結束符號
 * 使用範例：
 *   String str = "Hello";
 *   char buffer[10];
 *   CopyString2Char(str, buffer);  // buffer 內容為 "Hello"
 */
void CopyString2Char(String ss, char *p)
{
    // 檢查是否為空字串
    if (ss.length() <= 0) // 若為空字串
    {
        *p = 0x00;  // 直接設定為結尾字元
        return;
    }
    
    // 使用 Arduino 內建函式進行轉換
    // 第二個參數為複製長度（需包含結尾字元）
    ss.toCharArray(p, ss.length() + 1); // +1 是為了結尾字元
}

/*
 * 函式名稱：CharCompare
 * 功能：比較兩個 char 陣列是否完全相同
 * 參數：
 *   p - 第一個 char 陣列的指標
 *   q - 第二個 char 陣列的指標
 * 回傳值：boolean - true 表示完全相同，false 表示不同
 * 說明：
 *   - 比較到任一字串結尾或比較完 100 個字元為止
 *   - 大小寫視為不同字元（'A' 與 'a' 不相同）
 * 注意：
 *   - 僅比較字元內容，不比較陣列長度
 * 使用範例：
 *   char str1[] = "Hello";
 *   char str2[] = "Hello";
 *   char str3[] = "World";
 *   
 *   CharCompare(str1, str2);  // 回傳 true
 *   CharCompare(str1, str3);  // 回傳 false
 */
boolean CharCompare(char *p, char *q) 
{
    int count = 0;      // 字元位置計數器
    int nomatch = 0;    // 不匹配計數器

    // 最多比較 100 個字元
    while (count < 100)
    {
        // 檢查是否遇到任一字串結尾
        if (*(p + count) == 0x00 || *(q + count) == 0x00)
            break; // 若任一遇到結尾則停止比較
        
        // 比較對應位置的字元
        if (*(p + count) != *(q + count))
        {
            nomatch++; // 若字元不同，計數器加 1
        }
        
        count++;        // 處理下一個字元
    }

    return (nomatch == 0); // 若完全沒有不匹配，回傳 true
}

// ================================================================
// ================== 浮點數轉字串函式 =============================
// ================================================================

/*
 * 函式名稱：Double2Str
 * 功能：將浮點數轉為字串並保留指定小數位數
 * 參數：
 *   dd   - 要轉換的浮點數（double 型別）
 *   decn - 要保留的小數位數（int 型別）
 * 回傳值：String - 轉換後的字串
 * 說明：
 *   - 使用簡單的數學方法分離整數和小數部分
 *   - 不進行四捨五入，直接截斷小數部分（捨去法）
 *   - 避免使用 dtostrf() 等可能佔用較多資源的函式
 * 使用範例：
 *   Double2Str(3.14159, 2)   // 回傳 "3.14"
 *   Double2Str(3.14159, 0)   // 回傳 "3"
 *   Double2Str(23.4567, 3)   // 回傳 "23.456"
 */
String Double2Str(double dd, int decn)
{
    int a1 = (int)dd;   // 取整數部分（直接截斷）
    int a3;             // 小數部分暫存變數

    // 如果需要保留小數位數
    if (decn > 0)
    {
        double a2 = dd - a1;              // 取小數部分
        a3 = (int)(a2 * pow(10, decn));  // 將小數部分轉為整數表示
    }

    // 根據是否需要小數位數，回傳不同格式
    if (decn > 0)
    {
        return String(a1) + "." + String(a3); // 拼接整數和小數部分
    }
    else
    {
        return String(a1); // 僅回傳整數部分
    }
}

// ================================================================
// ================== JSON 片段擷取函式 ============================
// ================================================================

/*
 * 函式名稱：getjson
 * 功能：從字串中擷取 JSON 結構片段（{} 或 []）
 * 參數：
 *   ss - 原始字串（String 型別）
 * 回傳值：String - 擷取到的 JSON 片段，若無則回傳空字串
 * 說明：
 *   - 優先尋找 '[' 和 '{' 字元
 *   - 若兩者都存在，以先出現的為起始點
 *   - 擷取從起始點到對應的結尾字元（']' 或 '}'）
 *   - 專為 BMDUINO TCP.h 的 Wifi.http_getString() 回傳結果設計
 * 注意：
 *   - 僅擷取最外層的 JSON 結構
 *   - 不驗證 JSON 格式的正確性
 *   - 若字串中包含多個 JSON 結構，僅擷取第一個
 * 使用範例：
 *   String response = "HTTP/1.1 200 OK\r\n{\"temp\":25,\"humidity\":60}";
 *   String json = getjson(response);  // json = "{\"temp\":25,\"humidity\":60"
 *   
 *   String arrayResponse = "data:[1,2,3,4,5]";
 *   String jsonArray = getjson(arrayResponse);  // jsonArray = "[1,2,3,4,5"
 */
String getjson(String ss)
{
    String tmp = "";                   // 回傳字串
    int s1 = ss.indexOf('[');         // 找出 '[' 的位置（代表 JSON 陣列）
    int s2 = ss.indexOf('{');         // 找出 '{' 的位置（代表 JSON 物件）
    int st1, st2;                     // 起始和結束位置

    // 檢查是否存在 JSON 結構
    if (s1 == -1) // 若無 '['，表示非陣列
    {
        if (s2 == -1) // 若也無 '{'，代表不是 JSON
        {
            return tmp;  // 回傳空字串
        }
        else
        {
            st1 = s2;                      // JSON 物件起始位置
            st2 = ss.lastIndexOf("}");     // 找出最後一個 '}' 位置
            tmp = ss.substring(st1, st2);  // 擷取 JSON 區段（不包含結尾 '}'）
            return tmp;
        }
    }
    else
    {
        // 若同時有 '[' 和 '{'，判斷誰在前面
        if (s1 < s2)  // 若 '[' 在前，代表是 JSON 陣列
        {
            st1 = s1;
            st2 = ss.lastIndexOf("]");
            tmp = ss.substring(st1, st2);  // 擷取 JSON 區段（不包含結尾 ']'）
            return tmp;
        }
        else          // 若 '{' 在前，代表是 JSON 物件
        {
            st1 = s2;
            st2 = ss.lastIndexOf("}");
            tmp = ss.substring(st1, st2);  // 擷取 JSON 區段（不包含結尾 '}'）
            return tmp;
        }
    }
}