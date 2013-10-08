#include "cgoalview.h"

CGoalView::CGoalView(QGraphicsItem *a_pParent, QGraphicsScene *a_pScene) :
    QGraphicsItem(a_pParent, a_pScene)
{
    m_iBorderPenWidth = 4;
    m_iFontSize = 20;
    m_cMinBR.setCoords(-150, -50, 150, 50);
    m_blFontBold = false;

    //config border pen
    m_cBorderPen.setColor(Qt::black);
    m_cBorderPen.setStyle(Qt::SolidLine);

    //config text pen
    m_cTextPen.setColor(Qt::blue);

    //config text font
    m_cTextFont.setFamily("Courier New");
    m_cTextFont.setPointSize(m_iFontSize);

    //config background brush
    m_cBgBrush.setColor(Qt::yellow);
    m_cBgBrush.setStyle(Qt::SolidPattern);

    //initialize bounding rect
    m_cBR = m_cMinBR;

    this->setAcceptHoverEvents(true);
}

QRectF CGoalView::boundingRect() const
{
    return m_cBR;
}

void CGoalView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);

    //draw border
    m_cBorderPen.setWidth(m_iBorderPenWidth);
    painter->setPen(m_cBorderPen);
    QRectF l_cRect(m_cBR.x() + m_iBorderPenWidth / 2, \
                   m_cBR.y() + m_iBorderPenWidth / 2, \
                   m_cBR.width() - m_iBorderPenWidth, \
                   m_cBR.height() - m_iBorderPenWidth);
    painter->drawEllipse(l_cRect);
    //draw background
    QPainterPath l_cPath;
    l_cPath.addEllipse(l_cRect);
    painter->fillPath(l_cPath, m_cBgBrush);
    //draw text
    m_cTextFont.setBold(m_blFontBold);
    painter->setFont(m_cTextFont);
    painter->setPen(m_cTextPen);
    painter->drawText(m_cBR, Qt::AlignCenter, "Goal");

    painter->restore();
}

void CGoalView::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    m_iBorderPenWidth = 8;
    m_blFontBold = true;
    this->update(m_cBR);
}

void CGoalView::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    m_iBorderPenWidth = 4;
    m_blFontBold = false;
    this->update(m_cBR);
}
