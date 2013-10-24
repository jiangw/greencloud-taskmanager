#include "cmember.h"

CMember::CMember()
{
}

CMember::CMember(CMember *a_pMember)
{
    m_wstrName = a_pMember->GetName();
}

void CMember::SetName(std::wstring a_wstrName)
{
    m_wstrName = a_wstrName;
}

std::wstring CMember::GetName()
{
    return m_wstrName;
}
