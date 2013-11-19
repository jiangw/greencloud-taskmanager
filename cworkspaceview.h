#ifndef CWORKSPACEVIEW_H
#define CWORKSPACEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QList>

#include "gconfig.h"
#include "cworkspace.h"
#include "cgoalitem.h"
#include "ctextinputitem.h"
#include "clabelitem.h"
#include "clineitem.h"
#include "cmemberitem.h"

#include "cdaywidget.h"
#include "cmonthwidget.h"
#include "cyearwidget.h"
#include "cplanwidget.h"
#include "cgoalwidget.h"
#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/cbuttonwidget.h"
#include "../GraphicsWidgetLib/cwidgetlist.h"

class CWorkSpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    CWorkSpaceView(CWorkSpace* a_pWorkSpace, QWidget* a_pParent);
    virtual ~CWorkSpaceView();
    
signals:
    
public slots:
    void SLOT_DragModeSwitched(bool a_blFlag);
    void SLOT_AddGoalActionProc();
    void SLOT_AddPlanActionProc();
    void SLOT_AddDayItemActionProc();
    void SLOT_AddMonthItemActionProc();
    void SLOT_AddYearItemActionProc();
    void SLOT_ShowGoalProc(CGoalItem* a_pGoalItem);
    void SLOT_RemoveItemProc(QGraphicsItem* a_pGraphicsItem, \
                         CItemAnimation* a_pItemAnim);
    void SLOT_DeleteItemProc(QGraphicsItem* a_pItem);
    void SLOT_CenterOnItemProc(QGraphicsItem* a_pItem);

    //for time+task=plan framework
    void SLOT_AddGoalWidgetToWidgetListEmit();

    //may be removed from this class
//    void SLOT_AddGoalTitleProc(CGoalItem* a_pGoalItem);
//    void SLOT_AddGoalMembersProc(CGoalItem* a_pGoalItem);
//    void SLOT_RequestMembersProc(CGoalItem* a_pGoalItem);
//    void SLOT_AddMemberItemProc();
//    void SLOT_RemoveMemberItemProc();
//    void SLOT_RemoveMemberItemGroupProc();
//    void SLOT_AddGoalIntroProc(CGoalItem* a_pGoalItem);
//    void SLOT_AddGoalBkgrndProc(CGoalItem* a_pGoalItem);
//    void SLOT_AddGoalStepsProc(CGoalItem* a_pGoalItem);
//    void SLOT_AddGoalResProc(CGoalItem* a_pGoalItem);

signals:
    //for time+task=plan framework
    void SIGNAL_AddWidgetToWidgetList(CGraphicsWidget* a_pNewWidget);

private:
    CWorkSpace* m_pWorkSpace;
    QList<CGoalItem *> m_pGoalItemList;
    QList<CMemberItem *> m_pMemberGroup;

};

#endif // CWORKSPACEVIEW_H
