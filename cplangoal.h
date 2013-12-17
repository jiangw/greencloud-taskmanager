#ifndef CPLANGOAL_H
#define CPLANGOAL_H

#include <QString>

struct STask
{
    QString m_qstrTaskTag; //each task has an unique tag in a goal
    QString m_qstrDescription;
    bool m_blIsFinished;
    STask* m_pNext;
};

class CPlanGoal
{
public:
    CPlanGoal();
    CPlanGoal(QString a_qstrGoalName, Qt::GlobalColor a_EColorTag);
    ~CPlanGoal();
    void ClearTaskList();

    void SetGoalId(int a_iGoalId){m_iGoalId = a_iGoalId;}
    int GetGoalId() const{return m_iGoalId;}
    void SetGoalName(QString a_qstrGoalName){m_qstrGoalName = a_qstrGoalName;}
    QString GetGoalName() const{return m_qstrGoalName;}
    void SetGoalColor(Qt::GlobalColor a_EColor){m_EColorTag = a_EColor;}
    Qt::GlobalColor GetGoalColor() const{return m_EColorTag;}
    void AddTask(QString a_qstrTaskTag, QString a_qstrTaskDesc, bool a_blFinished = false);
    void DeleteTask(QString a_qstrTaskTag);
    void FinishTask(QString a_qstrTaskTag, bool a_blIsFinished);
    int TaskNumber(){return m_iTaskCounter;}
    const STask* GetTaskByTag(QString a_qstrTaskTag);
    const STask* GetTaskByIndex(int a_iTaskIndex);
    int GetTaskIndex(QString a_qstrTaskTag);
    bool HasTask(QString a_qstrTaskTag);
    const STask* GetTaskListHead()const{return m_pTaskListHead;}

private:
    STask* TaskExist(QString a_qstrTaskTag);

    int m_iGoalId;
    QString m_qstrGoalName; //each goal has an unique goal name
    Qt::GlobalColor m_EColorTag; //each goal has an unique color tag
    STask* m_pTaskListHead;
    STask* m_pTaskListTail;
    int m_iTaskCounter;
};

#endif // CPLANGOAL_H
