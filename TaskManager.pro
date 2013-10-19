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
    cgoalItem.cpp \
    ctextinputitem.cpp \
    cbuttonitem.cpp \
    citemanimation.cpp \
    clabelitem.cpp \
    clineitem.cpp \
    cselectitem.cpp \
    cmemberitem.cpp \
    cgoal.cpp

HEADERS  += mainwindow.h \
    cworkspace.h \
    cworkspaceview.h \
    ctestbox.h \
    gconfig.h \
    cgoalitem.h \
    ctextinputitem.h \
    cbuttonitem.h \
    citemanimation.h \
    clabelitem.h \
    clineitem.h \
    cselectitem.h \
    cmemberitem.h \
    cgoal.h

RESOURCES += \
    res.qrc
