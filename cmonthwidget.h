#ifndef CMONTHWIDGET_H
#define CMONTHWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/ctextwidget.h"
#include "gconfig.h"

class CMonthWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CMonthWidget(CGraphicsWidget* a_pParent,\
                 int a_iYear = QDate::currentDate().year());
    void ResetWidget();

    int GetFirstDayPos(int a_iYear, int a_iMonth);
    int GetDaysInMonth(int a_iYear, int a_iMonth);
    const bool* GetSelMonthMask();
    const int* GetSelDayMask();
    void EnableMultiSelection(bool a_blMutliSel);

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CMonthWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    int CurrMonth();
    int MonthBarId2MonthId(int a_iMonthBarId);
    void InitMasks();
    void SetDayMask(int a_iDay);

    //override from CGraphicsWidget
    void LeftButtonClicked(QPointF a_CMousePos);
    void RightButtonClicked(QPointF a_CMousePos);

public slots:
    void SLOT_SetYearProc(int a_iYear);

signals:
    void SIGNAL_DaySel(QDate a_CSelDate); //emit this signal when a day is selected
    void SIGNAL_DayDesel(QDate a_CDeselDate); //emit this signal when a day is deselected,
                                              //this signal is valid only when MultiSelection is true

private:
    QFont m_CMonthFont;
    QFont m_CDayNameFont;
    QPointF m_CMouseLastPos;
    CTextWidget* m_pDateLabel;
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
    bool m_blMultiSel; //true: multiple selection. false: single selection
};

#endif // CMONTHWIDGET_H
