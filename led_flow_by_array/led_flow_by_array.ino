// 宣告一個整數陣列 ledpin[]，裡面包含六個 LED 接在的腳位號碼
int ledpin[] = {8, 9, 10, 11, 12, 13};  // 設定六個 LED 燈的 GPIO 腳位

// setup() 函式只在開機或重設時執行一次
void setup() 
{
  Serial.begin(9600);  // 啟動序列通訊，設定傳輸速率為 9600 bps
  Serial.println("Start System");  // 印出啟動訊息到序列監控器

  // 透過迴圈將 GPIO 腳位 8 到 13 都設定為「輸出」模式，並初始化為 LOW（關閉 LED）
  for (int i = 8; i <= 13; i++)
  {
    pinMode(i, OUTPUT);      // 將 i 號腳位設定為輸出模式
    digitalWrite(i, LOW);    // 將該腳位預設為 LOW（LED 關閉）==>預設關閉燈泡
  }
}

// loop() 函式會無限重複執行，是 Arduino 程式的核心邏輯區塊
void loop() 
{
  // 外層迴圈：i 從 D8 遞增到 D13，控制目前要亮哪一個 LED
  for (int i = 8; i <= 13; i++) //控制目前要亮哪一個 LED(D8 遞增到 D13)
  {
    // 內層迴圈：j 從 D8 到 D13，遍歷所有 LED 腳位來控制每一顆的開與關
    for (int j = 8; j <= 13; j++) //就是透過本迴圈來導引六顆燈亮滅
    {
       if (i == j)  // 如果 i 等於 j，表示該顆 LED 是目前應該點亮的
      {
        //如果與上一層 亮第i個燈，將該腳位設為 HIGH（點亮）
        digitalWrite(j, HIGH);  // 將該腳位設為 HIGH（點亮）
      }
      else  // 其他 LED 都關閉
      {
         //如果與上一層 亮第i個燈不同，將該腳位設為 LOW（關閉）
        digitalWrite(j, LOW);  // 將該腳位設為 LOW（關閉）
      }
    }

    delay(1000);  // 等待 1 秒，讓該 LED 保持亮著
  }
}
