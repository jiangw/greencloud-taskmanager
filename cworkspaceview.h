#ifndef CWORKSPACEVIEW_H
#define CWORKSPACEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>

#include "gconfig.h"
#include "cworkspace.h"
#include "cgoalitem.h"
#include "ctextinputitem.h"
#include "cbuttonitem.h"

class CWorkSpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    CWorkSpaceView(CWorkSpace* a_pWorkSpace, QWidget* a_pParent);
    virtual ~CWorkSpaceView();
    
signals:
    
public slots:
    void SLOT_DragModeSwitched(bool a_blFlag);
    void SLOT_AddGoalAction();
    void SLOT_AddGoalTitle(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalIntro(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalBkgrnd(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalSteps(CGoalItem* a_pGoalItem);
    void SLOT_ShowGoal(CGoalItem* a_pGoalItem);

private:
    CWorkSpace* m_pWorkSpace;
    QPoint m_cLastMousePos;
    TASKMANAGER::SWorkSpaceFlag m_sLastFlag;
    CGoalItem* m_pGoalItem;
};

#endif // CWORKSPACEVIEW_H
