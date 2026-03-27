from PyQt5 import QtWidgets
from PyQt5.QtGui import *
import sys, cv2, threading

class MyWidget(QtWidgets.QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('oxxo.studio')
        self.resize(480, 320)
        self.setUpdatesEnabled(True)
        self.ui()
        self.ocv = True

    def ui(self):
        self.label = QtWidgets.QLabel(self)
        self.label.setGeometry(0, 0, 480, 320)

    def closeEvent(self):
        self.ocv = False

    def opencv(self):
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Cannot open camera")
            exit()
        while self.ocv:
            ret, frame = cap.read()
            if not ret:
                print("Cannot receive frame")
                break
            frame = cv2.resize(frame, (480, 320))
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            height, width, channel = frame.shape
            bytesPerline = channel * width
            qimg = QImage(frame, width, height, bytesPerline, QImage.Format_RGB888)
            self.label.setPixmap(QPixmap.fromImage(qimg))


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    Form = MyWidget()
    video = threading.Thread(target=Form.opencv)
    video.start()
    Form.show()
    sys.exit(app.exec_())