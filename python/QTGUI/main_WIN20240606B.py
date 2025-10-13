# 匯入需要使用的模塊

from PyQt5 import QtWidgets, QtCore, QtGui

# 匯入 UI 模組
import WIN20240606B as ui

#真正使用UI介面程式之處
#若修正該UI介面與介面程式，請對應修改

# 定義結束視窗的函式
def endwindow():
    exit()
    return

def caldata():
    print(window.t1.text())
    print(window.t2.text())
    #print((int)window.t1.text()+(int)window.t2.text())
    if (window.t1.text().isnumeric()):
        s1 = float(window.t1.text())
    else:
        print("First Number is not number")
        return
    if (window.t2.text().isnumeric()):
        s2 = float(window.t2.text())
    else:
        print("Second Number is not number")
        return
    #    window.resultsum.setText()
    fw = window.calsign.itemText(window.calsign.currentIndex())
    print(fw)
    match fw:
        case '+':
            tmp = s1+s2
        case '-':
            tmp = s1-s2
        case '*':
            tmp = s1*s2
        case '/':
            tmp = s1/s2
        case '%':
            tmp = s1%s2
    window.resultsum.setText(str(tmp))

    # 定義主要的視窗類別
class Main(QtWidgets.QMainWindow, ui.Ui_MainWindow):
    def exitmain(self):
        exit()
        return

    def __init__(self):
        super().__init__()      #繼承爸媽的DNA
        self.setupUi(self)
        self.sysexit.clicked.connect(endwindow)  # 點擊離開函式
        #self.sysexit.clicked.connect(self.exitmain())
        self.cal.clicked.connect(caldata)





# 判斷是否為主程式執行
if __name__ == '__main__':
    import sys

    # 創建應用程式
    app = QtWidgets.QApplication(sys.argv)
    # 創建主要的視窗
    window = Main()
    window2 = Main()

    # 顯示主要的視窗
    window.show()
    window2.show()
    # 執行應用程式
    sys.exit(app.exec_())
