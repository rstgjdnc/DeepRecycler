#-------------------------------------------------
#
# Project created by QtCreator 2018-05-27T14:47:43
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += multimedia   multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeepRecycler
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    RobotArmController.cpp \
    MainWindow.cpp \
    main.cpp

HEADERS += \
    RobotArmController.h \
    MainWindow.h

FORMS += \
    RobotArmController.ui \
    MainWindow.ui

INCLUDEPATH += D:\OpenCV\opencv-3.2.0\build\include

LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_core320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_highgui320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_imgcodecs320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_imgproc320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_features2d320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_calib3d320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_shape320.dll
LIBS += D:\OpenCV\opencv-3.2.0-build\bin\libopencv_videoio320.dll
