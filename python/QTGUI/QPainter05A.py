from PyQt5 import QtWidgets
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import sys

app = QtWidgets.QApplication(sys.argv)
MainWindow = QtWidgets.QMainWindow()
MainWindow.setObjectName("MainWindow")
MainWindow.setWindowTitle("oxxo.studio")
MainWindow.resize(300, 200)

def draw(self):
    qpainter = QPainter()
    qpainter.begin(MainWindow)

    qimage = QImage('mona.jpg')
    qpainter.drawImage(QRect(0, 0, 100, 150), qimage)
    qpainter.drawImage(QRect(150, 30, 150, 150), qimage)

    qpainter.end()

MainWindow.paintEvent = draw
MainWindow.show()
sys.exit(app.exec_())