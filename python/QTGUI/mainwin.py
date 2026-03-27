# 匯入需要使用的模塊

from PyQt5 import QtWidgets, QtCore, QtGui

# 匯入 UI 模組
import RelayControl as ui
#真正使用UI介面程式之處
#若修正該UI介面與介面程式，請對應修改




# 定義主要的視窗類別
class Main(QtWidgets.QMainWindow, ui.Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.setupUi(self)


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
