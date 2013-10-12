#include "ctextinputitem.h"

CTextInputItem::CTextInputItem(QGraphicsItem *a_pParent)
    :QGraphicsTextItem(a_pParent)
{
    this->setTextInteractionFlags(Qt::TextEditable);
}

void CTextInputItem::setPos(qreal x, qreal y)
{
    QGraphicsTextItem::setPos(x, y);
}

void CTextInputItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsTextItem::paint(painter, option, widget);

    //draw out line
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 4));
    painter->drawRect(this->boundingRect());
    painter->restore();
}

void CTextInputItem::SLOT_SubmitGoalTitle()
{
    emit this->SIGNAL_SetGoalTitle(this->document());
}

void CTextInputItem::SLOT_Cancel()
{
    emit this->SIGNAL_Cancel();
}
