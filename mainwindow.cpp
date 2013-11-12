#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1111, 600);
    this->setWindowTitle(tr("Task Manager"));
    this->GVFCreate();
    this->MenusCreate();
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
    m_pFileMenu = menuBar()->addMenu("&File");

    m_pAddGoalAction = new QAction(this);
    m_pAddGoalAction->setText("Add &Goal");
    m_pAddGoalAction->setStatusTip("Add a new goal in the work space.");
    m_pAddGoalAction->setIcon(QIcon(":/sys/sys_elps.png"));
    m_pFileMenu->addAction(m_pAddGoalAction);
    connect(m_pAddGoalAction, SIGNAL(triggered()), m_pWorkSpaceView, SLOT(SLOT_AddGoalActionProc()));

    m_pAddPlanActtion = new QAction(this);
    m_pAddPlanActtion->setText("Add &Plan");
    m_pFileMenu->addAction(m_pAddPlanActtion);
    connect(m_pAddPlanActtion, SIGNAL(triggered()), \
            m_pWorkSpaceView, SLOT(SLOT_AddPlanActionProc()));

    m_pFileMenu->addSeparator();

    m_pExitAction = new QAction(this);
    m_pExitAction->setText("E&xit");
    m_pExitAction->setShortcut(QKeySequence::Quit); //Ctrl + Q
    m_pFileMenu->addAction(m_pExitAction);
    connect(m_pExitAction, SIGNAL(triggered()), this, SLOT(close()));

    //'View' Menu
    m_pViewMenu = menuBar()->addMenu("&View");
    m_pDragModeAction = new QAction(this);
    m_pDragModeAction->setText("Hand Tool");
    m_pDragModeAction->setStatusTip("Activate drag mode in workspace.");
    m_pDragModeAction->setCheckable(true);
    m_pViewMenu->addAction(m_pDragModeAction);
    connect(m_pDragModeAction, SIGNAL(toggled(bool)), \
            m_pWorkSpaceView, SLOT(SLOT_DragModeSwitched(bool)));

    //'Item' Menu
    m_pItemMenu = menuBar()->addMenu("&Item");
    m_pAddDayItemAction = new QAction(this);
    m_pAddDayItemAction->setText("Day Item");
    m_pAddDayItemAction->setStatusTip("Create a day item.");
    m_pAddMonthItemAction = new QAction(this);
    m_pAddMonthItemAction->setText("Month Item");
    m_pAddMonthItemAction->setStatusTip("Create a month item");
    m_pAddYearItemAction = new QAction(this);
    m_pAddYearItemAction->setText("Year Item");
    m_pAddYearItemAction->setStatusTip("Create a year item");

    m_pItemMenu->addAction(m_pAddDayItemAction);
    m_pItemMenu->addAction(m_pAddMonthItemAction);
    m_pItemMenu->addAction(m_pAddYearItemAction);
    connect(m_pAddDayItemAction, SIGNAL(triggered()), \
            m_pWorkSpaceView, SLOT(SLOT_AddDayItemActionProc()));
    connect(m_pAddMonthItemAction, SIGNAL(triggered()), \
            m_pWorkSpaceView, SLOT(SLOT_AddMonthItemActionProc()));
    connect(m_pAddYearItemAction, SIGNAL(triggered()), \
            m_pWorkSpaceView, SLOT(SLOT_AddYearItemActionProc()));

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

void MainWindow::ToolBarCreate()
{
    m_pSystemLabel = this->addToolBar(tr("System Labels"));
    m_pSystemLabel->setMovable(false);
    m_pSystemLabel->setVisible(false);
}
