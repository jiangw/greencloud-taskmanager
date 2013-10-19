#include "cgoal.h"

CGoal::CGoal()
{
    m_blTitle = false;
}

void CGoal::SetTitle(std::wstring a_wstrTitle)
{
    m_wstrTitle = a_wstrTitle;
    m_blTitle = true;
}

std::wstring CGoal::GetTitle()
{
    return m_wstrTitle;
}

bool CGoal::TitleStatus()
{
    return m_blTitle;
}
