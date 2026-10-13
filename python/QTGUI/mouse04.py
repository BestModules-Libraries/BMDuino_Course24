from PyQt5 import QtWidgets
import sys

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('oxxo.studio')
        self.resize(300, 200)
        self.ui()

    def ui(self):
        self.label = QtWidgets.QLabel(self)
        self.label.setGeometry(10,10,100,50)
        self.label.setStyleSheet('font-size:24px;')

    def mousePressEvent(self, event):
        print(event.button())
        if event.button() == 1:
            self.label.setText('按下左鍵')
        elif event.button() == 2:
            self.label.setText('按下右鍵')
        else:
            self.label.setText('')

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    Form = MyWidget()
    Form.show()
    sys.exit(app.exec_())