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
    bridge.cpp \
    port.cpp \
    interface.cpp \
    setportdialog.cpp \
    nvo3dialog.cpp

HEADERS  += mainwindow.h \
    persistantservice.h \
    controller.h \
    bridge.h \
    port.h \
    interface.h \
    setportdialog.h \
    nvo3dialog.h

FORMS    += mainwindow.ui \
    setportdialog.ui \
    nvo3dialog.ui
