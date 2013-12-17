#ifndef CPLAN_H
#define CPLAN_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QDataStream>

#include "cplantimehour.h"
#include "cplangoal.h"
#include "cgoalwidget.h"

#define MAXTASKNUMINTIMESEG 60

class CPlan : public QObject
{
    Q_OBJECT

public:
    ~CPlan();
    static void InitPlan(QObject* a_pParent = NULL);
    static CPlan* GetPlan();
    static void FreePlan(); //release memory.
                            //if you set parent of CPlan, you do not need to invoke this function
    void ClearPlan(); //clear content of the plan

    CPlanTimeHour* CreatePlanTimeHour(QDate a_CDate);
    CPlanTimeHour* GetPlanTimeHour(QDate a_CDate);
    QList<CPlanTimeHour *>& GetPlanTimeHourList();
    void RemovePlanTimeHour(CPlanTimeHour* a_pDelPlanTime);

    CPlanGoal* CreatePlanGoal(QString a_qstrGoalName, Qt::GlobalColor a_EColor);
    CPlanGoal* CreateEmptyPlanGoal();
    CPlanGoal *GetPlanGoalById(int a_iGoalId);
    CPlanGoal* GetPlanGoalByName(QString a_qstrGoalName);
    CPlanGoal* GetPlanGoalByColorTag(Qt::GlobalColor a_EColor);
    QList<CPlanGoal *>& GetPlanGoalList()\
    {return m_CPlanGoalFactory;}
    void RemovePlanGoal(int a_iGoalId);
    int GetPlanGoalIndex(QString a_qstrGoalName);

    bool NeedSave(){return m_blSaveFlag;}
    bool SavePlan(QString a_qstrFileName);
    bool LoadPlan(QString a_qstrFileName);
    void FailFileIOCatch(QFile* a_FailedFile);

public slots:
    void SLOT_HourSelMaskRequestProc(QDate a_CDate, bool* a_pHourSelMask, int a_iHoursPerDay);
    void SLOT_PlanGoalReviewProc(CGoalWidget* a_pGoalProposer);
    void SLOT_PlanGoalSyncProc(const CPlanGoal* a_pPlanGoal);
    void SLOT_PlanGoalRetractProc(int a_iGoalId);
    void SLOT_GoalTaskFinishStatSync(int a_iGoalId, QString a_qstrTaskTag, bool a_blIsFinished);

    void SLOT_UpdateTimePage();

signals:
    void SIGNAL_HourSelMaskRequestFinished();
    void SIGNAL_PlanWidgetUpdate();
    void SIGNAL_PlanGoalCreated(CPlanGoal* a_pNewPlanGoal);
    void SIGNAL_PlanGoalUpdated(CPlanGoal* a_pPlanGoal);
    void SIGNAL_PlanGoalRemoved(int a_iGoalId);

    void SIGNAL_ShowInMessageBox(QString a_qstrMsg);

private:
    static CPlan* s_pPlan;
    CPlan(QObject *parent = 0);
    void ClearPlanTimeHourFactory();
    void ClearPlanGoalFactory();

    QList<CPlanTimeHour *> m_CPlanTimeHourFactory;
    QList<CPlanTimeHour *> m_CPlanTimeHourInProgress;
    QList<CPlanGoal *> m_CPlanGoalFactory;
    int m_iGoalIdGen;
    bool m_blSaveFlag;
};

#endif // CPLAN_H
