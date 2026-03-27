# 匯入需要使用的模塊

from PyQt5 import QtWidgets, QtCore, QtGui

# 匯入 UI 模組
import showpic as ui


# 真正使用UI介面程式之處
# 若修正該UI介面與介面程式，請對應修改


# 定義主要的視窗類別
class Main(QtWidgets.QMainWindow, ui.Ui_MainWindow):
    __value = 10

    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.p1.clicked.connect(self.p1b)  # 點擊離開函式
        self.p2.clicked.connect(self.p2b)  # 點擊離開函式
        self.sysexit.clicked.connect(self.exitsystem)  # 點擊離開函式

    def showvalue(self):
        self.label.setText(str(self.__value))

    def p1b(self):
        self.__value = self.__value - 1
        self.showvalue()

    def p2b(self):
        self.__value = self.__value + 1
        self.showvalue()
    def exitsystem(self):
        exit()

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
