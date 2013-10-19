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
    enum EWorkSpaceStatus{IDLE, ADDGOALTITLE, ADDGOALMEMBERS, ADDGOALINTRO, \
                         ADDGOALBKGRND, ADDGOALSTEPS, ADDGOALRES, SHOWGOAL};
    //workspace flag struct
    struct SWorkSpaceFlag
    {
        EWorkSpaceStatus m_eWorkSpaceStatus;
    };
    //properties of graphic items
    const int g_iMouseClickDistThreshold = 3;
    const int g_iItemBorderWidth = 4;
    const int g_iItemFontSizeLarge = 25;
    const int g_iItemFontSizeSmall = 10;
    const int g_iItemDisappearFrames = 10;
    const int g_iItemAppearFrames = 5;
    const int g_iItemIntervalX = 8; //standard distance between two items in X direction
    const int g_iItemIntervalY = 10; //standard distance between two items in Y direction
}

#endif // GCONFIG_H
