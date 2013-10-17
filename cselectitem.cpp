#include "cselectitem.h"

CSelectItem::CSelectItem(QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_blSelected = false;
    m_cBR.setRect(0, 0, 18, 18);
}

bool CSelectItem::IsChecked()
{
    return m_blSelected;
}

QRectF CSelectItem::boundingRect() const
{
    return m_cBR;
}

void CSelectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    //draw outline
    painter->setPen(QPen(QBrush(Qt::black), 2));
    painter->drawRect(m_cBR);
    //draw selection box if item is selected
    if(m_blSelected)
    {
        QRectF l_cSelBox(m_cBR.x() + 3, m_cBR.y() + 3, m_cBR.width() - 6, m_cBR.height() - 6);
        painter->fillRect(l_cSelBox, Qt::SolidPattern);
    }

    painter->restore();
}

void CSelectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_cLastPos = event->pos();
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void CSelectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_cLastPos, event->pos()).length() < TASKMANAGER::g_iMouseClickDistThreshold)
    { //left button clicked
        m_blSelected = !m_blSelected;
        update(m_cBR);
    }
}

void CSelectItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CSelectItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}
