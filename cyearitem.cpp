#include "cyearitem.h"

CYearItem::CYearItem(QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_CBR.setRect(0, 0, 385, 60);
    m_iStartYear = QDate::currentDate().year();
}

QRectF CYearItem::boundingRect() const
{
    return m_CBR;
}

void CYearItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    //draw arrow circle
    painter->drawEllipse(0, 10, 50, 50);
    painter->drawEllipse(335, 10, 50, 50);

    //draw left arrow
    QPen l_COldPen = painter->pen();
    painter->setPen(QPen(QBrush(Qt::black), 2));
    painter->drawLine(7, 35, 25, 17);
    painter->drawLine(7, 35, 25, 52);
    painter->drawLine(7, 35, 42, 35);
    //draw right arrow
    painter->drawLine(377, 35, 360, 17);
    painter->drawLine(377, 35, 360, 52);
    painter->drawLine(342, 35, 377, 35);
    painter->setPen(l_COldPen);

    //draw time line
    painter->drawLine(50, 35, 65, 35);
    int x = 65 + 6 * 45;
//    painter->drawLine(x, 35, x + 15, 35);
    l_COldPen = painter->pen();
    for(int i=0; i<6; i++)
    {
        painter->setPen(l_COldPen);
        x = 65 + i * 45;
        if(-1 != m_pSelYears.indexOf(m_iStartYear + i))
        {
            painter->setPen(QPen(QBrush(Qt::black), 3));
        }
        painter->drawLine(x, 25, x, 35);
        painter->drawLine(x, 35, x + 45, 35);
        painter->setPen(l_COldPen);
        if(m_iStartYear + i == QDate::currentDate().year())
        {
            painter->setPen(Qt::yellow);
        }
        else if(m_iStartYear + i < QDate::currentDate().year())
        {
            painter->setPen(Qt::darkGray);
        }
        else
        {
            painter->setPen(Qt::black);
        }
        painter->drawText(x, 5, 40, 25, Qt::AlignLeft, QString("%1").arg(m_iStartYear + i));
    }

    painter->restore();
}

void CYearItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if(event->button() == Qt::LeftButton)
   {
       m_CMouseLastPos = event->pos();
   }
}

void CYearItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_CMouseLastPos, event->pos()).length() < \
            TASKMANAGER::g_iMouseClickDistThreshold)
    {
        if(m_CMouseLastPos.x() <= 50)
        {//click left arrow button
            m_iStartYear++;
        }
        else if(m_CMouseLastPos.x() >= 335)
        {//click right arrow button
            m_iStartYear--;
        }
        else
        {
            if(m_CMouseLastPos.x() - 65 > 0 && m_CMouseLastPos.y() <= 35)
            {
                int l_iSelYear = m_iStartYear + floor((m_CMouseLastPos.x() - 65) / 45);
                if(l_iSelYear >= QDate::currentDate().year())
                {
                    int l_iYearIdx = m_pSelYears.indexOf(l_iSelYear);
                    if(-1 == l_iYearIdx)
                    {//the year has not been selected, then select the year
                        m_pSelYears.push_back(l_iSelYear);
                    }
                    else
                    {//the year has been selected, the deselect the year
                        m_pSelYears.removeAt(l_iYearIdx);
                    }
                }
            }
        }
        update(m_CBR);
    }
}

void CYearItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CYearItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}
