#ifndef CDAYITEM_H
#define CDAYITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QList>
#include <qmath.h>
#include <QTime>
#include <QTimer>

#include "gconfig.h"
#include "citemanimation.h"

class CDayItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CDayItem(QGraphicsItem* a_pParent);
    virtual ~CDayItem();
    void Clear();

    //override from QGraphicsItem
    QRectF boundingRect() const;
    void SetRadius(int a_iRadius);
    void Render();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    virtual int CurrHour();

public slots:
    void SLOT_HourChangeProc();
    //override from CItemAnimation
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();

private:
    int m_iHoursPerDay;
    QRectF m_CBR; //bounding rect
    int m_iRadius; //radius of the time pie
    int m_iExtRad;
    QList<QPainterPath *> m_pHourSliceList;
    QList<QRectF *> m_pHourIdRectList;

    int m_iCurrHour;

};

#endif // CDAYITEM_H
