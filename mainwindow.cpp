#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CPlan::GetPlan()->LoadPlan("greenschedule.gcd");

    this->setWindowTitle("GreenSchedule");
    this->GVFCreate();
    this->MenusCreate();
    this->ToolBarCreate();
    this->setWindowFlags(Qt::FramelessWindowHint);

    m_pStatusBar = new QStatusBar(this);
    m_pStatusBar->setStatusTip("Welcome to use GreenSchedule.");
    this->setStatusBar(m_pStatusBar);
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::MenusCreate()
{
    //'File' Menu
    m_pStartMenu = menuBar()->addMenu("&Start");

    m_pSavePlan = new QAction(this);
    m_pSavePlan->setText("&Save Plan");
    m_pSavePlan->setShortcut(QString("Ctrl+S"));
    m_pStartMenu->addAction(m_pSavePlan);
    connect(m_pSavePlan, SIGNAL(triggered()), this, SLOT(SLOT_SavePlanProc()));

    m_pExportPlan = new QAction(this);
    m_pExportPlan->setText("&Export Plan");
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
    m_pDragModeAction->setText("&Hand Tool");
    m_pDragModeAction->setShortcut(QString("Ctrl+H"));
    m_pDragModeAction->setStatusTip("Activate drag mode in workspace.");
    m_pDragModeAction->setCheckable(true);
    m_pViewMenu->addAction(m_pDragModeAction);
    connect(m_pDragModeAction, SIGNAL(toggled(bool)), \
            m_pWorkSpaceView, SLOT(SLOT_DragModeSwitched(bool)));

    m_pResetViewAction = new QAction(this);
    m_pResetViewAction->setText("&Reset View");
    m_pResetViewAction->setShortcut(QString("Ctrl+R"));
    m_pResetViewAction->setStatusTip("Reset view of workspace.");
    m_pViewMenu->addAction(m_pResetViewAction);
    connect(m_pResetViewAction, SIGNAL(triggered()),\
            m_pWorkSpaceView, SLOT(SLOT_ResetViewActionProc()));

}

void MainWindow::GVFCreate()
{
    //initiate workspace
    m_pWorkSpace = new CWorkSpace(this);
    m_pWorkSpace->setBackgroundBrush(QBrush(QPixmap(QString(":/img/sceneback"))));
    m_pWorkSpace->setSceneRect(-300, -300, 2000, 600);

    //initiate workspace view
    m_pWorkSpaceView = new CWorkSpaceView(m_pWorkSpace, this);
    m_pWorkSpaceView->setSceneRect(-300, -300, 2000, 600);

    this->setCentralWidget(m_pWorkSpaceView);
}

void MainWindow::GVFViewAdjust()
{
    m_pWorkSpaceView->SLOT_ResetViewActionProc();
}

void MainWindow::ToolBarCreate()
{
    m_pSystemToolBar = this->addToolBar(tr("System ToolBar"));
    m_pSystemToolBar->setMovable(false);
    m_pSystemToolBar->setVisible(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->SLOT_SavePlanProc();
    event->accept();
}

void MainWindow::SLOT_SavePlanProc()
{
    if(CPlan::GetPlan()->NeedSave())
    {
        if(CPlan::GetPlan()->SavePlan("greenschedule.gcd"))
        {
            this->statusBar()->showMessage("The plan has been saved successfully.");
        }
        else
        {
            this->statusBar()->showMessage("Failed to save the plan.");
        }
    }
    else
    {
        this->statusBar()->showMessage("The plan is already up to date.");
    }
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
