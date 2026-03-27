#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "WiFi名稱";
const char* password = "WiFi密碼";
const char* mqttServer = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  client.setServer(mqttServer, 1883);
}

void loop() {
  if (!client.connected()) {
    client.connect("ESP32Client");
  }
  float temperature = 25.0;
  char msg[50];
  sprintf(msg, "%.2f", temperature);
  client.publish("iot/temperature", msg);
  delay(5000);
}
