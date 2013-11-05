#ifndef CYEARITEM_H
#define CYEARITEM_H

#include "gconfig.h"
#include "citemanimation.h"

#include <QGraphicsItem>
#include <QPainter>

class CYearItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT

public:
    CYearItem(QGraphicsItem* a_pParent);

    const QList<int>* GetSelYears(){return &m_pSelYears;}

    //override from QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //override from QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
    //override from CItemAnimation
    void SLOT_DeleteItemEmit();
    void SLOT_RemoveItemEmit();

private:
    QRectF m_CBR; //bounding rect
    int m_iStartYear;
    QList<int> m_pSelYears;
    QPointF m_CMouseLastPos;
};

#endif // CYEARITEM_H
