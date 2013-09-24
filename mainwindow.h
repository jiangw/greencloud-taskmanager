#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QGraphicsView>

#include "cworkspace.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MenusCreate();

    QMenu* m_pTestMenu;
    QAction* m_pTestAction;
    QAction* m_pExitAction;

    CWorkSpace* m_pWorkSpace;
    QGraphicsView* m_pWorkSpaceView;
};

#endif // MAINWINDOW_H
