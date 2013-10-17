#include "ctextinputitem.h"

CTextInputItem::CTextInputItem(QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_iFrameId = 0;
    m_pTextItem = new QGraphicsTextItem(this);
//    m_cDoc.setPlainText("Click to enter text.");
    m_pTextItem->setDocument(&m_cDoc);
    m_pTextItem->setTextInteractionFlags(Qt::TextEditorInteraction);

    this->setFlag(QGraphicsItem::ItemIsFocusable);
}

void CTextInputItem::SetInputTip(QString a_strTip)
{
    this->prepareGeometryChange();
    m_cDoc.setPlainText(a_strTip);
}

QString CTextInputItem::GetPlainText()
{
    return m_cDoc.toPlainText();
}

void CTextInputItem::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter)
    {
        std::cout << "[CTextInputItem] Enter key pressed." << std::endl;
    }
}

QRectF CTextInputItem::boundingRect() const
{
    return m_pTextItem->boundingRect();
}

void CTextInputItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    m_pTextItem->paint(painter, option, widget);
    //draw out line
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 4));
    painter->drawRect(this->boundingRect());
    painter->restore();
}

void CTextInputItem::SLOT_SubmitTextEmit()
{
    emit this->SIGNAL_SubmitText(&m_cDoc);
}

void CTextInputItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CTextInputItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CTextInputItem::SLOT_DisappearItemProc()
{
    this->setPos(this->pos().x() - 10, this->pos().y());
    this->setOpacity(this->opacity() - 0.09);
    if(++m_iFrameId >= TASKMANAGER::g_iItemDisappearFrames)
    {
        emit this->SIGNAL_AnimEnd();
    }
}
