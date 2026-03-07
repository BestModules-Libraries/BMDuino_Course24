from PyQt5 import QtWidgets
from PyQt5.QtGui import QPainter, QColor, QFont
import sys

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setObjectName("MainWindow")
        self.setWindowTitle('oxxo.studio')
        self.resize(300, 200)

    def paintEvent(self, event):
        qpainter = QPainter()
        qpainter.begin(self)

        font = QFont()
        font.setWeight(87)
        font.setPointSize(50)
        font.setFamily('Times')
        font.setStyle(QFont.StyleItalic)

        qpainter.setPen(QColor('#ff0000'))
        qpainter.setFont(font)
        qpainter.drawText(50,50,'hello')

        qpainter.setPen(QColor('#0000ff'))
        qpainter.setFont(QFont('Arial',30))
        qpainter.drawText(50,100,'hello')

        qpainter.end()

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    Form = MyWidget()
    Form.show()
    sys.exit(app.exec_())