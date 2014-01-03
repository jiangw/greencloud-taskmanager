#ifndef CGOALWIDGET_H
#define CGOALWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/csvgwidget.h"
#include "../GraphicsWidgetLib/cwidgetlist.h"
#include "../GraphicsWidgetLib/cbuttonwidget.h"
#include "../GraphicsWidgetLib/ctextwidget.h"
#include "../GraphicsWidgetLib/cmessagewidget.h"
#include "gconfig.h"
#include "ctaskwidget.h"
#include "ccolortagwidget.h"
#include "cplangoal.h"

class CGoalWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    enum EGoalWidgetMode{VIEW,\
                   EDIT};

    CGoalWidget(CGraphicsWidget* a_pParent);
    void SetGoalWidgetMode(EGoalWidgetMode a_EMode);
    EGoalWidgetMode GetGoalWidgetMode(){return m_EMode;}
    void SetGoalData(const CPlanGoal* a_pPlanGoal, bool a_blSimpleView = true);
    void SetGoalColorTag(CGraphicsWidget::gColor a_EColorTag);
    QString GetGoalName(){return m_pGoalNameLabel->GetText();}
    void AddTaskWidget(CTaskWidget* a_pTaskWidget);
    void PlanGoalRevise(int a_iGoalId);
    void PlanGoalSubmit();
    void SetSimpleView(bool a_blSimpleView);

    //override from CGraphicsWidget
    void ResetWidget();
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //override from CGraphicsWidget
    void LeftButtonClicked(QPointF a_CMousePos);

public slots:
    void SLOT_EditProc();
    void SLOT_DeletGoalVerify();
    void SLOT_DeleteProc();
    void SLOT_OKProc();
    void SLOT_CancelEditVerify();
    void SLOT_BackProc();
    void SLOT_AddTaskWidgetProc();
    void SLOT_ChildWidgetSizeChangeProc();
    void SLOT_GoalLabelSizeChangeProc();
    void SLOT_ColorTagChangeProc(CGraphicsWidget::gColor a_EColor);
    void SLOT_TaskWidgetDragDropEmit(QPointF a_CMouseScenePos, CGraphicsWidget* a_pTaskWidget);

    void SLOT_TaskStatusChangeProc(CTaskWidget* a_pTaskWidget);

signals:
    void SIGNAL_GoalTaskSend(QPointF a_CMouseScenePos, int a_iGoalId, int a_iTaskId);
    void SIGNAL_PlanGoalPropose(CGoalWidget* a_pWhoAmI);
    void SIGNAL_PlanGoalSubmit(const CPlanGoal* a_pNewPlanGoal);
    void SIGNAL_PlanGoalRetract(int a_iGoalId);
    void SIGNAL_TaskFinishStatSync(int a_iGoalId, int a_iTaskId, bool a_blIsFinished);

private:
    int GoalLabelWidth();
    int GoalLabelHeight();
    bool IsGoalChanged();
    void GoalDataRecover();

    EGoalWidgetMode m_EMode;

    QString m_qstrGoalName;
    QFont m_CGoalNameFont;
    CTextWidget* m_pGoalNameLabel;

    int m_iControllerWidth, m_iControllerHeight;
    CSvgWidget* m_pSvgWidgetEdit; //svg widget for Edit action
    CSvgWidget* m_pSvgWidgetDel; //svg widget for Delete action
    CSvgWidget* m_pSvgWidgetOK; //svg widget for OK action
    CSvgWidget* m_pSvgWidgetBack; //svg widget for Back action
    CColorTagWidget* m_pColorTag; //widget for setting goal's color tag

    void SetTaskModeBatch(CTaskWidget::ETaskMode a_ETaskMode);
    void SetLabelHeaderForTaskWidgetList(QString a_qstrLabelText);
    void SetAddTaskBtnForTaskWidgetList();
    void UpdateGoalWidget();
    CWidgetList* m_pTaskWidgetList;

    CPlanGoal m_CPlanGoal;
    int m_iTaskIdGen;
    bool m_blSimpleView;
};

#endif // CGOALWIDGET_H
