#ifndef CLINEITEM_H
#define CLINEITEM_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QRectF>

#include "gconfig.h"
#include "citemanimation.h"

class CLineItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CLineItem(QLineF a_cLine, int a_iLineWidth, QGraphicsItem* a_pParent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    qreal Length();

public slots:
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();
    void SLOT_DisappearItemProc();

private:
    QGraphicsLineItem* m_pLine;
    QPen m_CPen;
    qreal m_dFinalLen;
};

#endif // CLINEITEM_H
