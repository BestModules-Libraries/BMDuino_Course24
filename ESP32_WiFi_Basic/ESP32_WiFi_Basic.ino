#include <WiFi.h>
const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi已連線，IP位址：");
  Serial.println(WiFi.localIP());
}

void loop() {
  // 可進行資料上傳或接收
}
