#ifndef CBUTTONITEM_H
#define CBUTTONITEM_H

#include <QGraphicsTextItem>
#include <QRectF>
#include <QPointF>

#include "gconfig.h"

class CButtonItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit CButtonItem(QGraphicsItem* a_pParent, QGraphicsScene* a_pScene = NULL);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void SIGNAL_LeftButtonClicked();

private:
    QRectF m_cBR; //bounding rect
    QPointF m_cLastPos;
};

#endif // CBUTTONITEM_H
