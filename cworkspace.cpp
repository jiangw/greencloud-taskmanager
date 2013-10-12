#include "cworkspace.h"

CWorkSpace::CWorkSpace(QObject *parent) :
    QGraphicsScene(parent)
{
    m_eStatus = TASKMANAGER::IDLE;
}

CWorkSpace::~CWorkSpace()
{
}
