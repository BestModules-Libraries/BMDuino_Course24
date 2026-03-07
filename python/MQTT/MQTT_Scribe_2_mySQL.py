# commlib 是你自定義的模組，推測其內含的 get_local_ip() 是取得本地 IP，getsystime() 是取得系統目前時間字串（如 20220802143022 格式）。
#
# 資料表 dhtdata 應包含欄位：MAC、IP、temperature、humidity、systime
#

# 匯入 MQTT 用的 paho 套件，並命名為 mqtt，方便後續使用
import paho.mqtt.client as mqtt

# 匯入 json 套件，用來處理 JSON 格式資料的編碼與解碼
import json

# 匯入自定義的通用工具模組 commlib，內含例如 get_local_ip()、getsystime() 等函式
from commlib import *

# ------------------------------ #
# 匯入 pymysql 套件，並命名為 DB，作為資料庫連線與操作的工具
import pymysql as DB

# 建立資料庫連線物件，參數說明如下：
# host：資料庫主機名稱（localhost 表示本機）
# port：連接埠號（MySQL 預設為 3306）
# user：登入資料庫使用者名稱
# passwd：登入密碼
# db：要連接的資料庫名稱
# charset：資料編碼格式，使用 utf8
db = DB.connect(host='localhost', port=3306, user='big', passwd='12345678', db='big', charset='utf8')

# 建立資料庫游標，用來執行 SQL 指令
cursor = db.cursor()

# 測試用的 SQL 指令，插入一筆固定的資料到 dhtdata 資料表
sql1 = "INSERT INTO dhtdata (id, MAC, crtdatetime, temperature, humidity, systime) VALUES (NULL, 'a01110001', current_timestamp(), '25.3', '88.9', '20220624220101');"

# ------------------------------ #
# 設定 MQTT Broker 的連線資訊
broker_address = "broker.emqx.io"  # MQTT Broker 的位址（使用公用 EMQX）
port = 1883  # 使用非加密的 MQTT 預設埠號
username = ""  # 使用者名稱（留空表示匿名登入）
password = ""  # 密碼（留空表示匿名登入）
topic = "/nuk/sensor/"  # 要訂閱的主題（topic）

# 模擬從 MQTT 接收到的資料格式（JSON）
# {
#   "Device": "E89F6DE8F3BC",
#   "Temperature": 24,
#   "Humidity": 77
# }

# SQL 字串模板，將接收到的 JSON 解析後組合為 SQL 插入語法
sqlstr0 = "INSERT INTO dhtdata (MAC, IP, temperature, humidity, systime) VALUES ('%s', '%s', %f, %f, '%s');"


# ------------------------------ #
# 定義連線成功時要執行的回調函數（callback）
def on_connect(client, userdata, flags, rc):
    # rc 為回傳碼，0 表示成功
    print("Connected with result code " + str(rc))
    # 連線成功後訂閱指定主題
    client.subscribe(topic)


# 定義當接收到主題訊息時要執行的回調函數
def on_message(client, userdata, msg):
    print("Data coming from " + msg.topic)  # 顯示主題名稱
    print("Payload is :\n" + str(msg.payload))  # 原始資料（bytes 格式）

    # 將 bytes 格式轉成 UTF-8 字串
    payload = msg.payload.decode("utf-8")

    # 將 JSON 字串轉成 Python 字典物件
    jsondata = json.loads(payload)

    # 將 JSON 格式美化後列印出來（格式化輸出）
    jsonStr = json.dumps(jsondata, ensure_ascii=False, indent=4)
    print(jsonStr)

    # 顯示個別欄位值
    print("Device:", jsondata["Device"])
    print("Temperature:", jsondata["Temperature"])
    print("Humidity:", jsondata["Humidity"])

    # 根據接收到的 JSON 組合 SQL 語法，並將目前系統時間與本機 IP 一併存入資料表
    sqlstr = sqlstr0 % (
        jsondata["Device"],
        get_local_ip(),
        float(jsondata["Temperature"]),
        float(jsondata["Humidity"]),
        getsystime()
    )

    print(sqlstr)  # 顯示組合完成的 SQL 指令

    # 執行 SQL 指令，將資料插入資料庫中
    cursor.execute(sqlstr)


# ------------------------------ #
# 建立 MQTT 客戶端物件
client = mqtt.Client()

# 設定使用者帳號與密碼（如需登入）
client.username_pw_set(username, password)

# 設定連線與訊息處理的回調函數
client.on_connect = on_connect  # 設定當成功連線後要執行的函式
client.on_message = on_message  # 設定當收到訊息時要執行的函式

# 與 MQTT Broker 建立連線
client.connect(broker_address, port, 60)
# keepalive = 60 秒，若超過 60 秒未傳訊息，Broker 會發送 PING 以保持連線存活

# 進入 MQTT 事件循環，持續監聽與處理訂閱的主題訊息
client.loop_forever()
