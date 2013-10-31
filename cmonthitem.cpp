#include "cmonthitem.h"

CMonthItem::CMonthItem(QGraphicsItem *a_pParent, int a_iYear)
    :QGraphicsItem(a_pParent)
{
    m_iYear = a_iYear;
    if(m_iYear == QDate::currentDate().year())
    { //if the year is current year, current month will be shown
        m_iShowMonth = QDate::currentDate().month();
        m_iZeroMonth = m_iShowMonth - 1;
    }
    else
    { //use default setting of showing the first month
        m_iShowMonth = 1;
        m_iZeroMonth = 0;
    }
    m_iMonthBarHeight = 30;
    m_iMonthBarCellNum = 9;
    m_iDayNameBarHeight = 20;
    m_iDayTableCellHeight = 25;
    m_iDateLabelHeight = 18;

    m_CBR.setRect(0, 0, this->ItemWidth(), this->ItemHeight());

    m_CMonthFont.setFamily("Comic Sans MS");
    m_CMonthFont.setPointSize(TASKMANAGER::g_iItemFontSizeMid);

    m_CDayNameFont.setFamily("Courier New");
    m_CDayNameFont.setPointSize(TASKMANAGER::g_iItemFontSizeSmall);

    m_pDateLabel = new QGraphicsSimpleTextItem(this);
    m_pDateLabel->setPos(0, m_CBR.height() - m_iDateLabelHeight);
    m_pDateLabel->setFont(m_CDayNameFont);
    m_pDateLabel->setText(QString("- %1.%2 -").arg(m_iYear).arg(m_iShowMonth));

    for(int i=0; i<12; i++)
    {
        m_pSelMonthMask[i] = false;
        m_pSelDayMask[i] = 0;
    }
}

int CMonthItem::GetFirstDayPos(int a_iYear, int a_iMonth)
{
    QDate l_CDate(a_iYear, a_iMonth, 1);

    return l_CDate.dayOfWeek() - 1;
}

int CMonthItem::GetDaysInMonth(int a_iYear, int a_iMonth)
{
    return QDate(a_iYear, a_iMonth, 1).daysInMonth();
}

const bool* CMonthItem::GetSelMonthMask()
{
    return m_pSelMonthMask;
}

const int* CMonthItem::GetSelDayMask()
{
    return m_pSelDayMask;
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
    QPen l_COldPen = painter->pen();
    for(int i=0; i<m_iMonthBarCellNum; i++)
    {
        painter->setPen(l_COldPen); //reset pen to original setting
        l_CRect.setRect(i * m_CBR.width() / m_iMonthBarCellNum, 0, m_CBR.width() / m_iMonthBarCellNum, m_iMonthBarHeight);
        painter->drawRect(l_CRect);
        if(0 == i)
        {
            painter->drawText(l_CRect, Qt::AlignCenter, "<");
        }
        else if(m_iMonthBarCellNum - 1 == i)
        {
            painter->drawText(l_CRect, Qt::AlignCenter, ">");
        }
        else
        {
            QPen l_CPen; //create pen to render month number
            int l_iMonthId = this->MonthBarId2MonthId(i);
            if(l_iMonthId == QDate::currentDate().month() \
                    && m_iYear == QDate::currentDate().year())
            {
                l_CPen.setColor(Qt::yellow);
            }
            if(m_iYear < QDate::currentDate().year() \
                    || (m_iYear == QDate::currentDate().year() \
                        && l_iMonthId < QDate::currentDate().month()))
            {
                l_CPen.setColor(Qt::darkGray);
            }
            if(l_iMonthId == m_iShowMonth)
            {
                QFont l_CBoldFont = painter->font();
                l_CBoldFont.setBold(true);
                painter->setFont(l_CBoldFont); //bold current month
            }
            if(m_pSelMonthMask[l_iMonthId - 1])
            {
                painter->fillRect(l_CRect.x() + 2, l_CRect.y() + 2, \
                                  l_CRect.width() - 4, l_CRect.height() - 4, \
                                  Qt::cyan);
            }
            painter->setPen(l_CPen);
            painter->drawText(l_CRect, Qt::AlignCenter, QString("%1").arg(l_iMonthId));
            painter->setFont(m_CMonthFont);
        }
    }

    painter->restore();
    //draw day-name bar
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(m_CDayNameFont);
    l_CRect.setRect(0, m_iMonthBarHeight, m_CBR.width(), m_iDayNameBarHeight);
    painter->drawRect(l_CRect);
    for(int i=0; i<7; i++)
    {
        l_CRect.setRect(i * m_CBR.width() / 7, m_iMonthBarHeight, \
                        m_CBR.width() / 7, m_iDayNameBarHeight);
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

    painter->restore();

    painter->save();
    //draw day-number table
    l_CRect.setRect(0, m_iMonthBarHeight + m_iDayNameBarHeight, m_CBR.width(), \
                    m_iDayTableCellHeight * 6);
    painter->drawRect(l_CRect);

    for(int i=1; i<=5; i++)
    {
        painter->drawLine(QPointF(0, l_CRect.topLeft().y() + i * m_iDayTableCellHeight), \
                          QPointF(m_CBR.width(), l_CRect.topRight().y() + i * m_iDayTableCellHeight));
    }
    for(int i=1; i<=6; i++)
    {
        painter->drawLine(QPointF(l_CRect.topLeft().x() + i * m_CBR.width() / 7, l_CRect.topLeft().y()), \
                          QPointF(l_CRect.topLeft().x() + i * m_CBR.width() / 7, l_CRect.bottomLeft().y()));
    }

    int l_iRow = 0;
    int l_iCol = this->GetFirstDayPos(m_iYear, m_iShowMonth);
    int i = 0;
    int l_iDays = this->GetDaysInMonth(m_iYear, m_iShowMonth);
    QPen l_CPen = painter->pen();
    while(i<l_iDays)
    {
        l_CRect.setRect(l_iCol * m_CBR.width() / 7, m_iMonthBarHeight + m_iDayNameBarHeight + l_iRow * m_iDayTableCellHeight, \
                        m_CBR.width() / 7, m_iDayTableCellHeight);
        QDate l_CDate = QDate(m_iYear, m_iShowMonth, i + 1);

        if(l_CDate < QDate::currentDate())
        {
            l_CPen.setColor(Qt::darkGray);
        }
        else
        {
            l_CPen.setColor(Qt::black);
            if(l_CDate == QDate::currentDate())
            {
                painter->fillRect(l_CRect.x() + 2, l_CRect.y() + 2, l_CRect.width() - 4, l_CRect.height() - 4, \
                                  Qt::yellow);
            }
            if(m_pSelDayMask[m_iShowMonth - 1] & (1 << i))
            {
                painter->fillRect(l_CRect.x() + 2, l_CRect.y() + 2, l_CRect.width() - 4, l_CRect.height() - 4, \
                                  Qt::cyan);
            }
        }
        painter->setPen(l_CPen);
        painter->drawText(l_CRect, Qt::AlignCenter, QString("%1").arg(i + 1));

        if(++l_iCol == 7)
        {
            l_iRow++;
            l_iCol = 0;
        }
        i++;
    }

    painter->restore();

    //draw date label box
    painter->drawRect(0, m_CBR.height() - m_iDateLabelHeight, \
                      m_CBR.width(), m_iDateLabelHeight);
}

int CMonthItem::CurrMonth()
{
    return QDate::currentDate().month();
}

int CMonthItem::ItemWidth()
{
    return 270;
}

int CMonthItem::ItemHeight()
{
    return m_iMonthBarHeight + m_iDayNameBarHeight + m_iDayTableCellHeight * 6 \
            + m_iDateLabelHeight;
}

int CMonthItem::MonthBarId2MonthId(int a_iMonthBarId)
{
    int l_iMonthId = 0;
    if(a_iMonthBarId >=1 && a_iMonthBarId <=7)
    {
        l_iMonthId = a_iMonthBarId + m_iZeroMonth;
        if(l_iMonthId > 12)
        {
            l_iMonthId -= 12;
        }
    }
    return l_iMonthId;
}

void CMonthItem::SetDayMask(int a_iDay)
{
    if(m_pSelDayMask[m_iShowMonth - 1] & (1 << (a_iDay - 1)))
    {
        m_pSelDayMask[m_iShowMonth - 1] = \
                m_pSelDayMask[m_iShowMonth - 1] & ~(1 << (a_iDay - 1));
    }
    else
    {
        m_pSelDayMask[m_iShowMonth - 1] = \
                m_pSelDayMask[m_iShowMonth - 1] | (1 << (a_iDay - 1));
    }
}

void CMonthItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_CMouseLastPos = event->pos();
    }
    if(event->button() == Qt::RightButton)
    {
        m_CMouseLastPos = event->pos();
    }
}

void CMonthItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_CMouseLastPos, event->pos()).length() < \
            TASKMANAGER::g_iMouseClickDistThreshold)
    {//left button clicked
        if(m_CMouseLastPos.y() < m_iMonthBarHeight) //click month bar
        { //show month
            int l_iMonthBarId = qFloor(m_CMouseLastPos.x() / (m_CBR.width() / m_iMonthBarCellNum));
            if(l_iMonthBarId == 0)
            { //click the button of left moving month
                if(--m_iZeroMonth < 0)
                {
                    m_iZeroMonth = 11;
                }
                update(m_CBR.x(), m_CBR.y(), m_CBR.width(), m_iMonthBarHeight);
            }
            else if(l_iMonthBarId == m_iMonthBarCellNum - 1)
            { //click the button of right moving month
                if(++m_iZeroMonth > 11)
                {
                    m_iZeroMonth = 0;
                }
                update(m_CBR.x(), m_CBR.y(), m_CBR.width(), m_iMonthBarHeight);
            }
            else
            { //click month number buttons
                m_iShowMonth = this->MonthBarId2MonthId(l_iMonthBarId);
                m_pDateLabel->setText(QString("- %1.%2 -").arg(m_iYear).arg(m_iShowMonth));
                update(m_CBR);
            }
        }
        else if(m_CMouseLastPos.y() > m_iMonthBarHeight + m_iDayNameBarHeight &&\
                m_CMouseLastPos.y() < m_CBR.width() - m_iDateLabelHeight) //click days table
        {
            qreal x = m_CMouseLastPos.x();
            qreal y = m_CMouseLastPos.y() - m_iMonthBarHeight - m_iDayNameBarHeight;
            int l_iSelDayIdx = floor(y / m_iDayTableCellHeight) * 7 + \
                    ceil(x / (m_CBR.width() / 7.0));
            int l_iSelDay = l_iSelDayIdx - this->GetFirstDayPos(m_iYear, m_iShowMonth);
            if(l_iSelDay >= 1 && l_iSelDay <= this->GetDaysInMonth(m_iYear, m_iShowMonth) && \
                    QDate(m_iYear, m_iShowMonth, l_iSelDay) >= QDate::currentDate())
            {
                this->SetDayMask(l_iSelDay);
            }
#ifdef PF_TEST
            CTestBox* l_pTB = CTestBox::GetTestBox();
            l_pTB->ShowMsg(QString("[CMonthItem] Day index: %1").\
                           arg(l_iSelDayIdx).toStdWString());
            if(m_pSelDayMask[m_iShowMonth - 1] & (1 << (l_iSelDay - 1)))
            {
                l_pTB->ShowMsg(QString("[CMonthItem] Select day :%1 of month: %2")\
                               .arg(l_iSelDay).arg(m_iShowMonth).toStdWString());
            }
            else
            {
                l_pTB->ShowMsg(QString("[CMonthItem] Deselect day :%1 of month: %2")\
                               .arg(l_iSelDay).arg(m_iShowMonth).toStdWString());
            }
#endif
            update(m_CBR);
        }
    }
    if(event->button() == Qt::RightButton &&
            QLineF(m_CMouseLastPos, event->pos()).length() < \
            TASKMANAGER::g_iMouseClickDistThreshold)
    {//right button clicked
        if(m_CMouseLastPos.y() < m_iMonthBarHeight) //click month bar
        { //select month
            int l_iMonthBarId = qFloor(m_CMouseLastPos.x() / (m_CBR.width() / m_iMonthBarCellNum));
            if(l_iMonthBarId != 0 && l_iMonthBarId != m_iMonthBarCellNum - 1)
            { //click month number buttons
                int l_iSelMonth = this->MonthBarId2MonthId(l_iMonthBarId);
                if(m_iYear >= QDate::currentDate().year() &&
                        l_iSelMonth >= QDate::currentDate().month())
                {
                    m_pSelMonthMask[l_iSelMonth - 1] = !m_pSelMonthMask[l_iSelMonth - 1];
                    if(m_pSelMonthMask[l_iSelMonth - 1])
                    {
                        if(m_iYear == QDate::currentDate().year() &&
                                l_iSelMonth == QDate::currentDate().month())
                        {
                            for(int i=0; i<QDate::currentDate().daysInMonth(); i++)
                            {
                                if(i + 1 >= QDate::currentDate().day())
                                {
                                    m_pSelDayMask[l_iSelMonth - 1] = \
                                            m_pSelDayMask[l_iSelMonth - 1] | (1 << i);
                                }
                            }
                        }
                        else
                        {
                            m_pSelDayMask[l_iSelMonth - 1] = 0xFFFFFFFF;
                        }
                    }
                    else
                    {
                        m_pSelDayMask[l_iSelMonth - 1] = 0;
                    }
                    update(m_CBR);
#ifdef PF_TEST
                    if(m_pSelMonthMask[l_iSelMonth - 1])
                    {
                        CTestBox::GetTestBox()->ShowMsg(\
                                    QString("[CMonthItem] Select month: %1")\
                                    .arg(l_iSelMonth).toStdWString());
                    }
                    else
                    {
                        CTestBox::GetTestBox()->ShowMsg(\
                                    QString("[CMonthItem] Deselect month :%1")\
                                    .arg(l_iSelMonth).toStdWString());
                    }
#endif
                }
            }
        }
    }
}

void CMonthItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CMonthItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}
