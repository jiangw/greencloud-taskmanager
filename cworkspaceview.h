#ifndef CWORKSPACEVIEW_H
#define CWORKSPACEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPoint>
#include <QMouseEvent>
#include <QTimer>
#include <QList>

#include "gconfig.h"
#include "cworkspace.h"

#include "cdaywidget.h"
#include "cmonthwidget.h"
#include "cyearwidget.h"
#include "cplanwidget.h"
#include "cgoalwidget.h"
#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/cbuttonwidget.h"
#include "../GraphicsWidgetLib/cwidgetlist.h"

class CWorkSpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    CWorkSpaceView(CWorkSpace* a_pWorkSpace, QWidget* a_pParent);

    bool ExportPlan2Img(QString a_qstrImgFileName);
    bool ExportPlan2Svg(QString a_qstrSvgFileName);
    bool ExportWorkSpace2Img(QString a_qstrImgFileName);
    bool ExportWorkSpace2Svg(QString a_qstrSvgFileName);
    
signals:
    
public slots:
    //utils
    void SLOT_CenterOnGraphicsWidgetProc(CGraphicsWidget* a_pWidget);
    void SLOT_DeleteWidgetFromSceneProc(CGraphicsWidget* a_pDelWidget);

    //handle window actions from 'Start' menu
    void SLOT_CreatePlanActionProc();

    //handle window actions from 'View' menu
    void SLOT_DragModeSwitched(bool a_blFlag);
    void SLOT_ResetViewActionProc();

    //for time+task=plan framework
    void SLOT_AddGoalWidgetToWidgetListEmit();

signals:
    //for time+task=plan framework
    void SIGNAL_AddWidgetToWidgetList(CGraphicsWidget* a_pNewWidget);

private:
    CWorkSpace* m_pWorkSpace;
    CPlanWidget* m_pPlanWidget;
    CMonthWidget* m_pMonthWidget;
    CDayWidget* m_pDayWidget;
    CWidgetList* m_pGoalWidgetList;

};

#endif // CWORKSPACEVIEW_H
