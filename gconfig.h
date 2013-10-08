#ifndef GCONFIG_H
#define GCONFIG_H

#include <iostream>

//set programming flag
#define PF_TEST
//#define PF_RUN

namespace TASKMANAGER
{
    //workspace status
    enum EWorkSpaceStatus{IDLE, DRAWSYSLABEL};
    //system labels
    enum ESysLabel{NONE, SYSRECT, SYSELPS};
    //workspace flag struct
    struct SWorkSpaceFlag
    {
        EWorkSpaceStatus m_eWorkSpaceStatus;
        ESysLabel m_eSysLabel;
    };
    //properties of graphic items
    const int ItemBorderWidth = 4;
    const int ItemFontSize = 25;
}

#endif // GCONFIG_H
