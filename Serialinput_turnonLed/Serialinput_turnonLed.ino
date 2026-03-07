// 宣告一個整數陣列 ledpin[]，代表接在腳位 D8 ~ D13 的 6 顆 LED 燈
int ledpin[] = {8, 9, 10, 11, 12, 13};  // 六顆 LED 燈分別接在這些數位腳位上

int num = -1;         // 儲存從序列輸入轉成的數字（代表要亮的燈號）
String readstr = "";  // 用來暫存從序列接收到的字串輸入
int i, j;             // 迴圈控制用變數

// setup() 函式：當 Arduino 上電或按下重設鍵時只會執行一次
void setup() 
{
  Serial.begin(9600);  // 啟動序列通訊，設定傳輸速率為 9600 bps（與電腦端同步）
  
  // 顯示啟動訊息到序列監控器
  Serial.println("Start System");  
  Serial.println("Please Input a Number n to Turn on Ledn");  // 提示使用者輸入 LED 號碼

  // 將所有 LED 所接的腳位設為「輸出模式」並先全部關閉
  for (i = 0; i <= 5; i++)  // 共有六個腳位（索引從 0 到 5）
  {
    pinMode(ledpin[i], OUTPUT);     // 將第 i 個 LED 的腳位設為輸出
    digitalWrite(ledpin[i], LOW);   // 將該腳位設為低電位（關閉 LED）
  }
}

// loop() 函式：主要的重複執行區，每秒跑數百次（Arduino 核心迴圈）
void loop() 
{
  // 檢查是否有資料從序列埠輸入（例如使用者從電腦序列監控器輸入）
  if (Serial.available() > 0)
  {
    readstr = Serial.readString();    // 從序列埠讀取整行字串（直到換行符為止）
    num = readstr.toInt();            // 將字串轉為整數，例如 "3" → 3

    // 顯示使用者輸入的字串資料，方便除錯或觀察
    Serial.print("From Serial:(");
    Serial.print(readstr);
    Serial.print(")\n");
  }

  // 透過迴圈控制 LED 開與關：只亮指定編號那一顆，其餘全部關掉
  for (j = 1; j <= 6; j++)  // j 表示第 j 顆燈，從 1 到 6
  {
    if (num == j)
    {
      // 若輸入的燈號等於 j，則點亮該燈（陣列索引需減 1）
      digitalWrite(ledpin[j - 1], HIGH);
    }
    else
    {
      // 其餘非輸入燈號的 LED 全部關閉
      digitalWrite(ledpin[j - 1], LOW);
    }
  }

  // 本程式未使用 delay()，因此會即時回應每次輸入
}
