#include <QApplication>

#include "gconfig.h"
#include "mainwindow.h"
#include "cplan.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("GreenSchedule");
    CPlan::InitPlan(&a);
    MainWindow w;
    w.showMaximized();
    w.GVFViewAdjust();
//    w.show();
    
    return a.exec();
}
