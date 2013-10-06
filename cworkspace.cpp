#include "cworkspace.h"

CWorkSpace::CWorkSpace(QObject *parent) :
    QGraphicsScene(parent)
{
    m_eStatus = TASKMANAGER::IDLE;
    m_eSysLabel = TASKMANAGER::NONE;
}

CWorkSpace::~CWorkSpace()
{
}

void CWorkSpace::SysLabelDraw(QPointF a_cPos)
{
    switch(m_eSysLabel)
    {
    case TASKMANAGER::SYSRECT:
        this->addRect(a_cPos.rx(), a_cPos.ry(), 20, 20);
        break;
    case TASKMANAGER::SYSELPS:
        break;
    default:
        break;
    }

    emit this->Signal_SysLabelDrawn();
}
