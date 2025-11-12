#define _Debug 1 // 除錯模式開啟 (1: 開啟, 0: 關閉)
#include <String.h> // 引入處理字串的函數庫

#define IOon HIGH
#define IOoff LOW


// 除錯訊息輸出函數 (不換行)
void DebugMsg(String msg)// 除錯訊息輸出函數 (不換行)
{
    if (_Debug != 0)  //除錯訊息(啟動)
    {
        Serial.print(msg) ; // 顯示訊息:msg變數內容
    }
}

// 除錯訊息輸出函數 (換行)
void DebugMsgln(String msg)// 除錯訊息輸出函數 (換行)
{
    if (_Debug != 0)  //除錯訊息(啟動)
    {
        Serial.println(msg) ; // 顯示訊息:msg變數內容
    }
}

//----------Common Lib
void GPIOControl(int GP,int cmd)  //GPIO 控制輸出高低電位函式
{
  // GP == GPIO 號碼
  //cmd =高電位或低電位 ，cmd =>1 then 高電位， cmd =>0 then 低電位
  if (cmd==1) //cmd=1 ===>GPIO is IOon
  {
      digitalWrite(GP,IOon);
  }
  else if(cmd==0) //cmd=0 ===>GPIO is IOoff
  {
      digitalWrite(GP,IOoff) ;
  }
}


// 計算 num 的 expo 次方
long POW(long num, int expo)
{
  long tmp = 1;  //暫存變數

  if (expo > 0) //次方大於零
  { 
    for (int i = 0; i < expo; i++)  //利用迴圈累乘
  {
      tmp = tmp * num;  // 不斷乘以num
    }
    return tmp;   //回傳產生變數
  } 
  else 
  {
    return tmp;  // 若expo小於或等於0，返回1
  }
}

// 生成指定長度的空格字串
String SPACE(int sp)  //sp為傳入產生空白字串長度
{
  String tmp = "";  //產生空字串
  for (int i = 0; i < sp; i++)  //利用迴圈累加空白字元
  {
    tmp.concat(' ');  // 加入空格
  }
  return tmp; //回傳產生空白字串
}

// 生成指定長度的字元C字串
String genstr(char c,int sp)  //sp為傳入產生空白字串長度
{
  String tmp = "";  //產生空字串
  for (int i = 0; i < sp; i++)  //利用迴圈累加空白字元
  {
    tmp.concat(c);  // 加入空格
  }
  return tmp; //回傳產生空白字串
}

// 轉換數字為指定長度與進位制的字串，並補零
String strzero(long num, int len, int base)
{
  //num 為傳入的數字
  //len為傳入的要回傳字串長度之數字
  // base 幾進位
  String retstring = String("");  //產生空白字串
  int ln = 1; //暫存變數
  int i = 0;  //計數器
  char tmp[10]; //暫存回傳內容變數
  long tmpnum = num;  //目前數字
  int tmpchr = 0; //字元計數器
  char hexcode[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  //產生字元的對應字串內容陣列
  while (ln <= len) //開始取數字
  {
    tmpchr = (int)(tmpnum % base);  //取得第n個字串的數字內容，如1='1'、15='F'
    tmp[ln - 1] = hexcode[tmpchr];  //根據數字換算對應字串
    ln++;
    tmpnum = (long)(tmpnum / base); // 求剩下數字
  }
  for (i = len - 1; i >= 0; i--)
  {
    retstring.concat(tmp[i]);//連接字串
  }
  return retstring;  //回傳內容
}


// 轉換指定進位制的字串為數值
unsigned long unstrzero(String hexstr, int base) 
{
  String chkstring; //暫存字串
  int len = hexstr.length();  // 取得長度
  unsigned int i = 0;
  unsigned int tmp = 0; //取得文字之字串位置變數
  unsigned int tmp1 = 0;  //取得文字之對應字串位置變數
  unsigned long tmpnum = 0; //目前數字
  String hexcode = String("0123456789ABCDEF");   //產生字元的對應字串內容陣列
  for (i = 0; i < len; i++)
  {
    hexstr.toUpperCase(); //先轉成大寫文字
    tmp = hexstr.charAt(i); //取第i個字元
    tmp1 = hexcode.indexOf(tmp);  //根據字元，判斷十進位數字
    tmpnum = tmpnum + tmp1 * POW(base, (len - i - 1));  //計算數字
  }
  return tmpnum;  //回傳內容
} 


// 轉換數字為 16 進位字串，若小於 16 則補 0
String print2HEX(int number) 
{
  String ttt;   //暫存字串
  if (number >= 0 && number < 16) //判斷是否在區間
  {
    ttt = String("0") + String(number, HEX);  //產生前補零之字串
  }
  else
  {
    ttt = String(number, HEX);//產生字串
  }
  return ttt; //回傳內容
} 

// 將 char 陣列轉為字串
String chrtoString(char *p)
{
    String tmp; //暫存字串
    char c; //暫存字元
    int count = 0;  //計數器
    while (count < 100) //100個字元以內
    {
        c = *(p+count); //取得字串之每一個字元內容
        if (c != 0x00)  //是否未結束
        {
            tmp.concat(String(c));  //字元累積到字串
        }
        else
        {
            return tmp; //回傳內容
        }
        count++;  // 計數器加一
    }
     return tmp; //回傳內容
}

// 複製 String 到 char 陣列
void CopyString2Char(String ss, char *p)
{
  if (ss.length() <= 0) //是否為空字串
  {
    *p = 0x00;  //加上字元陣列結束0x00
    return; //結束
  }
  ss.toCharArray(p, ss.length() + 1); //利用字串轉字元命令
}

// 比較兩個 char 陣列是否相同
boolean CharCompare(char *p, char *q) 
{
  // *p第一字元陣列的指標 :陣列第一字元的字元指標(用&chararray[0]取得)
  boolean flag = false; //是否結束旗標
  int count = 0;  //計數器
  int nomatch = 0;  //不相同比對計數器
  while (count < 100)  ////是否結束
  {
      if (*(p + count) == 0x00 || *(q + count) == 0x00) //是否結束
          break;  //離開
      if (*(p + count) != *(q + count)) //比較不同
      {
          nomatch++;      //不相同比對計數器累加
      }
      count++;    //計數器累加
  }
  return nomatch == 0;  //回傳是否有不同
}

// 將 double 轉為字串，保留指定小數位數
String Double2Str(double dd, int decn)
{
  //double dd==>傳入之浮點數
  //int decn==>傳入之保留指定小數位數
  int a1 = (int)dd; // 先取整數位數字
  int a3;   //小數點站存變數
  if (decn > 0) //保留指定小數位數大於零
  {
      double a2 = dd - a1;  //取小數位數字
      a3 = (int)(a2 * pow(10, decn)); // 將取得之小數位數字放大10的decn倍
  }
  if (decn > 0) //保留指定小數位數大於零
  {
      return String(a1) + "." + String(a3); 
      //將整數位轉乘之文字+小數點+小數點之擴大長度之數字轉換文字==>產生新字串回傳
  }
  else
  {
      return String(a1);//將整數位轉乘之文字==>產生新字串回傳
  }
}

String getjson(String ss)
{
  //Search array
  String tmp = "" ;
  int s1 = ss.indexOf('[');
  int s2 = ss.indexOf('{');
  int st1,st2 ;
  if (s1 == -1) //not array
    {
      if (s2 == -1) //not found '{'
        {
          return tmp ;  //return tmpty string
        }
        else
        {
          st1 = s2 ;  //get start point of '{}'
          st2 = ss.lastIndexOf("}") ; // get last start point of '}'
          tmp = ss.substring(st1,st2) ; //get json doc
          return tmp ;
        }
    }
    else
    {
        if (s1 < s2)  // json array
        {
          st1 = s1;
          st2 = ss.lastIndexOf("]") ; // get last start point of '}'
          tmp = ss.substring(st1,st2) ; //get json doc
          return tmp ;

        }
        else
        {
          st1 = s2;
          st2 = ss.lastIndexOf("}") ; // get last start point of '}'
          tmp = ss.substring(st1,st2) ; //get json doc
          return tmp ;
        }
    }

}