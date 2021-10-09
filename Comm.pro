#-------------------------------------------------
#
# Project created by QtCreator 2018-01-08T16:41:58
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Comm
TEMPLATE = app


SOURCES += main.cpp\
        CWidCenterCtrl.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    CWidCenterCtrl.h

FORMS    += mainwindow.ui
