#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1300, 600);
    this->setWindowTitle("GreenSchedule");
    this->GVFCreate();
    this->MenusCreate();
    this->ToolBarCreate();

    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->setStatusTip("Welcome to use Task Manager.");
    this->setStatusBar(m_pStatusBar);

    this->move(50, 50);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::MenusCreate()
{
    //'File' Menu
    m_pStartMenu = menuBar()->addMenu("&Start");

    m_pCreatePlanAction = new QAction(this);
    m_pCreatePlanAction->setText("&New Plan");
    m_pCreatePlanAction->setStatusTip("Create a new plan.");
    m_pStartMenu->addAction(m_pCreatePlanAction);
    connect(m_pCreatePlanAction, SIGNAL(triggered()), \
            m_pWorkSpaceView, SLOT(SLOT_CreatePlanActionProc()));

    m_pExportPlan = new QAction(this);
    m_pExportPlan->setText("Export Plan");
    m_pExportPlan->setShortcut(QString("Ctrl+E"));
    m_pStartMenu->addAction(m_pExportPlan);
    connect(m_pExportPlan, SIGNAL(triggered()), this, SLOT(SLOT_ExportPlanProc()));

    m_pExportWorkspace = new QAction(this);
    m_pExportWorkspace->setText("Export Workspace");
    m_pExportWorkspace->setShortcut(QString("Ctrl+Shift+E"));
    m_pStartMenu->addAction(m_pExportWorkspace);
    connect(m_pExportWorkspace, SIGNAL(triggered()), this, SLOT(SLOT_ExportWorkspaceProc()));

    m_pStartMenu->addSeparator();

    m_pExitAction = new QAction(this);
    m_pExitAction->setText("E&xit");
    m_pExitAction->setShortcut(QKeySequence::Quit); //Ctrl + Q
    m_pStartMenu->addAction(m_pExitAction);
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

    m_pResetViewAction = new QAction(this);
    m_pResetViewAction->setText("Reset View");
    m_pResetViewAction->setStatusTip("Reset view of workspace.");
    m_pViewMenu->addAction(m_pResetViewAction);
    connect(m_pResetViewAction, SIGNAL(triggered()),\
            m_pWorkSpaceView, SLOT(SLOT_ResetViewActionProc()));

}

void MainWindow::GVFCreate()
{
    m_pWorkSpace = new CWorkSpace(this);
    m_pWorkSpaceView = new CWorkSpaceView(m_pWorkSpace, this);

    //initiate workspace
    m_pWorkSpace->setBackgroundBrush(QBrush(QPixmap(QString(":/img/sceneback"))));
    m_pWorkSpace->setSceneRect(-1500, -1500, 3000, 3000);

    this->setCentralWidget(m_pWorkSpaceView);
}

void MainWindow::ToolBarCreate()
{
    m_pSystemToolBar = this->addToolBar(tr("System ToolBar"));
    m_pSystemToolBar->setMovable(false);
    m_pSystemToolBar->setVisible(false);
}

void MainWindow::SLOT_ExportPlanProc()
{
    QString l_qstrFileName = QFileDialog::getSaveFileName(this, "Export Plan",\
                                                          "/home/jiang/untitled",\
                                                          "SVG Files (*.svg);;Images (*.jpg);;XML Files (*.xml)");
    if(!l_qstrFileName.isEmpty())
    {
        m_pWorkSpaceView->ExportPlan2Svg(l_qstrFileName);
    }
}

void MainWindow::SLOT_ExportWorkspaceProc()
{
    QString l_qstrFileName = QFileDialog::getSaveFileName(this, "Export Workspace",\
                                                          "/home/jiang/untitled",\
                                                          "SVG Files (*.svg);;XML Files (*.xml)");
    if(!l_qstrFileName.isEmpty())
    {
        m_pWorkSpaceView->ExportWorkSpace2Svg(l_qstrFileName);
    }
}
