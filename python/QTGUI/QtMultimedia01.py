from PyQt5 import QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtMultimedia import *
import sys, os

app = QtWidgets.QApplication(sys.argv)
main = QtWidgets.QMainWindow()
main.setObjectName("MainWindow")
main.setWindowTitle("oxxo.studio")
main.resize(300, 300)

player = QMediaPlayer()                     # 建立播放器
path = os.getcwd() + '/'                    # 取得目前工作路徑
qurl = QUrl.fromLocalFile(path+'test.mp3')  # 取得音樂路徑
qmusic = QMediaContent(qurl)                # 建立音樂內容
player.setMedia(qmusic)                     # 將播放器與音樂內容綁定
player.setVolume(100)                       # 設定音量
player.play()                               # 播放

main.show()
sys.exit(app.exec_())