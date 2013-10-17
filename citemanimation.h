#ifndef CITEMANIMATION_H
#define CITEMANIMATION_H

#include <QObject>
#include <QGraphicsItem>

#include "gconfig.h"

class CItemAnimation : public QObject
{
    Q_OBJECT
public:
    explicit CItemAnimation(QObject* a_pParent = 0);

public slots:
    //remove a item from scene, it contains two steps:
    //step 1 - disappear the item
    //step 2 - delete the item
    virtual void SLOT_RemoveItemEmit() = 0; //reimplement this method to emit SIGNAL_RemoveItem
    virtual void SLOT_AppearItemProc(); //reimplement this method to show appearing animation
    virtual void SLOT_DisappearItemProc(); //reimplement this method to show disappearing animation
    virtual void SLOT_DeleteItemEmit() = 0; //reimplement this method to emit SIGNAL_DeleteItem

signals:
    void SIGNAL_RemoveItem(QGraphicsItem* a_pGraphicsItem, CItemAnimation* a_pItemAnim); //emit this signal when the item needs to be removed
    void SIGNAL_DisappearItem(CItemAnimation* a_pItemAnim); //emit this signal when the item needs to be disappeared
    void SIGNAL_DeleteItem(QGraphicsItem* a_pGraphicsItem); //emit this signal when the item needs to be deleted
    void SIGNAL_AnimEnd(); //emit this signal when the item animation is over

protected:
    int m_iFrameId;
};

#endif // CITEMANIMATION_H
