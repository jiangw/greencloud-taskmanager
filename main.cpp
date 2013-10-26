#include <QApplication>

#include "gconfig.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //init testbox
#ifdef PF_TEST
    CTestBox::InitTestBox();
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
