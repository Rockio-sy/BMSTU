# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'window.ui'
#
# Created by: PyQt5 UI code generator 5.15.7
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1000, 700)
        MainWindow.setMinimumSize(QtCore.QSize(1000, 700))
        MainWindow.setMaximumSize(QtCore.QSize(1000, 700))
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalLayout = QtWidgets.QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.gridLayout_2 = QtWidgets.QGridLayout()
        self.gridLayout_2.setObjectName("gridLayout_2")
        spacerItem = QtWidgets.QSpacerItem(40, 20, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Minimum)
        self.gridLayout_2.addItem(spacerItem, 0, 0, 1, 1)
        self.horizontalLayout.addLayout(self.gridLayout_2)
        self.frame = QtWidgets.QFrame(self.centralwidget)
        self.frame.setMaximumSize(QtCore.QSize(300, 16777215))
        self.frame.setBaseSize(QtCore.QSize(0, 0))
        self.frame.setFrameShape(QtWidgets.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtWidgets.QFrame.Raised)
        self.frame.setObjectName("frame")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.frame)
        self.verticalLayout.setObjectName("verticalLayout")
        self.groupBox = QtWidgets.QGroupBox(self.frame)
        self.groupBox.setObjectName("groupBox")
        self.verticalLayout_6 = QtWidgets.QVBoxLayout(self.groupBox)
        self.verticalLayout_6.setObjectName("verticalLayout_6")
        self.label = QtWidgets.QLabel(self.groupBox)
        self.label.setObjectName("label")
        self.verticalLayout_6.addWidget(self.label)
        self.speed = QtWidgets.QDoubleSpinBox(self.groupBox)
        self.speed.setMaximum(100.0)
        self.speed.setProperty("value", 10.0)
        self.speed.setObjectName("speed")
        self.verticalLayout_6.addWidget(self.speed)
        self.label_2 = QtWidgets.QLabel(self.groupBox)
        self.label_2.setObjectName("label_2")
        self.verticalLayout_6.addWidget(self.label_2)
        self.acceleration = QtWidgets.QDoubleSpinBox(self.groupBox)
        self.acceleration.setMaximum(100.0)
        self.acceleration.setProperty("value", 9.81)
        self.acceleration.setObjectName("acceleration")
        self.verticalLayout_6.addWidget(self.acceleration)
        self.label_3 = QtWidgets.QLabel(self.groupBox)
        self.label_3.setObjectName("label_3")
        self.verticalLayout_6.addWidget(self.label_3)
        self.density = QtWidgets.QDoubleSpinBox(self.groupBox)
        self.density.setMinimum(500.0)
        self.density.setMaximum(10000.0)
        self.density.setProperty("value", 1000.0)
        self.density.setObjectName("density")
        self.verticalLayout_6.addWidget(self.density)
        self.verticalLayout.addWidget(self.groupBox)
        self.groupBox_painting = QtWidgets.QGroupBox(self.frame)
        self.groupBox_painting.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.groupBox_painting.setAutoFillBackground(False)
        self.groupBox_painting.setObjectName("groupBox_painting")
        self.verticalLayout_7 = QtWidgets.QVBoxLayout(self.groupBox_painting)
        self.verticalLayout_7.setObjectName("verticalLayout_7")
        self.verticalLayout_5 = QtWidgets.QVBoxLayout()
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.guro = QtWidgets.QRadioButton(self.groupBox_painting)
        self.guro.setChecked(True)
        self.guro.setObjectName("guro")
        self.verticalLayout_5.addWidget(self.guro)
        self.fong = QtWidgets.QRadioButton(self.groupBox_painting)
        self.fong.setObjectName("fong")
        self.verticalLayout_5.addWidget(self.fong)
        self.verticalLayout_7.addLayout(self.verticalLayout_5)
        self.triangle = QtWidgets.QCheckBox(self.groupBox_painting)
        self.triangle.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.triangle.setObjectName("triangle")
        self.verticalLayout_7.addWidget(self.triangle)
        self.verticalLayout.addWidget(self.groupBox_painting)
        self.groupBox_2 = QtWidgets.QGroupBox(self.frame)
        self.groupBox_2.setObjectName("groupBox_2")
        self.verticalLayout_8 = QtWidgets.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_8.setObjectName("verticalLayout_8")
        self.label_4 = QtWidgets.QLabel(self.groupBox_2)
        self.label_4.setObjectName("label_4")
        self.verticalLayout_8.addWidget(self.label_4)
        self.label_5 = QtWidgets.QLabel(self.groupBox_2)
        self.label_5.setObjectName("label_5")
        self.verticalLayout_8.addWidget(self.label_5)
        self.verticalLayout.addWidget(self.groupBox_2)
        spacerItem1 = QtWidgets.QSpacerItem(20, 80, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Fixed)
        self.verticalLayout.addItem(spacerItem1)
        self.stopButton = QtWidgets.QPushButton(self.frame)
        self.stopButton.setMinimumSize(QtCore.QSize(0, 30))
        self.stopButton.setObjectName("stopButton")
        self.verticalLayout.addWidget(self.stopButton)
        self.startButton = QtWidgets.QPushButton(self.frame)
        self.startButton.setObjectName("startButton")
        self.verticalLayout.addWidget(self.startButton)
        self.horizontalLayout.addWidget(self.frame)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1000, 22))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Course Project")) # النافذة الرئيسية
        self.groupBox.setTitle(_translate("MainWindow", "Model Parameters")) # لوحة التحكم
        self.label.setText(_translate("MainWindow", "Initial drop velocity:")) # السرعة الابتدائية للتقطير
        self.label_2.setText(_translate("MainWindow", "Drop acceleration:")) # تسارع التنقيط
        self.label_3.setText(_translate("MainWindow", "Liquid density:")) # كثافة الماء
        self.groupBox_painting.setTitle(_translate("MainWindow", "Painting method")) # طريقة الرسم
        self.guro.setText(_translate("MainWindow", "Guro coloring method")) # طريقة غورو
        self.fong.setText(_translate("MainWindow", "Fong coloring method")) # طريقة فونغا
        self.triangle.setText(_translate("MainWindow", "Polygon rendering")) # تثديم المضلع
        self.groupBox_2.setTitle(_translate("MainWindow", "Camera")) # الكاميرا
        self.label_4.setText(_translate("MainWindow", "Movement:    Buttons W, A, S, D")) # للتحريك
        self.label_5.setText(_translate("MainWindow", "     Rotation:      Buttons  i, k, j, l")) # الدوران
        self.startButton.setText(_translate("MainWindow", "Start")) # بدء
        self.stopButton.setText(_translate("MainWindow", "Stop")) # ايقاف


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
