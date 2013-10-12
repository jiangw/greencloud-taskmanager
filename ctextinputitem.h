#ifndef CTEXTINPUTITEM_H
#define CTEXTINPUTITEM_H

#include <QGraphicsTextItem>
#include <QTextDocument>

#include "gconfig.h"

class CTextInputItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    CTextInputItem(QGraphicsItem* a_pParent);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPos(qreal x, qreal y);

public slots:
    void SLOT_SubmitGoalTitle();
    void SLOT_Cancel();

signals:
    void SIGNAL_SetGoalTitle(QTextDocument* a_pDoc);
    void SIGNAL_Cancel();
};

#endif // CTEXTINPUTITEM_H
