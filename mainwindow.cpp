#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800, 600);
    this->setWindowTitle(tr("Task Manager"));
    this->GVFCreate();
    this->MenusCreate();
//    this->DockWgtCreate();
    this->ToolBarCreate();

    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->setStatusTip("Welcome to use Task Manager.");
    this->setStatusBar(m_pStatusBar);

    this->move(200, 50);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::MenusCreate()
{
    //'File' Menu
    m_pFileMenu = menuBar()->addMenu(tr("&File"));

    m_pAddGoalAction = new QAction(this);
    m_pAddGoalAction->setText(tr("Add &Goal"));
    m_pAddGoalAction->setStatusTip(tr("Add a new goal in the work space."));
    m_pAddGoalAction->setIcon(QIcon(":/sys/sys_elps.png"));
    m_pFileMenu->addAction(m_pAddGoalAction);
    connect(m_pAddGoalAction, SIGNAL(triggered()), m_pWorkSpaceView, SLOT(SLOT_AddGoalActionProc()));

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
            m_pWorkSpaceView, SLOT(SLOT_DragModeSwitched(bool)));
}

void MainWindow::GVFCreate()
{
    m_pWorkSpace = new CWorkSpace(this);
    m_pWorkSpaceView = new CWorkSpaceView(m_pWorkSpace, this);

    //initiate workspace
    m_pWorkSpace->setBackgroundBrush(QBrush(Qt::gray));
    m_pWorkSpace->setSceneRect(-1500, -1500, 3000, 3000);

    this->setCentralWidget(m_pWorkSpaceView);
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
    m_pSystemLabel->setVisible(false);
}
