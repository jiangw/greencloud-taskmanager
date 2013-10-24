#include <QApplication>

#include "gconfig.h"
#include "mainwindow.h"
#include "ctestbox.h"

int main(int argc, char *argv[])
{
    //init testbox
#ifdef PF_TEST
    CTestBox::GetTestBox();
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    system("exit");
    
    return a.exec();
}
