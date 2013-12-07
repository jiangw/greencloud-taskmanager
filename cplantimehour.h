#ifndef CPLANTIMEHOUR_H
#define CPLANTIMEHOUR_H

#include <iostream>
#include <QList>
#include <QDate>
#include <QString>

struct STaskAbstractHour
{ //this struct contains a simple description about a task
  //which looses coupling with real goal and task classes
    QString m_qstrGoalName;
    QString m_qstrTaskTag;
    Qt::GlobalColor m_EGoalColorTag;

    STaskAbstractHour* m_pNext; //point to the next task in the tasklist of whole day
    STaskAbstractHour* m_pNextInTimeSeg; //point to the next task in the tasklist of the same time segment
};

struct STimeSegHour
{
    int m_iStartClock;
    int m_iEndClock;
    int m_iTaskNum;

    STaskAbstractHour* m_pTaskListHead; //point to the first task of tasklist
    STimeSegHour* m_pNext; //point to the next time segment
};

class CPlanTimeHour
{
public:
    CPlanTimeHour(QDate a_CDate);
    ~CPlanTimeHour();
    void Clear(); //Release momory allocated for time segments and tasks list
    void ClearTimeSegList(); //release memory for time segments list and reset counter
    void ClearTaskList(); //release memory for tasks list and reset counter

    bool IsEmpty();
    const QDate& Date()\
    {return m_CDate;}
    void SetTimeSeg(const bool* a_pDayHourMask, int a_iHoursPerDay = 24);
    STimeSegHour *GetTimeSeg(int a_iTimeSegIndex);
    bool ConvertTimeSegToHourMask(bool* a_pDayHourMask, int a_iHoursPerDay = 24);
    int TimeSegCounter(){return m_iTimeSegCounter;}
    int TaskCounter(){return m_iTaskCounter;}

private:
    void AddTimeSeg(int a_iStartClock, int a_iEndClock);
    void ClearTaskAssignment(STimeSegHour* a_pTimeSeg);

    QList<STimeSegHour *> m_CTimeSegList;
    QDate m_CDate; //the day for planning
    STimeSegHour* m_pTimeSegListHead; //first time segment in time segments list
    STimeSegHour* m_pTimeSegListTail; //last time segment in time segments list
    STaskAbstractHour* m_pTaskListHead; //first task in tasks list
    STaskAbstractHour* m_pTaskListTail; //las task in tasks list
    int m_iTimeSegCounter, m_iTaskCounter; //the number of time segments and tasks
};

#endif // CPLANTIMEHOUR_H
