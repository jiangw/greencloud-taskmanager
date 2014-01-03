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
    m_pSvgWidgetBack = new CSvgWidget(":/icon/back", m_iControllerWidth, m_iControllerHeight,\
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
            this, SLOT(SLOT_DeletGoalVerify()));
    connect(m_pSvgWidgetOK, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_OKProc()));
    connect(m_pSvgWidgetBack, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_CancelEditVerify()));
    connect(m_pColorTag, SIGNAL(SIGNAL_ColorChanged(CGraphicsWidget::gColor)),\
            this, SLOT(SLOT_ColorTagChangeProc(CGraphicsWidget::gColor)));

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

    m_CPlanGoal.SetGoalId(-1); //set goal as invalid
    m_CPlanGoal.SetGoalColor(m_pColorTag->GetColor());
    m_iTaskIdGen = -1;
    m_blSimpleView = true;
}

void CGoalWidget::SetGoalWidgetMode(EGoalWidgetMode a_EMode)
{
    m_EMode = a_EMode;
    this->UpdateGoalWidget();
}

void CGoalWidget::SetGoalData(const CPlanGoal *a_pPlanGoal, bool a_blSimpleView)
{
    m_CPlanGoal.SetGoalId(a_pPlanGoal->GetGoalId());
    m_CPlanGoal.SetGoalName(a_pPlanGoal->GetGoalName());
    m_CPlanGoal.SetGoalColor(a_pPlanGoal->GetGoalColor());
    m_pGoalNameLabel->SetText(a_pPlanGoal->GetGoalName());
    m_pColorTag->SetColor((CGraphicsWidget::gColor)a_pPlanGoal->GetGoalColor());

    const STask* l_pTaskListIter = a_pPlanGoal->GetTaskListHead();
    m_iTaskIdGen = 0;
    while(l_pTaskListIter)
    {
        m_CPlanGoal.AddTask(l_pTaskListIter->m_iTaskId,\
                            l_pTaskListIter->m_qstrTaskTag,\
                            l_pTaskListIter->m_qstrDescription,\
                            l_pTaskListIter->m_blIsFinished);
        CTaskWidget* l_pNewTaskWidget = new CTaskWidget(l_pTaskListIter->m_iTaskId, NULL);
        l_pNewTaskWidget->SetTaskData(l_pTaskListIter->m_qstrTaskTag,\
                                      l_pTaskListIter->m_qstrDescription,\
                                      l_pTaskListIter->m_blIsFinished);
        this->AddTaskWidget(l_pNewTaskWidget);
        m_iTaskIdGen++;
        l_pTaskListIter = l_pTaskListIter->m_pNext;
    }

    this->SetSimpleView(a_blSimpleView);
    this->UpdateBoundingRect();
}

void CGoalWidget::SetGoalColorTag(CGraphicsWidget::gColor a_EColorTag)
{
    m_pColorTag->SetColor(a_EColorTag);
    this->update(m_iControllerWidth, m_iControllerHeight / 2, this->GoalLabelWidth(),\
                 this->GoalLabelHeight());
}

void CGoalWidget::AddTaskWidget(CTaskWidget *a_pTaskWidget)
{
    m_pTaskWidgetList->AddWidget(a_pTaskWidget);
    connect(a_pTaskWidget, SIGNAL(SIGNAL_MouseDragRelease(QPointF,CGraphicsWidget*)),\
            this, SLOT(SLOT_TaskWidgetDragDropEmit(QPointF,CGraphicsWidget*)));
    connect(a_pTaskWidget, SIGNAL(SIGNAL_TaskFinishStatChange(CTaskWidget*)),\
            this, SLOT(SLOT_TaskStatusChangeProc(CTaskWidget*)));
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
        m_CPlanGoal.AddTask(l_pTaskWidget->GetTaskId(),\
                            l_pTaskWidget->GetTaskTag(),\
                            l_pTaskWidget->GetTaskDescription(),\
                            l_pTaskWidget->IsTaskFinished());
        l_pWidgetListHead = l_pWidgetListHead->m_pNext;
    }
    //turn to VIEW mode
    m_blSimpleView = false;
    this->SetGoalWidgetMode(VIEW);

    emit this->SIGNAL_PlanGoalSubmit(&m_CPlanGoal);
}

void CGoalWidget::SetSimpleView(bool a_blSimpleView)
{
    m_blSimpleView = a_blSimpleView;
    if(VIEW == m_EMode)
    {
        this->UpdateGoalWidget();
    }
}

void CGoalWidget::ResetWidget()
{
    m_pTaskWidgetList->ResetWidget();
    m_pColorTag->ResetWidget();
    m_pGoalNameLabel->ResetWidget();
    m_CPlanGoal.SetGoalId(-1);
    m_CPlanGoal.ClearTaskList();

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
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QRectF l_CRect(m_iControllerWidth + 2, m_iControllerHeight / 2 + 2,\
                   this->GoalLabelWidth() - 4, this->GoalLabelHeight() - 4);
    QPainterPath l_CPath;
    l_CPath.addEllipse(l_CRect);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 3));

    painter->fillPath(l_CPath, QBrush(QColor((QRgb)m_pColorTag->GetColor())));
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
        this->SetSimpleView(!m_blSimpleView);
    }
}

void CGoalWidget::SLOT_EditProc()
{
    this->SetGoalWidgetMode(EDIT);
}

void CGoalWidget::SLOT_DeletGoalVerify()
{
    if(-1 != m_CPlanGoal.GetGoalId())
    {
        CMessageWidget* l_pMsgBox = new CMessageWidget(this->scene());
        l_pMsgBox->SetMsgType(CMessageWidget::MS_YESNO);
        l_pMsgBox->SetQuestion("Do you want to delete the goal?");
        l_pMsgBox->SetMsgBoxWidth(300);
        l_pMsgBox->SetMsgBoxPos(this->pos().x(),\
                                this->pos().y() + m_iControllerHeight + this->GoalLabelHeight());
        connect(l_pMsgBox, SIGNAL(SIGNAL_Ok()), this, SLOT(SLOT_DeleteProc()));
    }
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

void CGoalWidget::SLOT_CancelEditVerify()
{
    //check for changes
    if(this->IsGoalChanged())
    {
        CMessageWidget* l_pMsgBox = new CMessageWidget(this->scene());
        l_pMsgBox->SetMsgType(CMessageWidget::MS_YESNO);
        l_pMsgBox->SetInfo("The goal has been modified:");
        l_pMsgBox->SetQuestion("Do you want to discard unsaved changes?");
        l_pMsgBox->SetMsgBoxWidth(400);
        l_pMsgBox->SetMsgBoxPos(this->pos().x(),\
                                this->pos().y() + m_iControllerHeight + this->GoalLabelHeight());
        connect(l_pMsgBox, SIGNAL(SIGNAL_Ok()), this, SLOT(SLOT_BackProc()));
    }
    else
    {
        this->SLOT_BackProc();
    }
}

void CGoalWidget::SLOT_BackProc()
{
    if(m_CPlanGoal.GetGoalId() == -1)
    {
        m_EMode = VIEW;
        this->setVisible(false);
    }
    else
    {
        this->GoalDataRecover();
        m_blSimpleView = false;
        this->SetGoalWidgetMode(VIEW);
    }
}

void CGoalWidget::SLOT_AddTaskWidgetProc()
{
    CTaskWidget* l_pTaskWidget = new CTaskWidget(++m_iTaskIdGen, NULL);
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
    m_pSvgWidgetBack->setPos(m_iControllerWidth + this->GoalLabelWidth(), 0);
    this->UpdateBoundingRect();

}

void CGoalWidget::SLOT_ColorTagChangeProc(CGraphicsWidget::gColor a_EColor)
{
    Q_UNUSED(a_EColor)

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
                                    m_CPlanGoal.GetGoalId(),\
                                   ((CTaskWidget *)a_pTaskWidget)->GetTaskId());
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
    m_CPlanGoal.FinishTask(l_pTask->m_iTaskId, a_pTaskWidget->IsTaskFinished());
    if(a_pTaskWidget->IsTaskFinished())
    {
        a_pTaskWidget->SetTaskMode(CTaskWidget::VIEW);
    }
    emit this->SIGNAL_TaskFinishStatSync(m_CPlanGoal.GetGoalId(),\
                                         l_pTask->m_iTaskId,\
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
            if(CTaskWidget::EDIT == a_ETaskMode)
            {
                ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->FreezeWidget(true);
            }
            else
            {
                if(m_blSimpleView)
                {
                    ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->FreezeWidget(true);
                }
                else
                {
                    ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->FreezeWidget(false);
                }
            }
        }
        l_pWidgetListHead = l_pWidgetListHead->m_pNext;
    }
    m_pTaskWidgetList->UpdateWidgetList();
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

void CGoalWidget::UpdateGoalWidget()
{
    switch(m_EMode)
    {
    case VIEW:
        m_pGoalNameLabel->SetEditable(false);
        if(m_blSimpleView)
        {
            m_pSvgWidgetEdit->setVisible(false);
            m_pSvgWidgetDel->setVisible(false);
        }
        else
        {
            m_pSvgWidgetEdit->setVisible(true);
            m_pSvgWidgetDel->setVisible(true);
        }
        m_pSvgWidgetOK->setVisible(false);
        m_pSvgWidgetBack->setVisible(false);
        m_pColorTag->setVisible(false);
        this->SetLabelHeaderForTaskWidgetList("Task List");
        this->SetTaskModeBatch(CTaskWidget::VIEW);
        break;
    case EDIT:
        m_pGoalNameLabel->SetEditable(true);
        m_pSvgWidgetEdit->setVisible(false);
        m_pSvgWidgetDel->setVisible(false);
        m_pSvgWidgetOK->setVisible(true);
        m_pSvgWidgetBack->setVisible(true);
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

int CGoalWidget::GoalLabelWidth()
{
    return m_pGoalNameLabel->boundingRect().width() * 2;
}

int CGoalWidget::GoalLabelHeight()
{
    return m_pGoalNameLabel->boundingRect().height() * 2;
}

bool CGoalWidget::IsGoalChanged()
{
    bool l_blGoalChanged = false;
    if(m_CPlanGoal.GetGoalName() != m_pGoalNameLabel->GetText())
    {
        l_blGoalChanged = true;
    }
    if(!l_blGoalChanged)
    {
        if(m_CPlanGoal.GetGoalColor() != (QRgb)m_pColorTag->GetColor())
        {
            l_blGoalChanged = true;
        }
    }
    if(!l_blGoalChanged)
    {
        if(m_pTaskWidgetList->ListLength() != m_CPlanGoal.TaskNumber())
        {
            l_blGoalChanged = true;
        }
        if(!l_blGoalChanged)
        {
            CWidgetNode* l_pWidgetIter = m_pTaskWidgetList->GetWidgetList();
            while(l_pWidgetIter)
            {
                CTaskWidget* l_pTaskWidget = (CTaskWidget *)l_pWidgetIter->m_pWidget;
                const STask* l_pOldTask = m_CPlanGoal.GetTaskById(l_pTaskWidget->GetTaskId());
                if(NULL == l_pOldTask)
                {
                    l_blGoalChanged = true;
                    break;
                }
                if(l_pTaskWidget->GetTaskTag() != l_pOldTask->m_qstrTaskTag\
                        || l_pTaskWidget->GetTaskDescription() != l_pOldTask->m_qstrDescription)
                {
                    l_blGoalChanged = true;
                    break;
                }
                l_pWidgetIter = l_pWidgetIter->m_pNext;
            }
        }
    }

    return l_blGoalChanged;
}

void CGoalWidget::GoalDataRecover()
{
    m_pTaskWidgetList->ResetWidget();
    this->SetGoalData(&m_CPlanGoal);
}
