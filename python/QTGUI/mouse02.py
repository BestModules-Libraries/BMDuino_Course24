from PyQt5 import QtWidgets
import sys

app = QtWidgets.QApplication(sys.argv)

Form = QtWidgets.QWidget()
Form.setWindowTitle('oxxo.studio')
Form.resize(300, 200)

def mousePress(self):
    print('press')

Form.mousePressEvent  = mousePress

label = QtWidgets.QLabel(Form)
label.setGeometry(10,10,100,50)
label.setStyleSheet('font-size:24px;')

def mouseMove(self):
    label.setText(f'{self.x()}, {self.y()}')   # 透過 QLabel 顯示滑鼠座標

Form.setMouseTracking(True)        # 設定不需要按下滑鼠，就能偵測滑鼠移動
Form.mouseMoveEvent  = mouseMove   # 滑鼠移動事件發生時，執行 mouseMove 函式

Form.show()
sys.exit(app.exec_())