#include "cgoaltagwidget.h"

CGoalTagWidget::CGoalTagWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_iGoalId = -1;
    m_iWidth = 40;
    m_iHeight = 40;
    m_blSelected = false;
    this->InitBoundingRect();
}

void CGoalTagWidget::InitGoalData(int a_iGoalId, QString a_qstrGoalName, Qt::GlobalColor a_EColorTag)
{
    m_iGoalId = a_iGoalId;
    m_qstrGoalName = a_qstrGoalName;
    m_EColorTag = a_EColorTag;

    this->setToolTip(m_qstrGoalName);
    update(this->boundingRect());
}

void CGoalTagWidget::SetGoalName(QString a_qstrGoalName)
{
    m_qstrGoalName = a_qstrGoalName;
    this->setToolTip(m_qstrGoalName);
}

void CGoalTagWidget::SetGoalColorTag(Qt::GlobalColor a_EColorTag)
{
    m_EColorTag = a_EColorTag;
    update(this->boundingRect());
}

void CGoalTagWidget::SetWidgetSelection(bool a_blSelected)
{
    m_blSelected = a_blSelected;
    update(this->boundingRect());

    if(m_blSelected)
    {
        emit this->SIGNAL_Selected(this);
    }
}

void CGoalTagWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QBrush l_CBrush(m_EColorTag);
    QPainterPath l_CPath;
    QRectF l_CRect = this->boundingRect();
    l_CPath.addRoundedRect(l_CRect.x() + 2, l_CRect.y() + 2, l_CRect.width() - 4,\
                           l_CRect.height() - 4, 5, 5);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillPath(l_CPath, l_CBrush);

    if(m_blSelected)
    {
        painter->setPen(QPen(QBrush(Qt::black), 3));
        painter->drawRoundedRect(this->boundingRect().x() + 1,\
                                 this->boundingRect().y() + 1,\
                                 this->boundingRect().width() - 2,\
                                 this->boundingRect().width() - 2, 8, 8);
    }

    painter->restore();
}

void CGoalTagWidget::LeftButtonClicked(QPointF a_CMousePos)
{
    Q_UNUSED(a_CMousePos)

    m_blSelected = true;
    update(this->boundingRect());
    emit this->SIGNAL_Selected(this);
}

void CGoalTagWidget::SLOT_GoalUpdateProc(int a_iGoalId, QString a_qstrGoalName, Qt::GlobalColor a_EColorTag)
{
    if(m_iGoalId == a_iGoalId)
    {
        if(a_qstrGoalName != m_qstrGoalName)
        {
            this->SetGoalName(a_qstrGoalName);
        }
        if(a_EColorTag != m_EColorTag)
        {
            this->SetGoalColorTag(a_EColorTag);
        }
    }
}
