#include "cplan.h"

CPlan* CPlan::s_pPlan = NULL;

CPlan::CPlan(QObject *parent) :
    QObject(parent)
{
    m_iGoalIdGen = 0;
    m_blSaveFlag = false;

    QTimer* l_pTimer = new QTimer(this);
    connect(l_pTimer, SIGNAL(timeout()), this, SLOT(SLOT_UpdateTimePage()));
    l_pTimer->start(30 * 60 * 1000); //update time page in every 30 minutes
}

CPlan::~CPlan()
{
    this->ClearPlanTimeHourFactory();
    this->ClearPlanGoalFactory();
}

void CPlan::InitPlan(QObject *a_pParent)
{
    if(NULL == s_pPlan)
    {
        s_pPlan = new CPlan(a_pParent);
    }
}

CPlan* CPlan::GetPlan()
{
    if(NULL == s_pPlan)
    {
        s_pPlan = new CPlan();
    }
    return s_pPlan;
}

void CPlan::FreePlan()
{
    if(NULL != s_pPlan)
    {
        delete s_pPlan;
    }
}

CPlanTimeHour* CPlan::CreatePlanTimeHour(QDate a_CDate)
{
    CPlanTimeHour* l_pPlanTimeHour = this->GetPlanTimeHour(a_CDate);
    if(NULL == l_pPlanTimeHour)
    { //the plan-time was not created before
        l_pPlanTimeHour = new CPlanTimeHour(a_CDate);
        //insert new object by time order
        int l_iInsertPos = m_CPlanTimeHourFactory.length();
        for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
        {
            if(m_CPlanTimeHourFactory[i]->Date() > a_CDate)
            {
                l_iInsertPos = i;
                break;
            }
        }
        m_CPlanTimeHourFactory.insert(l_iInsertPos, l_pPlanTimeHour);
    }

    m_blSaveFlag = true;
    return l_pPlanTimeHour;
}

CPlanTimeHour* CPlan::GetPlanTimeHour(QDate a_CDate)
{
    CPlanTimeHour* l_pPlanTimeHour = NULL;
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(m_CPlanTimeHourFactory[i]->Date() == a_CDate)
        {
            l_pPlanTimeHour = m_CPlanTimeHourFactory[i];
            break;
        }
    }
    return l_pPlanTimeHour;
}

QList<CPlanTimeHour*>& CPlan::GetPlanTimeHourListInProgress()
{
    m_CPlanTimeHourInProgress.clear();
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(!m_CPlanTimeHourFactory[i]->IsHistory())
        {
            m_CPlanTimeHourInProgress.append(m_CPlanTimeHourFactory[i]);
        }
    }
    return m_CPlanTimeHourInProgress;
}

void CPlan::UpdatePlanTimeHour(CPlanTimeHour *a_pPlanTimeHour,\
                               const bool *a_pDayHourMask, int a_iHoursPerDay)
{
    if(NULL != a_pPlanTimeHour)
    {
        a_pPlanTimeHour->SetTimeSeg(a_pDayHourMask, a_iHoursPerDay);
    }
    m_blSaveFlag = true;
}

void CPlan::RemovePlanTimeHour(CPlanTimeHour *a_pDelPlanTime)
{
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(m_CPlanTimeHourFactory[i] == a_pDelPlanTime)
        {
            m_CPlanTimeHourFactory.removeAt(i);
            break;
        }
    }
    m_blSaveFlag = true;
}

CPlanGoal* CPlan::CreatePlanGoal(QString a_qstrGoalName, CGraphicsWidget::gColor a_EColor)
{
    CPlanGoal* l_pPlanGoal = NULL;
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        if(m_CPlanGoalFactory[i]->GetGoalName() == a_qstrGoalName)
        { //the plan-goal is already created
            l_pPlanGoal = m_CPlanGoalFactory[i];
            break;
        }
    }
    if(NULL == l_pPlanGoal)
    { //the plan-goal was not created before
        l_pPlanGoal = new CPlanGoal(a_qstrGoalName, a_EColor);
        l_pPlanGoal->SetGoalId(m_iGoalIdGen);
        m_iGoalIdGen++;
        m_CPlanGoalFactory.append(l_pPlanGoal);
        m_blSaveFlag = true;
        emit this->SIGNAL_PlanGoalCreated(l_pPlanGoal);
    }
    else
    {
        l_pPlanGoal = NULL;
    }

    return l_pPlanGoal;
}

CPlanGoal* CPlan::CreateEmptyPlanGoal()
{
    return this->CreatePlanGoal("", (CGraphicsWidget::gColor)0);
}

CPlanGoal *CPlan::GetPlanGoalById(int a_iGoalId)
{
    CPlanGoal* l_pPlanGoal = NULL;
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        if(m_CPlanGoalFactory[i]->GetGoalId() == a_iGoalId)
        {
            l_pPlanGoal = m_CPlanGoalFactory[i];
            break;
        }
    }

    return l_pPlanGoal;
}

CPlanGoal* CPlan::GetPlanGoalByName(QString a_qstrGoalName)
{
    CPlanGoal* l_pPlanGoal = NULL;
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        if(m_CPlanGoalFactory[i]->GetGoalName() == a_qstrGoalName)
        {
            l_pPlanGoal = m_CPlanGoalFactory[i];
            break;
        }
    }

    return l_pPlanGoal;
}

CPlanGoal* CPlan::GetPlanGoalByColorTag(CGraphicsWidget::gColor a_EColor)
{
    CPlanGoal* l_pPlanGoal = NULL;
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        if(m_CPlanGoalFactory[i]->GetGoalColor() == a_EColor)
        {
            l_pPlanGoal = m_CPlanGoalFactory[i];
            break;
        }
    }

    return l_pPlanGoal;
}

void CPlan::RemovePlanGoal(int a_iGoalId)
{
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        if(m_CPlanGoalFactory[i]->GetGoalId() == a_iGoalId)
        {
            for(int j=0; j<m_CPlanTimeHourFactory.length(); j++)
            {
                const STask* l_pTask = m_CPlanGoalFactory[i]->GetTaskListHead();
                while(l_pTask)
                {
                    m_CPlanTimeHourFactory[j]->DeleteTaskAbstract(a_iGoalId,\
                                                                  l_pTask->m_iTaskId);
                    l_pTask = l_pTask->m_pNext;
                }
            }
            m_CPlanGoalFactory.removeAt(i);
            break;
        }
    }

    m_blSaveFlag = true;
    emit this->SIGNAL_PlanWidgetUpdate();
}

int CPlan::GetPlanGoalIndex(int a_iGoalId)
{
    int l_iIdx = -1;
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        if(m_CPlanGoalFactory[i]->GetGoalId() == a_iGoalId)
        {
            l_iIdx = i;
            break;
        }
    }
    return l_iIdx;
}

bool CPlan::SavePlan(QString a_qstrFileName)
{
    QFile l_COutFile(a_qstrFileName);
    if(!l_COutFile.open(QIODevice::WriteOnly))
    {
        std::cerr << "Cannot open saving file." << std::endl;
        return false;
    }
    QDataStream l_COut(&l_COutFile);
    /*------------save PlanGoal list------------*/
    //save the length of PlanGoal list
    l_COut << m_CPlanGoalFactory.length();
    //save the data of each PlanGoal
    for(int i=0; i<m_CPlanGoalFactory.length(); i++)
    {
        CPlanGoal* l_pPlanGoal = m_CPlanGoalFactory[i];
        l_COut << l_pPlanGoal->GetGoalName();
        l_COut << (unsigned int)(l_pPlanGoal->GetGoalColor());
        l_COut << l_pPlanGoal->TaskNumber();
        //save task list
        int l_iTaskCounter = 0;//used to verify task number
        const STask* l_pTaskListIter = l_pPlanGoal->GetTaskListHead();
        while(l_pTaskListIter)
        {
            l_COut << l_pTaskListIter->m_qstrTaskTag;
            l_COut << l_pTaskListIter->m_qstrDescription;
            l_COut << l_pTaskListIter->m_blIsFinished;
            l_pTaskListIter = l_pTaskListIter->m_pNext;
            l_iTaskCounter++;
        }
        //verify task number
        if(l_pPlanGoal->TaskNumber() != l_iTaskCounter)
        {
            std::cerr << "Task number of GOAL: " << l_pPlanGoal->GetGoalName().toStdString()\
                      << " is wrong." << std::endl;
            this->FailFileIOCatch(&l_COutFile);
            return false;
        }
    }
    /*------------save PlanTimeHour list------------*/
    //save the number of PlanTimeHour list
    l_COut << m_CPlanTimeHourFactory.length();
    //save the data of each PlanTimeHour
    int l_iTaskIndices[MAXTASKNUMINTIMESEG]; //used to save task indices in time segments
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        CPlanTimeHour* l_pPlanTimeHour = m_CPlanTimeHourFactory[i];
        //save date
        l_COut << l_pPlanTimeHour->Date().year();
        l_COut << l_pPlanTimeHour->Date().month();
        l_COut << l_pPlanTimeHour->Date().day();
        //save the number of task list
        l_COut << l_pPlanTimeHour->TaskCounter();
        int l_iCounter = 0; //used to verify task number
        //save task list
        STaskAbstractHour* l_pTaskAbsListIter = l_pPlanTimeHour->GetTaskAbstract(0);
        while(l_pTaskAbsListIter)
        {
            int l_iGoalIndex = CPlan::GetPlan()->GetPlanGoalIndex(l_pTaskAbsListIter->m_iGoalId);
            int l_iTaskIndex = CPlan::GetPlanGoalById(l_pTaskAbsListIter->m_iGoalId)\
                                        ->GetTaskIndex(l_pTaskAbsListIter->m_iTaskId);
            l_COut << l_iGoalIndex;
            l_COut << l_iTaskIndex;
            l_pTaskAbsListIter = l_pTaskAbsListIter->m_pNext;
            l_iCounter++;
        }
        if(l_iCounter != l_pPlanTimeHour->TaskCounter())
        {
            std::cerr << "TaskAbstract number is wrong." << std::endl;
            this->FailFileIOCatch(&l_COutFile);
            return false;
        }
        //save the number of time segments
        l_COut << l_pPlanTimeHour->TimeSegCounter();
        //save time segments list
        l_iCounter = 0; //used to verify the number of time segments
        STimeSegHour* l_pTimeSegHourListHead = l_pPlanTimeHour->GetTimeSeg(0);
        while(l_pTimeSegHourListHead)
        {
            l_COut << l_pTimeSegHourListHead->m_iStartClock;
            l_COut << l_pTimeSegHourListHead->m_iEndClock;
            QList<STaskAbstractHour*> l_CTaskAbsListInTimeSeg = l_pTimeSegHourListHead->m_CTaskList;
            for(int j=0; j<l_CTaskAbsListInTimeSeg.length(); j++)
            {
                l_iTaskIndices[j] = l_pPlanTimeHour->GetTaskIndex(\
                                                l_CTaskAbsListInTimeSeg[j]->m_iGoalId,\
                                                l_CTaskAbsListInTimeSeg[j]->m_iTaskId);
            }
            l_COut << l_CTaskAbsListInTimeSeg.length();
            l_COut.writeRawData((char *)&l_iTaskIndices,\
                                l_CTaskAbsListInTimeSeg.length() * sizeof(int));
            l_pTimeSegHourListHead = l_pTimeSegHourListHead->m_pNext;
            l_iCounter++;
        }
        if(l_iCounter != l_pPlanTimeHour->TimeSegCounter())
        {
            std::cerr << "TimeSeg number is wrong." << std::endl;
            this->FailFileIOCatch(&l_COutFile);
            return false;
        }
    }

    l_COutFile.close();
    m_blSaveFlag = false;
    return true;
}

bool CPlan::LoadPlan(QString a_qstrFileName)
{
    QFile l_CInFile(a_qstrFileName);
    if(!l_CInFile.open(QIODevice::ReadOnly))
    {
        std::cerr << "Cannot load plan file." << std::endl;
        emit this->SIGNAL_ShowInMessageBox("Cannot load plan file.");
        return false;
    }
    QDataStream l_CIn(&l_CInFile);
    //clear current plan
    this->ClearPlan();
    /*------------load PlanGoal list------------*/
    //load goals number
    int l_iGoalNumber;
    l_CIn >> l_iGoalNumber;
    //load goals list
    QString l_qstrGoalName, l_qstrTaskTag, l_qstrTaskDesc;
    int l_iTaskNumber;
    unsigned int l_iGoalColorTag;
    bool l_blTaskFinishStat;
    for(int i=0; i<l_iGoalNumber; i++)
    {
        //load goal info
        l_CIn >> l_qstrGoalName;
        l_CIn >> l_iGoalColorTag;
        CPlanGoal* l_pGoal = this->CreatePlanGoal(l_qstrGoalName,\
                                                  (CGraphicsWidget::gColor)l_iGoalColorTag);
        //load task list
        l_CIn >> l_iTaskNumber;
        int l_iTaskIdGen = -1;
        for(int j=0; j<l_iTaskNumber; j++)
        {
            l_CIn >> l_qstrTaskTag;
            l_CIn >> l_qstrTaskDesc;
            l_CIn >> l_blTaskFinishStat;
            l_pGoal->AddTask(++l_iTaskIdGen, l_qstrTaskTag, l_qstrTaskDesc, l_blTaskFinishStat);
        }
    }
    /*------------load PlanTimeHour list------------*/
    //load PlanTimeHour number
    int l_iPlanTimeHourNum;
    l_CIn >> l_iPlanTimeHourNum;
    //load time segments and tasks list
    int l_iTaskIndices[MAXTASKNUMINTIMESEG];
    int l_iYear, l_iMonth, l_iDay, l_iGoalIndex, l_iTaskIndex;
    for(int i=0; i<l_iPlanTimeHourNum; i++)
    {
        //read date
        l_CIn >> l_iYear;
        l_CIn >> l_iMonth;
        l_CIn >> l_iDay;
        CPlanTimeHour* l_pPlanTimeHour = this->CreatePlanTimeHour(\
                                                    QDate(l_iYear, l_iMonth, l_iDay));
        //load tasks list
        l_CIn >> l_iTaskNumber;
        for(int j=0; j<l_iTaskNumber; j++)
        {
            l_CIn >> l_iGoalIndex;
            l_CIn >> l_iTaskIndex;
            if(l_iGoalIndex < 0 || l_iGoalIndex >= m_CPlanGoalFactory.length()\
                    || l_iTaskIndex < 0)
            {
                std::cerr << "Invalid goal or task." << std::endl;
                emit this->SIGNAL_ShowInMessageBox("Invalid goal or task.");
                l_pPlanTimeHour->CreateTaskAbstract(-1, "Error", -1, "Error", Qt::white);
            }
            else
            {
                STaskAbstractHour* l_pTaskAbs = l_pPlanTimeHour->CreateTaskAbstract(\
                                                    m_CPlanGoalFactory[l_iGoalIndex]->GetGoalId(),\
                                                    m_CPlanGoalFactory[l_iGoalIndex]->GetGoalName(),\
                                                    m_CPlanGoalFactory[l_iGoalIndex]->GetTaskByIndex(\
                                                            l_iTaskIndex)->m_iTaskId,\
                                                    m_CPlanGoalFactory[l_iGoalIndex]->GetTaskByIndex(\
                                                            l_iTaskIndex)->m_qstrTaskTag,\
                                                    m_CPlanGoalFactory[l_iGoalIndex]->GetGoalColor());
                l_pTaskAbs->m_blIsFinished =\
                                m_CPlanGoalFactory[l_iGoalIndex]->GetTaskByIndex(l_iTaskIndex)->m_blIsFinished;
            }
        }
        //load time segments
        int l_iTimeSegNum, l_iStartClock, l_iEndClock, l_iTaskNumInTimeSeg;
        l_CIn >> l_iTimeSegNum;
        for(int j=0; j<l_iTimeSegNum; j++)
        {
            l_CIn >> l_iStartClock;
            l_CIn >> l_iEndClock;
            STimeSegHour* l_pTimeSeg = l_pPlanTimeHour->CreateTimeSeg(l_iStartClock, l_iEndClock);
            l_CIn >> l_iTaskNumInTimeSeg;
            l_CIn.readRawData((char *)&l_iTaskIndices, l_iTaskNumInTimeSeg * sizeof(int));
            for(int k=0; k<l_iTaskNumInTimeSeg; k++)
            {
                l_pPlanTimeHour->AssignTaskToTimeSeg(l_pTimeSeg, l_pPlanTimeHour->GetTaskAbstract(l_iTaskIndices[k]));
            }
        }
    }

    l_CInFile.close();
    this->SLOT_UpdateTimePage();
    m_blSaveFlag = false;
    return true;
}

void CPlan::FailFileIOCatch(QFile *a_FailedFile)
{
    if(!a_FailedFile->remove())
    {
        a_FailedFile->close();
        a_FailedFile->remove();
    }
}

void CPlan::SLOT_HourSelMaskRequestProc(QDate a_CDate, bool *a_pHourSelMask, int a_iHoursPerDay)
{
    bool l_blDateFound = false;
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        if(m_CPlanTimeHourFactory[i]->Date() == a_CDate)
        {
            l_blDateFound = true;
            m_CPlanTimeHourFactory[i]->ConvertTimeSegToHourMask(a_pHourSelMask, a_iHoursPerDay);
            break;
        }
    }
    if(l_blDateFound)
    {
        emit this->SIGNAL_HourSelMaskRequestFinished();
    }
}

void CPlan::SLOT_PlanGoalReviewProc(CGoalWidget *a_pGoalProposer)
{
    if(NULL == this->GetPlanGoalByName(a_pGoalProposer->GetGoalName()))
    { //the goal is valid
        CPlanGoal* l_pNewGoal = this->CreateEmptyPlanGoal();
        a_pGoalProposer->PlanGoalRevise(l_pNewGoal->GetGoalId());
    }
    else
    { //the goal was created before
        emit this->SIGNAL_ShowInMessageBox("Invalid goal name.");
    }
}

void CPlan::SLOT_PlanGoalSyncProc(const CPlanGoal *a_pPlanGoal)
{
    CPlanGoal* l_pPlanGoal = this->GetPlanGoalById(a_pPlanGoal->GetGoalId());
    if(NULL == l_pPlanGoal)
    {
        std::cerr << "Error[CPlan]: Failed to synchronize Goal '"\
                  << a_pPlanGoal->GetGoalName().toStdString()\
                  << "'. Cannot find the goal in current Plan."<< std::endl;
        return;
    }
    //synchronize the goal
    l_pPlanGoal->SetGoalName(a_pPlanGoal->GetGoalName());
    l_pPlanGoal->SetGoalColor(a_pPlanGoal->GetGoalColor());
    l_pPlanGoal->ClearTaskList();
    const STask* l_pTaskListHead = a_pPlanGoal->GetTaskListHead();
    while(l_pTaskListHead)
    {
        l_pPlanGoal->AddTask(l_pTaskListHead->m_iTaskId,\
                             l_pTaskListHead->m_qstrTaskTag,\
                             l_pTaskListHead->m_qstrDescription,\
                             l_pTaskListHead->m_blIsFinished);
        l_pTaskListHead = l_pTaskListHead->m_pNext;
    }
    //update goal data in time segments
    this->UpdateGoalInPlanTimeHour(l_pPlanGoal);

    m_blSaveFlag = true;
    emit this->SIGNAL_PlanWidgetUpdate();
    emit this->SIGNAL_PlanGoalUpdated(l_pPlanGoal);
}

void CPlan::SLOT_PlanGoalRetractProc(int a_iGoalId)
{
    this->RemovePlanGoal(a_iGoalId);
    emit this->SIGNAL_PlanGoalRemoved(a_iGoalId);
}

void CPlan::SLOT_GoalTaskFinishStatSync(int a_iGoalId, int a_iTaskId,\
                                        bool a_blIsFinished)
{
    CPlanGoal* l_pPlanGoal = this->GetPlanGoalById(a_iGoalId);
    if(NULL != l_pPlanGoal)
    {
        l_pPlanGoal->FinishTask(a_iTaskId, a_blIsFinished);
        for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
        {
            CPlanTimeHour* l_pPlanTimeHour = m_CPlanTimeHourFactory[i];
            STaskAbstractHour* l_pTaskListHead = l_pPlanTimeHour->GetTaskAbstract(0);
            while(l_pTaskListHead)
            {
                if(l_pTaskListHead->m_qstrGoalName == l_pPlanGoal->GetGoalName()\
                        && l_pTaskListHead->m_iTaskId == a_iTaskId)
                {
                    l_pTaskListHead->m_blIsFinished = a_blIsFinished;
                    break;
                }
                l_pTaskListHead = l_pTaskListHead->m_pNext;
            }
        }
    }

    m_blSaveFlag = true;
    emit this->SIGNAL_PlanWidgetUpdate();
}

void CPlan::SLOT_UpdateTimePage()
{
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        m_CPlanTimeHourFactory[i]->UpdateTimeSegPage();
    }
    emit this->SIGNAL_PlanWidgetUpdate();
}

void CPlan::ClearPlan()
{
    this->ClearPlanGoalFactory();
    this->ClearPlanTimeHourFactory();
    m_iGoalIdGen = 0;
    m_blSaveFlag = true;
}

void CPlan::ClearPlanTimeHourFactory()
{
    while(!m_CPlanTimeHourFactory.isEmpty())
    {
        CPlanTimeHour* l_pDelPlanTimeHour = m_CPlanTimeHourFactory.takeFirst();
        delete l_pDelPlanTimeHour;
    }
}

void CPlan::ClearPlanGoalFactory()
{
    while(!m_CPlanGoalFactory.isEmpty())
    {
        CPlanGoal* l_pDelGoal = m_CPlanGoalFactory.takeFirst();
        delete l_pDelGoal;
    }
}

void CPlan::UpdateGoalInPlanTimeHour(const CPlanGoal *a_pUpdatedGoal)
{
    for(int i=0; i<m_CPlanTimeHourFactory.length(); i++)
    {
        CPlanTimeHour* l_pPlanTimeHour = m_CPlanTimeHourFactory[i];
        STaskAbstractHour* l_pTaskListIter = l_pPlanTimeHour->GetTaskAbstract(0);
        QList<int> l_CDelGoalIdList, l_CDelTaskIdList;
        while(l_pTaskListIter)
        {
            if(a_pUpdatedGoal->GetGoalId() == l_pTaskListIter->m_iGoalId)
            {
                if(a_pUpdatedGoal->HasTask(l_pTaskListIter->m_iTaskId))
                {
                    l_pTaskListIter->m_qstrGoalName = a_pUpdatedGoal->GetGoalName();
                    l_pTaskListIter->m_qstrTaskTag = a_pUpdatedGoal->GetTaskById(\
                                                            l_pTaskListIter->m_iTaskId)->m_qstrTaskTag;
                    l_pTaskListIter->m_EGoalColorTag = a_pUpdatedGoal->GetGoalColor();
                }
                else
                {
                    l_CDelGoalIdList.append(l_pTaskListIter->m_iGoalId);
                    l_CDelTaskIdList.append(l_pTaskListIter->m_iTaskId);
                }
            }
            l_pTaskListIter = l_pTaskListIter->m_pNext;
        }
        for(int i=0; i<l_CDelGoalIdList.length(); i++)
        {
            l_pPlanTimeHour->DeleteTaskAbstract(l_CDelGoalIdList[i],\
                                                l_CDelTaskIdList[i]);
        }
    }
}
