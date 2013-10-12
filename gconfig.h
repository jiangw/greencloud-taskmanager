#ifndef GCONFIG_H
#define GCONFIG_H

#include <iostream>

//set programming flag
#define PF_TEST
//#define PF_RUN

//Tool box for testing code
#ifdef PF_TEST
#include "ctestbox.h"
#endif

namespace TASKMANAGER
{
    //workspace status
    enum EWorkSpaceStatus{IDLE, DRAWSYSLABEL};
    //workspace flag struct
    struct SWorkSpaceFlag
    {
        EWorkSpaceStatus m_eWorkSpaceStatus;
    };
    //properties of graphic items
    const int ItemBorderWidth = 4;
    const int ItemFontSize = 25;
}

#endif // GCONFIG_H
