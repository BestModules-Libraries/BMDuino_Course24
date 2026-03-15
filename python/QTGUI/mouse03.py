from PyQt5 import QtWidgets
import sys

app = QtWidgets.QApplication(sys.argv)

Form = QtWidgets.QWidget()
Form.setWindowTitle('oxxo.studio')
Form.resize(300, 200)

label = QtWidgets.QLabel(Form)
label.setGeometry(10,10,100,50)
label.setStyleSheet('font-size:24px;')

def mousePress(self):
    print(self.button())
    if self.button() == 1:
        label.setText('按下左鍵')
    elif self.button() == 2:
        label.setText('按下右鍵')
    else:
        label.setText('')

Form.mousePressEvent  = mousePress    # 新增按下滑鼠事件，事件發生時執行 mousePress 函式

Form.show()
sys.exit(app.exec_())