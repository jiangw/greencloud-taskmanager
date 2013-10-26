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
#include "cbuttonitem.h"
#include "clabelitem.h"
#include "clineitem.h"
#include "cmemberitem.h"
#include "cdayitem.h"

class CWorkSpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    CWorkSpaceView(CWorkSpace* a_pWorkSpace, QWidget* a_pParent);
    virtual ~CWorkSpaceView();

    void ConnDelItemToCommonBtns(QList<CButtonItem *>* a_pCommonBtns, CItemAnimation* a_pDelItem);
    
signals:
    
public slots:
    void SLOT_DragModeSwitched(bool a_blFlag);
    void SLOT_AddGoalActionProc();
    void SLOT_AddDayItemActionProc();
    void SLOT_ShowGoalProc(CGoalItem* a_pGoalItem);
    void SLOT_RemoveItemProc(QGraphicsItem* a_pGraphicsItem, \
                         CItemAnimation* a_pItemAnim);
    void SLOT_DeleteItemProc(QGraphicsItem* a_pItem);
    void SLOT_CenterOnItemProc(QGraphicsItem* a_pItem);

    //may be removed from this class
    void SLOT_AddGoalTitleProc(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalMembersProc(CGoalItem* a_pGoalItem);
    void SLOT_RequestMembersProc(CGoalItem* a_pGoalItem);
    void SLOT_AddMemberItemProc();
    void SLOT_RemoveMemberItemProc();
    void SLOT_RemoveMemberItemGroupProc();
    void SLOT_AddGoalIntroProc(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalBkgrndProc(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalStepsProc(CGoalItem* a_pGoalItem);
    void SLOT_AddGoalResProc(CGoalItem* a_pGoalItem);

private:
    CWorkSpace* m_pWorkSpace;
    QPoint m_cLastMousePos;
    TASKMANAGER::SWorkSpaceFlag m_sLastFlag;
    QList<CGoalItem *> m_pGoalItemList;
    CGoalItem* m_pGoalItem; //will be removed
    QList<CMemberItem *> m_pMemberGroup;

};

#endif // CWORKSPACEVIEW_H
