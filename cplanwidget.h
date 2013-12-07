#ifndef CPLANWIDGET_H
#define CPLANWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"
#include "cdaywidget.h"
#include "cplan.h"

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
    void PlanTimeHourPaint(QPainter *painter);

public slots:
    void SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget* a_pWhoAmI);
    void SLOT_GoalTaskRecieve(QPointF a_CMouseScenePos, QString a_qstrTaskTag,\
                              Qt::GlobalColor a_EGoalColorTag);

private:
    QList<QDate *> m_CDateList;
    QList<STimeSeg *> m_CTimeSegList;
    CPlan* m_pPlan;
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
