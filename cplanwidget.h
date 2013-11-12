#ifndef CPLANWIDGET_H
#define CPLANWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"
#include "cdaywidget.h"

struct STimeSeg
{
    bool IsEmpty()\
    {\
        return m_CStartClockList.isEmpty();\
    }
    QList<int> m_CStartClockList;
    QList<int> m_CEndClockList;
};

class CPlanWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CPlanWidget(CGraphicsWidget* a_pParent);
    virtual ~CPlanWidget();
    void Clear();

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CPlanWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget* a_pWhoAmI);

private:
    STimeSeg* ConvertHourMask2TimeSeg(const bool* a_pHourMask);

    QList<QDate *> m_CDateList;
    QList<STimeSeg *> m_CTimeSegList;
    int m_iHeightPerTimeLine; //= m_iTimeSegHeight + m_iDateTagHeight
    int m_iTimeSegHeight;
    int m_iWidthPerHour;
    int m_iClockTagWidth;
    int m_iDateTagWidth;
    int m_iDateTagHeight;
};

#endif // CPLANWIDGET_H
