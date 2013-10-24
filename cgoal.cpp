#include "cgoal.h"

CGoal::CGoal()
{
    m_blTitle = false;
    m_blMembers = false;
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

void CGoal::AddMember(CMember *a_pMember)
{
    CMember* l_pNewMember = new CMember(a_pMember);
    m_pMembers.push_back(l_pNewMember);

    m_blMembers = true;

#ifdef PF_TEST
    TB_cout << L"Add Member: " << m_pMembers.back()->GetName() << std::endl;
#endif
}

void CGoal::SetMembers(std::list<CMember *> &a_pMembers)
{
    for(std::list<CMember *>::iterator l_ppMember = a_pMembers.begin(); \
        l_ppMember != a_pMembers.end(); l_ppMember++)
    {
        CMember* l_pNewMember = new CMember(*l_ppMember);
        m_pMembers.push_back(l_pNewMember);
    }
    m_blMembers = true;
}

bool CGoal::MembersStatus()
{
    return m_blMembers;
}
