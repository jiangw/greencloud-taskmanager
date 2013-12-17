#ifndef CGOALTAGWIDGET_H
#define CGOALTAGWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"

class CGoalTagWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CGoalTagWidget(CGraphicsWidget* a_pParent);
    void InitGoalData(int a_iGoalId, QString a_qstrGoalName, Qt::GlobalColor a_EColorTag);
    int GetGoalId(){return m_iGoalId;}
    void SetGoalName(QString a_qstrGoalName);
    QString GetGoalName(){return m_qstrGoalName;}
    void SetGoalColorTag(Qt::GlobalColor a_EColorTag);
    Qt::GlobalColor GetGoalColorTag(){return m_EColorTag;}
    void SetWidgetSelection(bool a_blSelected);

    //override from CGraphicsWidget
    int WidgetWidth(){return m_iWidth;}
    int WidgetHeight(){return m_iHeight;}
    QString WidgetClassName(){return "CGoalTagWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void LeftButtonClicked(QPointF a_CMousePos);

public slots:
    void SLOT_GoalUpdateProc(int a_iGoalId, QString a_qstrGoalName,\
                             Qt::GlobalColor a_EColorTag);


signals:
    void SIGNAL_Selected(CGoalTagWidget* a_pWhoAmI);

private:
    int m_iWidth, m_iHeight;
    bool m_blSelected;

    int m_iGoalId;
    QString m_qstrGoalName;
    Qt::GlobalColor m_EColorTag;
};

#endif // CGOALTAGWIDGET_H
