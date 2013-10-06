#ifndef GCONFIG_H
#define GCONFIG_H

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
}

#endif // GCONFIG_H