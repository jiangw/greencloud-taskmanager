#include "cplanwidget.h"

CPlanWidget::CPlanWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_iTimeSegHeight = 40;
    m_iClockTagWidth = 15;
    m_iWidthPerHour = 2 * m_iClockTagWidth;
    m_iDateTagWidth = 200;
    m_iDateTagHeight = 20;
    m_iHeightPerTimeLine = m_iTimeSegHeight + m_iDateTagHeight;
    m_CTaskTagFont.setPointSize(TASKMANAGER::g_iItemFontSizeSmall);
    m_CTaskTagFont.setFamily("Liberation Sans Narrow");
    m_CTaskTagPen.setWidth(1);

    this->InitBoundingRect(this->WidgetWidth(), this->WidgetHeight());
}

CPlanWidget::~CPlanWidget()
{
    this->Clear();
}

void CPlanWidget::Clear()
{
    for(int i=0; i<m_CDateList.length(); i++)
    {
        delete m_CDateList[i];
        this->DeleteTimeSeg(m_CTimeSegList[i]);
    }
    m_CDateList.clear();
    m_CTimeSegList.clear();
}

void CPlanWidget::DeleteTimeSeg(STimeSeg *a_pDelTimeSeg)
{
    for(int i=0; i<a_pDelTimeSeg->m_CTaskListList.length(); i++)
    {
        delete a_pDelTimeSeg->m_CTaskListList[i];
    }
    delete a_pDelTimeSeg;
}

STimeSeg *CPlanWidget::ReplaceTimeSeg(STimeSeg *a_pOld, STimeSeg *a_pNew)
{
    delete a_pOld;
    return a_pNew;
}

int CPlanWidget::WidgetWidth()
{
    return m_iWidthPerHour * 17;
}

int CPlanWidget::WidgetHeight()
{
    return 500;
}

void CPlanWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 2, Qt::DotLine));
    painter->drawRect(this->boundingRect());

    painter->restore();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    //draw time line
    QFont l_COldFont = painter->font();
    QFont l_CNewFont = l_COldFont;
    l_CNewFont.setPointSize(TASKMANAGER::g_iItemFontSizeSmall);
    QPen l_COldPen = painter->pen();
    QPen l_CNewPen = l_COldPen;
    l_CNewPen.setWidth(3);
    l_CNewPen.setCapStyle(Qt::RoundCap);

    for(int i=0; i<m_CDateList.length(); i++)
    {
        //time tag
        painter->drawText(5, i * m_iHeightPerTimeLine, m_iDateTagWidth, m_iDateTagHeight,\
                          Qt::AlignLeft, m_CDateList[i]->toString("yyyy.MM.dd dddd :"));
        //time segment
        STimeSeg* l_pTimeSeg = m_CTimeSegList[i];
        int x = 10;
        int y = i * m_iHeightPerTimeLine + m_iDateTagHeight;
        if(!l_pTimeSeg->IsEmpty())
        {
            for(int j=0; j<l_pTimeSeg->m_CStartClockList.length(); j++)
            {
                painter->setFont(l_CNewFont);
                painter->setPen(l_CNewPen);
                //draw time line
                int l_iSegWidth = (l_pTimeSeg->m_CEndClockList[j] -\
                        l_pTimeSeg->m_CStartClockList[j]) * m_iWidthPerHour;
                painter->drawLine(x, y + 15, x, y + 30);
                painter->drawLine(x + l_iSegWidth, y + 15,\
                                  x + l_iSegWidth, y + 30);
                painter->drawLine(x, y + 20, x + l_iSegWidth, y + 20);
                painter->drawText(x, y, m_iClockTagWidth, m_iClockTagWidth,\
                                  Qt::AlignLeft | Qt::AlignBottom,\
                                  QString("%1").arg(l_pTimeSeg->m_CStartClockList[j]));
                painter->drawText(x + l_iSegWidth - m_iClockTagWidth, y,\
                                  m_iClockTagWidth, m_iClockTagWidth,\
                                  Qt::AlignRight | Qt::AlignBottom,\
                                  QString("%1").arg(l_pTimeSeg->m_CEndClockList[j]));
                //draw task tag
                painter->setPen(m_CTaskTagPen);
                painter->setFont(m_CTaskTagFont);
                for(int k=0; k<l_pTimeSeg->m_CTaskListList[j]->m_qstrTaskTagList.length(); k++)
                {
                    QRectF l_CTaskTagRect(x + 3 + k * 25, y + 23, 23, 12);
                    painter->fillRect(l_CTaskTagRect,\
                                      l_pTimeSeg->m_CTaskListList[j]->m_EGoalColorTagList[k]);
                    painter->drawText(l_CTaskTagRect, Qt::AlignCenter,\
                                      l_pTimeSeg->m_CTaskListList[j]->m_qstrTaskTagList[k]);
                }

                x += l_iSegWidth + TASKMANAGER::g_iItemIntervalX;
            }
        }

        painter->setFont(l_COldFont);
        painter->setPen(l_COldPen);
    }

    painter->restore();
}

void CPlanWidget::SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget *a_pWhoAmI)
{
    if(qstrcmp(a_pWhoAmI->WidgetClassName().toAscii(), "CDayWidget") == 0\
            && this->contains(this->mapFromScene(a_CMouseScenePos)))
    { //add day time line
        CDayWidget* l_pDayWidget = (CDayWidget *)a_pWhoAmI;
        QDate l_CDayWidgetDate = l_pDayWidget->GetDate();
        int l_iIdxOfDate = -1;
        for(int i=0; i<m_CDateList.length(); i++)
        {
            if(*(m_CDateList[i]) == l_CDayWidgetDate)
            { //the date has been added to the plan, then update information of this date
                m_CTimeSegList.replace(i,\
                                       this->ReplaceTimeSeg(m_CTimeSegList[i],\
                                                            this->ConvertHourMask2TimeSeg(\
                                                                l_pDayWidget->GetHourSelMask())));
                l_iIdxOfDate = i;
                break;
            }
        }
        if(-1 == l_iIdxOfDate)
        { //a new date
            QDate* l_pNewDate = new QDate(l_pDayWidget->GetDate());
            m_CDateList.append(l_pNewDate);
            m_CTimeSegList.append(this->ConvertHourMask2TimeSeg(\
                                      l_pDayWidget->GetHourSelMask()));
        }
        update(this->boundingRect());
    }
}

void CPlanWidget::SLOT_GoalTaskRecieve(QPointF a_CMouseScenePos, QString a_qstrTaskTag,\
                                       Qt::GlobalColor a_EGoalColorTag)
{
    QPointF l_CMouseLocalPos = this->mapFromScene(a_CMouseScenePos);
    if(this->contains(l_CMouseLocalPos))
    {
#ifdef PF_TEST
        CTestBox::GetTestBox()->ShowMsg(QString("[CPlanWidget] Add Task: %1").arg(a_qstrTaskTag));
#endif
        //compute date index
        int l_iDateIdx = qFloor(l_CMouseLocalPos.y() / m_iHeightPerTimeLine);
        if(l_iDateIdx < m_CDateList.length() &&\
                fmod(l_CMouseLocalPos.y(), m_iHeightPerTimeLine) > m_iDateTagHeight)
        {
            //compute time segment index
            STimeSeg* l_pTimeSeg = m_CTimeSegList[l_iDateIdx];
            int l_iTimeSegIdx = -1;
            int l_iPosX = 10;
            for(int i=0; i<l_pTimeSeg->m_CStartClockList.length(); i++)
            {
                int l_iSegLen = m_iWidthPerHour\
                        * (l_pTimeSeg->m_CEndClockList[i] - l_pTimeSeg->m_CStartClockList[i]);
                if(l_CMouseLocalPos.x() >= l_iPosX\
                        && l_CMouseLocalPos.x() <= (l_iPosX + l_iSegLen))
                {
                    l_iTimeSegIdx = i;
                    break;
                }
                l_iPosX += l_iSegLen + TASKMANAGER::g_iItemIntervalX;
            }
            if(-1 != l_iTimeSegIdx)
            {
                l_pTimeSeg->m_CTaskListList[l_iTimeSegIdx]->Append(a_qstrTaskTag,\
                                                                   a_EGoalColorTag);
                update(QRectF(0, l_iDateIdx * m_iHeightPerTimeLine, this->boundingRect().width(),\
                              m_iHeightPerTimeLine));
            }
        }
    }
}

STimeSeg* CPlanWidget::ConvertHourMask2TimeSeg(const bool *a_pHourMask)
{
    STimeSeg* l_pTimeSeg = new STimeSeg;
    bool l_blSegStart = false;
    for(int i=0; i<24; i++)
    {
        if(a_pHourMask[i])
        {
            if(!l_blSegStart)
            {
                l_pTimeSeg->m_CStartClockList.append(i);
                l_blSegStart = true;
            }
        }
        else
        {
            if(l_blSegStart)
            {
                l_pTimeSeg->m_CEndClockList.append(i);
                STaskList* l_pNewTaskList = new STaskList;
                l_pTimeSeg->m_CTaskListList.append(l_pNewTaskList);
                l_blSegStart = false;
            }
        }
    }
    if(l_blSegStart)
    {
        l_pTimeSeg->m_CEndClockList.append(24);
    }

    return l_pTimeSeg;
}
