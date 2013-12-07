#include "cplanwidget.h"

CPlanWidget::CPlanWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_pPlan = CPlan::GetPlan();

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
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 2, Qt::DotLine));
    painter->drawRect(this->boundingRect());

    painter->restore();

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    //draw plan
    this->PlanTimeHourPaint(painter);

    painter->restore();
}

void CPlanWidget::PlanTimeHourPaint(QPainter *painter)
{
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

    QList<CPlanTimeHour *>& l_pPlanTimeHourList = m_pPlan->GetPlanTimeHourList();

    for(int i=0; i<l_pPlanTimeHourList.length(); i++)
    {
        CPlanTimeHour* l_pPlanTimeHour = l_pPlanTimeHourList[i];
        if(l_pPlanTimeHour->IsEmpty())
        {
            continue;
        }
        //date tag
        painter->drawText(5, i * m_iHeightPerTimeLine, m_iDateTagWidth, m_iDateTagHeight,\
                          Qt::AlignLeft, l_pPlanTimeHour->Date().toString("yyyy.MM.dd dddd :"));
        //hour segments
        int x = 10;
        int y = i * m_iHeightPerTimeLine + m_iDateTagHeight;
        STimeSegHour* l_pTimeSegListIter = l_pPlanTimeHour->GetTimeSeg(0);
        for(int j=0; j<l_pPlanTimeHour->TimeSegCounter(); j++)
        {
            painter->setFont(l_CNewFont);
            painter->setPen(l_CNewPen);
            int l_iSegWidth = (l_pTimeSegListIter->m_iEndClock -\
                                    l_pTimeSegListIter->m_iStartClock) * m_iWidthPerHour;
            painter->drawLine(x, y + 15, x, y + 30);
            painter->drawLine(x + l_iSegWidth, y + 15,\
                              x + l_iSegWidth, y + 30);
            painter->drawLine(x, y + 20, x + l_iSegWidth, y + 20);
            painter->drawText(x, y, m_iClockTagWidth, m_iClockTagWidth,\
                              Qt::AlignLeft | Qt::AlignBottom,\
                              QString("%1").arg(l_pTimeSegListIter->m_iStartClock));
            painter->drawText(x + l_iSegWidth - m_iClockTagWidth, y,\
                              m_iClockTagWidth, m_iClockTagWidth,\
                              Qt::AlignRight | Qt::AlignBottom,\
                              QString("%1").arg(l_pTimeSegListIter->m_iEndClock));
            //draw task tag
            painter->setPen(m_CTaskTagPen);
            painter->setFont(m_CTaskTagFont);
            STaskAbstractHour* l_pTaskListIter = l_pTimeSegListIter->m_pTaskListHead;
            for(int k=0; k<l_pTimeSegListIter->m_iTaskNum; k++)
            {
                QRectF l_CTaskTagRect(x + 3 + k * 25, y + 23, 23, 12);
                painter->fillRect(l_CTaskTagRect,\
                                  l_pTaskListIter->m_EGoalColorTag);
                painter->drawText(l_CTaskTagRect, Qt::AlignCenter,\
                                  l_pTaskListIter->m_qstrTaskTag);
                l_pTaskListIter = l_pTaskListIter->m_pNextInTimeSeg;
            }

            x += l_iSegWidth + TASKMANAGER::g_iItemIntervalX;
            l_pTimeSegListIter = l_pTimeSegListIter->m_pNext;
        }

        painter->setFont(l_COldFont);
        painter->setPen(l_COldPen);
    }

    painter->restore();
}

void CPlanWidget::SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget *a_pWhoAmI)
{
    if(a_pWhoAmI->WidgetClassName() == QString("CDayWidget")\
            && this->contains(this->mapFromScene(a_CMouseScenePos)))
    { //process hours plan
        CDayWidget* l_pDayWidget = (CDayWidget *)a_pWhoAmI;
        QDate l_CDayWidgetDate = l_pDayWidget->GetDate();

        CPlanTimeHour* l_pNewPlanTimeHour = m_pPlan->GetPlanTimeHour(l_CDayWidgetDate);
        if(NULL == l_pNewPlanTimeHour)
        { //create a new hours plan for the date
            l_pNewPlanTimeHour = m_pPlan->CreatePlanTimeHour(l_CDayWidgetDate);
        }
        l_pNewPlanTimeHour->SetTimeSeg(l_pDayWidget->GetHourSelMask(),\
                                       l_pDayWidget->GetHoursPerDay());
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
