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

void CWorkSpaceView::SLOT_AddGoalAction()
{
    if(NULL == m_pGoalItem)
    {
        m_pGoalItem = new CGoalItem(NULL, m_pWorkSpace);
        m_pGoalItem->setVisible(false);
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalTitle(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalTitle(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalIntro(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalIntro(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalBkgrnd(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalBkgrnd(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AddGoalSteps(CGoalItem*)), \
                this, SLOT(SLOT_AddGoalSteps(CGoalItem*)));
        connect(m_pGoalItem, SIGNAL(SIGNAL_ShowGoal(CGoalItem*)), \
                this, SLOT(SLOT_ShowGoal(CGoalItem*)));
        //animate item shape
        QTimer* l_pTimer = new QTimer(this);
        connect(l_pTimer, SIGNAL(timeout()), m_pGoalItem, SLOT(SLOT_Animation()));
        connect(m_pGoalItem, SIGNAL(SIGNAL_AnimationOver()), l_pTimer, SLOT(stop()));
        l_pTimer->start(30);
    }
}

void CWorkSpaceView::SLOT_AddGoalTitle(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    std::cout << "Add goal title." << std::endl;
    CTestBox::PrintGraphicsItemLocation(a_pGoalItem);
    Q_UNUSED(a_pGoalItem);
#endif

    //draw tip
    QGraphicsTextItem* l_pTip = m_pWorkSpace->addText("What's your goal?");
#ifdef PF_TEST
    std::cout << "Title tip location" << std::endl;
    CTestBox::PrintGraphicsItemLocation(l_pTip);
#endif
    QPointF l_cPos;
    l_cPos.setX(a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + 4);
    l_cPos.setY(-l_pTip->boundingRect().height() * 1.1);
    l_pTip->setPos(l_cPos);
    //draw line
    m_pWorkSpace->addLine(\
                a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2, \
                a_pGoalItem->pos().y(), \
                a_pGoalItem->pos().x() + a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                a_pGoalItem->pos().y(), QPen(QBrush(Qt::black), 4));
    //draw textinput
    CTextInputItem* l_pTextInput = new CTextInputItem(NULL);
    l_pTextInput->setDocument(new QTextDocument("Input your goal title."));
    m_pWorkSpace->addItem(l_pTextInput);
    l_pTextInput->setPos(\
                a_pGoalItem->boundingRect().width() / 2 + l_pTip->boundingRect().width() + 10, \
                -l_pTextInput->boundingRect().height() / 2);
    //draw button
    CButtonItem* l_pOK = new CButtonItem(NULL);
    l_pOK->setDocument(new QTextDocument("OK"));
    CButtonItem* l_pCancel = new CButtonItem(NULL);
    l_pCancel->setDocument(new QTextDocument("Cancel"));
    m_pWorkSpace->addItem(l_pOK);
    m_pWorkSpace->addItem(l_pCancel);
    l_pOK->setPos(l_pTextInput->pos().x() + l_pOK->boundingRect().width() / 2, \
                  l_pTextInput->pos().y() - l_pOK->boundingRect().height() * 0.8);
    l_pCancel->setPos(l_pOK->pos().x() + l_pOK->boundingRect().width() * 1.3, \
                      l_pOK->pos().y());

    connect(l_pOK, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_SubmitGoalTitle()));
    connect(l_pTextInput, SIGNAL(SIGNAL_SetGoalTitle(QTextDocument*)), \
            a_pGoalItem, SLOT(SLOT_SetGoalTitle(QTextDocument*)));
    connect(l_pCancel, SIGNAL(SIGNAL_LeftButtonClicked()), \
            l_pTextInput, SLOT(SLOT_Cancel()));
    connect(l_pTextInput, SIGNAL(SIGNAL_Cancel()), \
            a_pGoalItem, SLOT(SLOT_ShowGoal()));
}

void CWorkSpaceView::SLOT_AddGoalIntro(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    std::cout << "Add goal introduction." << std::endl;
    Q_UNUSED(a_pGoalItem);
#endif
}

void CWorkSpaceView::SLOT_AddGoalBkgrnd(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    std::cout << "Add goal background." << std::endl;
    Q_UNUSED(a_pGoalItem);
#endif
}

void CWorkSpaceView::SLOT_AddGoalSteps(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    std::cout << "Add goal steps." << std::endl;
    Q_UNUSED(a_pGoalItem);
#endif
}

void CWorkSpaceView::SLOT_ShowGoal(CGoalItem *a_pGoalItem)
{
#ifdef PF_TEST
    std::cout << "Show goal." << std::endl;
    Q_UNUSED(a_pGoalItem);
#endif
}
