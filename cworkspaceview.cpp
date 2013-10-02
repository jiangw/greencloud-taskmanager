#include "cworkspaceview.h"

CWorkSpaceView::CWorkSpaceView(CWorkSpace *a_pWorkSpace, QWidget *a_pParent) :
    QGraphicsView(a_pWorkSpace, a_pParent)
{
    m_pWorkSpace = a_pWorkSpace;
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
        m_pWorkSpace->SysLabelDraw(mapToScene(m_cLastMousePos));
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}
