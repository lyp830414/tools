from PyQt5.QtWidgets import QApplication, QWidget, QLCDNumber, QDesktopWidget, QVBoxLayout
from mainForm import MainForm
import time
import sys
import os


if __name__ == '__main__':
    app = QApplication(sys.argv)

    win = MainForm(app)
    win.show()
    #while True:
    #    pass
    sys.exit(app.exec_())