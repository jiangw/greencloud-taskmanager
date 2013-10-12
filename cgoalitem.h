#ifndef CGOALVIEW_H
#define CGOALVIEW_H

#include <QGraphicsItem>
#include <QTextDocument>
#include <QRectF>
#include <QLineF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QGraphicsSceneMouseEvent>

#include "gconfig.h"

class CGoalItem :public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    CGoalItem(QGraphicsItem *a_pParent, QGraphicsScene *a_pScene);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF m_cMinBR; //minimum bounding rect

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void SIGNAL_AnimationOver();
    void SIGNAL_AddGoalTitle(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalIntro(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalBkgrnd(CGoalItem* a_pGoalItem);
    void SIGNAL_AddGoalSteps(CGoalItem* a_pGoalItem);
    void SIGNAL_ShowGoal(CGoalItem* a_pGoalItem);
    
public slots:
    void SLOT_Animation();
    void SLOT_SetGoalTitle(QTextDocument* a_pDoc);
    void SLOT_ShowGoal();

private:
    //View
    QRectF m_cBR; //bounding rect
    int m_iBorderPenWidth;
    int m_iFontSize;
    bool m_blFontBold;
    QPen m_cTextPen;
    QFont m_cTextFont;
    QPen m_cBorderPen;
    QBrush m_cBgBrush;
    QPointF m_cLastPos;
    //Animation
    int m_iFrameId;
    //Model properties (临时使用，Model类完成后这些变量将移到其中)
    bool m_blTitle;
    bool m_blIntro;
    bool m_blBkgrnd;
    bool m_blSteps;
};

#endif // CGOALVIEW_H
