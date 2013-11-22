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

void CPlanWidget::ResetWidget()
{
    this->Clear();
    update(this->boundingRect());
}

void CPlanWidget::DeleteTimeSeg(STimeSeg *a_pDelTimeSeg)
{
    for(int i=0; i<a_pDelTimeSeg->m_CTaskListList.length(); i++)
    {
        delete a_pDelTimeSeg->m_CTaskListList[i];
    }
    delete a_pDelTimeSeg;
}

void CPlanWidget::RenderToImg(QImage *a_pImg)
{
    if(NULL != a_pImg)
    {
        QPainter l_CPainter(a_pImg);
        this->paint(&l_CPainter, NULL, NULL);
    }
}

void CPlanWidget::RenderToSvg(QSvgGenerator* a_pSVG)
{
    if(NULL != a_pSVG)
    {
        QPainter l_CPainter;
        l_CPainter.begin(a_pSVG);
        this->paint(&l_CPainter, NULL, NULL);
        l_CPainter.end();
    }
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

    //draw plan line
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
            //insert new date by the order of time
            int l_iDatePos = -1;
            for(int i=0; i<m_CDateList.length(); i++)
            {
                if((*l_pNewDate) < (*m_CDateList[i]))
                {
                    l_iDatePos = i;
                    break;
                }
            }
            if(-1 != l_iDatePos)
            {
                m_CDateList.insert(l_iDatePos, l_pNewDate);
                m_CTimeSegList.insert(l_iDatePos, this->ConvertHourMask2TimeSeg(\
                                                        l_pDayWidget->GetHourSelMask()));
            }
            else
            {
                m_CDateList.append(l_pNewDate);
                m_CTimeSegList.append(this->ConvertHourMask2TimeSeg(\
                                          l_pDayWidget->GetHourSelMask()));
            }
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

void CPlanWidget::SLOT_HourSelMaskRequestProc(QDate a_CDate, int a_iHoursPerDay)
{
    STimeSeg* l_pTimeSeg = NULL;
    for(int i=0; i<m_CDateList.length(); i++)
    {
        if(a_CDate == *(m_CDateList[i]))
        {
            l_pTimeSeg = m_CTimeSegList[i];
            break;
        }
    }
    bool* l_pHourSelMask = this->ConvertTimeSeg2HourMask(l_pTimeSeg, a_iHoursPerDay);
    bool l_blFeedback = true;
    if(NULL == l_pHourSelMask)
    {
        l_blFeedback = false;
    }

    emit this->SIGNAL_HourSelMaskSend(l_pHourSelMask, l_blFeedback);
}

void CPlanWidget::SLOT_HourSelMaskRecieveFeedbackProc(bool *a_pHourSelMask)
{
    delete [] a_pHourSelMask;
}

STimeSeg* CPlanWidget::ConvertHourMask2TimeSeg(const bool *a_pHourMask, int a_iHoursPerDay)
{
    STimeSeg* l_pTimeSeg = new STimeSeg;
    bool l_blSegStart = false;
    for(int i=0; i<a_iHoursPerDay; i++)
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
        l_pTimeSeg->m_CEndClockList.append(a_iHoursPerDay);
    }

    return l_pTimeSeg;
}

bool* CPlanWidget::ConvertTimeSeg2HourMask(STimeSeg *a_pTimeSeg, int a_iHoursPerDay)
{
    bool* l_pHourMask = NULL;
    if(NULL != a_pTimeSeg && !a_pTimeSeg->IsEmpty())
    {
        l_pHourMask = new bool[a_iHoursPerDay];
        for(int i=0; i<a_iHoursPerDay; i++)
        {
            l_pHourMask[i] = false;
        }

        for(int i=0; i<a_pTimeSeg->m_CStartClockList.length(); i++)
        {
            for(int j=a_pTimeSeg->m_CStartClockList[i]; j<a_pTimeSeg->m_CEndClockList[i];\
                j++)
            {
                l_pHourMask[j] = true;
            }
        }
    }

    return l_pHourMask;
}
