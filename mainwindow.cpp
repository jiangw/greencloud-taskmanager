#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(600, 300);
    this->setWindowTitle(tr("Task Manager"));
    this->MenusCreate();

    m_pWorkSpace = new CWorkSpace(this);
    m_pWorkSpace->setBackgroundBrush(QBrush(Qt::blue));
    m_pWorkSpace->addText(tr("Hello World."));

    m_pWorkSpaceView = new QGraphicsView(m_pWorkSpace);

    this->setCentralWidget(m_pWorkSpaceView);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::MenusCreate()
{
    m_pTestMenu = menuBar()->addMenu(tr("File"));

    m_pTestAction = new QAction(this);
    m_pTestAction->setText(tr("For New Actions..."));
    m_pTestMenu->addAction(m_pTestAction);

    m_pTestMenu->addSeparator();

    m_pExitAction = new QAction(this);
    m_pExitAction->setText(tr("E&xit"));
    m_pExitAction->setShortcut(QKeySequence::Quit); //Ctrl + Q
    m_pTestMenu->addAction(m_pExitAction);
    connect(m_pExitAction, SIGNAL(triggered()), this, SLOT(close()));
}
