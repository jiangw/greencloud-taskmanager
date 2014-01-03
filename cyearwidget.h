#ifndef CYEARWIDGET_H
#define CYEARWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"

class CYearWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CYearWidget(CGraphicsWidget* a_pParent);
    const QList<int>* GetSelYears(){return &m_pSelYears;}
    void SetDurationLength(int a_iYearNumber);
    void EnableMultiSelection(bool a_blMultiSel);

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CYearWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void LeftButtonClicked(QPointF a_CMousePos);

signals:
    void SIGNAL_YearSelected(int a_iYear);

private:
    int m_iStartYear;
    int m_iDurationLength;
    int m_iLineLenPerYear;
    bool m_blMultiSel;
    QList<int> m_pSelYears;
};

#endif // CYEARWIDGET_H
