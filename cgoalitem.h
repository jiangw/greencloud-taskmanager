#ifndef CGOALVIEW_H
#define CGOALVIEW_H

#include <QGraphicsItem>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextLayout>
#include <QRectF>
#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QList>

#include "gconfig.h"
#include "citemanimation.h"
#include "cmemberitem.h"
#include "ctextinputitem.h"
#include "cbuttonitem.h"
#include "clabelitem.h"
#include "clineitem.h"
#include "cgoal.h"

class CGoalItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT

public:
    enum EGoalItemStatus{IDLE, TITLEINPUT, INTROINPUT, BKGRNDINPUT, MEMBERINPUT, \
                        RESINPUT, TASKINPUT};

    CGoalItem(QGraphicsItem *a_pParent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF m_cMinBR; //minimum bounding rect

    //Goal operations
    void ConnDelItemToCommonBtns(QList<CButtonItem *>* a_pCommonBtns, CItemAnimation* a_pDelItem);
    void CreateGoalTitleUI(); //create UI to add goal title
    void CreateTasksUI(); //create UI to add goal tasks

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void SIGNAL_AddGoalTitle(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalMembers(CGoalItem* a_pGoalItem);
    void SIGNAL_RequestMembers(CGoalItem* a_pGoalItem);
    void SIGNAL_RemoveMemberItems();
    void SIGNAL_AddGoalIntro(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalBkgrnd(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalSteps(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalRes(CGoalItem* a_pGoalItem);
    void SIGNAL_ShowGoal(CGoalItem* a_pGoalItem);
    
public slots:
    void SLOT_SetGoalTitleProc(QTextDocument* a_pDoc);
    void SLOT_SetGoalMembersProc(QList<CMemberItem *>* a_ppMembers);
    void SLOT_SetGoalIntroProc(QTextDocument* a_pDoc);
    void SLOT_SetGoalBkgrndProc(QTextDocument* a_pDoc);
    void SLOT_SetGoalStepsProc(QTextDocument* a_pDoc);
    void SLOT_SetGoalResProc(QTextDocument* a_pDoc);
    void SLOT_ShowGoalEmit();
    void SLOT_RequestMembersEmit();

    //override from CItemAnimation
    void SLOT_AppearItemProc();
    void SLOT_RemoveItemEmit(); //remove the item itself
    void SLOT_DeleteItemEmit();
    //slot for removing specific child items
    void SLOT_RemoveItemEmit(QGraphicsItem* a_pGraphicsItem, CItemAnimation* a_pItemAnim);

private:
    //painting options
    QRectF m_cBR; //bounding rect
    int m_iBorderPenWidth;
    int m_iFontSize;
    bool m_blFontBold;
    QPen m_cTextPen;
    QFont m_cTextFont;
    QPen m_cBorderPen;
    QBrush m_cBgBrush;
    QPointF m_cLastPos;

    //goal item status
    EGoalItemStatus m_eStatus;

    //Models
    CGoal m_cGoal;
    bool m_blIntro;
    bool m_blBkgrnd;
    bool m_blSteps;
    bool m_blRes;
};

#endif // CGOALVIEW_H
