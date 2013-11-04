#include "cselectitem.h"

CSelectItem::CSelectItem(QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_blSelected = false;
    m_CBR.setRect(0, 0, 18, 18);
}

bool CSelectItem::IsChecked()
{
    return m_blSelected;
}

QRectF CSelectItem::boundingRect() const
{
    return m_CBR;
}

void CSelectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    //draw outline
    painter->setPen(QPen(QBrush(Qt::black), 2));
    painter->drawRect(m_CBR);
    //draw selection box if item is selected
    if(m_blSelected)
    {
        QRectF l_CSelBox(m_CBR.x() + 3, m_CBR.y() + 3, m_CBR.width() - 6, m_CBR.height() - 6);
        painter->fillRect(l_CSelBox, Qt::SolidPattern);
    }

    painter->restore();
}

void CSelectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_CMouseLastPos = event->pos();
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void CSelectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_CMouseLastPos, event->pos()).length() < TASKMANAGER::g_iMouseClickDistThreshold)
    { //left button clicked
        m_blSelected = !m_blSelected;
        update(m_CBR);
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
