#ifndef CLABELITEM_H
#define CLABELITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>

#include "gconfig.h"
#include "citemanimation.h"

class CLabelItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CLabelItem(QString a_strText, QGraphicsItem* a_pParent);

    void SetText(QString a_strText);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();
    void SLOT_DisappearItemProc();

private:
    QString m_strLabel;
    QFont m_cFont;
};

#endif // CLABELITEM_H
