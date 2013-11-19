#ifndef CGOALWIDGET_H
#define CGOALWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/csvgwidget.h"
#include "../GraphicsWidgetLib/cwidgetlist.h"
#include "../GraphicsWidgetLib/cbuttonwidget.h"
#include "../GraphicsWidgetLib/ctextwidget.h"
#include "gconfig.h"
#include "ctaskwidget.h"

class CGoalWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    enum EGoalMode{VIEW,\
                   EDIT};
    enum EGoalColorTag{PROGRAMMING = Qt::blue,\
                       ECONOMICS = Qt::green,\
                       LEARNENGLISH = Qt::yellow};

    CGoalWidget(CGraphicsWidget* a_pParent);
    void SetGoalMode(EGoalMode a_EMode);
    void SetGoalColorTag(EGoalColorTag a_EColorTag);

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    //override from CGraphicsWidget
    void LeftButtonClicked(QPointF a_CMousePos);

public slots:
    void SLOT_EditProc();
    void SLOT_DeleteProc();
    void SLOT_OKProc();
    void SLOT_BackProc();
    void SLOT_AddTaskWidgetProc();
    void SLOT_ChildWidgetSizeChangeProc();
    void SLOT_GoalLabelSizeChangeProc();
    void SLOT_ColorTagChangeProc();

private:
    int GoalLabelWidth();
    int GoalLabelHeight();

    EGoalMode m_EMode;
    EGoalColorTag m_EColorTag;

    QString m_qstrGoalName;
    int m_iGoalNameLabelWidth, m_iGoalNameLabelHeight;
    QFont m_CGoalNameFont;
    CTextWidget* m_pGoalNameLabel;

    CSvgWidget* m_pUpArrow;

    int m_iControllerWidth, m_iControllerHeight;
    CSvgWidget* m_pSvgWidgetEdit; //svg widget for Edit action
    CSvgWidget* m_pSvgWidgetDel; //svg widget for Delete action
    CSvgWidget* m_pSvgWidgetOK; //svg widget for OK action
    CSvgWidget* m_pSvgWidgetBack; //svg widget for Back action

    void SetTaskModeBatch(CTaskWidget::ETaskMode a_eTaskMode);
    void SetLabelHeaderForTaskWidgetList(QString a_qstrLabelText);
    void SetAddTaskBtnForTaskWidgetList();
    CWidgetList* m_pTaskWidgetList;
};

#endif // CGOALWIDGET_H
