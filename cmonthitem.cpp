#include "cmonthitem.h"

CMonthItem::CMonthItem(QGraphicsItem *a_pParent, int a_iYear)
    :QGraphicsItem(a_pParent)
{
    m_iCurrYear = a_iYear;
    m_iMonthBarHeight = 30;
    m_iDayNameBarHeight = 20;
    m_iDayTableCellHeight = 25;
    m_iStartMonth = 0;
    m_CBR.setRect(0, 0, this->ItemWidth(), this->ItemHeight());

    m_CMonthFont.setFamily("Comic Sans MS");
    m_CMonthFont.setPointSize(TASKMANAGER::g_iItemFontSizeMid);

    m_CDayNameFont.setFamily("Courier New");
    m_CDayNameFont.setPointSize(TASKMANAGER::g_iItemFontSizeSmall);
}

QRectF CMonthItem::boundingRect() const
{
    return m_CBR;
}

void CMonthItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF l_CRect;
    //draw month bar
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(m_CMonthFont);
    for(int i=0; i<9; i++)
    {
        l_CRect.setRect(i * m_CBR.width() / 9, 0, m_CBR.width() / 9, m_iMonthBarHeight);
        painter->drawRect(l_CRect);
        if(0 == i)
        {
            painter->drawText(l_CRect, Qt::AlignCenter, "<");
        }
        else if(8 == i)
        {
            painter->drawText(l_CRect, Qt::AlignCenter, ">");
        }
        else
        {
            painter->drawText(l_CRect, Qt::AlignCenter, QString("%1").arg(i + m_iStartMonth));
        }
    }

    painter->restore();
    //draw day-name bar
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(m_CDayNameFont);
    l_CRect.setRect(0, m_iMonthBarHeight, this->ItemWidth(), m_iDayNameBarHeight);
    painter->drawRect(l_CRect);
    for(int i=0; i<7; i++)
    {
        l_CRect.setRect(i * this->ItemWidth() / 7, m_iMonthBarHeight, \
                        this->ItemWidth() / 7, m_iDayNameBarHeight);
        QString l_strDayName;
        if(0 < i) // draw seperator lines
        {
            painter->drawLine(l_CRect.topLeft(), l_CRect.bottomLeft());
        }
        switch(i)
        {
        case 0:
            l_strDayName = "Mon";
            break;
        case 1:
            l_strDayName = "Tue";
            break;
        case 2:
            l_strDayName = "Wed";
            break;
        case 3:
            l_strDayName = "Thu";
            break;
        case 4:
            l_strDayName = "Fri";
            break;
        case 5:
            l_strDayName = "Sat";
            break;
        case 6:
            l_strDayName = "Sun";
            break;
        default:
            l_strDayName = "?";
            break;
        }

        painter->drawText(l_CRect, Qt::AlignCenter, l_strDayName);
    }

    //draw day-number bar
    l_CRect.setRect(0, m_iMonthBarHeight + m_iDayNameBarHeight, this->ItemWidth(), \
                    m_iDayTableCellHeight * 6);
    painter->drawRect(l_CRect);

    for(int i=1; i<=5; i++)
    {
        painter->drawLine(QPointF(0, l_CRect.topLeft().y() + i * m_iDayTableCellHeight), \
                          QPointF(this->ItemWidth(), l_CRect.topRight().y() + i * m_iDayTableCellHeight));
    }
    for(int i=1; i<=6; i++)
    {
        painter->drawLine(QPointF(l_CRect.topLeft().x() + i * this->ItemWidth() / 7, l_CRect.topLeft().y()), \
                          QPointF(l_CRect.topLeft().x() + i * this->ItemWidth() / 7, l_CRect.bottomLeft().y()));
    }

    painter->restore();
}

int CMonthItem::CurrYear()
{
    return QDate::currentDate().year();
}

int CMonthItem::ItemWidth()
{
    return 270;
}

int CMonthItem::ItemHeight()
{
    return m_iMonthBarHeight + m_iDayNameBarHeight + m_iDayTableCellHeight * 6;
}

void CMonthItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CMonthItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}
