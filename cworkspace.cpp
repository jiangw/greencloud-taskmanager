#include "cworkspace.h"

CWorkSpace::CWorkSpace(QObject *parent) :
    QGraphicsScene(parent)
{
    m_eStatus = GLOBALCONST::IDLE;
    m_eSysLabel = GLOBALCONST::NONE;
}

CWorkSpace::~CWorkSpace()
{
}

void CWorkSpace::SysLabelDraw(QPointF a_cPos)
{
    switch(m_eSysLabel)
    {
    case GLOBALCONST::SYSRECT:
        this->addRect(a_cPos.rx(), a_cPos.ry(), 20, 20);
        break;
    case GLOBALCONST::SYSELPS:
        break;
    default:
        break;
    }

    emit this->Signal_SysLabelDrawn();
}
