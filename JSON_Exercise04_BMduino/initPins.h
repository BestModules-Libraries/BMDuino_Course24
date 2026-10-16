//---------------------- 參數定義區 ----------------------//
#define _Debug 1      // 偵錯開關，可用來控制是否印出除錯訊息
#define _debug 1      // 同上，視用途可分開控制
#define initDelay 6000    // 初始化等待延遲時間（單位：毫秒）
#define loopdelay 2000    // 主迴圈延遲時間（單位：毫秒）

//---------------------- 全域變數區 ----------------------//
String IPData = "127.0.0.1";   // 儲存裝置目前的 IP 位址
String APname = "NUKIOT";      // 儲存熱點名稱（SSID）
String MacData = "E89F6DE8F3BC"; // 儲存裝置的 MAC 位址（網卡硬體編號）

//---------------------- 初始化 WiFi 函式 ----------------------//
void initWiFi()  // 模擬 WiFi 初始化後輸出結果
{
    Serial.println("WiFi connected");         // 印出已連線訊息
    Serial.print("AP Name: ");                // 顯示 AP 名稱標籤
    Serial.println(APname);                   // 顯示熱點名稱
    Serial.print("IP address: ");             // 顯示 IP 標籤
    Serial.println(IPData);                   // 顯示當前取得的 IP 位址
}

//---------------------- 顯示網路資訊函式 ----------------------//
void ShowInternet()
{
    Serial.print("MAC:");
    Serial.print(MacData);
    Serial.print("\n");
    Serial.print("SSID:");
    Serial.print(APname);
    Serial.print("\n");
    Serial.print("IP:");
    Serial.print(IPData);
    Serial.print("\n");

    // 若有 OLED 顯示器可使用下列函式：
    // OledLineText(1,"MAC:"+MacData);
    // OledLineText(2,"IP:"+IPData);
}

//---------------------- 數學與字串通用函式區 ----------------------//

// 計算 num 的 expo 次方，例如 POW(2, 3) = 8
long POW(long num, int expo)
{
  long tmp = 1;
  if (expo > 0)
  { 
    for(int i = 0 ; i < expo ; i++)
      tmp = tmp * num;
    return tmp;
  } 
  else
  {
    return tmp;
  }
}

// 產生指定數量的空白字元並回傳為字串
String SPACE(int sp)
{
  String tmp = "";
  for (int i = 0 ; i < sp; i++)
    tmp.concat(' ');
  return tmp;
}

// 將數字轉為固定長度的字串，並補零（支援不同進位）
String strzero(long num, int len, int base)
{
  String retstring = "";
  char tmp[10];
  int tmpchr = 0;
  long tmpnum = num;
  char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

  int ln = 1;
  while (ln <= len)
  {
    tmpchr = (int)(tmpnum % base);
    tmp[ln-1] = hexcode[tmpchr];
    tmpnum = (long)(tmpnum / base);
    ln++;
  }

  // 倒序輸出
  for (int i = len - 1; i >= 0; i--)
    retstring.concat(tmp[i]);

  return retstring;
}

// 將十六進位字串轉為十進位整數（例如 "1A" 轉為 26）
unsigned long unstrzero(String hexstr, int base)
{
  int len = hexstr.length();
  unsigned long tmpnum = 0;
  String hexcode = "0123456789ABCDEF";
  hexstr.toUpperCase(); // 統一轉大寫

  for (int i = 0 ; i < len; i++)
  {
    int tmp = hexstr.charAt(i);
    int tmp1 = hexcode.indexOf(tmp);
    tmpnum += tmp1 * POW(base, (len - i - 1));
  }
  return tmpnum;
}

// 將 int 轉為 2 位數的十六進位字串（前面補0）
String print2HEX(int number)
{
  String ttt;
  if (number >= 0 && number < 16)
    ttt = "0" + String(number, HEX);
  else
    ttt = String(number, HEX);
  return ttt;
}

// 模擬取得 MAC 位址的函式（實務上應由 WiFi 套件取得）
String GetMacAddress()
{
  String Tmp = "AABBCCDDEEFF";
  Tmp.toUpperCase();
  return Tmp;
}

// 將字元指標（char*）轉為 String 型態
String chrtoString(char *p)
{
  String tmp;
  char c;
  int count = 0;
  while (count < 100)
  {
    c = *p;
    if (c != 0x00)
      tmp.concat(String(c));
    else
      return tmp;
    count++;
    p++;
  }
}

// 將 String 內容複製到 char 陣列
void CopyString2Char(String ss, char *p)
{
  if (ss.length() <= 0)
  {
    *p = 0x00;
    return;
  }
  ss.toCharArray(p, ss.length() + 1); // 包含結尾 '\0'
}

// 比較兩個 char* 字串是否相同
boolean CharCompare(char *p, char *q)
{
  int count = 0;
  int nomatch = 0;
  while (count < 100)
  {
    if (*(p + count) == 0x00 || *(q + count) == 0x00)
      break;
    if (*(p + count) != *(q + count))
      nomatch++;
    count++;
  }
  return nomatch == 0;
}

// 將 double 浮點數轉為字串，並保留指定小數位數
String Double2Str(double dd, int decn)
{
  int a1 = (int)dd; // 取整數部分
  int a3;
  if (decn > 0)
  {
    double a2 = dd - a1; // 取小數部分
    a3 = (int)(a2 * (10 ^ decn)); // 轉為整數形式小數
    return String(a1) + "." + String(a3);
  }
  else
  {
    return String(a1);
  }
}
