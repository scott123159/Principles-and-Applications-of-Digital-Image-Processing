QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gaussianblurdialog.cpp \
    imgproc.cpp \
    localenhancementdialog.cpp \
    main.cpp \
    hw3.cpp \
    marrhildrethedgedetectiondialog.cpp \
    medianblurdialog.cpp

HEADERS += \
    gaussianblurdialog.h \
    hw3.h \
    imgproc.h \
    localenhancementdialog.h \
    marrhildrethedgedetectiondialog.h \
    medianblurdialog.h

FORMS += \
    gaussianblurdialog.ui \
    hw3.ui \
    localenhancementdialog.ui \
    marrhildrethedgedetectiondialog.ui \
    medianblurdialog.ui

RC_ICONS = bime.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
