#include "cbuttonitem.h"

CButtonItem::CButtonItem(QString a_strText, QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_cBR.setRect(0, 0, 50, 24);
    m_strText = a_strText;
    m_iFrameId = 0;
}

QRectF CButtonItem::boundingRect() const
{
    return m_cBR;
}

void CButtonItem::SetButtonText(QString a_strText)
{
    m_strText = a_strText;
    update(this->boundingRect());
}

void CButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen((QBrush(Qt::black), 4)));
    //draw outline
    painter->drawRoundedRect(this->boundingRect(), 5, 5);
    //draw text
    painter->drawText(this->boundingRect(), Qt::AlignCenter, m_strText);

    painter->restore();
}

void CButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_cLastPos = event->pos();
    }
}

void CButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_cLastPos, event->pos()).length() < TASKMANAGER::g_iMouseClickDistThreshold) //left button clicked
    {
        emit this->SIGNAL_LeftButtonClicked();
    }
}

void CButtonItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CButtonItem::SLOT_DisappearItemProc()
{
    this->setPos(this->pos().x(), this->pos().y() - 5);
    this->setOpacity(this->opacity() - 0.09);
    if(++m_iFrameId >= TASKMANAGER::g_iItemDisappearFrames)
    {
        emit this->SIGNAL_AnimEnd();
    }
}

void CButtonItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}
