#include "cyearwidget.h"

CYearWidget::CYearWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    this->InitBoundingRect(this->WidgetWidth(), this->WidgetHeight());
    m_iStartYear = QDate::currentDate().year();
}

int CYearWidget::WidgetWidth()
{
    return 385;
}

int CYearWidget::WidgetHeight()
{
    return 60;
}

void CYearWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

void CYearWidget::LeftButtonClicked(QPointF a_CMousePos)
{
    if(a_CMousePos.x() <= 50)
    {//click left arrow button
        m_iStartYear--;
    }
    else if(a_CMousePos.x() >= 335)
    {//click right arrow button
        m_iStartYear++;
    }
    else
    {
        if(a_CMousePos.x() - 65 > 0 && a_CMousePos.y() <= 35)
        {
            int l_iSelYear = m_iStartYear + floor((a_CMousePos.x() - 65) / 45);
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
    update(this->boundingRect());
}
