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

    //initialize status
    m_eStatus = IDLE;

    //set item behaviors
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

void CGoalItem::ConnDelItemToCommonBtns(QList<CButtonItem *> *a_pCommonBtns, CItemAnimation *a_pDelItem)
{
    CButtonItem* l_pBtn;
    foreach(l_pBtn, *a_pCommonBtns)
    {
        connect(l_pBtn, SIGNAL(SIGNAL_LeftButtonClicked()), \
                a_pDelItem, SLOT(SLOT_RemoveItemEmit()));
    }
    connect(a_pDelItem, SIGNAL(SIGNAL_RemoveItem(QGraphicsItem*,CItemAnimation*)), \
            this, SLOT(SLOT_RemoveItemEmit(QGraphicsItem*,CItemAnimation*)));
}

void CGoalItem::CreateGoalTitleUI()
{
#ifdef PF_TEST
    CTestBox::GetTestBox()->ShowMsg(L"[CGoalItem] Create UI to edit goal title.", CTestBox::LOGMSG);
#endif
    QList<CButtonItem*> l_pCommonBtns;

    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("What's your goal?", this);
    l_pTip->setPos(this->boundingRect().width(), \
                   this->boundingRect().height() / 2 - l_pTip->boundingRect().height());

    //draw line
    CLineItem* l_pLine = new CLineItem(QLineF(this->boundingRect().width(), \
                                              this->boundingRect().height() / 2, \
                                              this->boundingRect().width() + l_pTip->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                                              this->boundingRect().height() / 2), 4, this);

    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(this);
    l_pTextInput->SetInputTip("Click to enter goal title.");
    l_pTextInput->setPos(\
                this->boundingRect().width() + l_pLine->Length(), \
                this->boundingRect().height() / 2 - l_pTextInput->boundingRect().height() / 2);

    //draw button
    CButtonItem* l_pOK = new CButtonItem("OK", this);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", this);
    l_pOK->setPos(l_pTextInput->pos().x(), \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() - TASKMANAGER::g_iItemIntervalY);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                      l_pOK->pos().y());
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    //connect signals to slots for setting new goal title
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
            this, SLOT(SLOT_SetGoalTitleProc(QTextDocument*)));

    //connect signals to slots for cancellation
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            this, SLOT(SLOT_ShowGoalEmit()));

    /*connect signals to slots for removing items*/
    //delete OK button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
    //delete Cancel button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
    //delete TextInput item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTextInput);
    //delete Label item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
    //delete Line item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);

    emit this->SIGNAL_CenterOnItem(l_pTextInput);
}

void CGoalItem::CreateTasksUI()
{
#ifdef PF_TEST
    CTestBox::GetTestBox()->ShowMsg(L"[CGoalItem] Create UI to edit goal tasks.");
#endif

    QList<CButtonItem *> l_pCommonBtns;
    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("What's your plan of tasks?", this);
    l_pTip->setPos(this->boundingRect().width(), \
                   this->boundingRect().height() / 2 - l_pTip->boundingRect().height());
    //draw the line
    CLineItem* l_pLine = new CLineItem(QLineF(this->boundingRect().width(), \
                                              this->boundingRect().height() / 2, \
                                              this->boundingRect().width() + l_pTip->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                                              this->boundingRect().height() / 2), 4, this);
    //draw buttons
    CButtonItem* l_pAddBtn = new CButtonItem("+", this);
    CButtonItem* l_pDelBtn = new CButtonItem("-", this);
    CButtonItem* l_pOK = new CButtonItem("OK", this);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", this);
    QPointF l_cPos(this->boundingRect().width() + l_pLine->Length() + 1, \
                   this->boundingRect().height() / 2 - l_pAddBtn->boundingRect().height() / 2);
    l_pAddBtn->setPos(l_cPos);
    l_pDelBtn->setPos(l_cPos.x() + l_pAddBtn->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                      l_cPos.y());
    l_pOK->setPos(l_pDelBtn->pos().x() + l_pDelBtn->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                  l_cPos.y());
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() + TASKMANAGER::g_iItemIntervalX, \
                      l_cPos.y());
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    /*connect signals and slots for cancellation*/
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            this, SLOT(SLOT_ShowGoalEmit()));

    /*connect signals and slots for removing members*/
    //delete Tip item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
    //delete Line item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);
    //delete OK button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
    //delete Cancel button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
    //delete Add button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pAddBtn);
    //delete Delete button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pDelBtn);

    emit this->SIGNAL_CenterOnItem(l_pAddBtn);
}

void CGoalItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    if(event->pos().x() <= m_cBR.width())
    {
        m_iBorderPenWidth = TASKMANAGER::g_iItemBorderWidth * 2;
        m_blFontBold = !m_blFontBold;
        this->update(m_cBR);
    }
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
            if(TITLEINPUT != m_eStatus)
            {
                this->CreateGoalTitleUI();
                m_eStatus = TITLEINPUT;
            }
        }
        else if(!m_cGoal.TasksStatus())
        {
            if(TASKINPUT != m_eStatus)
            {
                this->CreateTasksUI();
                m_eStatus = TASKINPUT;
            }
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

void CGoalItem::SLOT_RemoveItemEmit(QGraphicsItem *a_pGraphicsItem, CItemAnimation *a_pItemAnim)
{
    emit this->SIGNAL_RemoveItem(a_pGraphicsItem, a_pItemAnim);
}

void CGoalItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CGoalItem::SLOT_SetGoalTitleProc(QTextDocument *a_pDoc)
{
#ifdef PF_TEST
    std::wstringstream l_wstrStream;
    l_wstrStream << L"[CGoalItem] Set goal title as: \"" << a_pDoc->toPlainText().toStdWString() \
                 << L"\"";
    CTestBox::GetTestBox()->ShowMsg(l_wstrStream.str(), CTestBox::LOGMSG);
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
    m_eStatus = IDLE;
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
    emit this->SIGNAL_RemoveMemberItems();
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
