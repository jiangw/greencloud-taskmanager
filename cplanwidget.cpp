#include "cplanwidget.h"

CPlanWidget::CPlanWidget(CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_pPlan = CPlan::GetPlan();

    m_iTimeSegHeight = 50;
    m_iClockTagWidth = 15;
    m_iWidthPerHour = 2 * m_iClockTagWidth;
    m_iDateTagWidth = 190;
    m_iDateTagHeight = 15;
    m_iTimeLineStartY = 20;
    m_iHeightPerTimeLine = m_iTimeSegHeight + m_iDateTagHeight;
    m_CTaskTagFont.setPointSize(GREENSCHEDULE::g_iItemFontSizeSmall);
    m_CTaskTagFont.setFamily("Liberation Sans Narrow");
    m_CTaskTagPen.setWidth(1);
    m_CBackground.setTextureImage(QImage(":/img/planback"));

    m_ETimePage = GREENSCHEDULE::INPROGRESS;
    m_EPlanPageFlag = PPF_END;
    m_iDaysPerPage = 7;
    m_iTotalPages = 0;
    m_iPageNo = 1;
    m_pPageList = new CWidgetList(this);
    m_pPageList->SetWidgetOutline(false);
    m_pPageList->SetListOrientation(CWidgetList::HORIZONTAL);
    m_pPageList->SetPageLength(6);
    m_pPageListTitle = new CTextWidget(false, m_pPageList);
    m_pPageListTitle->SetText("  Pages");
    m_pPageListTitle->SetWidgetOutline(false);
    m_pPageListTitle->SetWidgetUnderline(false);
    m_pPageListTitle->SetFontSize(8);
    m_pPageListTitle->SetTextColor(CGraphicsWidget::indigo);

    m_pPageList->SetHeaderWidget(m_pPageListTitle);
    m_pPageList->setPos(10, 485);
    m_pCurrPage = NULL;

    this->InitBoundingRect(this->WidgetWidth(), this->WidgetHeight());

    QList<CPlanTimeHour *>& l_pPlanTimeHourList = m_pPlan->GetPlanTimeHourList(m_ETimePage);
    m_iTotalPages = qCeil((qreal)l_pPlanTimeHourList.length() / (qreal)m_iDaysPerPage);
    if(m_iTotalPages <= 0)
    {
        m_iTotalPages = 1;
    }
    m_pPageListTitle->SetText(QString("%1 Page(s) In Progress").arg(m_iTotalPages));

}

CPlanWidget::~CPlanWidget()
{
    this->Clear();
}

void CPlanWidget::Clear()
{
}

void CPlanWidget::ResetWidget()
{
    this->Clear();
    update(this->boundingRect());
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

void CPlanWidget::SetTimePage(GREENSCHEDULE::ETimePage a_ETimePage)
{
    if(m_ETimePage != a_ETimePage)
    {
        m_ETimePage = a_ETimePage;
        QString l_qstrTimePage = "In Progress";
        if(GREENSCHEDULE::HISTORY == m_ETimePage)
        {
            QList<CPlanTimeHour *>& l_pPlanTimeHourList = m_pPlan->GetPlanTimeHourList(GREENSCHEDULE::HISTORY);
            m_iTotalPages = qCeil((qreal)l_pPlanTimeHourList.length() / (qreal)m_iDaysPerPage);
            l_qstrTimePage = "In History";
        }
        else
        {
            QList<CPlanTimeHour *>& l_pPlanTimeHourList = m_pPlan->GetPlanTimeHourList(GREENSCHEDULE::INPROGRESS);
            m_iTotalPages = qCeil((qreal)l_pPlanTimeHourList.length() / (qreal)m_iDaysPerPage);
        }

        m_pPageListTitle->SetText(QString("%1 Page(s) %2").arg(m_iTotalPages).arg(l_qstrTimePage));

        update(this->boundingRect());
    }
}

void CPlanWidget::SetPlanPage(int a_iPageNO)
{
    m_iPageNo = a_iPageNO;
    m_EPlanPageFlag = PPF_SPECIFY;
    update(this->boundingRect());
}

int CPlanWidget::WidgetWidth()
{
    return m_iWidthPerHour * 17;
}

int CPlanWidget::WidgetHeight()
{
    return 550;
}

void CPlanWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillRect(this->boundingRect(), m_CBackground);
//    painter->setPen(QPen(QBrush(Qt::black), 2, Qt::DotLine));
//    painter->drawRect(this->boundingRect());

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

    QFontMetrics l_CFM(m_CTaskTagFont);
    //draw plan line
    QFont l_COldFont = painter->font();
    QFont l_CNewFont = l_COldFont;
    l_CNewFont.setPointSize(GREENSCHEDULE::g_iItemFontSizeSmall);
    QPen l_COldPen = painter->pen();
    QPen l_CPenInProgress = l_COldPen;
    l_CPenInProgress.setWidth(3);
    l_CPenInProgress.setCapStyle(Qt::RoundCap);
    QPen l_CPenHistory = l_CPenInProgress;
    l_CPenHistory.setColor(QColor(Qt::darkGray));

    l_COldPen.setColor(QColor(Qt::black));
    if(GREENSCHEDULE::HISTORY == m_ETimePage)
    {
        l_COldPen.setColor(QColor(CGraphicsWidget::gray));
    }
    l_COldFont.setFamily("STLiti");
    l_COldFont.setBold(true);

    QList<CPlanTimeHour *>& l_pPlanTimeHourList = m_pPlan->GetPlanTimeHourList(m_ETimePage);
    m_iTotalPages = qCeil((qreal)l_pPlanTimeHourList.length() / (qreal)m_iDaysPerPage);
    if(m_iTotalPages <= 0)
    {
        m_iTotalPages = 1;
    }
    switch(m_EPlanPageFlag)
    {
    case PPF_BEGIN:
        m_iPageNo = 1;
        break;
    case PPF_END:
        m_iPageNo = m_iTotalPages;
        break;
    default:
        break;
    }

    int l_iBase = (m_iPageNo - 1) * m_iDaysPerPage;
    for(int i=0; i<m_iDaysPerPage; i++)
    {
        if(i + l_iBase >= l_pPlanTimeHourList.length() || l_iBase < 0)
        {
            break;
        }
        painter->setFont(l_COldFont);
        painter->setPen(l_COldPen);

        CPlanTimeHour* l_pPlanTimeHour = l_pPlanTimeHourList[i + l_iBase];
        //date tag
        if(QDate::currentDate() == l_pPlanTimeHour->Date())
        {
            QPen l_CTmpPen = l_COldPen;
            l_CTmpPen.setColor(Qt::yellow);
            painter->setPen(l_CTmpPen);
        }
        painter->drawText(5, i * m_iHeightPerTimeLine + m_iTimeLineStartY, m_iDateTagWidth, m_iDateTagHeight,\
                          Qt::AlignLeft, l_pPlanTimeHour->Date().toString("- yyyy.MM.dd dddd -"));
        if(QDate::currentDate() == l_pPlanTimeHour->Date())
        {
            painter->setPen(l_COldPen);
        }

        //hour segments
        int x = 10;
        int y = i * m_iHeightPerTimeLine + m_iTimeLineStartY + m_iDateTagHeight;
        STimeSegHour* l_pTimeSegListIter = l_pPlanTimeHour->GetTimeSeg(0);
        for(int j=0; j<l_pPlanTimeHour->TimeSegCounter(); j++)
        {
            if(GREENSCHEDULE::HISTORY == l_pTimeSegListIter->m_ETimePage)
            {
                painter->setPen(l_CPenHistory);
            }
            else
            {
                painter->setPen(l_CPenInProgress);
            }
            painter->setFont(l_CNewFont);

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
            QList<STaskAbstractHour*> l_CTaskList = l_pTimeSegListIter->m_CTaskList;
            int l_iPrevTagsWidth = 0;
            for(int k=0; k<l_CTaskList.length(); k++)
            {
                QRectF l_CTaskTagRect(x + 3 + l_iPrevTagsWidth, y + 23,\
                                      l_CFM.boundingRect(l_CTaskList[k]->m_qstrTaskTag).width() + 2,\
                                      12);
                painter->fillRect(l_CTaskTagRect,\
                                  l_CTaskList[k]->m_EGoalColorTag);
                painter->drawText(l_CTaskTagRect, Qt::AlignCenter,\
                                  l_CTaskList[k]->m_qstrTaskTag);
                if(l_CTaskList[k]->m_blIsFinished)
                {
                    painter->drawLine(l_CTaskTagRect.topLeft().x(),\
                                      l_CTaskTagRect.topLeft().y() + 6,\
                                      l_CTaskTagRect.bottomRight().x(),\
                                      l_CTaskTagRect.topLeft().y() + 6);
                }
                l_iPrevTagsWidth += l_CTaskTagRect.width() + 2;
            }

            x += l_iSegWidth + GREENSCHEDULE::g_iItemIntervalX;
            l_pTimeSegListIter = l_pTimeSegListIter->m_pNext;
        }
    }
    if(m_iTotalPages != m_pPageList->ListLength())
    {
        m_pPageList->ResetWidget();
        for(int i=0; i<m_iTotalPages; i++)
        {
            CPageNOWidget* l_pNewPageNO = new CPageNOWidget(m_pPageList);
            l_pNewPageNO->SetPageNO(i+1);
            m_pPageList->AddWidget(l_pNewPageNO);
            connect(l_pNewPageNO, SIGNAL(SIGNAL_PageNOSelected(CPageNOWidget*)),
                    this, SLOT(SLOT_SetPageNOProc(CPageNOWidget*)));
            if(PPF_BEGIN == m_EPlanPageFlag && 0 == i)
            {
                m_pCurrPage = l_pNewPageNO;
                m_pCurrPage->Select(true);
            }
            if(PPF_END == m_EPlanPageFlag && m_iTotalPages == i + 1)
            {
                m_pCurrPage = l_pNewPageNO;
                m_pCurrPage->Select(true);
            }
            if(PPF_SPECIFY == m_EPlanPageFlag && m_iPageNo == i + 1)
            {
                m_pCurrPage = l_pNewPageNO;
                m_pCurrPage->Select(true);
            }
        }
    }

    painter->restore();
}

void CPlanWidget::SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget *a_pWhoAmI)
{
    if(GREENSCHEDULE::HISTORY == m_ETimePage)
    {
        this->SIGNAL_ShowInMessageBox("You cannot do anything in HISTORY view.");
        return;
    }
    if(a_pWhoAmI->WidgetClassName() == QString("CDayWidget")\
            && this->contains(this->mapFromScene(a_CMouseScenePos)))
    { //process hours plan
        CDayWidget* l_pDayWidget = (CDayWidget *)a_pWhoAmI;
        QDate l_CDayWidgetDate = l_pDayWidget->GetDate();
        int l_iFlag = 0;

        CPlanTimeHour* l_pNewPlanTimeHour = m_pPlan->GetPlanTimeHour(l_CDayWidgetDate);
        if(NULL == l_pNewPlanTimeHour)
        { //create a new hours plan for the date
            l_pNewPlanTimeHour = m_pPlan->CreatePlanTimeHour(l_CDayWidgetDate);
            l_iFlag = 1;
        }
        m_pPlan->UpdatePlanTimeHour(l_pNewPlanTimeHour,\
                                    l_pDayWidget->GetHourSelMask(),\
                                    l_pDayWidget->GetHoursPerDay());
        if(l_pNewPlanTimeHour->IsEmpty())
        {
            m_pPlan->RemovePlanTimeHour(l_pNewPlanTimeHour);
            l_iFlag = -1;
        }

        int l_iTotalPages = qCeil((qreal)m_pPlan->GetPlanTimeHourList(GREENSCHEDULE::INPROGRESS).length()\
                                  / (qreal)m_iDaysPerPage);
        if(l_iTotalPages > m_iTotalPages) //add a item and total-pages change
        {
            int l_iNewPlanTimeHourIdx = m_pPlan->GetPlanTimeHourIndex(l_CDayWidgetDate);
            m_iPageNo = qCeil((qreal)(l_iNewPlanTimeHourIdx + 1) / (qreal)m_iDaysPerPage);
            m_EPlanPageFlag = PPF_SPECIFY;
        }
        else if(l_iTotalPages < m_iTotalPages) //delete a item and total-pages change
        {
            if(m_iPageNo == m_iTotalPages)
            {
                m_EPlanPageFlag = PPF_END;
            }
        }
        else
        {
            if(1 == l_iFlag)
            {
                int l_iNewPlanTimeHourIdx = m_pPlan->GetPlanTimeHourIndex(l_CDayWidgetDate);
                int l_iPageNo = qCeil((qreal)(l_iNewPlanTimeHourIdx + 1) / (qreal)m_iDaysPerPage);
                if(l_iPageNo != m_iPageNo)
                {
                    m_iPageNo = l_iPageNo;
                    CWidgetNode* l_pWidgetIter = m_pPageList->GetWidgetList();
                    while(l_pWidgetIter)
                    {
                        CPageNOWidget* l_pPageNOWidget = (CPageNOWidget *)(l_pWidgetIter->m_pWidget);
                        if(l_pPageNOWidget->GetPageNO() == m_iPageNo)
                        {
                            l_pPageNOWidget->Select(true);
                            this->SLOT_SetPageNOProc(l_pPageNOWidget);
                        }
                        l_pWidgetIter = l_pWidgetIter->m_pNext;
                    }
                }
            }
        }
        update(this->boundingRect());
    }
}

void CPlanWidget::SLOT_GoalTaskRecieve(QPointF a_CMouseScenePos, int a_iGoalId, int a_iTaskId)
{
    if(GREENSCHEDULE::HISTORY == m_ETimePage)
    {
        this->SIGNAL_ShowInMessageBox("You cannot do anything in HISTORY view.");
        return;
    }
    int l_iCurrentPageLen;
    if(m_iPageNo * m_iDaysPerPage <= (m_pPlan->GetPlanTimeHourList(GREENSCHEDULE::INPROGRESS)).length())
    {
        l_iCurrentPageLen = m_iDaysPerPage;
    }
    else
    {
        l_iCurrentPageLen = (m_pPlan->GetPlanTimeHourList(GREENSCHEDULE::INPROGRESS)).length() % m_iDaysPerPage;
    }
    QPointF l_CMouseLocalPos = this->mapFromScene(a_CMouseScenePos);
    if(this->contains(l_CMouseLocalPos))
    {
        //compute date index
        int l_iDateIdx = qFloor((l_CMouseLocalPos.y() - m_iTimeLineStartY) / m_iHeightPerTimeLine);
        if(l_iDateIdx < l_iCurrentPageLen &&\
                fmod(l_CMouseLocalPos.y() - m_iTimeLineStartY, m_iHeightPerTimeLine) > m_iDateTagHeight)
        {
            //compute time segment index
            CPlanTimeHour* l_pPlanTimeHour = m_pPlan->GetPlanTimeHourList(GREENSCHEDULE::INPROGRESS)[l_iDateIdx];
            STimeSegHour* l_pTimeSegHourAssign = NULL;
            int l_iPosX = 10;
            STimeSegHour* l_pTimeSegHour = l_pPlanTimeHour->GetTimeSeg(0);
            for(int i=0; i<l_pPlanTimeHour->TimeSegCounter(); i++)
            {
                int l_iSegLen = m_iWidthPerHour\
                        * (l_pTimeSegHour->m_iEndClock - l_pTimeSegHour->m_iStartClock);
                if(l_CMouseLocalPos.x() >= l_iPosX\
                        && l_CMouseLocalPos.x() <= (l_iPosX + l_iSegLen))
                {
                    l_pTimeSegHourAssign = l_pTimeSegHour;
                    break;
                }
                l_iPosX += l_iSegLen + GREENSCHEDULE::g_iItemIntervalX;

                l_pTimeSegHour = l_pTimeSegHour->m_pNext;
            }
            if(NULL != l_pTimeSegHourAssign)
            {
                CPlanGoal* l_pPlanGoal = m_pPlan->GetPlanGoalById(a_iGoalId);
                const STask* l_pPlanGoalTask = l_pPlanGoal->GetTaskById(a_iTaskId);
                if(GREENSCHEDULE::INPROGRESS == l_pTimeSegHourAssign->m_ETimePage)
                {
                    l_pPlanTimeHour->AssignTaskToTimeSeg(l_pTimeSegHourAssign, a_iGoalId,\
                                                         l_pPlanGoal->GetGoalName(),\
                                                         l_pPlanGoalTask->m_iTaskId,\
                                                         l_pPlanGoalTask->m_qstrTaskTag,\
                                                         l_pPlanGoal->GetGoalColor());

                    update(QRectF(0, l_iDateIdx * m_iHeightPerTimeLine + m_iTimeLineStartY, this->boundingRect().width(),\
                                  m_iHeightPerTimeLine));
                    emit this->SIGNAL_PlanChanged();
                }
                else
                {
                    emit this->SIGNAL_ShowInMessageBox("Tasks cannot be assigned to historical time.");
                }
            }
        }
    }
}

void CPlanWidget::SLOT_WidgetUpdateProc()
{
    update(this->boundingRect());
}

void CPlanWidget::SLOT_SetPageNOProc(CPageNOWidget *a_pSelPage)
{

    if(NULL != m_pCurrPage)
    {
        if(m_pCurrPage != a_pSelPage)
        {
            m_pCurrPage->Select(false);
            m_pCurrPage = a_pSelPage;
            this->SetPlanPage(m_pCurrPage->GetPageNO());
        }

    }
}

void CPlanWidget::SLOT_ShowPlanHistoryProc()
{
    m_EPlanPageFlag = PPF_END;
    this->SetTimePage(GREENSCHEDULE::HISTORY);
}

void CPlanWidget::SLOT_ShowPlanInProgressProc()
{
    if(GREENSCHEDULE::HISTORY == m_ETimePage)
    {
        m_EPlanPageFlag = PPF_BEGIN;
    }
    this->SetTimePage(GREENSCHEDULE::INPROGRESS);
}
