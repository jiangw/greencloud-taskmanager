#ifndef CBUTTONITEM_H
#define CBUTTONITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QString>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "gconfig.h"
#include "citemanimation.h"

class CButtonItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit CButtonItem(QString a_strText, QGraphicsItem* a_pParent);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    void SetButtonText(QString a_strText);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void SLOT_DeleteItemEmit();
    void SLOT_DisappearItemProc();
    void SLOT_RemoveItemEmit();

signals:
    void SIGNAL_LeftButtonClicked();

private:
    QRectF m_CBR; //bounding rect
    QPointF m_CMouseLastPos;
    QString m_strText;
};

#endif // CBUTTONITEM_H
