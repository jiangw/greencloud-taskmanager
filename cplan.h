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
    QList<CPlanTimeHour *>& GetPlanTimeHourList(GREENSCHEDULE::ETimePage a_ETimePage);
    void UpdatePlanTimeHour(CPlanTimeHour* a_pPlanTimeHour,\
                            const bool *a_pDayHourMask, int a_iHoursPerDay);
    void RemovePlanTimeHour(CPlanTimeHour* a_pDelPlanTime);
    int GetPlanTimeHourIndex(QDate a_CDate,\
                             GREENSCHEDULE::ETimePage a_ETimePage = GREENSCHEDULE::INPROGRESS);

    CPlanGoal* CreatePlanGoal(QString a_qstrGoalName, CGraphicsWidget::gColor a_EColor);
    CPlanGoal* CreateEmptyPlanGoal();
    CPlanGoal *GetPlanGoalById(int a_iGoalId);
    CPlanGoal* GetPlanGoalByName(QString a_qstrGoalName);
    CPlanGoal* GetPlanGoalByColorTag(CGraphicsWidget::gColor a_EColor);
    QList<CPlanGoal *>& GetPlanGoalList()\
    {return m_CPlanGoalFactory;}
    void RemovePlanGoal(int a_iGoalId);
    int GetPlanGoalIndex(int a_iGoalId);

    bool NeedSave(){return m_blSaveFlag;}
    bool SavePlan(QString a_qstrFileName);
    bool LoadPlan(QString a_qstrFileName);
    void FailFileIOCatch(QFile* a_FailedFile);

public slots:
    void SLOT_HourSelMaskRequestProc(QDate a_CDate, bool* a_pHourSelMask, int a_iHoursPerDay);
    void SLOT_PlanGoalReviewProc(CGoalWidget* a_pGoalProposer);
    void SLOT_PlanGoalSyncProc(const CPlanGoal* a_pPlanGoal);
    void SLOT_PlanGoalRetractProc(int a_iGoalId);
    void SLOT_GoalTaskFinishStatSync(int a_iGoalId, int a_iTaskId, bool a_blIsFinished);

    void SLOT_UpdateTimePage();

    void SLOT_EnablePlanSave();

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
    void UpdateGoalInPlanTimeHour(const CPlanGoal* a_pUpdatedGoal);
    QList<CPlanTimeHour *>& GetPlanTimeHourListInProgress();
    QList<CPlanTimeHour *>& GetPlanTimeHourListHistory();

    QList<CPlanTimeHour *> m_CPlanTimeHourFactory;
    QList<CPlanTimeHour *> m_CPlanTimeHourInProgress;
    QList<CPlanTimeHour *> m_CPlanTimeHourHistory;
    QList<CPlanGoal *> m_CPlanGoalFactory;
    int m_iGoalIdGen;
    bool m_blSaveFlag;
};

#endif // CPLAN_H
