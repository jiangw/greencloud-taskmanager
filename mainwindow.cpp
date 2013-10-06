#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600, 400);
    this->setWindowTitle(tr("Task Manager"));
    this->GVFCreate();
    this->MenusCreate();
//    this->DockWgtCreate();
    this->ToolBarCreate();

    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->setStatusTip("Welcome to use Task Manager.");
    this->setStatusBar(m_pStatusBar);

    this->move(300, 150);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::MenusCreate()
{
    //'File' Menu
    m_pFileMenu = menuBar()->addMenu(tr("File"));

    m_pTestAction = new QAction(this);
    m_pTestAction->setText(tr("For New Actions..."));
    m_pTestAction->setStatusTip(tr("this is saved for creating new action."));
    m_pFileMenu->addAction(m_pTestAction);

    m_pFileMenu->addSeparator();

    m_pExitAction = new QAction(this);
    m_pExitAction->setText(tr("E&xit"));
    m_pExitAction->setShortcut(QKeySequence::Quit); //Ctrl + Q
    m_pFileMenu->addAction(m_pExitAction);
    connect(m_pExitAction, SIGNAL(triggered()), this, SLOT(close()));

    //'View' Menu
    m_pViewMenu = menuBar()->addMenu(tr("View"));
    m_pDragModeAction = new QAction(this);
    m_pDragModeAction->setText(tr("Hand Tool"));
    m_pDragModeAction->setStatusTip(tr("Activate drag mode in workspace."));
    m_pDragModeAction->setCheckable(true);
    m_pViewMenu->addAction(m_pDragModeAction);
    connect(m_pDragModeAction, SIGNAL(toggled(bool)), \
            m_pWorkSpaceView, SLOT(Slot_DragModeSwitched(bool)));
}

void MainWindow::GVFCreate()
{
    m_pWorkSpace = new CWorkSpace(this);
    m_pWorkSpaceView = new CWorkSpaceView(m_pWorkSpace, this);

    //initiate workspace
    m_pWorkSpace->setBackgroundBrush(QBrush(Qt::gray));
    m_pWorkSpace->setSceneRect(-1500, -1500, 3000, 3000);

    //connect signals and slots
    connect(m_pWorkSpace, SIGNAL(Signal_SysLabelDrawn()), this, SLOT(Slot_SysLabelDrawn()));

    this->setCentralWidget(m_pWorkSpaceView);

#ifdef PF_TEST
    CTestBox::AddTextItemToScene(m_pWorkSpace);
#endif
}

void MainWindow::DockWgtCreate()
{
    m_pCustomLabel = new QDockWidget(this);
    m_pCustomLabel->setWindowTitle(tr("Cumstom Labels"));
    m_pCustomLabel->resize(200, 400);
    m_pCustomLabel->setFloating(true);

    this->addDockWidget(Qt::LeftDockWidgetArea, m_pCustomLabel);
    m_pCustomLabel->move(90, 150);
}

void MainWindow::ToolBarCreate()
{
    m_pSystemLabel = this->addToolBar(tr("System Labels"));
    m_pSystemLabel->setMovable(false);

    m_pSysRect = new QAction(this);
    m_pSysRect->setIcon(QIcon(tr(":/sys/sys_rect.png")));
    m_pSysRect->setStatusTip(tr("Add a rectangle to work space."));
    m_pSysRect->setCheckable(true);
    connect(m_pSysRect, SIGNAL(toggled(bool)), this, SLOT(Slot_SysRectChecked(bool)));
    m_pSystemLabel->addAction(m_pSysRect);
    m_pSysLabelGroup << m_pSysRect; //append the action to action group

    m_pSysElps = new QAction(this);
    m_pSysElps->setIcon(QIcon(tr(":/sys/sys_elps.png")));
    m_pSysElps->setStatusTip(tr("Add an ellipse to work space."));
    m_pSysElps->setCheckable(true);
    connect(m_pSysElps, SIGNAL(toggled(bool)), this, SLOT(Slot_SysElpsChecked(bool)));
    m_pSystemLabel->addAction(m_pSysElps);
    m_pSysLabelGroup << m_pSysElps; //append the action to action group

    this->addToolBar(m_pSystemLabel);
}

void MainWindow::Slot_SysRectChecked(bool a_pStatus)
{
    if(a_pStatus) //if the action is cheched
    {
        foreach(QAction* l_pAction, m_pSysLabelGroup)
        {
            if(l_pAction != m_pSysRect)
            {
                l_pAction->setChecked(false);
            }
        }
        if(m_pDragModeAction->isChecked())
        {
            m_pDragModeAction->setChecked(false);
        }

        m_pWorkSpace->m_eStatus = TASKMANAGER::DRAWSYSLABEL;
        m_pWorkSpace->m_eSysLabel = TASKMANAGER::SYSRECT;
    }
}

void MainWindow::Slot_SysElpsChecked(bool a_pStatus)
{
    if(a_pStatus) //if the action is checked
    {
        foreach(QAction* l_pAction, m_pSysLabelGroup)
        {
            if(l_pAction != m_pSysElps)
            {
                l_pAction->setChecked(false);
            }
        }
        if(m_pDragModeAction->isChecked())
        {
            m_pDragModeAction->setChecked(false);
        }

        m_pWorkSpace->m_eStatus = TASKMANAGER::DRAWSYSLABEL;
        m_pWorkSpace->m_eSysLabel = TASKMANAGER::SYSELPS;
    }
}

void MainWindow::Slot_SysLabelDrawn()
{
    switch(m_pWorkSpace->m_eSysLabel)
    {
    case TASKMANAGER::SYSRECT:
        m_pSysRect->setChecked(false);
        break;
    case TASKMANAGER::SYSELPS:
        m_pSysElps->setChecked(false);
        break;
    default:
        break;
    }
    m_pWorkSpace->m_eSysLabel = TASKMANAGER::NONE;
}
