#ifndef CDAYITEM_H
#define CDAYITEM_H

#include <QRectF>
#include <QPainter>
#include <QList>
#include <qmath.h>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QGraphicsSimpleTextItem>

#include "gconfig.h"
#include "citemanimation.h"

class CDayItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CDayItem(QGraphicsItem* a_pParent, const QDate& a_pDate = QDate::currentDate());
    virtual ~CDayItem();
    void Clear();

    void SetRadius(int a_iRadius);
    void Render();
    void SetDate(const QDate &a_pDate);
    void SetHoursPerDay(int a_iHoursPerDay);
    int GetHoursPerDay();
    bool* GetHourSelMask();

    //override from QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    int CurrHour(); //get current hour
    int ItemWidth(); //compute item width
    int ItemHeight(); //computer item height

    //override from QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
    void SLOT_HourChangeProc();
    //override from CItemAnimation
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();

private:
    int m_iHoursPerDay;
    QRectF m_CBR; //bounding rect
    QGraphicsSimpleTextItem* m_pDateLabel;
    int m_iRadius; //radius of the time pie
    int m_iExtRad;
    int m_iExtHeight;
    QList<QPainterPath *> m_pHourSliceList;
    QList<QRectF *> m_pHourIdRectList;
    bool* m_pHourSelMask;

    int m_iCurrHour;
    QDate m_CDate;
    bool m_blIsToday;

    QPointF m_CMouseLastPos;

};

#endif // CDAYITEM_H
