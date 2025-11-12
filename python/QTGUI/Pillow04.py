from PyQt5 import QtWidgets
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import sys
from PIL import Image, ImageQt, ImageEnhance

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('oxxo.studio')
        self.resize(300, 300)
        self.setUpdatesEnabled(True)
        self.ui()

    def ui(self):
        img = Image.open('mona.jpg')
        img = ImageEnhance.Brightness(img).enhance(5) # 提高亮度
        img = ImageEnhance.Contrast(img).enhance(5)   # 提高對比
        qimg = ImageQt.toqimage(img)
        self.canvas = QPixmap(300,300).fromImage(qimg)
        self.label = QtWidgets.QLabel(self)
        self.label.setGeometry(0, 0, 300, 300)
        self.label.setPixmap(self.canvas)

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    Form = MyWidget()
    Form.show()
    sys.exit(app.exec_())