#include "cworkspaceview.h"

CWorkSpaceView::CWorkSpaceView(CWorkSpace *a_pWorkSpace, QWidget *a_pParent) :
    QGraphicsView(a_pWorkSpace, a_pParent)
{
    m_pWorkSpace = a_pWorkSpace;
    m_pPlanWidget = NULL;
    m_pMonthWidget = NULL;
    m_pDayWidget = NULL;
    m_pGoalTagWidgetList = NULL;
    m_pGoalEditor = NULL;
    m_pCurrSelGoal = NULL;
    m_pShowPlanHistory = NULL;
    m_pShowPlanInProgress = NULL;

    //receive mouse move events even if no button is pressed
    this->setMouseTracking(true);
    this->InitWorkSpace();

    connect(CPlan::GetPlan(), SIGNAL(SIGNAL_ShowInMessageBox(QString)),\
            this, SLOT(SLOT_ShowMsgBoxProc(QString)));
    connect(CPlan::GetPlan(), SIGNAL(SIGNAL_PlanGoalCreated(CPlanGoal*)),\
            this, SLOT(SLOT_AddGoalTagProc(CPlanGoal*)));
    connect(CPlan::GetPlan(), SIGNAL(SIGNAL_PlanGoalUpdated(CPlanGoal*)),\
            this, SLOT(SLOT_UpdateGoalTagProc(CPlanGoal*)));
    connect(CPlan::GetPlan(), SIGNAL(SIGNAL_PlanGoalRemoved(int)),\
            this, SLOT(SLOT_RemoveGoalTagProc(int)));
}

bool CWorkSpaceView::ExportPlan2Img(QString a_qstrImgFileName)
{
    QImage l_CImage(m_pPlanWidget->boundingRect().width(),\
                    m_pPlanWidget->boundingRect().height(),\
                    QImage::Format_ARGB32);
    std::cout << l_CImage.dotsPerMeterX() << "," << l_CImage.dotsPerMeterY() << std::endl;
    l_CImage.fill(Qt::white);
    m_pPlanWidget->RenderToImg(&l_CImage);

    return l_CImage.save(a_qstrImgFileName);
}

bool CWorkSpaceView::ExportPlan2Svg(QString a_qstrSvgFileName)
{
    QSvgGenerator l_CSvg;
    l_CSvg.setFileName(a_qstrSvgFileName);
    l_CSvg.setSize(QSize(m_pPlanWidget->boundingRect().width(),\
                         m_pPlanWidget->boundingRect().height()));
    l_CSvg.setResolution(90);

    m_pPlanWidget->RenderToSvg(&l_CSvg);

    return true;
}

bool CWorkSpaceView::ExportWorkSpace2Svg(QString a_qstrSvgFileName)
{
    QRectF l_CRenderRect;
    l_CRenderRect.setX(m_pMonthWidget->pos().x() - 5);
    l_CRenderRect.setY(m_pPlanWidget->pos().y() - 5);
    l_CRenderRect.setWidth(m_pMonthWidget->boundingRect().width()\
                           + m_pPlanWidget->boundingRect().width()\
                           + m_pGoalTagWidgetList->boundingRect().width() + 30);
    int l_iHeight;
    if(m_pPlanWidget->boundingRect().height() > m_pGoalTagWidgetList->boundingRect().height())
    {
        l_iHeight = m_pPlanWidget->boundingRect().height();
    }
    else
    {
        l_iHeight = m_pGoalTagWidgetList->boundingRect().height();
    }
    l_CRenderRect.setHeight(l_iHeight + 10);

    QSvgGenerator l_CSvg;
    l_CSvg.setFileName(a_qstrSvgFileName);
    l_CSvg.setSize(QSize(l_CRenderRect.width(), l_CRenderRect.height()));
    l_CSvg.setResolution(90);

    QPainter l_CPainter;
    l_CPainter.begin(&l_CSvg);
    QBrush l_COldBrush = m_pWorkSpace->backgroundBrush();
    m_pWorkSpace->setBackgroundBrush(QBrush(Qt::transparent));
    m_pWorkSpace->render(&l_CPainter, QRectF(), l_CRenderRect);
    l_CPainter.end();
    m_pWorkSpace->setBackgroundBrush(l_COldBrush);

    return true;
}

void CWorkSpaceView::InitWorkSpace()
{
    if(NULL == m_pMonthWidget)
    {
        m_pMonthWidget = new CMonthWidget(NULL);
        m_pMonthWidget->EnableMultiSelection(false);
        m_pWorkSpace->addItem(m_pMonthWidget);
    }

    if(NULL == m_pDayWidget)
    {
        m_pDayWidget = new CDayWidget(NULL);
        m_pWorkSpace->addItem(m_pDayWidget);
        connect(m_pMonthWidget, SIGNAL(SIGNAL_DaySel(QDate)),\
                m_pDayWidget, SLOT(SLOT_SetDateProc(QDate)));
    }

    if(NULL == m_pPlanWidget)
    {
        m_pPlanWidget = new CPlanWidget(NULL);
//        m_pPlanWidget->SetTimePage(GREENSCHEDULE::HISTORY);
        m_pWorkSpace->addItem(m_pPlanWidget);

        connect(m_pDayWidget, SIGNAL(SIGNAL_MouseDragRelease(QPointF,CGraphicsWidget*)),\
                m_pPlanWidget, SLOT(SLOT_MouseDragDropProc(QPointF,CGraphicsWidget*)));
        connect(m_pDayWidget, SIGNAL(SIGNAL_HourSelMaskRequest(QDate,bool*,int)),\
                CPlan::GetPlan(), SLOT(SLOT_HourSelMaskRequestProc(QDate,bool*,int)));
        connect(CPlan::GetPlan(), SIGNAL(SIGNAL_HourSelMaskRequestFinished()),\
                m_pDayWidget, SLOT(SLOT_HourSelMaskRecieveProc()));
        connect(CPlan::GetPlan(), SIGNAL(SIGNAL_PlanWidgetUpdate()),\
                m_pPlanWidget, SLOT(SLOT_WidgetUpdateProc()));
        connect(m_pPlanWidget, SIGNAL(SIGNAL_ShowInMessageBox(QString)),\
                this, SLOT(SLOT_ShowMsgBoxProc(QString)));
    }

    //add widget list for storing goal widgets
    if(NULL == m_pGoalTagWidgetList)
    {
        m_pGoalTagWidgetList = new CWidgetList(NULL);
        m_pGoalTagWidgetList->SetWidgetOutline(false);
        m_pGoalTagWidgetList->SetListOrientation(CWidgetList::HORIZONTAL);
        m_pGoalTagWidgetList->SetPageLength(10);
//        m_pGoalTagWidgetList->PropagateMouseEventToChildren(true);
        m_pWorkSpace->addItem(m_pGoalTagWidgetList);
        //set header of widget list as push button
        CButtonWidget* l_pAddGoalBtn = new CButtonWidget("Add Goal", NULL);
        m_pGoalTagWidgetList->SetHeaderWidget(l_pAddGoalBtn);
        connect(l_pAddGoalBtn, SIGNAL(SIGNAL_LeftButtonClicked()),\
                this, SLOT(SLOT_AddGoalProc()));
//        connect(l_pAddGoalBtn, SIGNAL(SIGNAL_LeftButtonClicked()),\
//                this, SLOT(SLOT_AddGoalWidgetToWidgetListProc()));

        QList<CPlanGoal *>& l_CPlanGoalList = CPlan::GetPlan()->GetPlanGoalList();
        for(int i=0; i<l_CPlanGoalList.length(); i++)
        {
            CGoalTagWidget* l_pNewGoalTagWidget = new CGoalTagWidget(NULL);
            l_pNewGoalTagWidget->InitGoalData(l_CPlanGoalList[i]->GetGoalId(),\
                                              l_CPlanGoalList[i]->GetGoalName(),\
                                              (CGraphicsWidget::gColor)l_CPlanGoalList[i]->GetGoalColor());
            this->AddGoalTagWidgetToWidgetList(l_pNewGoalTagWidget);
        }
    }


    if(NULL == m_pGoalEditor)
    {
        m_pGoalEditor = new CGoalWidget(NULL);
        m_pGoalEditor->setVisible(false);
        m_pWorkSpace->addItem(m_pGoalEditor);

        //communication between CGoalWidget and CPlanWidget
        connect(m_pGoalEditor,\
                SIGNAL(SIGNAL_GoalTaskSend(QPointF,int,int)),\
                m_pPlanWidget,\
                SLOT(SLOT_GoalTaskRecieve(QPointF,int,int)));
        //communication between CGoalWidget and CPlan
        connect(m_pGoalEditor, SIGNAL(SIGNAL_PlanGoalPropose(CGoalWidget*)),\
                CPlan::GetPlan(), SLOT(SLOT_PlanGoalReviewProc(CGoalWidget*)));
        connect(m_pGoalEditor, SIGNAL(SIGNAL_PlanGoalSubmit(const CPlanGoal*)),\
                CPlan::GetPlan(), SLOT(SLOT_PlanGoalSyncProc(const CPlanGoal*)));
        connect(m_pGoalEditor, SIGNAL(SIGNAL_PlanGoalRetract(int)),\
                CPlan::GetPlan(), SLOT(SLOT_PlanGoalRetractProc(int)));
        connect(m_pGoalEditor, SIGNAL(SIGNAL_TaskFinishStatSync(int,int,bool)),\
                CPlan::GetPlan(), SLOT(SLOT_GoalTaskFinishStatSync(int,int,bool)));
        connect(m_pGoalEditor, SIGNAL(SIGNAL_ShowInMessageBox(QString)),\
                this, SLOT(SLOT_ShowMsgBoxProc(QString)));
    }

    if(NULL == m_pShowPlanHistory)
    {
        m_pShowPlanHistory = new CButtonWidget("History", NULL);
        m_pWorkSpace->addItem(m_pShowPlanHistory);
        connect(m_pShowPlanHistory, SIGNAL(SIGNAL_ButtonTriggered()),\
                m_pPlanWidget, SLOT(SLOT_ShowPlanHistoryProc()));
    }

    if(NULL == m_pShowPlanInProgress)
    {
        m_pShowPlanInProgress = new CButtonWidget("On Going", NULL);
        m_pWorkSpace->addItem(m_pShowPlanInProgress);
        connect(m_pShowPlanInProgress, SIGNAL(SIGNAL_ButtonTriggered()),\
                m_pPlanWidget, SLOT(SLOT_ShowPlanInProgressProc()));
    }

    m_pMonthWidget->setPos(-270, -280);
    m_pDayWidget->setPos(-240, m_pMonthWidget->pos().y()\
                         + m_pMonthWidget->boundingRect().height()\
                         + GREENSCHEDULE::g_iItemIntervalY);
    m_pPlanWidget->setPos(GREENSCHEDULE::g_iItemIntervalX, m_pMonthWidget->pos().y() - 10);
    m_pGoalTagWidgetList->setPos(m_pPlanWidget->pos().x() + m_pPlanWidget->boundingRect().width()\
                              + GREENSCHEDULE::g_iItemIntervalX,\
                              m_pMonthWidget->pos().y());
    m_pGoalEditor->setPos(m_pGoalTagWidgetList->pos().x(),\
                          m_pGoalTagWidgetList->pos().y() + 100);
    m_pShowPlanInProgress->setPos(m_pPlanWidget->pos().x(),\
                                  m_pPlanWidget->pos().y() +  m_pPlanWidget->WidgetHeight());
    m_pShowPlanHistory->setPos(m_pShowPlanInProgress->pos().x()\
                               + m_pShowPlanInProgress->WidgetWidth() + GREENSCHEDULE::g_iItemIntervalX,\
                               m_pShowPlanInProgress->pos().y());
}

void CWorkSpaceView::SLOT_DragModeSwitched(bool a_blFlag)
{
    if(a_blFlag)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
    }
    else
    {
        this->setDragMode(QGraphicsView::NoDrag);
        m_pPlanWidget->setEnabled(!a_blFlag);
    }
}

void CWorkSpaceView::SLOT_CenterOnGraphicsWidgetProc(CGraphicsWidget *a_pWidget)
{
    this->centerOn(a_pWidget);
}

void CWorkSpaceView::SLOT_ResetViewActionProc()
{
    this->ensureVisible(-300, -300, 1000, 600);
}

void CWorkSpaceView::SLOT_AddGoalProc()
{
    if(this->InitGoalEditor())
    {
        if(NULL != m_pCurrSelGoal)
        {
            m_pCurrSelGoal->SetWidgetSelection(false);
        }
        m_pGoalEditor->SetGoalWidgetMode(CGoalWidget::EDIT);
    }
}

void CWorkSpaceView::SLOT_AddGoalTagProc(CPlanGoal *a_pPlanGoal)
{
    CGoalTagWidget* l_pNewGoalTagWidget = new CGoalTagWidget(NULL);
    l_pNewGoalTagWidget->InitGoalData(a_pPlanGoal->GetGoalId(),\
                                a_pPlanGoal->GetGoalName(),\
                                (CGraphicsWidget::gColor)a_pPlanGoal->GetGoalColor());
    this->AddGoalTagWidgetToWidgetList(l_pNewGoalTagWidget);
}

void CWorkSpaceView::SLOT_UpdateGoalTagProc(CPlanGoal *a_pPlanGoal)
{
    CWidgetNode* l_pWidgetIter = m_pGoalTagWidgetList->GetWidgetList();
    while(l_pWidgetIter)
    {
        CGoalTagWidget* l_pGoalTagWidget = (CGoalTagWidget *)l_pWidgetIter->m_pWidget;
        if(a_pPlanGoal->GetGoalId() == l_pGoalTagWidget->GetGoalId())
        {
            if(a_pPlanGoal->GetGoalColor() != l_pGoalTagWidget->GetGoalColorTag())
            {
                l_pGoalTagWidget->SetGoalColorTag((CGraphicsWidget::gColor)a_pPlanGoal->GetGoalColor());
            }
            if(a_pPlanGoal->GetGoalName() != l_pGoalTagWidget->GetGoalName())
            {
                l_pGoalTagWidget->SetGoalName(a_pPlanGoal->GetGoalName());
            }
            break;
        }
        l_pWidgetIter = l_pWidgetIter->m_pNext;
    }
}

void CWorkSpaceView::SLOT_RemoveGoalTagProc(int a_iGoalId)
{
    CWidgetNode* l_pWidgetIter = m_pGoalTagWidgetList->GetWidgetList();
    while(l_pWidgetIter)
    {
        CGoalTagWidget* l_pGoalTagWidget = (CGoalTagWidget *)l_pWidgetIter->m_pWidget;
        if(l_pGoalTagWidget->GetGoalId() == a_iGoalId)
        {
            m_pGoalTagWidgetList->RemoveWidget(l_pWidgetIter->m_pWidget);
        }
        l_pWidgetIter = l_pWidgetIter->m_pNext;
    }
}

void CWorkSpaceView::SLOT_ShowGoalProc(CGoalTagWidget* a_pGoalTag)
{
    if(m_pCurrSelGoal != a_pGoalTag)
    {
        if(this->InitGoalEditor())
        {
            //set goal data
            CPlanGoal* l_pPlanGoal = CPlan::GetPlan()->GetPlanGoalById(a_pGoalTag->GetGoalId());
            m_pGoalEditor->SetGoalData(l_pPlanGoal);
            m_pGoalEditor->SetGoalWidgetMode(CGoalWidget::VIEW);
            if(NULL != m_pCurrSelGoal)
            {
                m_pCurrSelGoal->SetWidgetSelection(false);
            }
            m_pCurrSelGoal = a_pGoalTag;
        }
        else
        {
            a_pGoalTag->SetWidgetSelection(false);
        }
    }
}

void CWorkSpaceView::SLOT_DeleteWidgetFromSceneProc(CGraphicsWidget *a_pDelWidget)
{
    m_pWorkSpace->removeItem(a_pDelWidget);
}

void CWorkSpaceView::SLOT_ShowMsgBoxProc(QString a_qstrMsg)
{
    CMessageWidget* l_pMsgBox = new CMessageWidget(m_pWorkSpace);
    l_pMsgBox->SetQuestion(a_qstrMsg);
    l_pMsgBox->SetMsgBoxWidth(500);
    l_pMsgBox->SetMsgBoxPos(0,\
                            -l_pMsgBox->GetMsgBoxHeight() / 2);
}

void CWorkSpaceView::AddGoalTagWidgetToWidgetList(CGoalTagWidget *a_pGoalTagWidget)
{
    connect(a_pGoalTagWidget, SIGNAL(SIGNAL_Selected(CGoalTagWidget*)),\
            this, SLOT(SLOT_ShowGoalProc(CGoalTagWidget*)));
    m_pGoalTagWidgetList->AddWidget(a_pGoalTagWidget);

    if(!m_pGoalTagWidgetList->HasOutLine())
    {
        m_pGoalTagWidgetList->SetWidgetOutline(true);
    }
}

bool CWorkSpaceView::InitGoalEditor()
{
    if(!m_pGoalEditor->isVisible())
    {
        m_pGoalEditor->setVisible(true);
    }
    if(CGoalWidget::EDIT == m_pGoalEditor->GetGoalWidgetMode())
    {
        this->SLOT_ShowMsgBoxProc("You are editting a goal, please finish it before.");
        return false;
    }
    m_pGoalEditor->ResetWidget();
    return true;
}
