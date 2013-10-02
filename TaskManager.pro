#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T15:56:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TaskManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cworkspace.cpp \
    ctask.cpp \
    cworkspaceview.cpp

HEADERS  += mainwindow.h \
    cworkspace.h \
    ctask.h \
    globalconst.h \
    cworkspaceview.h

RESOURCES += \
    res.qrc
