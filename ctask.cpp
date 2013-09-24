#include "ctask.h"

CTask::CTask()
{
}

CTask::~CTask()
{
}

bool CTask::SetPreAllocTime(QDateTime a_cStartTime, QDateTime a_cEndTime)
{
    if(a_cStartTime < a_cEndTime) // the start time must be earlier than the end time
    {
        m_cPreStartTime = a_cStartTime;
        m_cPreEndTime = a_cEndTime;
        return true;
    }
    else
    {
        return false;
    }
}

bool CTask::TaskStart()
{
    if(this->GetStatus() == NOTSTART)
    {
        this->SetStatus(ONGOING);
        this->m_cStartTime = QDateTime::currentDateTime();
        return true;
    }
    else
    {
        return false;
    }
}

bool CTask::TaskEnd()
{
    if(this->GetStatus() == ONGOING)
    {
        this->SetStatus(FINISHED);
        m_cEndTime = QDateTime::currentDateTime();
        return true;
    }
    else
    {
        return false;
    }
}

bool CTask::TaskAbandon()
{
    if(this->GetStatus() == ONGOING)
    {
        this->SetStatus(ABANDONED);
        m_cEndTime = QDateTime::currentDateTime();
        return true;
    }
    else
    {
        return false;
    }
}
