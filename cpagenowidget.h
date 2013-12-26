#ifndef CPageNOWidget_H
#define CPageNOWidget_H

#include "../GraphicsWidgetLib/cgraphicswidget.h"

class CPageNOWidget : public CGraphicsWidget
{
    Q_OBJECT

public:
    CPageNOWidget(CGraphicsWidget* a_pParent);
    void SetPageNO(int a_iPageNO);
    int GetPageNO(){return m_iPageNo;}
    void Select(bool a_blSel);

    //override from CGraphicsWidget
    int WidgetWidth(){return m_iWidth;}
    int WidgetHeight(){return m_iHeight;}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void LeftButtonClicked(QPointF a_CMousePos);

signals:
    void SIGNAL_PageNOSelected(CPageNOWidget* a_pSelPage);

private:
    int m_iWidth, m_iHeight, m_iPageNo;
    bool m_blSel;
};

#endif // CPageNOWidget_H
