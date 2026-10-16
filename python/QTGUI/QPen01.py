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

    # 左邊的正方形
    qpainter.setPen(QPen(QColor('#000000'), 5, Qt.DotLine, Qt.FlatCap, Qt.MiterJoin))
    qpainter.drawRect(30,50,100,100)

    # 右邊的正方形
    qpainter.setPen(QPen(QColor('#ff0000'), 10, Qt.DashDotDotLine, Qt.RoundCap, Qt.RoundJoin))
    qpainter.drawRect(160,50,100,100)

    qpainter.end()

MainWindow.paintEvent = draw
MainWindow.show()
sys.exit(app.exec_())