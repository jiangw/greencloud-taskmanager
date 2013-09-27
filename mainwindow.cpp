#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600, 400);
    this->setWindowTitle(tr("Task Manager"));
    this->MenusCreate();
    this->GVFCreate();
    this->DockWgtCreate();
    this->ToolBarCreate();

    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->setStatusTip("I'm status bar.");
    this->setStatusBar(m_pStatusBar);

    this->move(300, 150);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::MenusCreate()
{
    m_pTestMenu = menuBar()->addMenu(tr("File"));

    m_pTestAction = new QAction(this);
    m_pTestAction->setText(tr("For New Actions..."));
    m_pTestAction->setStatusTip(tr("this is saved for creating new action."));
    m_pTestMenu->addAction(m_pTestAction);

    m_pTestMenu->addSeparator();

    m_pExitAction = new QAction(this);
    m_pExitAction->setText(tr("E&xit"));
    m_pExitAction->setShortcut(QKeySequence::Quit); //Ctrl + Q
    m_pTestMenu->addAction(m_pExitAction);
    connect(m_pExitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::GVFCreate()
{
    m_pWorkSpace = new CWorkSpace(this);
    m_pWorkSpaceView = new QGraphicsView(m_pWorkSpace);

    //initiate work space
    m_pWorkSpace->setBackgroundBrush(QBrush(Qt::blue));
    m_pWorkSpace->addText(tr("Hello world"));

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

    m_pSysRect = new QAction(this);
    m_pSysRect->setIcon(QIcon(tr(":/sys/sys_rect.png")));
    m_pSysRect->setStatusTip(tr("Add a rectangle to work space."));
    m_pSysRect->setCheckable(true);
    m_pSystemLabel->addAction(m_pSysRect);

    m_pSysElps = new QAction(this);
    m_pSysElps->setIcon(QIcon(tr(":/sys/sys_elps.png")));
    m_pSysElps->setStatusTip(tr("Add a ellipse to work space."));
    m_pSysElps->setCheckable(true);
    m_pSystemLabel->addAction(m_pSysElps);

    this->addToolBar(m_pSystemLabel);
}
