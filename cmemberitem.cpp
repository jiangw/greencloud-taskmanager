#include "cmemberitem.h"

CMemberItem::CMemberItem(QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_pSelItem = new CSelectItem(this);
    m_pTextInputItem = new CTextInputItem(this);

    m_pSelItem->setPos(0, 0);
    m_pTextInputItem->setPos(\
                m_pSelItem->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, 0);
}

void CMemberItem::SetInputTip(QString a_strTip)
{
    this->prepareGeometryChange();
    m_pTextInputItem->SetInputTip(a_strTip);
}

bool CMemberItem::IsChecked()
{
    return m_pSelItem->IsChecked();
}

QString CMemberItem::GetMemberName()
{
    return m_pTextInputItem->GetPlainText();
}

QRectF CMemberItem::boundingRect() const
{
    return QRectF(m_pSelItem->pos().x(), m_pSelItem->pos().y(), \
                  m_pSelItem->boundingRect().width() + m_pTextInputItem->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                  m_pSelItem->boundingRect().height() > m_pTextInputItem->boundingRect().height() ? \
                      m_pSelItem->boundingRect().height() : m_pTextInputItem->boundingRect().height());
}

void CMemberItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    m_pSelItem->paint(painter, option, widget);
//    m_pTextInputItem->paint(painter, option, widget);
}

void CMemberItem::SLOT_RemoveItemEmit()
{
    this->SIGNAL_RemoveItem(this, this);
}

void CMemberItem::SLOT_DeleteItemEmit()
{
    this->SIGNAL_DeleteItem(this);
}
