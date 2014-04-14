#-------------------------------------------------
#
# Project created by QtCreator 2014-03-31T15:05:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ovs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    persistantservice.cpp \
    controller.cpp \
    bridge.cpp

HEADERS  += mainwindow.h \
    persistantservice.h \
    controller.h \
    bridge.h

FORMS    += mainwindow.ui
