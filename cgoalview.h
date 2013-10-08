#ifndef CGOALVIEW_H
#define CGOALVIEW_H

#include <QGraphicsItem>
#include <QTextDocument>
#include <QRectF>
#include <QPainter>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QGraphicsSceneMouseEvent>

#include "gconfig.h"

class CGoalView :public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CGoalView(QGraphicsItem *a_pParent, QGraphicsScene *a_pScene);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF m_cMinBR; //minimum bounding rect

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

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
    
signals:
    
public slots:
    
};

#endif // CGOALVIEW_H
