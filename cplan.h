#ifndef CPLAN_H
#define CPLAN_H

#include <QObject>
#include <QList>

#include "cplantimehour.h"

class CPlan : public QObject
{
    Q_OBJECT

public:
    ~CPlan();
    static void InitPlan(QObject* a_pParent = NULL);
    static CPlan* GetPlan();
    static void FreePlan(); //if you set parent, you do not need to invoke this function

    CPlanTimeHour* CreatePlanTimeHour(QDate a_CDate);
    CPlanTimeHour* GetPlanTimeHour(QDate a_CDate);
    QList<CPlanTimeHour *>& GetPlanTimeHourList()\
    {return m_CPlanTimeHourFactory;}

public slots:
    void SLOT_HourSelMaskRequestProc(QDate a_CDate, bool* a_pHourSelMask, int a_iHoursPerDay);

signals:
    void SIGNAL_HourSelMaskRequestFinished();

private:
    static CPlan* s_pPlan;
    CPlan(QObject *parent = 0);
    void ClearPlanTimeHourFactory();

    QList<CPlanTimeHour *> m_CPlanTimeHourFactory;
};

#endif // CPLAN_H
