#include "cgoalitem.h"

CGoalItem::CGoalItem(QGraphicsItem *a_pParent) :
    QGraphicsItem(a_pParent)
{
    m_iBorderPenWidth = TASKMANAGER::g_iItemBorderWidth;
    m_iFontSize = TASKMANAGER::g_iItemFontSizeLarge;
    m_cMinBR.setRect(0, 0, 300, 100);
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

void CGoalItem::AddGoalTitleUI()
{
//    QList<CButtonItem*> l_pCommonBtns;

//    //draw the tip
//    CLabelItem* l_pTip = new CLabelItem("What's your goal?", NULL);
//    m_pWorkSpace->addItem(l_pTip);
//    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
//                   -l_pTip->boundingRect().height() * 1.1);
//    //draw line
//    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
//                                              a_pGoalItem->pos().y(), \
//                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
//                                              a_pGoalItem->pos().y()), 4, NULL);
//    m_pWorkSpace->addItem(l_pLine);
//    //draw textinput
//    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
//    l_pTextInput->SetInputTip("Click to enter goal title.");
//    m_pWorkSpace->addItem(l_pTextInput);
//    l_pTextInput->setPos(\
//                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
//                -l_pTextInput->boundingRect().height() / 2);
//    this->centerOn(l_pTextInput);
//    //draw button
//    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
//    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
//    l_pOK->setPos(l_pTextInput->pos().x(), \
//                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 1.2);
//    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
//                      l_pOK->pos().y());
//    m_pWorkSpace->addItem(l_pOK);
//    m_pWorkSpace->addItem(l_pCancel);
//    l_pCommonBtns.append(l_pOK);
//    l_pCommonBtns.append(l_pCancel);

//    //connect signals to slots for setting new goal title
//    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
//            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
//    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
//            a_pGoalItem, SLOT(SLOT_SetGoalTitleProc(QTextDocument*)));

//    //connect signals to slots for cancellation
//    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
//            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));

//    /*connect signals to slots for removing items*/
//    //delete OK button
//    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
//    //delete Cancel button
//    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
//    //delete TextInput item
//    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTextInput);
//    //delete Label item
//    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
//    //delete Line item
//    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);
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
            this->AddGoalTitleUI();
        }
        else if(!m_cGoal.MembersStatus())
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

    //Update size of goal view accroding to the goal title
    QFontMetrics l_cFM(m_cTextFont);
    int l_iDocWidth = 0, l_iDocHeight = 0;
    QTextBlock l_cTxtBlock;
    for(l_cTxtBlock = a_pDoc->begin(); l_cTxtBlock != a_pDoc->end(); \
        l_cTxtBlock = l_cTxtBlock.next())
    {
        int l_iDocWidthTmp = l_cFM.width(l_cTxtBlock.text());
        if(l_iDocWidthTmp > l_iDocWidth)
        {
            l_iDocWidth = l_iDocWidthTmp;
        }
        l_iDocHeight += l_cFM.height();
    }
    if(l_iDocWidth > m_cMinBR.width())
    {
        this->prepareGeometryChange();
        m_cBR.setWidth(l_iDocWidth * 1.1);
    }
    if(l_iDocHeight > m_cMinBR.height())
    {
        this->prepareGeometryChange();
        m_cBR.setHeight(l_iDocHeight * 1.1);
    }

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
    CMemberItem* l_pMemberItem;
    foreach(l_pMemberItem, *a_ppMembers)
    {
        if(l_pMemberItem->GetMemberName().isEmpty() || \
                l_pMemberItem->GetMemberName() == "")
        {
            continue;
        }
        CMember l_cNewMember;
        l_cNewMember.SetName(l_pMemberItem->GetMemberName().toStdWString());
        m_cGoal.AddMember(&l_cNewMember);
    }
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
