#include "cplangoal.h"

CPlanGoal::CPlanGoal()
{
    m_qstrGoalName = "";
    m_EColorTag = Qt::green;
    m_pTaskListHead = NULL;
    m_pTaskListTail = NULL;
    m_iTaskCounter = 0;
}

CPlanGoal::CPlanGoal(QString a_qstrGoalName, QRgb a_EColorTag)
{
    m_qstrGoalName = a_qstrGoalName;
    m_EColorTag = a_EColorTag;
    m_pTaskListHead = NULL;
    m_pTaskListTail = NULL;
    m_iTaskCounter = 0;
}

CPlanGoal::~CPlanGoal()
{
    this->ClearTaskList();
}

void CPlanGoal::ClearTaskList()
{
    STask* l_pTask = m_pTaskListHead;
    while(m_pTaskListHead)
    {
        l_pTask = m_pTaskListHead->m_pNext;
        delete m_pTaskListHead;
        m_pTaskListHead = l_pTask;
    }
    m_pTaskListTail = NULL;
    m_iTaskCounter = 0;
}

void CPlanGoal::AddTask(int a_iTaskId, QString a_qstrTaskTag, QString a_qstrTaskDesc,\
                        bool a_blFinished)
{
    if(NULL == this->TaskExist(a_iTaskId))
    {
        STask* l_pNewTask = new STask;
        l_pNewTask->m_iTaskId = a_iTaskId;
        l_pNewTask->m_qstrTaskTag = a_qstrTaskTag;
        l_pNewTask->m_qstrDescription = a_qstrTaskDesc;
        l_pNewTask->m_blIsFinished = a_blFinished;
        l_pNewTask->m_pNext = NULL;
        if(NULL == m_pTaskListHead)
        {
            m_pTaskListHead = l_pNewTask;
        }
        else
        {
            m_pTaskListTail->m_pNext = l_pNewTask;
        }
        m_pTaskListTail = l_pNewTask;
        m_iTaskCounter++;
    }
}

void CPlanGoal::UpdateTask(int a_iTaskId, QString a_qstrTaskTag, QString a_qstrTaskDesc,\
                           bool a_blFinished)
{
    STask* l_pTask = this->TaskExist(a_iTaskId);
    l_pTask->m_qstrTaskTag = a_qstrTaskTag;
    l_pTask->m_qstrDescription = a_qstrTaskDesc;
    l_pTask->m_blIsFinished = a_blFinished;
}

void CPlanGoal::DeleteTask(int a_iTaskId)
{
    STask* l_pDelTask = m_pTaskListHead;
    STask* l_pDelTaskPrev = NULL;
    while(NULL != l_pDelTask)
    {
        if(l_pDelTask->m_iTaskId == a_iTaskId)
        {
            if(NULL == l_pDelTaskPrev)
            { //delete list head
                m_pTaskListHead = l_pDelTask->m_pNext;
            }
            else if(m_pTaskListTail == l_pDelTask)
            { //delete list tail
                m_pTaskListTail = l_pDelTaskPrev;
            }
            else
            {
                l_pDelTaskPrev->m_pNext = l_pDelTask->m_pNext;
            }
            delete l_pDelTask;
            m_iTaskCounter--;
            break;
        }
        l_pDelTaskPrev = l_pDelTask;
        l_pDelTask = l_pDelTaskPrev->m_pNext;
    }
    if(0 == m_iTaskCounter)
    {
        m_pTaskListHead = NULL;
        m_pTaskListTail = NULL;
    }
}

void CPlanGoal::FinishTask(int a_iTaskId, bool a_blIsFinished)
{
    STask* l_pTask = this->TaskExist(a_iTaskId);
    if(NULL != l_pTask)
    {
        l_pTask->m_blIsFinished = a_blIsFinished;
    }
}

const STask* CPlanGoal::GetTaskById(int a_iTaskId) const
{
    return this->TaskExist(a_iTaskId);
}

const STask* CPlanGoal::GetTaskByTag(QString a_qstrTaskTag)
{
    STask* l_pTask = NULL;
    STask* l_pTaskIter = m_pTaskListHead;
    while(l_pTaskIter)
    {
        if(l_pTaskIter->m_qstrTaskTag == a_qstrTaskTag)
        {
            l_pTask = l_pTaskIter;
            break;
        }
        l_pTaskIter = l_pTaskIter->m_pNext;
    }
    return l_pTask;
}

const STask* CPlanGoal::GetTaskByIndex(int a_iTaskIndex)
{
    STask* l_pTask = NULL;
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
        int i = 0;
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

int CPlanGoal::GetTaskIndex(int a_iTaskId)
{
    STask* l_pTask = m_pTaskListHead;
    int l_iIdx = 0;
    while(l_pTask)
    {
        if(l_pTask->m_iTaskId == a_iTaskId)
        {
            break;
        }
        l_iIdx++;
        l_pTask = l_pTask->m_pNext;
    }
    if(l_iIdx == m_iTaskCounter)
    {
        l_iIdx = -1;
    }
    return l_iIdx;
}

bool CPlanGoal::HasTask(int a_iTaskId) const
{
    if(NULL == this->TaskExist(a_iTaskId))
    {
        return false;
    }
    else
    {
        return true;
    }
}

STask* CPlanGoal::TaskExist(int a_iTaskId) const
{
    STask* l_pTask = m_pTaskListHead;
    while(l_pTask)
    {
        if(l_pTask->m_iTaskId == a_iTaskId)
        {
            break;
        }
        l_pTask = l_pTask->m_pNext;
    }
    return l_pTask;
}
