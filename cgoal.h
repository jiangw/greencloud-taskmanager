#ifndef CGOAL_H
#define CGOAL_H

#include <iostream>

class CGoal
{
public:
    CGoal();

    void SetTitle(std::wstring a_wstrTitle);
    std::wstring GetTitle();

private:
    std::wstring m_wstrTitle;
};

#endif // CGOAL_H
