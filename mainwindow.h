#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>

#include "gconfig.h"
#include "cworkspace.h"
#include "cworkspaceview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MenusCreate();
    void GVFCreate(); //Graphics View Framework
    void ToolBarCreate();

    QMenu* m_pFileMenu;
    QAction* m_pAddGoalAction;
    QAction* m_pExitAction;
    QMenu* m_pViewMenu;
    QAction* m_pDragModeAction;

    QMenu* m_pItemMenu;
    QAction* m_pAddDayItemAction;
    QAction* m_pAddMonthItemAction;
    QAction* m_pAddYearItemAction;

    QToolBar* m_pSystemLabel; //will be removed
    QStatusBar* m_pStatusBar;

    CWorkSpace* m_pWorkSpace;
    CWorkSpaceView* m_pWorkSpaceView;
};

#endif // MAINWINDOW_H
