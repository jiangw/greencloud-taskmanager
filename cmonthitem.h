#ifndef CMONTHITEM_H
#define CMONTHITEM_H

#include "gconfig.h"
#include "citemanimation.h"

class CMonthItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CMonthItem(QGraphicsItem* a_pParent, int a_iYear = QDate::currentDate().year());

    int GetFirstDayPos(int a_iYear, int a_iMonth);
    int GetDaysInMonth(int a_iYear, int a_iMonth);
    const bool* GetSelMonthMask();
    const int* GetSelDayMask();

    //override from QGraphicsItem
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    int CurrMonth();
    int ItemWidth();
    int ItemHeight();
    int MonthBarId2MonthId(int a_iMonthBarId);
    void SetDayMask(int a_iDay);

    //override from QGraphicsItem
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public slots:
    //override from CItemAnimation
    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();

private:
    QRectF m_CBR;
    QFont m_CMonthFont;
    QFont m_CDayNameFont;
    QPointF m_CMouseLastPos;
    QGraphicsSimpleTextItem* m_pDateLabel;
    int m_iYear; //input year
    int m_iShowMonth; //current month to show
    int m_iMonthBarHeight;
    int m_iMonthBarCellNum;
    int m_iDayNameBarHeight;
    int m_iZeroMonth;
    int m_iDayTableCellHeight;
    int m_iDayTableCol, m_iDayTableRow;
    int m_iDateLabelHeight;

    bool m_pSelMonthMask[12];
    int m_pSelDayMask[12];
};

#endif // CMONTHITEM_H
