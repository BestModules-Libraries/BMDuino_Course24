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

    qpainter.setPen(QPen(QColor(0,0,0), 6, Qt.SolidLine))      # 實線
    qpainter.drawLine(50, 40, 250, 40)

    qpainter.setPen(QPen(QColor(0,0,0), 6, Qt.DashLine))       # 虛線
    qpainter.drawLine(50, 60, 250, 60)

    qpainter.setPen(QPen(QColor(0,0,0), 6, Qt.DotLine))        # 點狀線
    qpainter.drawLine(50, 80, 250, 80)

    qpainter.setPen(QPen(QColor(0,0,0), 6, Qt.DashDotLine))    # 一點狀線混合虛線
    qpainter.drawLine(50, 100, 250, 100)

    qpainter.setPen(QPen(QColor(0,0,0), 6, Qt.DashDotDotLine)) # 兩點狀線混合虛線
    qpainter.drawLine(50, 120, 250, 120)

    qpen = QPen()
    qpen.setColor(QColor(0,0,0))
    qpen.setWidth(6)
    space = 4          # 空白長度
    dashes = [1, space, 3, space, 9, space, 5, space, 1, space] # 自訂虛線樣式，偶數為空白
    qpen.setDashPattern(dashes)        # 設定虛線樣式
    qpen.setStyle(Qt.CustomDashLine)   # 自訂虛線
    qpainter.setPen(qpen)
    qpainter.drawLine(50, 140, 250, 140)

    qpainter.end()

MainWindow.paintEvent = draw
MainWindow.show()
sys.exit(app.exec_())