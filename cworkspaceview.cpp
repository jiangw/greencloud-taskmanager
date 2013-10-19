#include "cworkspaceview.h"

CWorkSpaceView::CWorkSpaceView(CWorkSpace *a_pWorkSpace, QWidget *a_pParent) :
    QGraphicsView(a_pWorkSpace, a_pParent)
{
    m_pWorkSpace = a_pWorkSpace;
    m_pGoalItem = NULL;

    this->setMouseTracking(true);
}

CWorkSpaceView::~CWorkSpaceView()
{
    m_pWorkSpace = NULL;
}

void CWorkSpaceView::ConnDelItemToCommonBtns(QList<CButtonItem *>* a_ppCommonBtns, CItemAnimation* a_pDelItem)
{
    CButtonItem* l_pBtn;
    foreach(l_pBtn, *a_ppCommonBtns)
    {
        connect(l_pBtn, SIGNAL(SIGNAL_LeftButtonClicked()), \
                a_pDelItem, SLOT(SLOT_RemoveItemEmit()));
    }
    connect(a_pDelItem, SIGNAL(SIGNAL_RemoveItem(QGraphicsItem*,CItemAnimation*)), \
            this, SLOT(SLOT_RemoveItemProc(QGraphicsItem*,CItemAnimation*)));
}

void CWorkSpaceView::SLOT_DragModeSwitched(bool a_blFlag)
{
    if(a_blFlag)
    {
        this->setDragMode(QGraphicsView::ScrollHandDrag);
        //save last flag of workspace
        m_sLastFlag.m_eWorkSpaceStatus = m_pWorkSpace->m_eStatus;
        //set new flag of workspace
        m_pWorkSpace->m_eStatus = TASKMANAGER::IDLE;
    }
    else
    {
        this->setDragMode(QGraphicsView::NoDrag);
        //restore the flag of workspace
        m_pWorkSpace->m_eStatus = m_sLastFlag.m_eWorkSpaceStatus;
    }
}

void CWorkSpaceView::SLOT_AddGoalActionProc()
{
    if(NULL == m_pGoalItem)
    {
        m_pGoalItem = new CGoalItem(NULL, m_pWorkSpace);
        m_pGoalItem->setVisible(false);
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalTitle(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalTitleProc(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalMembers(CGoalItem*)),
                this, SLOT(SLOT_AddGoalMembersProc(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalIntro(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalIntroProc(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalBkgrnd(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalBkgrndProc(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalSteps(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalStepsProc(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalRes(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalResProc(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_ShowGoal(CGoalItem*)), \
                this, SLOT(SLOT_ShowGoalProc(CGoalItem*)));
        //animate item shape
        QTimer* l_pTimer = new QTimer(this);
        connect(l_pTimer, SIGNAL(timeout()), m_pGoalItem, SLOT(SLOT_AppearItemProc()));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AnimEnd()), l_pTimer, SLOT(stop()));
        l_pTimer->start(30);
    }
}

void CWorkSpaceView::SLOT_AddGoalTitleProc(CGoalItem *a_pGoalItem)
{
    if(TASKMANAGER::ADDGOALTITLE != m_pWorkSpace->m_eStatus)
    {
        m_pWorkSpace->m_eStatus = TASKMANAGER::ADDGOALTITLE;
    }
    else
        return;

    QList<CButtonItem*> l_pCommonBtns;

    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("What's your goal?", NULL);
    m_pWorkSpace->addItem(l_pTip);
    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
                   -l_pTip->boundingRect().height() * 1.1);
    //draw line
    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                                              a_pGoalItem->pos().y(), \
                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                                              a_pGoalItem->pos().y()), 4, NULL);
    m_pWorkSpace->addItem(l_pLine);
    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
    l_pTextInput->SetInputTip("Click to enter goal title.");
    m_pWorkSpace->addItem(l_pTextInput);
    l_pTextInput->setPos(\
                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                -l_pTextInput->boundingRect().height() / 2);
    this->centerOn(l_pTextInput);
    //draw button
    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
    l_pOK->setPos(l_pTextInput->pos().x(), \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 1.2);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
                      l_pOK->pos().y());
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    //connect signals to slots for setting new goal title
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
            a_pGoalItem, SLOT(SLOT_SetGoalTitleProc(QTextDocument*)));

    //connect signals to slots for cancellation
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));

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
}

void CWorkSpaceView::SLOT_AddGoalMembersProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    CTestBox::PrintMsg(L"[CWorkSpaceView] Add goal members.");
#endif
    if(TASKMANAGER::ADDGOALMEMBERS != m_pWorkSpace->m_eStatus)
    {
        m_pWorkSpace->m_eStatus = TASKMANAGER::ADDGOALMEMBERS;
        m_pMemberGroup.clear();
    }
    else
        return;
    QList<CButtonItem *> l_pCommonBtns;
    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("Do you have companions?", NULL);
    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
                   -l_pTip->boundingRect().height() * 1.1);
    m_pWorkSpace->addItem(l_pTip);
    //draw the line
    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                                              a_pGoalItem->pos().y(), \
                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                                              a_pGoalItem->pos().y()), 4, NULL);
    m_pWorkSpace->addItem(l_pLine);
    //draw buttons
    CButtonItem* l_pAddBtn = new CButtonItem("+", NULL);
    CButtonItem* l_pDelBtn = new CButtonItem("-", NULL);
    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
    QPointF l_cPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pLine->Length() + 1, \
                   a_pGoalItem->pos().y() - l_pAddBtn->boundingRect().height() / 2);
    l_pAddBtn->setPos(l_cPos);
    l_pDelBtn->setPos(l_cPos.x() + 1.1 * l_pAddBtn->boundingRect().width(), l_cPos.y());
    l_pOK->setPos(l_pDelBtn->pos().x() + 1.1 * l_pDelBtn->boundingRect().width(), l_cPos.y());
    l_pCancel->setPos(l_pOK->pos().x() + 1.1 * l_pOK->boundingRect().width(), l_cPos.y());
    m_pWorkSpace->addItem(l_pAddBtn);
    m_pWorkSpace->addItem(l_pDelBtn);
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);
    //draw member input item
    CMemberItem* l_pMemberItem = new CMemberItem(NULL);
    l_pMemberItem->setPos(l_pAddBtn->pos().x(), l_pAddBtn->pos().y() + l_pAddBtn->boundingRect().height() + TASKMANAGER::g_iItemIntervalY);
    l_pMemberItem->SetInputTip("Click to input member name.");
    m_pWorkSpace->addItem(l_pMemberItem);
    m_pMemberGroup.append(l_pMemberItem);

    /*connect signals and slots for setting goal members*/
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_RequestMembersEmit()));
    connect(a_pGoalItem, SIGNAL(SIGNAL_RequestMembers(CGoalItem*)), \
            this, SLOT(SLOT_RequestMembersProc(CGoalItem*)));
    /*connect signals and slots for cancellation*/
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));
    /*connect signals and slots for adding members*/
    connect(l_pAddBtn, SIGNAL(SIGNAL_LeftButtonClicked()), \
            this, SLOT(SLOT_AddMemberItemProc()));
    /*connect signals and slots for removing members*/
    connect(l_pDelBtn, SIGNAL(SIGNAL_LeftButtonClicked()), \
            this, SLOT(SLOT_RemoveMemberItemProc()));
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
    //delete Member Items Group
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            this, SLOT(SLOT_RemoveMemberItemGroupProc()));

    //set view focus
    this->centerOn(l_pAddBtn);
}

void CWorkSpaceView::SLOT_AddGoalIntroProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    CTestBox::PrintMsg(L"[CWorkSpaceView] Add goal introduction.");
#endif
    if(TASKMANAGER::ADDGOALINTRO != m_pWorkSpace->m_eStatus)
    {
        m_pWorkSpace->m_eStatus = TASKMANAGER::ADDGOALINTRO;
    }
    else
        return;

    QList<CButtonItem*> l_pCommonBtns;

    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("Describe your goal in detail?", NULL);
    m_pWorkSpace->addItem(l_pTip);
    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
                   -l_pTip->boundingRect().height() * 1.1);
    //draw line
    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                                              a_pGoalItem->pos().y(), \
                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                                              a_pGoalItem->pos().y()), 4, NULL);
    m_pWorkSpace->addItem(l_pLine);
    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
    l_pTextInput->SetInputTip("Click to enter goal introduction.");
    l_pTextInput->setPos(\
                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                -l_pTextInput->boundingRect().height() / 2);
    m_pWorkSpace->addItem(l_pTextInput);
    //draw button
    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
    l_pOK->setPos(l_pTextInput->pos().x(), \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 1.2);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
                      l_pOK->pos().y());
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    /*connect signals and slots for setting goal introduction*/
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
            a_pGoalItem, SLOT(SLOT_SetGoalIntroProc(QTextDocument*)));
    /*connect signals and slots for cancellation*/
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));
    /*connect signals and slots for removing items*/
    //delete OK button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
    //delete Cancel button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
    //delete Tip item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
    //delete Line item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);
    //delete Text Input item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTextInput);

    this->centerOn(l_pTextInput);
}

void CWorkSpaceView::SLOT_AddGoalBkgrndProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    CTestBox::PrintMsg(L"[CWorkSpaceView] Add goal background.");
#endif
    if(TASKMANAGER::ADDGOALBKGRND != m_pWorkSpace->m_eStatus)
    {
        m_pWorkSpace->m_eStatus = TASKMANAGER::ADDGOALBKGRND;
    }
    else
        return;

    QList<CButtonItem*> l_pCommonBtns;

    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("Why do you have the goal?", NULL);
    m_pWorkSpace->addItem(l_pTip);
    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
                   -l_pTip->boundingRect().height() * 1.1);
    //draw line
    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                                              a_pGoalItem->pos().y(), \
                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                                              a_pGoalItem->pos().y()), 4, NULL);
    m_pWorkSpace->addItem(l_pLine);
    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
    l_pTextInput->SetInputTip("Click to enter goal background.");
    l_pTextInput->setPos(\
                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                -l_pTextInput->boundingRect().height() / 2);
    m_pWorkSpace->addItem(l_pTextInput);
    //draw button
    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
    l_pOK->setPos(l_pTextInput->pos().x(), \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 1.2);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
                      l_pOK->pos().y());
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    /*connect signals and slots for setting goal background*/
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
            a_pGoalItem, SLOT(SLOT_SetGoalBkgrndProc(QTextDocument*)));
    /*connect signals and slots for cancellation*/
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));
    /*connect signals and slots for removing items*/
    //delete OK button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
    //delete Cancel button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
    //delete Tip item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
    //delete Line item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);
    //delete Text Input item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTextInput);

    this->centerOn(l_pTextInput);
}

void CWorkSpaceView::SLOT_AddGoalStepsProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    CTestBox::PrintMsg(L"[CWorkSpaceView] Add goal steps.");
#endif
    if(TASKMANAGER::ADDGOALSTEPS != m_pWorkSpace->m_eStatus)
    {
        m_pWorkSpace->m_eStatus = TASKMANAGER::ADDGOALSTEPS;
    }
    else
        return;

    QList<CButtonItem*> l_pCommonBtns;

    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("List steps to achieve the goal?", NULL);
    m_pWorkSpace->addItem(l_pTip);
    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
                   -l_pTip->boundingRect().height() * 1.1);
    //draw line
    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                                              a_pGoalItem->pos().y(), \
                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                                              a_pGoalItem->pos().y()), 4, NULL);
    m_pWorkSpace->addItem(l_pLine);
    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
    l_pTextInput->SetInputTip("Click to enter goal steps.");
    l_pTextInput->setPos(\
                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                -l_pTextInput->boundingRect().height() / 2);
    m_pWorkSpace->addItem(l_pTextInput);
    //draw button
    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
    l_pOK->setPos(l_pTextInput->pos().x(), \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 1.2);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
                      l_pOK->pos().y());
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    /*connect signals and slots for setting goal steps*/
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
            a_pGoalItem, SLOT(SLOT_SetGoalStepsProc(QTextDocument*)));
    /*connect signals and slots for cancellation*/
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));
    /*connect signals and slots for removing items*/
    //delete OK button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
    //delete Cancel button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
    //delete Tip item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
    //delete Line item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);
    //delete Text Input item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTextInput);

    this->centerOn(l_pTextInput);
}

void CWorkSpaceView::SLOT_AddGoalResProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    CTestBox::PrintMsg(L"[CWorkSpaceView] Add goal resources.");
#endif
    if(TASKMANAGER::ADDGOALRES != m_pWorkSpace->m_eStatus)
    {
        m_pWorkSpace->m_eStatus = TASKMANAGER::ADDGOALRES;
    }
    else
        return;

    QList<CButtonItem*> l_pCommonBtns;

    //draw the tip
    CLabelItem* l_pTip = new CLabelItem("What resources will you use?", NULL);
    m_pWorkSpace->addItem(l_pTip);
    l_pTip->setPos(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4, \
                   -l_pTip->boundingRect().height() * 1.1);
    //draw line
    CLineItem* l_pLine = new CLineItem(QLineF(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                                              a_pGoalItem->pos().y(), \
                                              a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                                              a_pGoalItem->pos().y()), 4, NULL);
    m_pWorkSpace->addItem(l_pLine);
    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
    l_pTextInput->SetInputTip("Click to enter goal resources.");
    l_pTextInput->setPos(\
                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                -l_pTextInput->boundingRect().height() / 2);
    m_pWorkSpace->addItem(l_pTextInput);
    //draw button
    CButtonItem* l_pOK = new CButtonItem("OK", NULL);
    CButtonItem* l_pCancel = new CButtonItem("Cancel", NULL);
    l_pOK->setPos(l_pTextInput->pos().x(), \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 1.2);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
                      l_pOK->pos().y());
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pCommonBtns.append(l_pOK);
    l_pCommonBtns.append(l_pCancel);

    /*connect signals and slots for setting goal steps*/
    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitTextEmit()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SubmitText(QTextDocument*)), \
            a_pGoalItem, SLOT(SLOT_SetGoalResProc(QTextDocument*)));
    /*connect signals and slots for cancellation*/
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            a_pGoalItem, SLOT(SLOT_ShowGoalEmit()));
    /*connect signals and slots for removing items*/
    //delete OK button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pOK);
    //delete Cancel button
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pCancel);
    //delete Tip item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTip);
    //delete Line item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pLine);
    //delete Text Input item
    this->ConnDelItemToCommonBtns(&l_pCommonBtns, l_pTextInput);

    this->centerOn(l_pTextInput);
}

void CWorkSpaceView::SLOT_ShowGoalProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    CTestBox::PrintMsg(L"[CWorkSpaceView] Show goal.");
#endif
    this->centerOn(a_pGoalItem);
    m_pWorkSpace->m_eStatus = TASKMANAGER::SHOWGOAL;
}

void CWorkSpaceView::SLOT_RemoveItemProc(QGraphicsItem *a_pGraphicsItem, CItemAnimation *a_pItemAnim)
{
    //delete the item from scene after its disappearing animaiton ends
    connect(a_pItemAnim, SIGNAL(SIGNAL_AnimEnd()), \
            a_pItemAnim, SLOT(SLOT_DeleteItemEmit()));
    connect(a_pItemAnim, SIGNAL(SIGNAL_DeleteItem(QGraphicsItem*)), \
            this, SLOT(SLOT_DeleteItemProc(QGraphicsItem*)));

    //disappear the item by showing animation
    QTimer* l_pTimer = new QTimer(this);
    connect(l_pTimer, SIGNAL(timeout()), a_pItemAnim, SLOT(SLOT_DisappearItemProc()));
    connect(a_pItemAnim, SIGNAL(SIGNAL_AnimEnd()), l_pTimer, SLOT(stop()));
    l_pTimer->start(30);
}

void CWorkSpaceView::SLOT_DeleteItemProc(QGraphicsItem *a_pItem)
{
    m_pWorkSpace->removeItem(a_pItem);
}

void CWorkSpaceView::SLOT_RequestMembersProc(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    std::cout << "[CWorkSpaceView] Set goal members" << std::endl;
#endif
    connect(a_pGoalItem, SIGNAL(SIGNAL_RemoveMemberItemsEmit()), \
            this, SLOT(SLOT_RemoveMemberItemGroupProc()));
    //set members for the goal
    //this slot will emit SIGNAL_RemoveMemberItemsEmit
    a_pGoalItem->SLOT_SetGoalMembersProc(&m_pMemberGroup);
}

void CWorkSpaceView::SLOT_AddMemberItemProc()
{
#ifdef PF_TEST
    std::cout << "[CWorkSpaceView] Add member item." << std::endl;
#endif
    CMemberItem* l_pLastItem = m_pMemberGroup.at(m_pMemberGroup.size() - 1);
    CMemberItem* l_pNewItem = new CMemberItem(NULL);
    l_pNewItem->SetInputTip("Click to input member name.");
    l_pNewItem->setPos(l_pLastItem->pos().x(), \
                       l_pLastItem->pos().y() + l_pLastItem->boundingRect().height() \
                       + TASKMANAGER::g_iItemIntervalY);
    m_pWorkSpace->addItem(l_pNewItem);
    m_pMemberGroup.append(l_pNewItem);
}

void CWorkSpaceView::SLOT_RemoveMemberItemProc()
{
#ifdef PF_TEST
    std::cout << "[CWorkSpaceView] Remove member item." << std::endl;
#endif
    qreal l_dMoveUpDist = 0;
    for(int i=0; i<m_pMemberGroup.size(); i++)
    {
        CMemberItem* l_pItem = m_pMemberGroup[i];
        if(l_dMoveUpDist != 0)
        {
            l_pItem->setPos(l_pItem->pos().x(), l_pItem->pos().y() - l_dMoveUpDist);
        }
        if(l_pItem->IsChecked())
        {
            m_pMemberGroup.removeAt(i);
            i--;
            l_dMoveUpDist += l_pItem->boundingRect().height() + TASKMANAGER::g_iItemIntervalY;
            //delete item from scene
            connect(l_pItem, SIGNAL(SIGNAL_RemoveItem(QGraphicsItem*,CItemAnimation*)), \
                    this, SLOT(SLOT_RemoveItemProc(QGraphicsItem*,CItemAnimation*)));
            l_pItem->SLOT_RemoveItemEmit();
        }
    }
}

void CWorkSpaceView::SLOT_RemoveMemberItemGroupProc()
{
#ifdef PF_TEST
    std::cout << "[CWorkSpaceView] Remove member items group." << std::endl;
#endif
    CMemberItem* l_pItem;
    foreach(l_pItem, m_pMemberGroup)
    {
        connect(l_pItem, SIGNAL(SIGNAL_RemoveItem(QGraphicsItem*,CItemAnimation*)), \
               this, SLOT(SLOT_RemoveItemProc(QGraphicsItem*,CItemAnimation*)));
        l_pItem->SLOT_RemoveItemEmit();
    }
}
