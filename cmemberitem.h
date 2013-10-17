#ifndef CMEMBERITEM_H
#define CMEMBERITEM_H

#include <QGraphicsItem>
#include <QRect>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>

#include "gconfig.h"
#include "citemanimation.h"
#include "ctextinputitem.h"
#include "cselectitem.h"

class CMemberItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CMemberItem(QGraphicsItem* a_pParent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void SetInputTip(QString a_strTip);
    bool IsChecked();
    QString GetMemberName();

public slots:
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();

private:
    QPointF m_cLastPos;
    CSelectItem* m_pSelItem;
    CTextInputItem* m_pTextInputItem;
};

#endif // CMEMBERITEM_H
