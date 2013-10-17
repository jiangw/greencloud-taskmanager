#include "citemanimation.h"

CItemAnimation::CItemAnimation(QObject *a_pParent)
    :QObject(a_pParent)
{
    m_iFrameId = 0;
}

void CItemAnimation::SLOT_DisappearItemProc()
{
    emit this->SIGNAL_AnimEnd();
}

void CItemAnimation::SLOT_AppearItemProc()
{
    emit this->SIGNAL_AnimEnd();
}
