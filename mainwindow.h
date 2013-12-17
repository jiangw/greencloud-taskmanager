#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QFileDialog>

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
    void GVFViewAdjust();
    void ToolBarCreate();

    //'Start' menu
    QMenu* m_pStartMenu;
    QAction* m_pSavePlan;
    QAction* m_pExportWorkspace;
    QAction* m_pExportPlan;
    QAction* m_pExitAction;
    //'View' menu
    QMenu* m_pViewMenu;
    QAction* m_pDragModeAction;
    QAction* m_pResetViewAction;

    //system tool bar
    QToolBar* m_pSystemToolBar;
    //status bar
    QStatusBar* m_pStatusBar;

    //graphics view framework
    CWorkSpace* m_pWorkSpace;
    CWorkSpaceView* m_pWorkSpaceView;

protected:
    void closeEvent(QCloseEvent *);

public slots:
    void SLOT_SavePlanProc();
    void SLOT_ExportPlanProc();
    void SLOT_ExportWorkspaceProc();
};

#endif // MAINWINDOW_H
