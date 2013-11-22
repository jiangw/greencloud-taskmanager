#ifndef CPLANWIDGET_H
#define CPLANWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"
#include "cdaywidget.h"

#include <QPainter>
#include <QImage>
#include <QSvgGenerator>

struct STaskList
{
    void Append(QString a_qstrTaskTag, Qt::GlobalColor a_EGoalColorTag)
    {
        bool l_blInList = false;
        for(int i=0; i<m_qstrTaskTagList.length(); i++)
        {
            if(a_qstrTaskTag == m_qstrTaskTagList[i])
            {
                l_blInList = true;
                break;
            }
        }
        if(!l_blInList)
        {
            m_qstrTaskTagList.append(a_qstrTaskTag);
            m_EGoalColorTagList.append(a_EGoalColorTag);
        }
    }

    QList<QString> m_qstrTaskTagList;
    QList<Qt::GlobalColor> m_EGoalColorTagList;
};

struct STimeSeg
{
    bool IsEmpty()\
    {\
        return m_CStartClockList.isEmpty();\
    }
    QList<int> m_CStartClockList;
    QList<int> m_CEndClockList;
    QList<STaskList *> m_CTaskListList;
};

class CPlanWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CPlanWidget(CGraphicsWidget* a_pParent);
    virtual ~CPlanWidget();
    void Clear();
    void ResetWidget();
    STimeSeg* ReplaceTimeSeg(STimeSeg* a_pOld, STimeSeg* a_pNew);
    void DeleteTimeSeg(STimeSeg* a_pDelTimeSeg);
    void RenderToImg(QImage* a_pImg);
    void RenderToSvg(QSvgGenerator *a_pSVG);

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CPlanWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget* a_pWhoAmI);
    void SLOT_GoalTaskRecieve(QPointF a_CMouseScenePos, QString a_qstrTaskTag,\
                              Qt::GlobalColor a_EGoalColorTag);
    void SLOT_HourSelMaskRequestProc(QDate a_CDate, int a_iHoursPerDay);
    void SLOT_HourSelMaskRecieveFeedbackProc(bool* a_pHourSelMask);

signals:
    void SIGNAL_HourSelMaskSend(bool* a_pHourSelMask, bool a_blFeedback);

private:
    STimeSeg* ConvertHourMask2TimeSeg(const bool* a_pHourMask, int a_iHoursPerDay = 24);
    bool* ConvertTimeSeg2HourMask(STimeSeg* a_pTimeSeg, int a_iHoursPerDay = 24);

    QList<QDate *> m_CDateList;
    QList<STimeSeg *> m_CTimeSegList;
    int m_iHeightPerTimeLine; //= m_iTimeSegHeight + m_iDateTagHeight
    int m_iTimeSegHeight;
    int m_iWidthPerHour;
    int m_iClockTagWidth;
    int m_iDateTagWidth;
    int m_iDateTagHeight;

    QFont m_CTaskTagFont;
    QPen m_CTaskTagPen;
};

#endif // CPLANWIDGET_H
