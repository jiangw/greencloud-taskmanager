#ifndef CDAYWIDGET_H
#define CDAYWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"

class CDayWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CDayWidget(CGraphicsWidget* a_pParent, const QDate& a_pDate = QDate::currentDate());
    virtual ~CDayWidget();
    void Clear();

    void SetRadius(int a_iRadius);
    void Render();
    void SetDate(const QDate &a_pDate);
    void SetHoursPerDay(int a_iHoursPerDay);
    int GetHoursPerDay();
    bool* GetHourSelMask();

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    int CurrHour(); //get current hour regard of the date

    //override from CGraphicsWidget
    void LeftButtonClicked(QPointF a_CMousePos);

public slots:
    void SLOT_HourChangeProc();

private:
    int m_iHoursPerDay;
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
};

#endif // CDAYWIDGET_H
