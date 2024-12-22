#-------------------------------------------------
#
# Project created by QtCreator 2024-12-21T20:37:46
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

INCLUDEPATH += "D:/Project/cppProject/ListenUp/include"
LIBS += -L"D:/Project/cppProject/ListenUp/lib" -lvlc -lvlccore -llibvlccore  -llibvlc




greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ListenUp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui



