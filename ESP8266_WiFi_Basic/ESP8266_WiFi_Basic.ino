#include <ESP8266WiFi.h>

const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  Serial.print("正在連線中");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n已連接到WiFi！");
  Serial.print("IP位址：");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 網路連線後的處理邏輯
}
