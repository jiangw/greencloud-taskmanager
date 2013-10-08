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
    cworkspaceview.cpp \
    ctestbox.cpp \
    cgoalItem.cpp

HEADERS  += mainwindow.h \
    cworkspace.h \
    cworkspaceview.h \
    ctestbox.h \
    gconfig.h \
    cgoalitem.h

RESOURCES += \
    res.qrc
