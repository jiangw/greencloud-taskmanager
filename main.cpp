#include <QApplication>

#include "gconfig.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("GreenSchedule");
    MainWindow w;
    w.show();
    
    return a.exec();
}
