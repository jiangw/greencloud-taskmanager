#-------------------------------------------------
#
# Project created by QtCreator 2013-09-24T15:56:27
#
#-------------------------------------------------

QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GreenSchedule
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cworkspace.cpp \
    cworkspaceview.cpp \
    ctestbox.cpp \
    citemanimation.cpp \
    cgoal.cpp \
    cmember.cpp \
    ../GraphicsWidgetLib/cgraphicswidget.cpp \
    cdaywidget.cpp \
    cmonthwidget.cpp \
    cyearwidget.cpp \
    cplanwidget.cpp \
    ../GraphicsWidgetLib/ccheckwidget.cpp \
    ../GraphicsWidgetLib/cbuttonwidget.cpp \
    cgoalwidget.cpp \
    ../GraphicsWidgetLib/csvgwidget.cpp \
    ../GraphicsWidgetLib/cwidgetlist.cpp \
    ../GraphicsWidgetLib/ctextwidget.cpp \
    ../GraphicsWidgetLib/ctexteditor.cpp \
    ctaskwidget.cpp \
    ccolortagwidget.cpp \
    cplan.cpp \
    cplantimehour.cpp \
    cplangoal.cpp \
    cgoaltagwidget.cpp

HEADERS  += mainwindow.h \
    cworkspace.h \
    cworkspaceview.h \
    ctestbox.h \
    gconfig.h \
    citemanimation.h \
    cgoal.h \
    cmember.h \
    ../GraphicsWidgetLib/cgraphicswidget.h \
    cdaywidget.h \
    cmonthwidget.h \
    cyearwidget.h \
    cplanwidget.h \
    ../GraphicsWidgetLib/ccheckwidget.h \
    ../GraphicsWidgetLib/cbuttonwidget.h \
    cgoalwidget.h \
    ../GraphicsWidgetLib/csvgwidget.h \
    ../GraphicsWidgetLib/cwidgetlist.h \
    ../GraphicsWidgetLib/ctextwidget.h \
    ../GraphicsWidgetLib/ctexteditor.h \
    ctaskwidget.h \
    ccolortagwidget.h \
    cplan.h \
    cplantimehour.h \
    cplangoal.h \
    cgoaltagwidget.h

RESOURCES += \
    res.qrc
