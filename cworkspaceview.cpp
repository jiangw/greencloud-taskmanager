#include "cworkspaceview.h"

CWorkSpaceView::CWorkSpaceView(CWorkSpace *a_pWorkSpace, QWidget *a_pParent) :
    QGraphicsView(a_pWorkSpace, a_pParent)
{
    m_pWorkSpace = a_pWorkSpace;
    m_pPlanWidget = NULL;
    m_pMonthWidget = NULL;
    m_pDayWidget = NULL;
    m_pGoalWidgetList = NULL;

    //receive mouse move events even if no button is pressed
    this->setMouseTracking(true);
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
    }
}

void CWorkSpaceView::SLOT_CreatePlanActionProc()
{
    if(NULL == m_pMonthWidget)
    {
        m_pMonthWidget = new CMonthWidget(NULL);
        m_pMonthWidget->EnableMultiSelection(false);
        m_pWorkSpace->addItem(m_pMonthWidget);
    }
    else
    {
        m_pMonthWidget->ResetWidget();
    }

    if(NULL == m_pDayWidget)
    {
        m_pDayWidget = new CDayWidget(NULL);
        m_pWorkSpace->addItem(m_pDayWidget);
        connect(m_pMonthWidget, SIGNAL(SIGNAL_DaySel(QDate)),\
                m_pDayWidget, SLOT(SLOT_SetDateProc(QDate)));
    }
    else
    {
        m_pDayWidget->ResetWidget();
    }

    if(NULL == m_pPlanWidget)
    {
        m_pPlanWidget = new CPlanWidget(NULL);
        m_pWorkSpace->addItem(m_pPlanWidget);

        connect(m_pDayWidget, SIGNAL(SIGNAL_MouseDragRelease(QPointF,CGraphicsWidget*)),\
                m_pPlanWidget, SLOT(SLOT_MouseDragDropProc(QPointF,CGraphicsWidget*)));
        connect(m_pDayWidget, SIGNAL(SIGNAL_HourSelMaskRequest(QDate,int)),\
                m_pPlanWidget, SLOT(SLOT_HourSelMaskRequestProc(QDate,int)));
        connect(m_pPlanWidget, SIGNAL(SIGNAL_HourSelMaskSend(bool*,bool)),\
                m_pDayWidget, SLOT(SLOT_HourSelMaskRecieveProc(bool*,bool)));
        connect(m_pDayWidget, SIGNAL(SIGNAL_HourSelMaskRecieveFeedback(bool*)),\
                m_pPlanWidget, SLOT(SLOT_HourSelMaskRecieveFeedbackProc(bool*)));
    }
    else
    {
        m_pPlanWidget->ResetWidget();
    }

    //add widget list for storing goal widgets
    if(NULL == m_pGoalWidgetList)
    {
        m_pGoalWidgetList = new CWidgetList(NULL);
        m_pWorkSpace->addItem(m_pGoalWidgetList);
        //set header of widget list as push button
        CButtonWidget* l_pAddGoalBtn = new CButtonWidget("Add Goal", NULL);
        m_pGoalWidgetList->SetHeaderWidget(l_pAddGoalBtn);
        connect(l_pAddGoalBtn, SIGNAL(SIGNAL_LeftButtonClicked()),\
                this, SLOT(SLOT_AddGoalWidgetToWidgetListEmit()));
        connect(this, SIGNAL(SIGNAL_AddWidgetToWidgetList(CGraphicsWidget*)),\
                m_pGoalWidgetList, SLOT(SLOT_AddWidget(CGraphicsWidget*)));
    }
    else
    {
        m_pGoalWidgetList->ResetWidget();
    }

    m_pMonthWidget->setPos(-270, -218);
    m_pDayWidget->setPos(-240, TASKMANAGER::g_iItemIntervalY);
    m_pPlanWidget->setPos(TASKMANAGER::g_iItemIntervalX, -250);
    m_pGoalWidgetList->setPos(m_pPlanWidget->pos().x() + m_pPlanWidget->boundingRect().width()\
                              + TASKMANAGER::g_iItemIntervalX,\
                              m_pPlanWidget->pos().y());

/*    this->ensureVisible(-310, -280,\
                        m_pMonthWidget->boundingRect().width()\
                        + m_pPlanWidget->boundingRect().width() + 490, 500);*/
    this->centerOn(m_pPlanWidget->pos().x() + m_pPlanWidget->boundingRect().width() * 0.7,\
                   0);
}

void CWorkSpaceView::SLOT_CenterOnItemProc(QGraphicsItem *a_pItem)
{
    this->centerOn(a_pItem);
}

void CWorkSpaceView::SLOT_ResetViewActionProc()
{
    this->centerOn(m_pPlanWidget->pos().x() + m_pPlanWidget->boundingRect().width() * 0.7,\
                   0);
}

void CWorkSpaceView::SLOT_AddGoalWidgetToWidgetListEmit()
{
    CGoalWidget* l_pNewGoalWidget = new CGoalWidget(NULL);
    connect(l_pNewGoalWidget, SIGNAL(SIGNAL_GoalTaskSend(QPointF,QString,Qt::GlobalColor)),\
            m_pPlanWidget, SLOT(SLOT_GoalTaskRecieve(QPointF,QString,Qt::GlobalColor)));

    emit this->SIGNAL_AddWidgetToWidgetList(l_pNewGoalWidget);
}

void CWorkSpaceView::SLOT_DeleteWidgetFromSceneProc(CGraphicsWidget *a_pDelWidget)
{
    m_pWorkSpace->removeItem(a_pDelWidget);
}
