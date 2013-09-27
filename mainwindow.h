#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QGraphicsView>
#include <QStatusBar>
#include <QDockWidget>
#include <QToolBar>

#include "cworkspace.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MenusCreate();
    void GVFCreate(); //Graphics View Framework
    void DockWgtCreate();
    void ToolBarCreate();

    QMenu* m_pTestMenu;
    QAction* m_pTestAction;
    QAction* m_pExitAction;
    QStatusBar* m_pStatusBar;

    QToolBar* m_pSystemLabel;
    QAction* m_pSysRect;
    QAction* m_pSysElps;

    CWorkSpace* m_pWorkSpace;
    QGraphicsView* m_pWorkSpaceView;

    QDockWidget* m_pCustomLabel;
};

#endif // MAINWINDOW_H
