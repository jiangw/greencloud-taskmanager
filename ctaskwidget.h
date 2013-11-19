#ifndef CTASKWIDGET_H
#define CTASKWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "../GraphicsWidgetLib/ccheckwidget.h"
#include "../GraphicsWidgetLib/ctextwidget.h"
#include "../GraphicsWidgetLib/csvgwidget.h"
#include "gconfig.h"

class CTaskWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    enum ETaskMode{VIEW, /*In VIEW mode, task widget can show task description and can flag the task to different status*/\
                   EDIT/*In Edit mode, task widget can delete and edit task*/};

    CTaskWidget(CGraphicsWidget* a_pParent);
    void SetTaskMode(ETaskMode a_ETaskMode);

    //override from CGraphicsWidget
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CTaskWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void SLOT_TaskStatusChangeProc();
    void SLOT_RemoveWidgetEmit();
    void SLOT_TextWidgetSizeChangeProc();

private:
    CTextWidget* m_pTagWidget; //used to edit and show task text tag
    CTextWidget* m_pDescWidget; //used to edit and show task description
    CCheckWidget* m_pCheckWidget; //used to flag task status
    CSvgWidget* m_pDelWidget; //used to delete task widget

    int m_iControllerSize; //for CCheckWidget and CSvgWidget
    int m_iTagWidth; //fix width of tag widget

    QFont m_CTagFont;
    QFont m_CDescFont;

    ETaskMode m_EMode;
    bool m_blIsTaskFinished;
};

#endif // CTASKWIDGET_H
