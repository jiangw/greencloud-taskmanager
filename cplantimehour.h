#ifndef CPLANTIMEHOUR_H
#define CPLANTIMEHOUR_H

#include <iostream>
#include <QList>
#include <QDate>
#include <QString>

#include "gconfig.h"

struct STaskAbstractHour
{ //this struct contains a simple description about a task
  //which looses coupling with real goal and task classes
    QString m_qstrGoalName;
    QString m_qstrTaskTag;
    Qt::GlobalColor m_EGoalColorTag;
    bool m_blIsFinished;

    STaskAbstractHour* m_pNext; //point to the next task in the tasklist of whole day
};

struct STimeSegHour
{
    int m_iStartClock;
    int m_iEndClock;
    GREENSCHEDULE::ETimePage m_ETimePage;
    QList<STaskAbstractHour*> m_CTaskList;
    STimeSegHour* m_pNext; //point to the next time segment
};

class CPlanTimeHour
{
public:
    CPlanTimeHour(QDate a_CDate);
    ~CPlanTimeHour();
    void Clear(); //Release momory allocated for time segments and tasks list
    void ClearTimeSegList(); //release memory for time segments list and reset counter
    void ClearTimeSegList(STimeSegHour* a_pTimeSegListIter);
    STimeSegHour* BackupTimeSegList();
    void ClearTaskList(); //release memory for tasks list and reset counter

    bool IsEmpty();
    bool IsHistory();
    const QDate& Date()\
    {return m_CDate;}
    void SetTimeSeg(const bool* a_pDayHourMask, int a_iHoursPerDay = 24);
    STimeSegHour* GetTimeSeg(int a_iTimeSegIndex);
    STimeSegHour* CreateTimeSeg(int a_iStartClock, int a_iEndClock);
    bool ConvertTimeSegToHourMask(bool* a_pDayHourMask, int a_iHoursPerDay = 24);
    int TimeSegCounter(){return m_iTimeSegCounter;}
    void UpdateTimeSegPage();

    void AssignTaskToTimeSeg(STimeSegHour* a_pTimeSegHour, STaskAbstractHour* a_pTaskAbs);
    void AssignTaskToTimeSeg(STimeSegHour* a_pTimeSegHour, QString a_qstrGoalName,\
                             QString a_qstrTaskTag, Qt::GlobalColor a_EColorTag);

    STaskAbstractHour* HasTaskAbstract(QString a_qstrGoalName, QString a_qstrTaskTag);
    STaskAbstractHour* CreateTaskAbstract(QString a_qstrGoalName, QString a_qstrTaskTag,\
                                          Qt::GlobalColor a_EColorTag);
    void DeleteTaskAbstract(QString a_qstrGoalName, QString a_qstrTaskTag);
    void DeleteTaskAbstract(STaskAbstractHour* a_pDelTask);
    bool TaskInTimeSeg(STimeSegHour* a_pTimeSegHour, STaskAbstractHour* a_pTask);
    STaskAbstractHour* GetTaskAbstract(int a_iTaskIndex);
    int GetTaskIndex(QString a_qstrGoalName, QString a_qstrTaskTag);
    int TaskCounter(){return m_iTaskCounter;}

private:
    STimeSegHour* AddTimeSeg(int a_iStartClock, int a_iEndClock);
    void AddTaskAbstractToGloablList(STaskAbstractHour* a_pNewTaskAbstract);
    void AddTaskAbstractToLocalList(STimeSegHour* a_pTimeSegHour,\
                                    STaskAbstractHour* a_pNewTaskAbstract);
    void ClearTaskAssignment(STimeSegHour* a_pTimeSeg);

    QDate m_CDate; //the day for planning
    STimeSegHour* m_pTimeSegListHead; //first time segment in time segments list
    STimeSegHour* m_pTimeSegListTail; //last time segment in time segments list
    STaskAbstractHour* m_pTaskListHead; //first task in tasks list
    STaskAbstractHour* m_pTaskListTail; //las task in tasks list
    int m_iTimeSegCounter, m_iTaskCounter; //the number of time segments and tasks
};

#endif // CPLANTIMEHOUR_H
