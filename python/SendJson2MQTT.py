import paho.mqtt.client as mqtt
# 使用paho套件，將paho.mqtt.client 套件下連線的物件import進來更名為mqtt
import json

# 定義要發佈的 JSON 資料
json_data = {
  "DEVICE": "E89F6DE869F8",
  "SENSOR": "RELAY",
  "SET": 4,
  "OPERATION": "HIGH"
}

# 設定MQTT Broker伺服器詳細資訊
broker_address = "broker.emqx.io"  # 設計MQTT Broker伺服器網址
port = 1883  # 設計MQTT Broker伺服器 通訊埠
username = ""  # 設計MQTT Broker伺服器 登錄使用者名稱
password = ""  # 設計MQTT Broker伺服器 登錄使用者密碼
pubtopic = "/ncnu/controller"  # 傳送到MQTT Broker伺服器 主題

# 建立 MQTT 客戶端物件
client = mqtt.Client()

# 設定帳戶名稱和密碼
client.username_pw_set(username, password)

# 連接到 MQTT Broker 伺服器
client.connect(broker_address, port, 60)

# 將 JSON 資料轉換為 JSON 格式字串
payload = json.dumps(json_data)

# 發佈 JSON 資料到指定主題
client.publish(pubtopic, payload)

# 結束 MQTT 連線
client.disconnect()
