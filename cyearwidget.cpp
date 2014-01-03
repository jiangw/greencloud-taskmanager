#include "cyearwidget.h"

CYearWidget::CYearWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_iStartYear = QDate::currentDate().year();
    m_iDurationLength = 6;
    m_iLineLenPerYear = 65;
    m_blMultiSel = true;

    this->InitBoundingRect();
}

void CYearWidget::SetDurationLength(int a_iYearNumber)
{
    m_iDurationLength = a_iYearNumber;
    this->UpdateBoundingRect();
}

void CYearWidget::EnableMultiSelection(bool a_blMultiSel)
{
    m_blMultiSel = a_blMultiSel;
}

int CYearWidget::WidgetWidth()
{
    return 115 + m_iLineLenPerYear * m_iDurationLength;
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
    painter->drawEllipse(this->boundingRect().width() - 50, 10, 50, 50);

    //draw left arrow
    QPen l_COldPen = painter->pen();
    painter->setPen(QPen(QBrush(Qt::black), 2));
    painter->drawLine(7, 35, 25, 17);
    painter->drawLine(7, 35, 25, 52);
    painter->drawLine(7, 35, 42, 35);
    //draw right arrow
    painter->drawLine(this->boundingRect().width() - 8, 35,\
                      this->boundingRect().width() - 25, 17);
    painter->drawLine(this->boundingRect().width() - 8, 35,\
                      this->boundingRect().width() - 25, 52);
    painter->drawLine(this->boundingRect().width() - 43, 35,\
                      this->boundingRect().width() - 8, 35);
    painter->setPen(l_COldPen);

    //draw time line
    painter->drawLine(50, 35, 65, 35);
    int x = 65;
//    painter->drawLine(x, 35, x + 15, 35);
    l_COldPen = painter->pen();
    for(int i=0; i<m_iDurationLength; i++)
    {
        painter->setPen(l_COldPen);
        x = 65 + i * m_iLineLenPerYear;
        if(-1 != m_pSelYears.indexOf(m_iStartYear + i))
        {
            painter->setPen(QPen(QBrush(Qt::black), 3));
        }
        painter->drawLine(x, 25, x, 35);
        painter->drawLine(x, 35, x + m_iLineLenPerYear, 35);
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
    else if(a_CMousePos.x() >= this->boundingRect().width() - 50)
    {//click right arrow button
        m_iStartYear++;
    }
    else
    {
        if(a_CMousePos.x() - 65 > 0 && a_CMousePos.y() <= 35)
        {
            int l_iSelYear = m_iStartYear + floor((a_CMousePos.x() - 65) / m_iLineLenPerYear);
            if(l_iSelYear >= QDate::currentDate().year())
            {
                if(!m_blMultiSel)
                {
                    m_pSelYears.clear();
                }
                int l_iYearIdx = m_pSelYears.indexOf(l_iSelYear);
                if(-1 == l_iYearIdx)
                {//the year has not been selected, then select the year
                    m_pSelYears.push_back(l_iSelYear);
                    emit this->SIGNAL_YearSelected(l_iSelYear);
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
