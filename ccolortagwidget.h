#ifndef CCOLORTAGWIDGET_H
#define CCOLORTAGWIDGET_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"
#include "gconfig.h"

class CColorTagWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    enum EShapeStype{RECTANGLE, ELLIPSE};
    CColorTagWidget(int a_iWidgetWidth, int a_iWidgetHeight, CGraphicsWidget* a_pParent);
    void SetWidgetSize(int a_iWidth, int a_iHeight);
    void SetColor(CGraphicsWidget::gColor a_EColor);
    CGraphicsWidget::gColor GetColor();
    void SetShapeStyle(EShapeStype a_EShape);
    void InitSelector();
    void SelectorSwitch(bool a_blSelOn);

    //override from CGraphicsWidget
    void ResetWidget();
    int WidgetWidth();
    int WidgetHeight();
    QString WidgetClassName()\
    {return "CColorTagWidget";}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void LeftButtonClicked(QPointF a_CMousePos);

public slots:
    void SLOT_SetColorProc(CGraphicsWidget::gColor a_EColor);

signals:
    void SIGNAL_ColorSelected(CGraphicsWidget::gColor a_EColor);
    void SIGNAL_ColorChanged(CGraphicsWidget::gColor a_EColor);

private:
    void InitColorList();
    void UpdateSelector();

    QList<CGraphicsWidget::gColor> m_ColorList;
    QList<CColorTagWidget *> m_CTagList;
    bool m_blSelSwitch;
    CGraphicsWidget::gColor m_EColor;
    EShapeStype m_EShapeStyle;
    int m_iWidgetWidth, m_iWidgetHeight;
};

#endif // CCOLORTAGWIDGET_H
