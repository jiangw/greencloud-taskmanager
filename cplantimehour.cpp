#include "cplantimehour.h"

CPlanTimeHour::CPlanTimeHour(QDate a_CDate)
{
    m_CDate = a_CDate;

    m_pTaskListHead = NULL;
    m_pTaskListTail = NULL;
    m_iTaskCounter = 0;
    m_pTimeSegListHead = NULL;
    m_pTimeSegListTail = NULL;
    m_iTimeSegCounter = 0;
}

CPlanTimeHour::~CPlanTimeHour()
{
    this->Clear();
}

void CPlanTimeHour::Clear()
{
    this->ClearTimeSegList();
    this->ClearTaskList();
}

void CPlanTimeHour::ClearTimeSegList()
{
    STimeSegHour* l_pNext = NULL;
    while(NULL != m_pTimeSegListHead)
    {
        l_pNext = m_pTimeSegListHead->m_pNext;
        //clear tasks assignment
        this->ClearTaskAssignment(m_pTimeSegListHead);
        delete m_pTimeSegListHead; //release memory
        m_pTimeSegListHead = l_pNext;
    }

    m_pTimeSegListTail = NULL;
    m_iTimeSegCounter = 0;
}

void CPlanTimeHour::ClearTaskList()
{
    //clear tasks list in time segments
    STimeSegHour* l_pTimeSeg = m_pTimeSegListHead;
    while(NULL != l_pTimeSeg)
    {
        l_pTimeSeg->m_iTaskNum = 0;
        l_pTimeSeg->m_pTaskListHead = NULL;
        l_pTimeSeg = l_pTimeSeg->m_pNext;
    }
    //release memory
    STaskAbstractHour* l_pNext = NULL;
    while(NULL != m_pTaskListHead)
    {
        l_pNext = m_pTaskListHead->m_pNext;
        delete m_pTaskListHead;
        m_pTaskListHead = l_pNext;
    }

    m_pTaskListTail = NULL;
    m_iTaskCounter = 0;
}

bool CPlanTimeHour::IsEmpty()
{
    if(NULL == m_pTimeSegListHead)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CPlanTimeHour::SetTimeSeg(const bool *a_pDayHourMask, int a_iHoursPerDay)
{
    //clear old time segments list
    this->ClearTimeSegList();

    bool l_blSegStart = false;
    int l_iStartClock;
    for(int i=0; i<a_iHoursPerDay; i++)
    {
        if(a_pDayHourMask[i])
        {
            if(!l_blSegStart)
            { //find the start clock of a time segment
                l_iStartClock = i;
                l_blSegStart = true;
            }
        }
        else
        {
            if(l_blSegStart)
            { //find the end clock of a time segment
                this->AddTimeSeg(l_iStartClock, i);
                l_blSegStart = false;
            }
        }
    }
    if(l_blSegStart)
    { //if the last hour of the day has been choosed, then we have to handle this
      //out of the previous loop
        this->AddTimeSeg(l_iStartClock, a_iHoursPerDay);
    }
}

STimeSegHour *CPlanTimeHour::GetTimeSeg(int a_iTimeSegIndex)
{
    int i=0;
    STimeSegHour* l_pTimeSeg = NULL;

    if(a_iTimeSegIndex <= 0)
    {
        l_pTimeSeg = m_pTimeSegListHead;
    }
    else if(a_iTimeSegIndex >= m_iTimeSegCounter - 1)
    {
        l_pTimeSeg = m_pTimeSegListTail;
    }
    else
    {
        l_pTimeSeg = m_pTimeSegListHead;
        while(l_pTimeSeg)
        {
            l_pTimeSeg = l_pTimeSeg->m_pNext;
            i++;
            if(i == a_iTimeSegIndex)
            {
                break;
            }
        }
    }
    return l_pTimeSeg;
}

bool CPlanTimeHour::ConvertTimeSegToHourMask(bool *a_pDayHourMask, int a_iHoursPerDay)
{
    if(NULL == a_pDayHourMask || NULL == m_pTimeSegListHead)
    {
        return false;
    }
    //initialize hours mask
    for(int i=0; i<a_iHoursPerDay; i++)
    {
        a_pDayHourMask[i] = false;
    }

    STimeSegHour* l_pTimeSeg = m_pTimeSegListHead;
    for(int i=0; i<m_iTimeSegCounter; i++)
    {
        for(int j=l_pTimeSeg->m_iStartClock; j<l_pTimeSeg->m_iEndClock; j++)
        {
            a_pDayHourMask[j] = true;
        }
        l_pTimeSeg = l_pTimeSeg->m_pNext;
    }

    return true;
}

void CPlanTimeHour::AddTimeSeg(int a_iStartClock, int a_iEndClock)
{
    //create new time segment
    STimeSegHour* l_pNewTimeSeg = new STimeSegHour;
    l_pNewTimeSeg->m_iStartClock = a_iStartClock;
    l_pNewTimeSeg->m_iEndClock = a_iEndClock;
    l_pNewTimeSeg->m_iTaskNum = 0;
    l_pNewTimeSeg->m_pTaskListHead = NULL;
    l_pNewTimeSeg->m_pNext = NULL;
    //add new time segment to time segments list
    if(NULL == m_pTimeSegListHead)
    {
        m_pTimeSegListHead = l_pNewTimeSeg;
    }
    else
    {
        m_pTimeSegListTail->m_pNext = l_pNewTimeSeg;
    }
    m_pTimeSegListTail = l_pNewTimeSeg;
    m_iTimeSegCounter++;
}

void CPlanTimeHour::ClearTaskAssignment(STimeSegHour *a_pTimeSeg)
{
    STaskAbstractHour* l_pTaskListHead = a_pTimeSeg->m_pTaskListHead;
    STaskAbstractHour* l_pNextTask = NULL;
    while(NULL != l_pTaskListHead)
    {
        l_pNextTask = l_pTaskListHead->m_pNextInTimeSeg;
        l_pTaskListHead->m_pNextInTimeSeg = NULL;
        l_pTaskListHead = l_pNextTask;
    }
    a_pTimeSeg->m_pTaskListHead = NULL;
    a_pTimeSeg->m_iTaskNum = 0;
}
