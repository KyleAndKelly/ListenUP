#-------------------------------------------------
#
# Project created by QtCreator 2024-12-21T20:37:46
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets
CONFIG   += c++11

INCLUDEPATH += C:\lib\ffmpeg\include

LIBS += -LC:\lib\ffmpeg/lib

LIBS += -lavformat -lavcodec -lavutil -lswscale

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ListenUp
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui



