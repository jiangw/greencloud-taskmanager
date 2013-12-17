#include "ctaskwidget.h"

CTaskWidget::CTaskWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_blIsTaskFinished = false;
    m_iControllerSize = 20;
    m_iTagWidth = 80;
    m_CDescFont.setFamily("Courier New");
    m_CDescFont.setPointSize(8);
    m_CTagFont = m_CDescFont;
    m_CTagFont.setItalic(true);

    m_pCheckWidget = new CCheckWidget(this);
    m_pCheckWidget->SetChecked(false);
    m_pCheckWidget->setPos(1, 1);
    connect(m_pCheckWidget, SIGNAL(SIGNAL_Checked()),\
            this, SLOT(SLOT_TaskStatusChangeProc()));

    m_pDelWidget = new CSvgWidget(":/icon/delete", m_iControllerSize, m_iControllerSize, this);
    m_pDelWidget->setPos(0, 0);
    connect(m_pDelWidget, SIGNAL(SIGNAL_LeftButtonClicked()),\
            this, SLOT(SLOT_RemoveWidgetEmit()));

    m_pTagWidget = new CTextWidget(true, this);
    m_pTagWidget->SetInputTip("Task tag");
    m_pTagWidget->SetFont(m_CTagFont);
    m_pTagWidget->SetFixedSize(m_iTagWidth, m_pTagWidget->WidgetHeight());
    m_pTagWidget->setPos(m_iControllerSize + GREENSCHEDULE::g_iItemIntervalX, 0);
    m_pTagWidget->setFlag(QGraphicsItem::ItemStacksBehindParent);
    m_pTagWidget->SetHorizontalExt(0);
    m_pTagWidget->SetVerticalExt(0);
    connect(m_pTagWidget, SIGNAL(SIGNAL_EditFinished()),\
            this, SLOT(SLOT_TaskTagEditFinishProc()));

    m_pDescWidget = new CTextWidget(true, this);
    m_pDescWidget->SetInputTip("Task description");
    m_pDescWidget->SetFont(m_CDescFont);
    m_pDescWidget->SetWidgetOutline(false);
    m_pDescWidget->SetWidgetUnderline(true);
    m_pDescWidget->setPos(m_pTagWidget->pos().x() + m_pTagWidget->boundingRect().width(), 0);
    m_pDescWidget->setFlag(QGraphicsItem::ItemStacksBehindParent);
    connect(m_pDescWidget, SIGNAL(SIGNAL_WidgetSizeChanged()),\
            this, SLOT(SLOT_TextWidgetSizeChangeProc()));

    this->SetTaskMode(EDIT);
    this->InitBoundingRect();
}

void CTaskWidget::SetTaskMode(ETaskMode a_ETaskMode)
{
    m_EMode = a_ETaskMode;
    switch(m_EMode)
    {
    case EDIT:
        m_pCheckWidget->setVisible(false);
        m_pDelWidget->setVisible(true);
        break;
    case VIEW:
        m_pCheckWidget->setVisible(true);
        m_pDelWidget->setVisible(false);
        break;
    default:
        break;
    }
}

QString CTaskWidget::GetTaskTag()
{
    return m_pTagWidget->GetText();
}

QString CTaskWidget::GetTaskDescription()
{
    return m_pDescWidget->GetText();
}

bool CTaskWidget::IsTaskFinished()
{
    return m_blIsTaskFinished;
}

void CTaskWidget::SetTaskData(QString a_qstrTaskTag, QString a_qstrTaskDesc,\
                              bool a_blIsFinished)
{
    m_pTagWidget->SetText(a_qstrTaskTag);
    m_pDescWidget->SetText(a_qstrTaskDesc);
    m_pCheckWidget->SetChecked(a_blIsFinished);
    m_blIsTaskFinished = a_blIsFinished;
}

void CTaskWidget::DisableCheck()
{
    m_pCheckWidget->setVisible(false);
}

int CTaskWidget::WidgetWidth()
{
    return m_iControllerSize + GREENSCHEDULE::g_iItemIntervalX\
            + m_pTagWidget->boundingRect().width()\
            + m_pDescWidget->boundingRect().width();
}

int CTaskWidget::WidgetHeight()
{
    int l_iHeight = m_iControllerSize;
    if(m_pTagWidget->boundingRect().height() > l_iHeight)
    {
        l_iHeight = m_pTagWidget->boundingRect().height();
    }
    if(m_pDescWidget->boundingRect().height() > l_iHeight)
    {
        l_iHeight = m_pDescWidget->boundingRect().height();
    }

    return l_iHeight;
}

void CTaskWidget::paint(QPainter */*painter*/, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
}

void CTaskWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch(m_EMode)
    {
    case EDIT:
        event->ignore();
        break;
    default:
        CGraphicsWidget::mousePressEvent(event);
        break;
    }
}

void CTaskWidget::MouseDragMove(QPointF a_CMousePos)
{
    Q_UNUSED(a_CMousePos)

    this->setCursor(Qt::ClosedHandCursor);
}

void CTaskWidget::MouseDragRelease(QPointF a_CMousePos)
{
    Q_UNUSED(a_CMousePos)

    this->setCursor(Qt::ArrowCursor);
}

void CTaskWidget::SLOT_TaskStatusChangeProc()
{
    if(m_pCheckWidget->IsChecked())
    {
        m_blIsTaskFinished = true;
    }
    else
    {
        m_blIsTaskFinished = false;
    }
    emit this->SIGNAL_TaskFinishStatChange(this);
}

void CTaskWidget::SLOT_RemoveWidgetEmit()
{
    emit this->SIGNAL_RemoveWidget(this);
}

void CTaskWidget::SLOT_TextWidgetSizeChangeProc()
{
    this->UpdateBoundingRect();

    emit this->SIGNAL_WidgetSizeChanged();
}

void CTaskWidget::SLOT_TaskTagEditFinishProc()
{
    m_pDescWidget->TakeInput();
}
