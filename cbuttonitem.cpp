#include "cbuttonitem.h"

CButtonItem::CButtonItem(QGraphicsItem *a_pParent, QGraphicsScene *a_pScene)
    :QGraphicsTextItem(a_pParent, a_pScene)
{
    m_cBR.setRect(-25, -12, 50, 24);
}

QRectF CButtonItem::boundingRect() const
{
    return m_cBR;
}

void CButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
//    QGraphicsTextItem::paint(painter, option, widget);
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen((QBrush(Qt::black), 4)));
    //draw outline
    painter->drawRoundedRect(this->boundingRect(), 5, 5);
//    painter->drawRect(this->boundingRect());
    //draw text
    painter->drawText(this->boundingRect(), Qt::AlignCenter, this->document()->toPlainText());

    painter->restore();
}

void CButtonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_cLastPos = event->pos();
    }
}

void CButtonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_cLastPos, event->pos()).length() < 3) //left button clicked
    {
        emit SIGNAL_LeftButtonClicked();
    }
}
