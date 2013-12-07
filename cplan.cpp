#include "cplan.h"

CPlan* CPlan::s_pPlan = NULL;

CPlan::CPlan(QObject *parent) :
    QObject(parent)
{
}

CPlan::~CPlan()
{
    this->ClearPlanTimeHourFactory();
}

void CPlan::InitPlan(QObject *a_pParent)
{
    if(NULL == s_pPlan)
    {
        s_pPlan = new CPlan(a_pParent);
    }
}

CPlan* CPlan::GetPlan()
{
    if(NULL == s_pPlan)
    {
        s_pPlan = new CPlan();
    }
    return s_pPlan;
}

void CPlan::FreePlan()
{
    if(NULL != s_pPlan)
    {
        delete s_pPlan;
    }
}

CPlanTimeHour* CPlan::CreatePlanTimeHour(QDate a_CDate)
{
    CPlanTimeHour* l_pPlanTimeHour = new CPlanTimeHour(a_CDate);
    //insert new object by time order
    int l_iInsertPos = m_CPlanTimeHourFactory.length();
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(m_CPlanTimeHourFactory[i]->Date() > a_CDate)
        {
            l_iInsertPos = i;
            break;
        }
    }
    m_CPlanTimeHourFactory.insert(l_iInsertPos, l_pPlanTimeHour);

    return l_pPlanTimeHour;
}

CPlanTimeHour* CPlan::GetPlanTimeHour(QDate a_CDate)
{
    CPlanTimeHour* l_pPlanTimeHour = NULL;
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(m_CPlanTimeHourFactory[i]->Date() == a_CDate)
        {
            l_pPlanTimeHour = m_CPlanTimeHourFactory[i];
            break;
        }
    }
    return l_pPlanTimeHour;
}

void CPlan::SLOT_HourSelMaskRequestProc(QDate a_CDate, bool *a_pHourSelMask, int a_iHoursPerDay)
{
    bool l_blDateFound = false;
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(m_CPlanTimeHourFactory[i]->Date() == a_CDate)
        {
            l_blDateFound = true;
            m_CPlanTimeHourFactory[i]->ConvertTimeSegToHourMask(a_pHourSelMask, a_iHoursPerDay);
            break;
        }
    }
    if(l_blDateFound)
    {
        emit this->SIGNAL_HourSelMaskRequestFinished();
    }
}

void CPlan::ClearPlanTimeHourFactory()
{
    while(!m_CPlanTimeHourFactory.isEmpty())
    {
        CPlanTimeHour* l_pDelPlanTimeHour = m_CPlanTimeHourFactory.takeFirst();
        delete l_pDelPlanTimeHour;
    }
}
