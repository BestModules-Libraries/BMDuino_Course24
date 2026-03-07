#include <ESP8266WiFi.h>

const char* ssid = "WiFi名稱";
const char* password = "WiFi密碼";

WiFiServer server(80);
int ledPin = 2;

void setup() {
  pinMode(ledPin, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    if (request.indexOf("/LED=ON") != -1) digitalWrite(ledPin, HIGH);
    if (request.indexOf("/LED=OFF") != -1) digitalWrite(ledPin, LOW);

    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
    client.print("<h1>控制LED燈</h1>");
    client.print("<a href='/LED=ON'>開燈</a><br>");
    client.print("<a href='/LED=OFF'>關燈</a>");
    client.stop();
  }
}
