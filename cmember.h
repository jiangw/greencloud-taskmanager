#ifndef CMEMBER_H
#define CMEMBER_H

#include <iostream>

#include "gconfig.h"

class CMember
{
public:
    CMember();
    CMember(CMember* a_pMember);

    void SetName(std::wstring a_wstrName);
    std::wstring GetName();

private:
    std::wstring m_wstrName;
};

#endif // CMEMBER_H
