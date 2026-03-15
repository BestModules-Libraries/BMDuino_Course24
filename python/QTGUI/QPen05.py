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

    qpainter.setPen(QPen(QColor(0,0,0), 2, Qt.SolidLine, Qt.FlatCap))
    qpainter.drawLine(50, 40, 250, 40)

    qpainter.setPen(QPen(QColor(0,0,0), 2, Qt.SolidLine, Qt.FlatCap))
    qpainter.drawLine(50, 80, 250, 80)

    qpainter.setPen(QPen(QColor(0,0,0), 2, Qt.SolidLine, Qt.FlatCap))
    qpainter.drawLine(50, 120, 250, 120)

    qpainter.setPen(QPen(QColor(255,0,0,50), 20, Qt.SolidLine, Qt.SquareCap))
    qpainter.drawLine(50, 40, 250, 40)

    qpainter.setPen(QPen(QColor(255,0,0,50), 20, Qt.SolidLine, Qt.FlatCap))
    qpainter.drawLine(50, 80, 250, 80)

    qpainter.setPen(QPen(QColor(255,0,0,50), 20, Qt.SolidLine, Qt.RoundCap))
    qpainter.drawLine(50, 120, 250, 120)

    qpainter.end()

MainWindow.paintEvent = draw
MainWindow.show()
sys.exit(app.exec_())