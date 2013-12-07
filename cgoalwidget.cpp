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
    m_pTaskWidgetList->SetPageLength(10);
    m_pTaskWidgetList->setPos(m_iControllerWidth + this->GoalLabelWidth(),\
                              m_iControllerHeight / 2 + this->GoalLabelHeight() / 2\
                              - m_pTaskWidgetList->GetHeaderWidget()->boundingRect().height());
    this->SetLabelHeaderForTaskWidgetList("Task List");
    connect(m_pTaskWidgetList, SIGNAL(SIGNAL_WidgetSizeChanged()),\
            this, SLOT(SLOT_ChildWidgetSizeChangeProc()));

    this->InitBoundingRect(this->WidgetWidth(), this->WidgetHeight());
    this->SetGoalMode(VIEW);
    this->SetGoalColorTag(m_pColorTag->GetColor());
}

void CGoalWidget::SetGoalMode(EGoalMode a_EMode)
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
        break;
    case EDIT:
        m_pGoalNameLabel->SetEditable(true);
        m_pSvgWidgetEdit->setVisible(false);
        m_pSvgWidgetDel->setVisible(false);
        m_pSvgWidgetOK->setVisible(true);
        m_pColorTag->setVisible(true);
        break;
    default:
        break;
    }
}

void CGoalWidget::SetGoalColorTag(Qt::GlobalColor a_EColorTag)
{
    m_EColorTag = a_EColorTag;
    update(this->boundingRect());
}

int CGoalWidget::WidgetWidth()
{
    return m_iControllerWidth + this->GoalLabelWidth()\
            + m_pTaskWidgetList->boundingRect().width();
}

int CGoalWidget::WidgetHeight()
{
    int l_iHeight = m_iControllerHeight / 2 + this->GoalLabelHeight() / 2;
    if(l_iHeight < m_pTaskWidgetList->boundingRect().height()\
            - m_pTaskWidgetList->GetHeaderWidget()->boundingRect().height())
    {
        l_iHeight += m_pTaskWidgetList->boundingRect().height()\
                        - m_pTaskWidgetList->GetHeaderWidget()->boundingRect().height();
    }
    else
    {
        l_iHeight += l_iHeight;
    }
    return l_iHeight;
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
    painter->setPen(QPen(QBrush(Qt::black), 4));

    painter->fillPath(l_CPath, QBrush((Qt::GlobalColor)m_EColorTag));
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
            m_pTaskWidgetList->SetCollapse(!m_pTaskWidgetList->Collapse());
        }
    }
}

void CGoalWidget::SLOT_EditProc()
{
    this->SetGoalMode(EDIT);
    this->SetAddTaskBtnForTaskWidgetList();
    this->SetTaskModeBatch(CTaskWidget::EDIT);
}

void CGoalWidget::SLOT_DeleteProc()
{
    emit this->SIGNAL_RemoveWidget(this);
}

void CGoalWidget::SLOT_OKProc()
{
    this->SetGoalMode(VIEW);
    this->SetLabelHeaderForTaskWidgetList("Task List");
    this->SetTaskModeBatch(CTaskWidget::VIEW);
}

void CGoalWidget::SLOT_BackProc()
{
    this->SetGoalMode(VIEW);
    this->SetLabelHeaderForTaskWidgetList("Task List");
    this->SetTaskModeBatch(CTaskWidget::VIEW);
}

void CGoalWidget::SLOT_AddTaskWidgetProc()
{
    CTaskWidget* l_pTaskWidget = new CTaskWidget(NULL);
    m_pTaskWidgetList->AddWidget(l_pTaskWidget);
    connect(l_pTaskWidget, SIGNAL(SIGNAL_RemoveWidget(CGraphicsWidget*)),\
            m_pTaskWidgetList, SLOT(SLOT_RemoveWidget(CGraphicsWidget*)));
    connect(l_pTaskWidget, SIGNAL(SIGNAL_MouseDragRelease(QPointF,CGraphicsWidget*)),\
            this, SLOT(SLOT_TaskWidgetDragDropEmit(QPointF,CGraphicsWidget*)));
}

void CGoalWidget::SLOT_ChildWidgetSizeChangeProc()
{
    this->UpdateBoundingRect();
}

void CGoalWidget::SLOT_GoalLabelSizeChangeProc()
{
    m_pTaskWidgetList->setPos(m_iControllerWidth + this->GoalLabelWidth(),\
                              m_iControllerHeight / 2 + this->GoalLabelHeight() / 2\
                              - m_pTaskWidgetList->GetHeaderWidget()->boundingRect().height());
    this->UpdateBoundingRect();

}

void CGoalWidget::SLOT_ColorTagChangeProc(Qt::GlobalColor a_EColor)
{
    m_EColorTag = a_EColor;
    this->update(m_iControllerWidth, m_iControllerHeight / 2, this->GoalLabelWidth(),\
                 this->GoalLabelHeight());
}

void CGoalWidget::SLOT_TaskWidgetDragDropEmit(QPointF a_CMouseScenePos, CGraphicsWidget *a_pTaskWidget)
{
    emit this->SIGNAL_GoalTaskSend(a_CMouseScenePos,\
                                   ((CTaskWidget *)a_pTaskWidget)->GetTaskTag(),\
                                   (Qt::GlobalColor)m_EColorTag);
}

void CGoalWidget::SetTaskModeBatch(CTaskWidget::ETaskMode a_eTaskMode)
{
    CWidgetNode* l_pWidgetListHead = m_pTaskWidgetList->GetWidgetList();
    while(l_pWidgetListHead)
    {
        ((CTaskWidget *)(l_pWidgetListHead->m_pWidget))->SetTaskMode(a_eTaskMode);
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
