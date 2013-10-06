#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QDockWidget>
#include <QToolBar>
#include <QList>
#include <QScrollBar>

#include "cworkspace.h"
#include "cworkspaceview.h"

#ifdef PF_TEST

#include "ctestbox.h"

#endif

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

    QMenu* m_pFileMenu;
    QAction* m_pTestAction;
    QAction* m_pExitAction;
    QMenu* m_pViewMenu;
    QAction* m_pDragModeAction;
    QStatusBar* m_pStatusBar;

    QToolBar* m_pSystemLabel;
    QAction* m_pSysRect;
    QAction* m_pSysElps;
    QList<QAction *> m_pSysLabelGroup;

    CWorkSpace* m_pWorkSpace;
    CWorkSpaceView* m_pWorkSpaceView;

    QDockWidget* m_pCustomLabel;

public slots:
    void Slot_SysRectChecked(bool a_pStatus);
    void Slot_SysElpsChecked(bool a_pStatus);
    void Slot_SysLabelDrawn();
};

#endif // MAINWINDOW_H
