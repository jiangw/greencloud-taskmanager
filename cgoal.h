#ifndef CGOAL_H
#define CGOAL_H

#include <iostream>

#include "gconfig.h"
#include "cmember.h"

class CGoal
{
public:
    CGoal();

    void SetTitle(std::wstring a_wstrTitle);
    std::wstring GetTitle();
    bool TitleStatus();

    void AddMember(CMember* a_pMember);
    void SetMembers(std::list<CMember *>& a_pMembers);
    std::list<CMember *>& GetMembers();
    bool MembersStatus();

private:
    std::wstring m_wstrTitle;
    std::list<CMember *> m_pMembers;
    //goal status
    bool m_blTitle; //if title is set
    bool m_blMembers; //if members are set
    bool m_blIntro; //if introduction is set
    bool m_blBkgrnd; //if background is set
    bool m_blSteps; //if steps are set
    bool m_blRes; //if resources are set
};

#endif // CGOAL_H
