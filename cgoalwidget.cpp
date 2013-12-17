#include "cgoalwidget.h"

CGoalWidget::CGoalWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_iControllerHeight = 30;
    m_iControllerWidth = 30;

    m_pGoalNameLabel = new CTextWidget(false, this);
    m_pGoalNameLabel->SetInputTip("Goal Name");
    m_pGoalNameLabel->SetMinimWidth(100);
    m_pGoalNameLabel->SetWidgetOutline(false);
    m_pGoalNameLabel->setPos(m_iControllerWidth + this->GoalLabelWidth() / 4,\
                             m_iControllerHeight / 2 + this->GoalLabelHeight() / 4);
    connect(m_pGoalNameLabel, SIGNAL(SIGNAL_WidgetSizeChanged()),\
            this, SLOT(SLOT_GoalLabelSizeChangeProc()));

    m_pSvgWidgetEdit = new CSvgWidget(":/icon/edit", m_iControllerWidth, m_iControllerHeight,\
                                      this);
    m_pSvgWidgetDel = new CSvgWidget(":/icon/delete", m_iControllerWidth, m_iControllerHeight,\
                                     this);
    m_pSvgWidgetOK = new CSvgWidget(":/icon/ok", m_iControllerWidth, m_iControllerHeight,\
                                    this);
    m_pColorTag = new CColorTagWidget(m_iControllerWidth, m_iControllerHeight, this);
    m_pColorTag->InitSelector();

    m_pSvgWidgetEdit->setPos(0, 0);
    m_pSvgWidgetOK->setPos(0, 0);
    m_pSvgWidgetDel->setPos(0, this->GoalLabelHeight());
    m_pColorTag->setPos(3, this->GoalLabelHeight());

    connect(m_pSvgWidgetEdit, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_EditProc()));
    connect(m_pSvgWidgetDel, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_DeleteProc()));
    connect(m_pSvgWidgetOK, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_OKProc()));
    connect(m_pColorTag, SIGNAL(SIGNAL_ColorChanged(Qt::GlobalColor)),\
            this, SLOT(SLOT_ColorTagChangeProc(Qt::GlobalColor)));

    m_pTaskWidgetList = new CWidgetList(this);
    m_pTaskWidgetList->SetWidgetOutline(false);
    m_pTaskWidgetList->SetPageLength(8);
    m_pTaskWidgetList->setPos(m_iControllerWidth,\
                              m_pColorTag->pos().y() + m_pColorTag->boundingRect().height()\
                              + GREENSCHEDULE::g_iItemIntervalY);
    this->SetLabelHeaderForTaskWidgetList("Task List");
    connect(m_pTaskWidgetList, SIGNAL(SIGNAL_WidgetSizeChanged()),\
            this, SLOT(SLOT_ChildWidgetSizeChangeProc()));

    this->InitBoundingRect(this->WidgetWidth(), this->WidgetHeight());
    this->SetGoalWidgetMode(VIEW);
    this->SetGoalColorTag(m_pColorTag->GetColor());

    m_CPlanGoal.SetGoalId(-1); //set goal as invalid
}

void CGoalWidget::SetGoalWidgetMode(EGoalWidgetMode a_EMode)
{
    m_EMode = a_EMode;
    switch(m_EMode)
    {
    case VIEW:
        m_pGoalNameLabel->SetEditable(false);
        m_pSvgWidgetEdit->setVisible(true);
        m_pSvgWidgetDel->setVisible(true);
        m_pSvgWidgetOK->setVisible(false);
        m_pColorTag->setVisible(false);
        this->SetLabelHeaderForTaskWidgetList("Task List");
        this->SetTaskModeBatch(CTaskWidget::VIEW);
        break;
    case EDIT:
        m_pGoalNameLabel->SetEditable(true);
        m_pSvgWidgetEdit->setVisible(false);
        m_pSvgWidgetDel->setVisible(false);
        m_pSvgWidgetOK->setVisible(true);
        m_pColorTag->setVisible(true);
        m_pColorTag->SelectorSwitch(true);
        this->SetAddTaskBtnForTaskWidgetList();
        this->SetTaskModeBatch(CTaskWidget::EDIT);
        break;
    default:
        break;
    }

    this->UpdateBoundingRect();
}

void CGoalWidget::SetGoalData(const CPlanGoal *a_pPlanGoal)
{
    m_CPlanGoal.SetGoalId(a_pPlanGoal->GetGoalId());
    m_CPlanGoal.SetGoalName(a_pPlanGoal->GetGoalName());
    m_CPlanGoal.SetGoalColor(a_pPlanGoal->GetGoalColor());
    m_pGoalNameLabel->SetText(a_pPlanGoal->GetGoalName());
    m_pColorTag->SetColor(a_pPlanGoal->GetGoalColor());

    const STask* l_pTaskListHead = a_pPlanGoal->GetTaskListHead();
    while(l_pTaskListHead)
    {
        CTaskWidget* l_pNewTaskWidget = new CTaskWidget(NULL);
        l_pNewTaskWidget->SetTaskData(l_pTaskListHead->m_qstrTaskTag,\
                                      l_pTaskListHead->m_qstrDescription,\
                                      l_pTaskListHead->m_blIsFinished);
        this->AddTaskWidget(l_pNewTaskWidget);
        l_pTaskListHead = l_pTaskListHead->m_pNext;
    }

    this->UpdateBoundingRect();
}

void CGoalWidget::SetGoalColorTag(Qt::GlobalColor a_EColorTag)
{
    m_CPlanGoal.SetGoalColor(a_EColorTag);
    update(this->boundingRect());
}

void CGoalWidget::AddTaskWidget(CTaskWidget *a_pTaskWidget)
{
    m_pTaskWidgetList->AddWidget(a_pTaskWidget);
    connect(a_pTaskWidget, SIGNAL(SIGNAL_RemoveWidget(CGraphicsWidget*)),\
            m_pTaskWidgetList, SLOT(SLOT_RemoveWidget(CGraphicsWidget*)));
    connect(a_pTaskWidget, SIGNAL(SIGNAL_MouseDragRelease(QPointF,CGraphicsWidget*)),\
            this, SLOT(SLOT_TaskWidgetDragDropEmit(QPointF,CGraphicsWidget*)));
    connect(a_pTaskWidget, SIGNAL(SIGNAL_TaskFinishStatChange(CTaskWidget*)),\
            this, SLOT(SLOT_TaskStatusChangeProc(CTaskWidget*)));

    QString l_qstrTaskTag = a_pTaskWidget->GetTaskTag();
    if(!l_qstrTaskTag.isNull() && !l_qstrTaskTag.isEmpty())
    {
        m_CPlanGoal.AddTask(l_qstrTaskTag, a_pTaskWidget->GetTaskDescription(),\
                            a_pTaskWidget->IsTaskFinished());
    }
}

void CGoalWidget::PlanGoalRevise(int a_iGoalId)
{
    //if goal is accepted, then submit its whole content
    if(-1 != a_iGoalId)
    { //turn to VIEW mode
        m_CPlanGoal.SetGoalId(a_iGoalId);
        this->PlanGoalSubmit();
    }
}

void CGoalWidget::PlanGoalSubmit()
{
    m_CPlanGoal.SetGoalName(m_pGoalNameLabel->GetText());
    m_CPlanGoal.SetGoalColor(m_pColorTag->GetColor());
    CWidgetNode* l_pWidgetListHead = m_pTaskWidgetList->GetWidgetList();
    m_CPlanGoal.ClearTaskList();
    while(l_pWidgetListHead)
    {
        CTaskWidget* l_pTaskWidget = (CTaskWidget*)(l_pWidgetListHead->m_pWidget);
        m_CPlanGoal.AddTask(l_pTaskWidget->GetTaskTag(),\
                            l_pTaskWidget->GetTaskDescription());
        m_CPlanGoal.FinishTask(l_pTaskWidget->GetTaskTag(),\
                               l_pTaskWidget->IsTaskFinished());
        l_pWidgetListHead = l_pWidgetListHead->m_pNext;
    }
    //turn to VIEW mode
    this->SetGoalWidgetMode(VIEW);

    emit this->SIGNAL_PlanGoalSubmit(&m_CPlanGoal);
}

void CGoalWidget::ResetWidget()
{
    m_pTaskWidgetList->ResetWidget();
    m_pColorTag->ResetWidget();
    m_pGoalNameLabel->ResetWidget();
    m_CPlanGoal.SetGoalId(-1);
    m_CPlanGoal.ClearTaskList();
    this->SetGoalColorTag(m_pColorTag->GetColor());

    this->UpdateBoundingRect();
}

int CGoalWidget::WidgetWidth()
{
    int l_iWidth = this->GoalLabelWidth();
    if(m_pTaskWidgetList->boundingRect().width() > l_iWidth)
    {
        l_iWidth = m_pTaskWidgetList->boundingRect().width();
    }
    return m_iControllerWidth + l_iWidth;
}

int CGoalWidget::WidgetHeight()
{
    return m_iControllerHeight + this->GoalLabelHeight()\
            + GREENSCHEDULE::g_iItemIntervalY\
            + m_pTaskWidgetList->boundingRect().height();
}

QString CGoalWidget::WidgetClassName()
{
    return "CGoalWidget";
}

void CGoalWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF l_CRect(m_iControllerWidth + 2, m_iControllerHeight / 2 + 2,\
                   this->GoalLabelWidth() - 4, this->GoalLabelHeight() - 4);
    QPainterPath l_CPath;
    l_CPath.addEllipse(l_CRect);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 3));

    painter->fillPath(l_CPath, QBrush(m_CPlanGoal.GetGoalColor()));
    painter->drawEllipse(l_CRect);

    painter->restore();
}

void CGoalWidget::LeftButtonClicked(QPointF a_CMousePos)
{
    QPainterPath l_CGoalNameAreaPath;
    l_CGoalNameAreaPath.addEllipse(m_iControllerWidth, m_iControllerHeight / 2,\
                                   this->GoalLabelWidth(), this->GoalLabelHeight());
    if(l_CGoalNameAreaPath.contains(a_CMousePos))
    {
        if(VIEW == m_EMode)
        {
            m_pSvgWidgetEdit->setVisible(!m_pSvgWidgetEdit->isVisible());
            m_pSvgWidgetDel->setVisible(!m_pSvgWidgetDel->isVisible());
//            m_pTaskWidgetList->SetCollapse(!m_pTaskWidgetList->IsCollapsed());
            //just show unfinished tasks
            CWidgetNode* l_pWidgeIter = m_pTaskWidgetList->GetWidgetList();
            while(l_pWidgeIter)
            {
                if(((CTaskWidget *)l_pWidgeIter->m_pWidget)->IsTaskFinished())
                {
                    l_pWidgeIter->m_pWidget->FreezeWidget(!l_pWidgeIter->m_pWidget->IsFreezed());
                }
                l_pWidgeIter = l_pWidgeIter->m_pNext;
            }
            m_pTaskWidgetList->UpdateWidgetList();
        }
    }
}

void CGoalWidget::SLOT_EditProc()
{
    this->SetGoalWidgetMode(EDIT);
}

void CGoalWidget::SLOT_DeleteProc()
{
    if(-1 != m_CPlanGoal.GetGoalId())
    {
        emit this->SIGNAL_PlanGoalRetract(m_CPlanGoal.GetGoalId());
    }
    this->ResetWidget();
    this->setVisible(false);
}

void CGoalWidget::SLOT_OKProc()
{
    //get goal name from UI
    QString l_qstrGoalName = m_pGoalNameLabel->GetText();
    if(l_qstrGoalName.isNull() || l_qstrGoalName.isEmpty())
    { //invalid goal name
        emit this->SIGNAL_ShowInMessageBox("Invalid Goal Name.");
        return;
    }
    //check current goal
    if(-1 == m_CPlanGoal.GetGoalId())
    { //create new goal
        emit this->SIGNAL_PlanGoalPropose(this);
    }
    else
    { //update goal
        this->PlanGoalSubmit();
    }
    /*
    this->SetGoalWidgetMode(VIEW);
    this->SetLabelHeaderForTaskWidgetList("Task List");
    this->SetTaskModeBatch(CTaskWidget::VIEW); */
}

void CGoalWidget::SLOT_AddTaskWidgetProc()
{
    CTaskWidget* l_pTaskWidget = new CTaskWidget(NULL);
    this->AddTaskWidget(l_pTaskWidget);
}

void CGoalWidget::SLOT_ChildWidgetSizeChangeProc()
{
    this->UpdateBoundingRect();
}

void CGoalWidget::SLOT_GoalLabelSizeChangeProc()
{
    m_pGoalNameLabel->setPos(m_iControllerWidth + this->GoalLabelWidth() / 4,\
                             m_iControllerHeight / 2 + this->GoalLabelHeight() / 4);
    this->UpdateBoundingRect();

}

void CGoalWidget::SLOT_ColorTagChangeProc(Qt::GlobalColor a_EColor)
{
    m_CPlanGoal.SetGoalColor(a_EColor);
    this->update(m_iControllerWidth, m_iControllerHeight / 2, this->GoalLabelWidth(),\
                 this->GoalLabelHeight());
}

void CGoalWidget::SLOT_TaskWidgetDragDropEmit(QPointF a_CMouseScenePos, CGraphicsWidget *a_pTaskWidget)
{
    if(((CTaskWidget *)a_pTaskWidget)->IsTaskFinished())
    {
        emit this->SIGNAL_ShowInMessageBox("The task has already been finished.");
    }
    else
    {
        emit this->SIGNAL_GoalTaskSend(a_CMouseScenePos,\
                                    m_CPlanGoal.GetGoalName(),\
                                   ((CTaskWidget *)a_pTaskWidget)->GetTaskTag(),\
                                    m_CPlanGoal.GetGoalColor());
    }
}

void CGoalWidget::SLOT_TaskStatusChangeProc(CTaskWidget *a_pTaskWidget)
{
    const STask* l_pTask = m_CPlanGoal.GetTaskByTag(a_pTaskWidget->GetTaskTag());
    if(NULL == l_pTask)
    {
        std::cerr << "Error[CGoalWidget]: Cannot find the task in current Goal." << std::endl;
        return;
    }
    m_CPlanGoal.FinishTask(l_pTask->m_qstrTaskTag, a_pTaskWidget->IsTaskFinished());
    if(a_pTaskWidget->IsTaskFinished())
    {
        a_pTaskWidget->SetTaskMode(CTaskWidget::VIEW);
    }
    emit this->SIGNAL_TaskFinishStatSync(m_CPlanGoal.GetGoalId(),\
                                         l_pTask->m_qstrTaskTag,\
                                         l_pTask->m_blIsFinished);
}

void CGoalWidget::SetTaskModeBatch(CTaskWidget::ETaskMode a_ETaskMode)
{
    CWidgetNode* l_pWidgetListHead = m_pTaskWidgetList->GetWidgetList();
    while(l_pWidgetListHead)
    {
        ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->SetTaskMode(a_ETaskMode);
        if(((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->IsTaskFinished())
        {
            ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->SetTaskMode(CTaskWidget::VIEW);
        }
        if(CTaskWidget::EDIT == a_ETaskMode)
        {
            ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->DisableCheck();
        }
        l_pWidgetListHead = l_pWidgetListHead->m_pNext;
    }
}

void CGoalWidget::SetLabelHeaderForTaskWidgetList(QString a_qstrLabelText)
{
    CTextWidget* l_pLabel = new CTextWidget(false, NULL);
    l_pLabel->SetText(a_qstrLabelText);
    l_pLabel->SetFontSize(10);
    l_pLabel->SetWidgetOutline(false);
    l_pLabel->SetWidgetUnderline(false);
    m_pTaskWidgetList->SetHeaderWidget(l_pLabel);
}

void CGoalWidget::SetAddTaskBtnForTaskWidgetList()
{
    CButtonWidget* l_pAddTaskBtn = new CButtonWidget("Add Task", NULL);
    l_pAddTaskBtn->SetTextFontSize(10);
    m_pTaskWidgetList->SetHeaderWidget(l_pAddTaskBtn);
    connect(l_pAddTaskBtn, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_AddTaskWidgetProc()));
}

int CGoalWidget::GoalLabelWidth()
{
    return m_pGoalNameLabel->boundingRect().width() * 2;
}

int CGoalWidget::GoalLabelHeight()
{
    return m_pGoalNameLabel->boundingRect().height() * 2;
}
