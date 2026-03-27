#define _Debug 1 // 除錯模式開啟 (1: 開啟除錯訊息輸出, 0: 關閉)
#include <String.h> // 引入 Arduino 內建字串處理函式庫，支援 String 類別操作

#define IOon HIGH   // 定義 IO 高電位為 HIGH
#define IOoff LOW   // 定義 IO 低電位為 LOW

//*********************************************//
//************ 自訂函式前置宣告區 ***************//
//*********************************************//
void DebugMsg(String msg);// 除錯訊息輸出函數 (不換行)
void DebugMsgln(String msg);// 除錯訊息輸出函數 (換行) 
void GPIOControl(int GP,int cmd);  //GPIO 控制輸出高低電位函式
long POW(long num, int expo);// 計算 num 的 expo 次方
String SPACE(int sp);  // 生成指定長度的空格字串
String genstr(char c,int sp);  // 生成指定長度的字元C字串
String strzero(long num, int len, int base); // 轉換數字為指定長度與進位制的字串，並補零
unsigned long unstrzero(String hexstr, int base); // 轉換指定進位制的字串為數值
String print2HEX(int number); // 轉換數字為 16 進位字串，若小於 16 則補 0
String chrtoString(char *p);// 將 char 陣列轉為字串
void CopyString2Char(String ss, char *p);// 複製 String 到 char 陣列
boolean CharCompare(char *p, char *q); // 比較兩個 char 陣列是否相同
String Double2Str(double dd, int decn);// 將 double 轉為字串，保留指定小數位數
String getjson(String ss);	//取得json

// ================================================================
// =============== 除錯訊息輸出相關函式 ============================
// ================================================================

// 除錯訊息輸出（不換行）
void DebugMsg(String msg)
{
  //輸出除錯訊息（不換行 / 換行）
    if (_Debug != 0)  // 若除錯模式啟用
    {
        Serial.print(msg); // 顯示訊息（不自動換行）
    }
}

// 除錯訊息輸出（換行）
void DebugMsgln(String msg)
{
  //輸出除錯訊息（不換行 / 換行）
    if (_Debug != 0)  // 若除錯模式啟用
    {
        Serial.println(msg); // 顯示訊息（自動換行）
    }
}


// ================================================================
// ================== GPIO 控制函式 ================================
// ================================================================

void GPIOControl(int GP, int cmd)
{
  //控制 GPIO 腳位輸出高低電位
  // GP  : 傳入 GPIO 腳位編號
  // cmd : 控制命令 (1=輸出高電位, 0=輸出低電位)

  if (cmd == 1) // 若命令為 1，設定腳位為高電位
  {
      digitalWrite(GP, IOon);
  }
  else if (cmd == 0) // 若命令為 0，設定腳位為低電位
  {
      digitalWrite(GP, IOoff);
  }
}


// ================================================================
// ================== 數學運算與字串生成函式 =======================
// ================================================================

// 計算 num 的 expo 次方（整數次方運算）
long POW(long num, int expo)
{
  //計算整數次方
  long tmp = 1;  // 初始化暫存變數為 1
  if (expo > 0)
  { 
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

// 產生指定長度的空格字串
String SPACE(int sp)
{
  //產生指定長度的空白或重複字元字串
  //sp為傳入產生空白字串長度
  String tmp = "";           // 初始化空字串
  for (int i = 0; i < sp; i++)
  {
    tmp.concat(' ');         // 每次加入一個空格字元
  }
  return tmp;                // 回傳空白字串
}


// 產生指定字元 c 的重複字串
String genstr(char c, int sp)
{
  //產生指定長度的空白或重複字元字串
  String tmp = "";           // 初始化空字串
  for (int i = 0; i < sp; i++)
  {
    tmp.concat(c);           // 將字元 c 重複加入字串
  }
  return tmp;
}


// ================================================================
// =========== 數字與字串之間的轉換 (進位制處理) ==================
// ================================================================

// 將數字轉為指定進位制字串，並補滿固定長度 (len)
String strzero(long num, int len, int base)
{
  //將數字轉為固定長度、指定進位制字串
  // num : 要轉換的數字
  // len : 轉換後的字串長度（若不夠會補 0）
  // base: 進位制 (2, 8, 10, 16)

  String retstring = "";          // 最終回傳字串
  int ln = 1;                     // 位數計數器
  char tmp[10];                   // 暫存每一位數字轉換後的字元
  long tmpnum = num;              // 複製原始數字
  int tmpchr = 0;                 // 暫存字元代碼
  char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  // 將數字依序轉換為字元（由低位數開始）
  while (ln <= len)
  {
    tmpchr = (int)(tmpnum % base);  // 取餘數（對應進位字元）
    tmp[ln - 1] = hexcode[tmpchr];  // 存入對應的字元
    ln++;
    tmpnum = tmpnum / base;         // 除以 base，繼續取下一位
  }

  // 將字元反向排列成正確順序
  for (int i = len - 1; i >= 0; i--)
  {
    retstring.concat(tmp[i]);
  }

  return retstring;
}


// 將指定進位制字串轉換為數值
unsigned long unstrzero(String hexstr, int base) 
{
  //將指定進位制字串轉回數字
  String hexcode = "0123456789ABCDEF";   // 定義進位對照表
  int len = hexstr.length();             // 取得字串長度
  unsigned long tmpnum = 0;              // 計算結果
  hexstr.toUpperCase();                  // 統一轉成大寫

  for (int i = 0; i < len; i++)
  {
    int tmpchr = hexstr.charAt(i);       // 取出第 i 個字元
    int tmpval = hexcode.indexOf(tmpchr); // 轉成對應的數字值
    tmpnum += tmpval * POW(base, (len - i - 1)); // 累加計算
  }

  return tmpnum;  // 回傳結果
}


// ================================================================
// =============== 數字與十六進位輸出相關函式 ======================
// ================================================================

// 將數字轉為兩位十六進位字串，若 <16 則前補 0
String print2HEX(int number) 
{
  //將數字轉為兩位十六進位字串（前補 0）
  String ttt;
  if (number >= 0 && number < 16)
  {
    ttt = "0" + String(number, HEX); // 補上前導 0
  }
  else
  {
    ttt = String(number, HEX);
  }
  return ttt;
}


// ================================================================
// =============== char 與 String 互轉函式 =========================
// ================================================================

// 將 char 陣列轉為 String
String chrtoString(char *p)
{
  //char 與 String 互轉
    String tmp = "";
    char c;
    int count = 0;

    while (count < 100)
    {
        c = *(p + count);     // 取出第 count 個字元
        if (c != 0x00)        // 若非字串結尾
        {
            tmp.concat(String(c)); // 累加到字串
        }
        else
        {
            return tmp;       // 若遇到結尾字元，回傳結果
        }
        count++;
    }
    return tmp;
}


// 將 String 複製到 char 陣列
void CopyString2Char(String ss, char *p)
{
  //char 與 String 互轉
  if (ss.length() <= 0) // 若為空字串
  {
    *p = 0x00;  // 加入結尾字元
    return;
  }
  ss.toCharArray(p, ss.length() + 1); // 使用內建函式轉換
}


// 比較兩個 char 陣列是否相同
boolean CharCompare(char *p, char *q) 
{
  //比較兩個字元陣列是否相同
  int count = 0;
  int nomatch = 0;

  while (count < 100)
  {
      if (*(p + count) == 0x00 || *(q + count) == 0x00)
          break; // 若任一遇到結尾則停止
      if (*(p + count) != *(q + count))
      {
          nomatch++; // 若字元不同，計數器加 1
      }
      count++;
  }

  return (nomatch == 0); // 若完全相同則回傳 true
}


// ================================================================
// ================== 浮點數轉字串函式 =============================
// ================================================================

String Double2Str(double dd, int decn)
{
  //將浮點數轉為字串並保留小數位數
  // dd: 傳入的浮點數
  // decn: 要保留的小數位數

  int a1 = (int)dd; // 取整數部分
  int a3;            // 小數部分暫存

  if (decn > 0)
  {
      double a2 = dd - a1;             // 取小數部分
      a3 = (int)(a2 * pow(10, decn)); // 小數轉整數表示
  }

  if (decn > 0)
  {
      return String(a1) + "." + String(a3); // 拼接成完整字串
  }
  else
  {
      return String(a1); // 若不需小數，僅回傳整數部分
  }
}


// ================================================================
// ================== JSON 片段擷取函式 ============================
// ================================================================

String getjson(String ss)
{
  //從字串中擷取 JSON 結構（支援 {} 與 []）
  // 此函式會分析傳入字串 ss，
  // 若內含 JSON 結構（{} 或 []），則擷取出 JSON 區段內容並回傳。

  String tmp = "";
  int s1 = ss.indexOf('[');  // 找出 '[' 的位置（代表 JSON 陣列）
  int s2 = ss.indexOf('{');  // 找出 '{' 的位置（代表 JSON 物件）
  int st1, st2;

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
          tmp = ss.substring(st1, st2);  // 擷取 JSON 區段
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
          tmp = ss.substring(st1, st2);
          return tmp;
      }
      else          // 若 '{' 在前，代表是 JSON 物件
      {
          st1 = s2;
          st2 = ss.lastIndexOf("}");
          tmp = ss.substring(st1, st2);
          return tmp;
      }
  }
}
