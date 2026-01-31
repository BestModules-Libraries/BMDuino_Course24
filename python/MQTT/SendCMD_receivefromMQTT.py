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
subtopic = "/ncnu/controller/status"  # 訂閱MQTT Broker伺服器 主題


# 定義連接伺服器函數
def on_connect(client, userdata, flags, rc):
    # MQTT 連接處理程序
    print("Connected with result code " + str(rc))
    # 訂閱主題
    client.subscribe(subtopic)  # 連接主題


# 定義訂閱Call Back函數
def on_message(client, userdata, msg):
    # call back function 處理程序
    # 使用 json.loads() 函數將訊息 payload 解析為 JSON 物件（即 Python 中的字典）
    parsed_json = json.loads(msg.payload)

    # 使用 json.dumps() 函數將 JSON 物件轉換為 JSON 格式的字串，
    # 使用 indent 參數進行美化輸出
    payload_str = json.dumps(parsed_json, indent=4)

    # 印出訊息主題及美化後的 JSON 字串
    print(msg.topic + "\n" + payload_str)

    # 遍歷 JSON 物件的所有鍵（key）並印出每個鍵值對（key-value pair）
    for key in parsed_json:
        print(key + ': ' + str(parsed_json[key]))


# 建立 MQTT 客戶端物件
client = mqtt.Client()

# 設定帳戶名稱和密碼
client.username_pw_set(username, password)

# 設定伺服器連線之函數和訂閱之 call back 函數
client.on_connect = on_connect  # 設定伺服器連線之函數
client.on_message = on_message  # 設定訂閱之 call back 函數

# 連接到 MQTT Broker 伺服器
client.connect(broker_address, port, 60)

# 將 JSON 資料轉換為 JSON 格式字串
payload = json.dumps(json_data)

# 發佈 JSON 資料到指定主題
client.publish(pubtopic, payload)

# 進入循環模式，等待 MQTT Broker 伺服器針對主題傳送的訊息後接收訊息
client.loop_forever()
