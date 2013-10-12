#include "cgoalitem.h"

CGoalItem::CGoalItem(QGraphicsItem *a_pParent, QGraphicsScene *a_pScene) :
    QGraphicsItem(a_pParent, a_pScene)
{
    m_iBorderPenWidth = TASKMANAGER::ItemBorderWidth;
    m_iFontSize = TASKMANAGER::ItemFontSize;
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

    //initialize frame id for animation
    m_iFrameId = 0;

    this->setAcceptHoverEvents(true);

    //临时成员
    m_blTitle = false;
    m_blIntro = false;
    m_blBkgrnd = false;
    m_blSteps = false;
}

QRectF CGoalItem::boundingRect() const
{
    return m_cBR;
}

void CGoalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

void CGoalItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    m_iBorderPenWidth = TASKMANAGER::ItemBorderWidth * 2;
    m_blFontBold = !m_blFontBold;
    this->update(m_cBR);
}

void CGoalItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
#ifdef PF_TEST
    std::cout << "[CGoalItem] Mouse is pressed." << std::endl;
    Q_UNUSED(event);
#endif
    m_cLastPos = event->pos();
    QGraphicsItem::mousePressEvent(event);
    event->setAccepted(true);
}

void CGoalItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && \
            QLineF(m_cLastPos, event->pos()).length() < 3) //left button clicked
    {
        if(!m_blTitle)
        {
            emit this->SIGNAL_AddGoalTitle(this);
        }
        else if(!m_blIntro)
        {
            emit this->SIGNAL_AddGoalIntro(this);
        }
        else if(!m_blBkgrnd)
        {
            emit this->SIGNAL_AddGoalBkgrnd(this);
        }
        else if(!m_blSteps)
        {
            emit this->SIGNAL_AddGoalSteps(this);
        }
        else
        {
            emit this->SIGNAL_ShowGoal(this);
        }
    }
}

void CGoalItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    m_iBorderPenWidth = TASKMANAGER::ItemBorderWidth;
    m_blFontBold = !m_blFontBold;
    this->update(m_cBR);
}

void CGoalItem::SLOT_Animation()
{
    if(!this->isVisible())
    {
        this->setVisible(true);
    }
    qreal l_pScale[5] = {0.2, 0.4, 0.6, 0.8, 1.0};
    this->setScale(l_pScale[m_iFrameId]);
    if(++m_iFrameId > 4)
    {
        m_iFrameId = 0;
        emit this->SIGNAL_AnimationOver();
    }
}

void CGoalItem::SLOT_SetGoalTitle(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    Q_UNUSED(a_pDoc);
    std::cout << "Set goal title as " << a_pDoc->toPlainText().toStdString() << std::endl;
#endif
    m_blTitle = true;

    emit this->SIGNAL_ShowGoal(this);
}

void CGoalItem::SLOT_ShowGoal()
{
    emit this->SIGNAL_ShowGoal(this);
}
