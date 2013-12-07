#include "ccolortagwidget.h"

CColorTagWidget::CColorTagWidget(int a_iWidgetWidth, int a_iWidgetHeight, CGraphicsWidget *a_pParent)
    :CGraphicsWidget(a_pParent)
{
    m_EColor = Qt::blue;
    m_EShapeStyle = ELLIPSE;
    m_iWidgetWidth = a_iWidgetWidth;
    m_iWidgetHeight = a_iWidgetHeight;
    m_blSelSwitch = false;
    this->InitColorList();

    this->InitBoundingRect();
}

void CColorTagWidget::SetWidgetSize(int a_iWidth, int a_iHeight)
{
    m_iWidgetWidth = a_iWidth;
    m_iWidgetHeight = a_iHeight;

    this->UpdateBoundingRect();
}

void CColorTagWidget::SetColor(Qt::GlobalColor a_EColor)
{
    m_EColor = a_EColor;

    this->update(this->boundingRect());
}

Qt::GlobalColor CColorTagWidget::GetColor()
{
    return m_EColor;
}

void CColorTagWidget::SetShapeStyle(EShapeStype a_EShape)
{
    m_EShapeStyle = a_EShape;

    this->update(this->boundingRect());
}

void CColorTagWidget::InitSelector()
{
    if(!m_CTagList.isEmpty())
    {
        return;
    }
    int x = this->boundingRect().width();
    int y = this->boundingRect().height() / 2;

    for(int i=0; i<m_ColorList.length()-1; i++)
    {
        CColorTagWidget* l_pColorTag = new CColorTagWidget(m_iWidgetHeight / 2,\
                                                           m_iWidgetHeight / 2, this);
        l_pColorTag->setPos(3 + x, y);
        l_pColorTag->setVisible(false);
        m_CTagList.append(l_pColorTag);
        x = x + 3 + l_pColorTag->boundingRect().width();

        connect(l_pColorTag, SIGNAL(SIGNAL_ColorSelected(Qt::GlobalColor)),\
                this, SLOT(SLOT_SetColorProc(Qt::GlobalColor)));
    }
}

void CColorTagWidget::SelectorSwitch(bool a_blSelOn)
{
    if(m_CTagList.isEmpty())
    {
        return;
    }

    m_blSelSwitch = a_blSelOn;
    if(m_blSelSwitch)
    {
        this->UpdateSelector();
    }
    for(int i=0; i<m_CTagList.length(); i++)
    {
        if(m_blSelSwitch)
        {
            m_CTagList[i]->setVisible(true);
        }
        else
        {
            m_CTagList[i]->setVisible(false);
        }
    }
}

int CColorTagWidget::WidgetWidth()
{
    return m_iWidgetWidth;
}

int CColorTagWidget::WidgetHeight()
{
    return m_iWidgetHeight;
}

void CColorTagWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QPainterPath l_CShape;
    switch(m_EShapeStyle)
    {
    case RECTANGLE:
        l_CShape.addRect(this->boundingRect());
        break;
    default: //ELLIPSE
        l_CShape.addEllipse(this->boundingRect());
        break;
    }
    painter->fillPath(l_CShape, QBrush(m_EColor));

    painter->restore();
}

void CColorTagWidget::LeftButtonClicked(QPointF a_CMousePos)
{
    Q_UNUSED(a_CMousePos)

    this->SelectorSwitch(!m_blSelSwitch);
    emit this->SIGNAL_ColorSelected(m_EColor);
}

void CColorTagWidget::SLOT_SetColorProc(Qt::GlobalColor a_EColor)
{
    this->SetColor(a_EColor);
    this->UpdateSelector();
    emit this->SIGNAL_ColorChanged(a_EColor);
}

void CColorTagWidget::InitColorList()
{
    m_ColorList.append(Qt::red);
    m_ColorList.append(Qt::darkRed);
    m_ColorList.append(Qt::green);
    m_ColorList.append(Qt::blue);
    m_ColorList.append(Qt::cyan);
    m_ColorList.append(Qt::darkCyan);
    m_ColorList.append(Qt::magenta);
    m_ColorList.append(Qt::yellow);
    m_ColorList.append(Qt::gray);
}

void CColorTagWidget::UpdateSelector()
{
    int j=0;
    for(int i=0; i<m_ColorList.length(); i++)
    {
        if(m_ColorList[i] != m_EColor)
        {
            m_CTagList[j]->SetColor(m_ColorList[i]);
            j++;
        }
    }
}
