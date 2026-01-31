# 匯入需要使用的模塊
import json
from PyQt5 import QtWidgets, QtCore, QtGui

# 匯入 UI 模組
import RelayControl as ui


# 定義結束視窗的函式
def endwindow():
    exit()


# 定義合併 JSON 的函式
def combinejson():
    # window.outjson("aaaaaa")
    window.setouttext()


# 定義主要的視窗類別
class Main(QtWidgets.QMainWindow, ui.Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.cmd1.clicked.connect(combinejson)  # 點擊執行時函式
        self.sysexit.clicked.connect(endwindow)  # 點擊離開時函式

    # 定義輸出 JSON 的函式
    def outjson(self, ss):
        self.out.setText(ss)

    # 定義取得 MAC Address 的函式
    def getmac(self):
        return self.macnum.currentText()

    # 定義取得 Relay 號碼的函式
    def getrelaynum(self):
        return self.relaynum.currentText()

    # 定義取得 Relay 開關狀態的函式
    def getrelayop(self):
        if self.relayop.currentText() == "開啟":
            return "HIGH"
        else:
            return "LOW"

    # 定義設置輸出文字的函式
    def setouttext(self):
        self.out.clear()
        data = {'DEVICE': self.getmac(), 'SENSOR': "RELAY", 'SET': self.getrelaynum(), 'OPERATION': self.getrelayop()}
        str1 = json.dumps(data, indent=4)
        self.out.setText(str1)


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
