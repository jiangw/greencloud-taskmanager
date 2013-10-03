#ifndef GLOBALCONST_H
#define GLOBALCONST_H

#include <QDebug>

namespace GLOBALCONST
{
    //workspace status
    typedef enum {IDLE, DRAWSYSLABEL}EStatus;
    //system labels
    typedef enum {NONE, SYSRECT, SYSELPS}ESysLabel;
    //workspace flag struct
    struct SWorkSpaceFlag
    {
        EStatus m_eStatus;
        ESysLabel m_eSysLabel;
    };
}
#endif // GLOBALCONST_H
