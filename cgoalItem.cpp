#include "cgoalitem.h"

CGoalItem::CGoalItem(QGraphicsItem *a_pParent, QGraphicsScene *a_pScene) :
    QGraphicsItem(a_pParent, a_pScene)
{
    m_iBorderPenWidth = TASKMANAGER::g_iItemBorderWidth;
    m_iFontSize = TASKMANAGER::g_iItemFontSizeLarge;
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

    //model properties
    m_blIntro = false;
    m_blMembers = false;
    m_blBkgrnd = false;
    m_blRes = false;
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
    QString l_strTitle = QString::fromStdWString(m_cGoal.GetTitle());
    if(l_strTitle.isEmpty())
    {
        l_strTitle = "Goal";
    }
    painter->drawText(m_cBR, Qt::AlignCenter, l_strTitle);

    painter->restore();
}

void CGoalItem::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    m_iBorderPenWidth = TASKMANAGER::g_iItemBorderWidth * 2;
    m_blFontBold = !m_blFontBold;
    this->update(m_cBR);
}

void CGoalItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_cLastPos = event->pos();
    QGraphicsItem::mousePressEvent(event);
    event->setAccepted(true);
}

void CGoalItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && \
            QLineF(m_cLastPos, event->pos()).length() < 3) //left button clicked
    {
        if(!m_cGoal.TitleStatus())
        {
            emit this->SIGNAL_AddGoalTitle(this);
        }
        else if(!m_blMembers)
        {
            emit this->SIGNAL_AddGoalMembers(this);
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
        else if(!m_blRes)
        {
            emit this->SIGNAL_AddGoalRes(this);
        }
        else
        {
            emit this->SIGNAL_ShowGoal(this);
        }
    }
}

void CGoalItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    m_iBorderPenWidth = TASKMANAGER::g_iItemBorderWidth;
    m_blFontBold = !m_blFontBold;
    this->update(m_cBR);
}

void CGoalItem::SLOT_AppearItemProc()
{
    if(!this->isVisible())
    {
        this->setVisible(true);
    }
    this->setScale((qreal)(m_iFrameId + 1) / (qreal)TASKMANAGER::g_iItemAppearFrames);
    if(++m_iFrameId >= TASKMANAGER::g_iItemAppearFrames)
    {
        m_iFrameId = 0;
        emit this->SIGNAL_AnimEnd();
    }
}

void CGoalItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CGoalItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CGoalItem::SLOT_SetGoalTitleProc(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    Q_UNUSED(a_pDoc);
    TB_cout << "[CGoalItem] Set goal title as \"" << a_pDoc->toPlainText().toStdWString() \
               << "\"" << std::endl;
#endif
    m_cGoal.SetTitle(a_pDoc->toPlainText().toStdWString());

    emit this->SIGNAL_ShowGoal(this);
}

void CGoalItem::SLOT_ShowGoalEmit()
{
    emit this->SIGNAL_ShowGoal(this);
}

void CGoalItem::SLOT_RequestMembersEmit()
{
    emit this->SIGNAL_RequestMembers(this);
}

void CGoalItem::SLOT_SetGoalMembersProc(QList<CMemberItem *> *a_ppMembers)
{
    CMemberItem* l_pMember;
    foreach(l_pMember, *a_ppMembers)
    {
#ifdef PF_TEST
        TB_cout << L"[CGoalItem] Add member: " << \
                     l_pMember->GetMemberName().toStdWString() << std::endl;
#endif
    }
    m_blMembers = true;
    emit this->SIGNAL_ShowGoal(this);
    emit this->SIGNAL_RemoveMemberItemsEmit();
}

void CGoalItem::SLOT_SetGoalIntroProc(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    TB_cout << L"[CGoalItem] Set goal introduction as \"" << \
                 a_pDoc->toPlainText().toStdWString() << "\"" << std::endl;
#endif
    m_blIntro = true;
    emit this->SIGNAL_ShowGoal(this);
}

void CGoalItem::SLOT_SetGoalBkgrndProc(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    TB_cout << L"[CGoalItem] Set goal background as \"" << \
                 a_pDoc->toPlainText().toStdWString() << "\"" << std::endl;
#endif
    m_blBkgrnd = true;
    emit this->SIGNAL_ShowGoal(this);
}

void CGoalItem::SLOT_SetGoalStepsProc(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    TB_cout << L"[CGoalItem] Set goal steps as \"" << \
                 a_pDoc->toPlainText().toStdWString() << "\"" << std::endl;
#endif
    m_blSteps = true;
    emit this->SIGNAL_ShowGoal(this);
}

void CGoalItem::SLOT_SetGoalResProc(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    TB_cout << L"[CGoalItem] Set goal resources as \"" << \
                 a_pDoc->toPlainText().toStdWString() << "\"" << std::endl;
#endif
    m_blRes = true;
    emit this->SIGNAL_ShowGoal(this);
}
