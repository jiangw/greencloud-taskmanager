#include "cworkspaceview.h"

CWorkSpaceView::CWorkSpaceView(CWorkSpace *a_pWorkSpace, QWidget *a_pParent) :
    QGraphicsView(a_pWorkSpace, a_pParent)
{
    m_pWorkSpace = a_pWorkSpace;
    m_pGoalView = NULL;

    this->setMouseTracking(true);
}

CWorkSpaceView::~CWorkSpaceView()
{
    m_pWorkSpace = NULL;
}

void CWorkSpaceView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_cLastMousePos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void CWorkSpaceView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && \
            m_cLastMousePos == event->pos()) //left button clicked
    {
        m_pWorkSpace->SysLabelDraw(mapToScene(event->pos()));
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void CWorkSpaceView::Slot_DragModeSwitched(bool a_blFlag)
{
    if(a_blFlag)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        //save last flag of workspace
        m_sLastFlag.m_eWorkSpaceStatus = m_pWorkSpace->m_eStatus;
        m_sLastFlag.m_eSysLabel = m_pWorkSpace->m_eSysLabel;
        //set new flag of workspace
        m_pWorkSpace->m_eStatus = TASKMANAGER::IDLE;
        m_pWorkSpace->m_eSysLabel = TASKMANAGER::NONE;
    }
    else
    {
        this->setDragMode(QGraphicsView::NoDrag);
        //restore the flag of workspace
        m_pWorkSpace->m_eStatus = m_sLastFlag.m_eWorkSpaceStatus;
        m_pWorkSpace->m_eSysLabel = m_sLastFlag.m_eSysLabel;
    }
}

void CWorkSpaceView::Slot_AddGoalAction()
{
    if(NULL == m_pGoalView)
    {
        m_pGoalView = new CGoalView(NULL, m_pWorkSpace);
    }
}
