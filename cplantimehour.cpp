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

void CPlanTimeHour::ClearTimeSegList(STimeSegHour *a_pTimeSegListIter)
{
    STimeSegHour* l_pNext = NULL;
    while(NULL != a_pTimeSegListIter)
    {
        l_pNext = a_pTimeSegListIter->m_pNext;
        this->ClearTaskAssignment(a_pTimeSegListIter);
        delete a_pTimeSegListIter;
        a_pTimeSegListIter = l_pNext;
    }
}

STimeSegHour* CPlanTimeHour::BackupTimeSegList()
{
    STimeSegHour* l_pTimeSegListHeadBack = m_pTimeSegListHead;
    m_pTimeSegListHead = NULL;
    m_pTimeSegListTail = NULL;
    m_iTimeSegCounter = 0;

    return l_pTimeSegListHeadBack;
}

void CPlanTimeHour::ClearTaskList()
{
    //clear tasks list in time segments
    STimeSegHour* l_pTimeSeg = m_pTimeSegListHead;
    while(NULL != l_pTimeSeg)
    {
        l_pTimeSeg->m_CTaskList.clear();
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

bool CPlanTimeHour::IsHistory()
{
    STimeSegHour* l_pTimeSeg = m_pTimeSegListHead;
    bool l_blIsHistory = true;
    while(l_pTimeSeg)
    {
        if(GREENSCHEDULE::INPROGRESS == l_pTimeSeg->m_ETimePage)
        {
            l_blIsHistory = false;
            break;
        }
        l_pTimeSeg = l_pTimeSeg->m_pNext;
    }
    return l_blIsHistory;
}

void CPlanTimeHour::SetTimeSeg(const bool *a_pDayHourMask, int a_iHoursPerDay)
{
    //backup old time segments list
    STimeSegHour* l_pOldTimeSegListHead = this->BackupTimeSegList();

    bool l_blSegStart = false;
    int l_iStartClock = 0;
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

    //preserve task-list for unmodified time segments
    STimeSegHour* l_pNewTimeSegListIter = m_pTimeSegListHead;
    while(l_pNewTimeSegListIter)
    {
        STimeSegHour* l_pOldTimeSegListIter = l_pOldTimeSegListHead;
        while(l_pOldTimeSegListIter)
        {
            if(l_pOldTimeSegListIter->m_iStartClock == l_pNewTimeSegListIter->m_iStartClock\
                    && l_pOldTimeSegListIter->m_iEndClock == l_pNewTimeSegListIter->m_iEndClock)
            {
                l_pNewTimeSegListIter->m_CTaskList.append(l_pOldTimeSegListIter->m_CTaskList);
                break;
            }
            l_pOldTimeSegListIter = l_pOldTimeSegListIter->m_pNext;
        }

        l_pNewTimeSegListIter = l_pNewTimeSegListIter->m_pNext;
    }
    //clear old time segments list
    this->ClearTimeSegList(l_pOldTimeSegListHead);
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

STimeSegHour* CPlanTimeHour::CreateTimeSeg(int a_iStartClock, int a_iEndClock)
{
    return this->AddTimeSeg(a_iStartClock, a_iEndClock);
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

void CPlanTimeHour::UpdateTimeSegPage()
{
    QDateTime l_CNow = QDateTime::currentDateTime();
    STimeSegHour* l_pTimeSegListHead = m_pTimeSegListHead;
    while(l_pTimeSegListHead)
    {
        int h, m;
        if(24 == l_pTimeSegListHead->m_iEndClock)
        {
            h = 23;
            m = 59;
        }
        else
        {
            h = l_pTimeSegListHead->m_iEndClock;
            m = 0;
        }
        QDateTime l_CTime(m_CDate, QTime(h, m));
        if(l_CTime <= l_CNow)
        {
            l_pTimeSegListHead->m_ETimePage = GREENSCHEDULE::HISTORY;
        }
        l_pTimeSegListHead = l_pTimeSegListHead->m_pNext;
    }
}

void CPlanTimeHour::AssignTaskToTimeSeg(STimeSegHour *a_pTimeSegHour, STaskAbstractHour *a_pTaskAbs)
{
    this->AddTaskAbstractToLocalList(a_pTimeSegHour, a_pTaskAbs);
}

void CPlanTimeHour::AssignTaskToTimeSeg(STimeSegHour *a_pTimeSegHour, QString a_qstrGoalName,\
                                        QString a_qstrTaskTag, Qt::GlobalColor a_EColorTag)
{
    //Add new task abstract
    STaskAbstractHour* l_pNewTaskAbstract = this->HasTaskAbstract(a_qstrGoalName,\
                                                                  a_qstrTaskTag);
    if(NULL == l_pNewTaskAbstract)
    {
        l_pNewTaskAbstract = this->CreateTaskAbstract(a_qstrGoalName,\
                                                      a_qstrTaskTag,\
                                                      a_EColorTag);
    }
    //add task abstract to local task-list of the time segment
    this->AddTaskAbstractToLocalList(a_pTimeSegHour, l_pNewTaskAbstract);
}

STaskAbstractHour* CPlanTimeHour::HasTaskAbstract(QString a_qstrGoalName, QString a_qstrTaskTag)
{
    STaskAbstractHour* l_pTask = m_pTaskListHead;
    while(l_pTask)
    {
        if(l_pTask->m_qstrGoalName == a_qstrGoalName\
                && l_pTask->m_qstrTaskTag == a_qstrTaskTag)
        {
            break;
        }
        l_pTask = l_pTask->m_pNext;
    }
    return l_pTask;
}

STaskAbstractHour* CPlanTimeHour::CreateTaskAbstract(QString a_qstrGoalName,\
                                                     QString a_qstrTaskTag,\
                                                     Qt::GlobalColor a_EColorTag)
{
    if(NULL != this->HasTaskAbstract(a_qstrGoalName, a_qstrTaskTag))
    {
        return NULL;
    }
    STaskAbstractHour* l_pNewTask = new STaskAbstractHour;
    l_pNewTask->m_qstrGoalName = a_qstrGoalName;
    l_pNewTask->m_qstrTaskTag = a_qstrTaskTag;
    l_pNewTask->m_EGoalColorTag = a_EColorTag;
    l_pNewTask->m_blIsFinished = false;
    l_pNewTask->m_pNext = NULL;

    this->AddTaskAbstractToGloablList(l_pNewTask);
    return l_pNewTask;
}

void CPlanTimeHour::DeleteTaskAbstract(QString a_qstrGoalName, QString a_qstrTaskTag)
{
    STaskAbstractHour* l_pDelTask = this->HasTaskAbstract(a_qstrGoalName, a_qstrTaskTag);
    this->DeleteTaskAbstract(l_pDelTask);
}

void CPlanTimeHour::DeleteTaskAbstract(STaskAbstractHour *a_pDelTask)
{
    if(NULL != a_pDelTask)
    {
        STaskAbstractHour* l_pTask = m_pTaskListHead;
        if(m_pTaskListHead == a_pDelTask)
        {
            m_pTaskListHead = a_pDelTask->m_pNext;
        }
        else
        {
            while(l_pTask)
            {
                if(l_pTask->m_pNext == a_pDelTask)
                {
                    l_pTask->m_pNext = a_pDelTask->m_pNext;
                }
                l_pTask = l_pTask->m_pNext;
            }
        }
        STimeSegHour* l_pTimeSeg = m_pTimeSegListHead;
        while(l_pTimeSeg)
        {
            for(int i=0; i<l_pTimeSeg->m_CTaskList.length(); i++)
            {
                if(l_pTimeSeg->m_CTaskList[i] == a_pDelTask)
                {
                    l_pTimeSeg->m_CTaskList.removeAt(i);
                    break;
                }
            }
            l_pTimeSeg = l_pTimeSeg->m_pNext;
        }
        delete a_pDelTask;
        m_iTaskCounter--;
        if(0 == m_iTaskCounter)
        {
            m_pTaskListTail = NULL;
        }
    }
}

bool CPlanTimeHour::TaskInTimeSeg(STimeSegHour *a_pTimeSegHour, STaskAbstractHour *a_pTask)
{
    bool l_blFlag = false;
    for(int i=0; i<a_pTimeSegHour->m_CTaskList.length(); i++)
    {
        if(a_pTimeSegHour->m_CTaskList[i]->m_qstrGoalName == a_pTask->m_qstrGoalName\
                && a_pTimeSegHour->m_CTaskList[i]->m_qstrTaskTag == a_pTask->m_qstrTaskTag)
        {
            l_blFlag = true;
            break;
        }
    }
    return l_blFlag;
}

STaskAbstractHour* CPlanTimeHour::GetTaskAbstract(int a_iTaskIndex)
{
    int i = 0;
    STaskAbstractHour* l_pTask = NULL;
    if(a_iTaskIndex <= 0)
    {
        l_pTask = m_pTaskListHead;
    }
    else if(a_iTaskIndex >= m_iTaskCounter - 1)
    {
        l_pTask = m_pTaskListTail;
    }
    else
    {
        l_pTask = m_pTaskListHead;
        while(l_pTask)
        {
            l_pTask = l_pTask->m_pNext;
            i++;
            if(i == a_iTaskIndex)
            {
                break;
            }
        }
    }
    return l_pTask;
}

int CPlanTimeHour::GetTaskIndex(QString a_qstrGoalName, QString a_qstrTaskTag)
{
    STaskAbstractHour* l_pTaskListHead = m_pTaskListHead;
    int l_iIdx = 0;
    while(l_pTaskListHead)
    {
        if(l_pTaskListHead->m_qstrGoalName == a_qstrGoalName\
                && l_pTaskListHead->m_qstrTaskTag == a_qstrTaskTag)
        {
            break;
        }
        l_pTaskListHead = l_pTaskListHead->m_pNext;
        l_iIdx++;
    }
    if(l_iIdx == m_iTaskCounter)
    {
        l_iIdx = -1;
    }
    return l_iIdx;
}

STimeSegHour* CPlanTimeHour::AddTimeSeg(int a_iStartClock, int a_iEndClock)
{
    //create new time segment
    STimeSegHour* l_pNewTimeSeg = new STimeSegHour;
    l_pNewTimeSeg->m_iStartClock = a_iStartClock;
    l_pNewTimeSeg->m_iEndClock = a_iEndClock;
    l_pNewTimeSeg->m_ETimePage = GREENSCHEDULE::INPROGRESS;
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

    return l_pNewTimeSeg;
}

void CPlanTimeHour::AddTaskAbstractToGloablList(STaskAbstractHour *a_pNewTaskAbstract)
{
    if(NULL == a_pNewTaskAbstract)
    {
        return;
    }
    if(NULL == m_pTaskListHead)
    {
        m_pTaskListHead = a_pNewTaskAbstract;
    }
    else
    {
        m_pTaskListTail->m_pNext = a_pNewTaskAbstract;
    }
    m_pTaskListTail = a_pNewTaskAbstract;
    m_iTaskCounter++;
}
void CPlanTimeHour::AddTaskAbstractToLocalList(STimeSegHour *a_pTimeSegHour,\
                                               STaskAbstractHour *a_pNewTaskAbstract)
{
    if(!this->TaskInTimeSeg(a_pTimeSegHour, a_pNewTaskAbstract)\
            && GREENSCHEDULE::INPROGRESS == a_pTimeSegHour->m_ETimePage)
    {
        a_pTimeSegHour->m_CTaskList.append(a_pNewTaskAbstract);
    }
}

void CPlanTimeHour::ClearTaskAssignment(STimeSegHour *a_pTimeSeg)
{
    a_pTimeSeg->m_CTaskList.clear();
}
