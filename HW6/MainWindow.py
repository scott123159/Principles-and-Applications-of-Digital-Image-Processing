# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'MainWindow.ui'
##
## Created by: Qt User Interface Compiler version 6.8.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QLabel, QMainWindow,
    QMenu, QMenuBar, QSizePolicy, QStatusBar,
    QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(800, 600)
        self.actionOpen = QAction(MainWindow)
        self.actionOpen.setObjectName(u"actionOpen")
        self.actionExit = QAction(MainWindow)
        self.actionExit.setObjectName(u"actionExit")
        self.actionUndo = QAction(MainWindow)
        self.actionUndo.setObjectName(u"actionUndo")
        self.actionRedo = QAction(MainWindow)
        self.actionRedo.setObjectName(u"actionRedo")
        self.actionTrapezoidal = QAction(MainWindow)
        self.actionTrapezoidal.setObjectName(u"actionTrapezoidal")
        self.actionWavy = QAction(MainWindow)
        self.actionWavy.setObjectName(u"actionWavy")
        self.actionCircular = QAction(MainWindow)
        self.actionCircular.setObjectName(u"actionCircular")
        self.actionLoad_Images = QAction(MainWindow)
        self.actionLoad_Images.setObjectName(u"actionLoad_Images")
        self.actionSLIC = QAction(MainWindow)
        self.actionSLIC.setObjectName(u"actionSLIC")
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.horizontalLayout = QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.image = QLabel(self.centralwidget)
        self.image.setObjectName(u"image")

        self.horizontalLayout.addWidget(self.image)

        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 800, 33))
        self.menuFile = QMenu(self.menubar)
        self.menuFile.setObjectName(u"menuFile")
        self.menuTransformation = QMenu(self.menubar)
        self.menuTransformation.setObjectName(u"menuTransformation")
        self.menuImage_Fusion = QMenu(self.menubar)
        self.menuImage_Fusion.setObjectName(u"menuImage_Fusion")
        self.menuImage_Segmentation = QMenu(self.menubar)
        self.menuImage_Segmentation.setObjectName(u"menuImage_Segmentation")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuFile.menuAction())
        self.menubar.addAction(self.menuTransformation.menuAction())
        self.menubar.addAction(self.menuImage_Fusion.menuAction())
        self.menubar.addAction(self.menuImage_Segmentation.menuAction())
        self.menuFile.addAction(self.actionOpen)
        self.menuFile.addAction(self.actionExit)
        self.menuTransformation.addAction(self.actionTrapezoidal)
        self.menuTransformation.addAction(self.actionWavy)
        self.menuTransformation.addAction(self.actionCircular)
        self.menuImage_Fusion.addAction(self.actionLoad_Images)
        self.menuImage_Segmentation.addAction(self.actionSLIC)

        self.retranslateUi(MainWindow)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"HW6", None))
        self.actionOpen.setText(QCoreApplication.translate("MainWindow", u"Open", None))
#if QT_CONFIG(shortcut)
        self.actionOpen.setShortcut(QCoreApplication.translate("MainWindow", u"Ctrl+O", None))
#endif // QT_CONFIG(shortcut)
        self.actionExit.setText(QCoreApplication.translate("MainWindow", u"Exit", None))
#if QT_CONFIG(shortcut)
        self.actionExit.setShortcut(QCoreApplication.translate("MainWindow", u"Ctrl+Q", None))
#endif // QT_CONFIG(shortcut)
        self.actionUndo.setText(QCoreApplication.translate("MainWindow", u"Undo", None))
#if QT_CONFIG(shortcut)
        self.actionUndo.setShortcut(QCoreApplication.translate("MainWindow", u"Ctrl+Z", None))
#endif // QT_CONFIG(shortcut)
        self.actionRedo.setText(QCoreApplication.translate("MainWindow", u"Redo", None))
#if QT_CONFIG(shortcut)
        self.actionRedo.setShortcut(QCoreApplication.translate("MainWindow", u"Ctrl+Y", None))
#endif // QT_CONFIG(shortcut)
        self.actionTrapezoidal.setText(QCoreApplication.translate("MainWindow", u"Trapezoidal", None))
        self.actionWavy.setText(QCoreApplication.translate("MainWindow", u"Wavy", None))
        self.actionCircular.setText(QCoreApplication.translate("MainWindow", u"Circular", None))
        self.actionLoad_Images.setText(QCoreApplication.translate("MainWindow", u"Load Images", None))
        self.actionSLIC.setText(QCoreApplication.translate("MainWindow", u"SLIC", None))
        self.image.setText("")
        self.menuFile.setTitle(QCoreApplication.translate("MainWindow", u"File", None))
        self.menuTransformation.setTitle(QCoreApplication.translate("MainWindow", u"Geometric Transformation", None))
        self.menuImage_Fusion.setTitle(QCoreApplication.translate("MainWindow", u"Image Fusion", None))
        self.menuImage_Segmentation.setTitle(QCoreApplication.translate("MainWindow", u"Image Segmentation", None))
    # retranslateUi

