#include "clineitem.h"

CLineItem::CLineItem(QLineF a_cLine, int a_iLineWidth, QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_iFrameId = 0;
    m_pLine = new QGraphicsLineItem(a_cLine, this);
    m_cPen.setBrush(QBrush(Qt::black));
    m_cPen.setWidth(a_iLineWidth);
    m_pLine->setPen(m_cPen);
}

QRectF CLineItem::boundingRect() const
{
    return m_pLine->boundingRect();
}

qreal CLineItem::Length()
{
    return m_pLine->line().length();
}

void CLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    m_pLine->paint(painter, option, widget);
    m_dFinalLen = m_pLine->line().length();
}

void CLineItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CLineItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CLineItem::SLOT_DisappearItemProc()
{
    QLineF l_cLine = m_pLine->line();
    l_cLine.setLength(l_cLine.length() - m_dFinalLen / TASKMANAGER::g_iItemDisappearFrames);
    m_pLine->setLine(l_cLine);
    update(this->boundingRect());
    this->setOpacity(this->opacity() - 0.09);
    if(++m_iFrameId >= TASKMANAGER::g_iItemDisappearFrames)
    {
        emit this->SIGNAL_AnimEnd();
    }
}
