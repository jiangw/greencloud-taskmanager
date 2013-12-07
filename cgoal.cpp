#include "cgoal.h"

CGoal::CGoal()
{
}

void CGoal::SetTitle(std::wstring a_wstrTitle)
{
    m_wstrTitle = a_wstrTitle;
}

std::wstring CGoal::GetTitle()
{
    return m_wstrTitle;
}
