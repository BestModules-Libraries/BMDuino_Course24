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

    qpainter.setPen(QPen(QColor(255,0,0), 25, Qt.SolidLine, Qt.FlatCap, Qt.BevelJoin))
    points = [QPoint(30,160),QPoint(60,50),QPoint(90,160)]
    qpainter.drawPolyline(points)

    qpainter.setPen(QPen(QColor(255,0,0), 25, Qt.SolidLine, Qt.FlatCap, Qt.MiterJoin))
    points = [QPoint(120,160),QPoint(150,50),QPoint(180,160)]
    qpainter.drawPolyline(points)

    qpainter.setPen(QPen(QColor(255,0,0), 25, Qt.SolidLine, Qt.FlatCap, Qt.RoundJoin))
    points = [QPoint(210,160),QPoint(240,50),QPoint(270,160)]
    qpainter.drawPolyline(points)

    qpainter.end()

MainWindow.paintEvent = draw
MainWindow.show()
sys.exit(app.exec_())