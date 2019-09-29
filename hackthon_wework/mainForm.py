import  PyQt5

from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5 import QtCore

from PyQt5.QtCore import *
from threading import *
from untitled import Ui_Form
import cv2

class MainForm(QMainWindow,Ui_Form):
    thstop = False

    def showcamre(self):
        # 参数0代表系统第一个摄像头,第二就用1 以此类推
        cap = cv2.VideoCapture(0)
        # 设置显示分辨率和FPS ,不设置的话会非常卡
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 600)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 400)
        cap.set(cv2.CAP_PROP_FPS, 20)
        while cap.isOpened():
            if self.thstop:
                return
            ret, frame = cap.read()
            if ret == False:
                continue
            # 水平翻转,很有必要
            frame = cv2.flip(frame, 1)
            # opencv 默认图像格式是rgb qimage要使用BRG,这里进行格式转换,不用这个的话,图像就变色了,困扰了半天,翻了一堆资料
            frame = cv2.cvtColor(frame, cv2.COLOR_RGB2BGR)
            # mat-->qimage
            a = QImage(frame.data, frame.shape[1], frame.shape[0], QImage.Format_RGB888)
            self.SetPic(a)

    def __init__(self, app):
        self.app = app
        super(MainForm, self).__init__()

        self.setupUi(self)

        self.lable1.setText("文本标签")
        self.lable1.setAutoFillBackground(True)
        palette = QPalette()
        palette.setColor(QPalette.Window, Qt.red)
        self.lable1.setPalette(palette)
        self.lable1.setAlignment(Qt.AlignCenter)
        self.lable1.resize(850, 620)
        self.lable1.move(self.lable1.x()+5, self.lable1.y())

        self.lable1.setWindowOpacity(0.1)

        self.lable = QLabel("iamlable", self)
        # self.lable.move(0,0)
        # label的对其方式,为左上对其
        self.lable.setAlignment(QtCore.Qt.AlignTop)
        self.lable.setAlignment(QtCore.Qt.AlignLeft)
        # 设置lable的大小
        self.lable.setGeometry(0, 0, 800, 600)
        # self.lable.size(800,600)
        self.lable.setScaledContents(True)

        self.lable.move(self.x() + 50, self.y())


        self.pushButton.resize(self.pushButton.width() + 50, self.pushButton.height() + 20)
        self.pushButton.setText("tool_button")
        self.toolButton_2.resize(self.toolButton_2.width() + 50, self.toolButton_2.height() + 20)
        self.toolButton_3.resize(self.toolButton_3.width() + 50, self.toolButton_3.height() + 20)
        self.toolButton_4.resize(self.toolButton_4.width() + 50, self.toolButton_4.height() + 20)
        self.radioButton.resize(self.radioButton.width() + 50, self.radioButton.height()+20)

        self.pushButton.move(self.pushButton.x() + 700, self.pushButton.y()+20)
        self.toolButton_2.move(self.toolButton_2.x() + 700, self.toolButton_2.y()+20)
        self.toolButton_3.move(self.toolButton_3.x() + 700, self.toolButton_3.y()+20)
        self.toolButton_4.move(self.toolButton_4.x() + 700, self.toolButton_4.y()+20)
        self.radioButton.move(self.radioButton.x() + 700, self.radioButton.y()+20)


        self.menu0 = QMenu()
        act0 = QAction(QIcon(r'C:\qtmain\img\0.png'), 'aaaelete', self)# parent=self.menu0)
        act0.setStatusTip('Exit application')

        act0.triggered.connect(lambda: print('hello world'))
        #act1 = QAction(QIcon(r'C:\qtmain\img\1.png'), '&delete', self)
        #act2 = QAction(QIcon(r'C:\qtmain\img\2.png'), '&delete', self)
        #act3 = QAction(QIcon(r'C:\qtmain\img\3.png'), '&delete', self)

        self.menu0.addAction(act0)
        #self.menu0.addAction(act1)
        #self.menu0.addAction(act2)
        #self.menu0.addAction(act3)

        self.pushButton.setMenu(self.menu0)

        self.setFixedSize(1500, 900)

        #self.setWindowFlags(QtCore.Qt.WindowMinimizeButtonHint)
        self.setWindowFlags(QtCore.Qt.CustomizeWindowHint)

        self.setWindowFlags(Qt.FramelessWindowHint | Qt.Tool | QtCore.Qt.WindowStaysOnTopHint)
        self.setAttribute(Qt.WA_TranslucentBackground)

        #self.setStyleSheet('QWidget#widget{border-radius:30px}')

        palette = QPalette()
        palette.setBrush(QPalette.Background, QBrush(QPixmap("1.jpg")))
        self.lable.setPalette(palette)
        #self.lable.show()
        th = Thread(target=self.showcamre)
        th.start()





    def SetPic(self, img):
        # self.lable.setPixmap(QPixmap(imgPath))
        # 图片显示
        self.lable.setPixmap(QPixmap.fromImage(img))
        # print(QPixmap(imgPath))

    thstop = False

    # 上面的这个来控制进程结束


