#ifndef CSELECTITEM_H
#define CSELECTITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QPoint>
#include <QGraphicsSceneMouseEvent>
#include <QLineF>

#include "gconfig.h"
#include "citemanimation.h"

class CSelectItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CSelectItem(QGraphicsItem* a_pParent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool IsChecked();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();

private:
    bool m_blSelected; //=true : selected. =false : unselected(default)
    QRectF m_CBR;
    QPointF m_CMouseLastPos; //save the last mouse position when it was clicked
};

#endif // CSELECTITEM_H
