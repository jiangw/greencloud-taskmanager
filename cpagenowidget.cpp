#include "cpagenowidget.h"

CPageNOWidget::CPageNOWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_iWidth = 20;
    m_iHeight = 20;
    m_iPageNo = -1;
    m_blSel = false;
    this->InitBoundingRect();
}

void CPageNOWidget::SetPageNO(int a_iPageNO)
{
    m_iPageNo = a_iPageNO;
    update(this->boundingRect());
}

void CPageNOWidget::Select(bool a_blSel)
{
    m_blSel = a_blSel;
    update(this->boundingRect());
}

void CPageNOWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    if(m_blSel)
    {
        painter->drawEllipse(this->boundingRect());
    }
    painter->drawText(this->boundingRect(), Qt::AlignCenter, QString("%1").arg(m_iPageNo));

    painter->restore();
}

void CPageNOWidget::LeftButtonClicked(QPointF a_CMousePos)
{
    Q_UNUSED(a_CMousePos)
    m_blSel = true;
    update(this->boundingRect());
    emit this->SIGNAL_PageNOSelected(this);
}
