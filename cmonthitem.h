#ifndef CMONTHITEM_H
#define CMONTHITEM_H

#include "gconfig.h"
#include "citemanimation.h"

class CMonthItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CMonthItem(QGraphicsItem* a_pParent, int a_iYear = QDate::currentDate().year());

    //override from QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    int CurrYear();
    int ItemWidth();
    int ItemHeight();

public slots:
    //override from CItemAnimation
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();

private:
    QRectF m_CBR;
    QFont m_CMonthFont;
    QFont m_CDayNameFont;
    int m_iCurrYear;
    int m_iMonthBarHeight;
    int m_iDayNameBarHeight;
    int m_iStartMonth;
    int m_iDayTableCellHeight;
    int m_iDayTableCol, m_iDayTableRow;
};

#endif // CMONTHITEM_H
