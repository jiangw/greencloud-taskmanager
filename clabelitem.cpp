#include "clabelitem.h"

CLabelItem::CLabelItem(QString a_strText, QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_iFrameId = 0;
    m_strLabel = a_strText;
    m_cFont.setFamily("Comic Sans MS");
    m_cFont.setPointSize(TASKMANAGER::g_iItemFontSizeSmall);
}

void CLabelItem::SetText(QString a_strText)
{
    this->prepareGeometryChange();
    m_strLabel = a_strText;
//    update(this->boundingRect());
}

QRectF CLabelItem::boundingRect() const
{
    QFontMetrics l_cFM(m_cFont);
    return QRectF(0, 0, l_cFM.width(m_strLabel), 30);
}

void CLabelItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    painter->setFont(m_cFont);
    painter->drawText(this->boundingRect(), m_strLabel);

    painter->restore();
}

void CLabelItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CLabelItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CLabelItem::SLOT_DisappearItemProc()
{
    this->setPos(this->pos().x(), this->pos().y() - 5);
    this->setOpacity(this->opacity() - 0.09);
    if(++m_iFrameId >= TASKMANAGER::g_iItemDisappearFrames)
    {
        emit this->SIGNAL_AnimEnd();
    }
}
