import paho.mqtt.client as mqtt
# 使用 paho 套件，將 paho.mqtt.client 模組引入並更名為 mqtt，以便後續使用

import json
# 引入 json 模組，用於處理 JSON 格式的數據

# 設定 MQTT Broker 伺服器詳細資訊
broker_address = "broker.emqx.io"  # 設定 MQTT Broker 伺服器的網址
port = 1883  # 設定 MQTT Broker 伺服器的通訊埠
username = ""  # 設定 MQTT Broker 伺服器的登入使用者名稱
password = ""  # 設定 MQTT Broker 伺服器的登入使用者密碼
topic = "/ncnu/controller"  # 訂閱的 MQTT Broker 伺服器主題

# 定義連接伺服器的回調函數
def on_connect(client, userdata, flags, rc):
    # 當連接到伺服器時的處理程序
    print("Connected with result code " + str(rc))
    # 訂閱主題
    client.subscribe(topic)  # 訂閱指定的主題

# 定義訂閱消息的回調函數
def on_message(client, userdata, msg):
    # 當接收到消息時的處理程序
    # 解析消息的 payload 為 JSON 物件（即 Python 字典）
    parsed_json = json.loads(msg.payload)

    # 將解析後的 JSON 物件轉換為格式化的 JSON 字串
    payload_str = json.dumps(parsed_json, indent=4)
    # 打印主題和格式化的 JSON 字串
    print(msg.topic + "\n" + payload_str)

    # 使用 for 迴圈遍歷 JSON 物件的所有鍵（key），並打印每個鍵值對
    for key in parsed_json:
        print(key + ': ' + str(parsed_json[key]))

# 建立 MQTT 客戶端物件
client = mqtt.Client()

# 設定帳戶名稱和密碼
client.username_pw_set(username, password)

# 設定連接伺服器和訂閱消息的回調函數
client.on_connect = on_connect  # 當連接伺服器時執行的函數
client.on_message = on_message  # 當接收到消息時執行的函數

# 連接到 MQTT Broker 伺服器
client.connect(broker_address, port, 60)
# 使用指定的地址和端口，連接到 MQTT Broker 伺服器，並設置 60 秒的 keepalive 時間間隔。
# 這行程式碼的作用是使用指定的地址和端口，建立與 MQTT broker 的連接，並設置 60 秒的 keepalive 時間間隔。
# 這個連接可以用於發布和訂閱 MQTT 消息，並在完成後使用 client.disconnect() 方法關閉連接。

# 進入循環模式，等待 MQTT Broker 伺服器針對主題傳送消息並接收消息
client.loop_forever()
