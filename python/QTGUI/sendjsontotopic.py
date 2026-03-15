import paho.mqtt.client as mqtt
import json

# 設定 MQTT Broker 的 IP 位址和連接埠
broker_address = "broker.emqx.io"
broker_port = 1883

# 建立 MQTT 用戶端
client = mqtt.Client()

# 連接到 MQTT Broker
client.connect(broker_address, broker_port)

# 準備要傳送的 JSON 資料
data = {
    "DEVICE": "E89F6DE869F8",
    "SENSOR": "RELAY",
    "SET": 4,
    "OPERATION": "HIGH"
}
payload = json.dumps(data)

# 傳送 MQTT 訊息
topic = "/ncnu/controller"
client.publish(topic, payload)

# 中斷 MQTT 連線
client.disconnect()
