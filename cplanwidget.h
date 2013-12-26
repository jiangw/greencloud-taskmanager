#ifndef CPLANWIDGET_H
#define CPLANWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/cwidgetlist.h"
#include "../GraphicsWidgetLib/ctextwidget.h"
#include "gconfig.h"
#include "cdaywidget.h"
#include "cplan.h"
#include "cpagenowidget.h"

#include <QPainter>
#include <QImage>
#include <QSvgGenerator>

class CPlanWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    enum EPlanPageFlag{PPF_BEGIN, PPF_END, PPF_SPECIFY};
    CPlanWidget(CGraphicsWidget* a_pParent);
    virtual ~CPlanWidget();
    void Clear();
    void ResetWidget();
    void RenderToImg(QImage* a_pImg);
    void RenderToSvg(QSvgGenerator *a_pSVG);
    void SetTimePage(GREENSCHEDULE::ETimePage a_ETimePage);
    void SetPlanPage(int a_iPageNO);

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CPlanWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void PlanTimeHourPaint(QPainter *painter);

public slots:
    void SLOT_MouseDragDropProc(QPointF a_CMouseScenePos, CGraphicsWidget* a_pWhoAmI);
    void SLOT_GoalTaskRecieve(QPointF a_CMouseScenePos, int a_iGoalId, int a_iTaskId);
    void SLOT_WidgetUpdateProc();
    void SLOT_SetPageNOProc(CPageNOWidget* a_pSelPage);
    void SLOT_ShowPlanInProgressProc();
    void SLOT_ShowPlanHistoryProc();

private:
    CPlan* m_pPlan;
    int m_iHeightPerTimeLine; //= m_iTimeSegHeight + m_iDateTagHeight
    int m_iTimeSegHeight;
    int m_iWidthPerHour;
    int m_iClockTagWidth;
    int m_iDateTagWidth;
    int m_iDateTagHeight;
    int m_iTimeLineStartY;
    QBrush m_CBackground;

    QFont m_CTaskTagFont;
    QPen m_CTaskTagPen;

    GREENSCHEDULE::ETimePage m_ETimePage;
    EPlanPageFlag m_EPlanPageFlag;
    int m_iDaysPerPage, m_iTotalPages, m_iPageNo;
    CWidgetList* m_pPageList;
    CTextWidget* m_pPageListTitle;
    CPageNOWidget* m_pCurrPage;
};

#endif // CPLANWIDGET_H
