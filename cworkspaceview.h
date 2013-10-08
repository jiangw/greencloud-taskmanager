#ifndef CWORKSPACEVIEW_H
#define CWORKSPACEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPoint>
#include <QMouseEvent>

#include "gconfig.h"
#include "cworkspace.h"
#include "cgoalview.h"

class CWorkSpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CWorkSpaceView(CWorkSpace* a_pWorkSpace, QWidget* a_pParent);
    virtual ~CWorkSpaceView();
    
signals:
    
public slots:
    void Slot_DragModeSwitched(bool a_blFlag);
    void Slot_AddGoalAction();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    CWorkSpace* m_pWorkSpace;
    QPoint m_cLastMousePos;
    TASKMANAGER::SWorkSpaceFlag m_sLastFlag;
    CGoalView* m_pGoalView;
};

#endif // CWORKSPACEVIEW_H
