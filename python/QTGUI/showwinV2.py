import paho.mqtt.client as mqtt
# 匯入需要使用的模塊
import json
from PyQt5 import QtWidgets, QtCore, QtGui

# 匯入 UI 模組
import RelayControl as ui


# 設定 MQTT Broker 的 IP 位址和連接埠
broker_address = "broker.emqx.io"
broker_port = 1883

# 建立 MQTT 用戶端
client = mqtt.Client()


def combinejson(): # 定義按下執行按鈕處理的函式
    jstr = window.setouttext()#執行視窗window內的setouttext()函式
    payload = json.dumps(jstr, indent=4)
    client.connect(broker_address, broker_port)
    # 傳送 MQTT 訊息
    topic = "/ncnu/controller"
    client.publish(topic, payload)
    # 中斷 MQTT 連線
    client.disconnect()
    return

# 定義結束視窗的函式
def endwindow():
    exit()
    return




# 定義主要的視窗類別
class Main(QtWidgets.QMainWindow, ui.Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.cmd1.clicked.connect(combinejson)  # 點擊執行函式
        self.sysexit.clicked.connect(endwindow)  # 點擊離開函式
    # 定義設置輸出文字的函式
    def setouttext(self):
        self.out.clear()
        data = {'DEVICE': self.macnum.currentText(), 'SENSOR': "RELAY", 'SET': int(self.relaynum.currentText()), 'OPERATION':self.getrelayop()}
        str1 = json.dumps(data, indent=4)
        self.out.setText(str1)
        return data

    # 定義取得 Relay 開關狀態的函式
    def getrelayop(self):
        if self.relayop.currentText() == "開啟":
            return "HIGH"
        else:
            return "LOW"



# 判斷是否為主程式執行
if __name__ == '__main__':
    import sys

    # 創建應用程式
    app = QtWidgets.QApplication(sys.argv)
    # 創建主要的視窗
    window = Main()
    # 顯示主要的視窗
    window.show()
    # 執行應用程式
    sys.exit(app.exec_())
