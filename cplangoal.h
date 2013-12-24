#ifndef CPLANGOAL_H
#define CPLANGOAL_H

#include <QString>
#include <QColor>

struct STask
{
    int m_iTaskId; //each task has an unique id in a goal
    QString m_qstrTaskTag;
    QString m_qstrDescription;
    bool m_blIsFinished;
    STask* m_pNext;
};

class CPlanGoal
{
public:
    CPlanGoal();
    CPlanGoal(QString a_qstrGoalName, QRgb a_EColorTag);
    ~CPlanGoal();
    void ClearTaskList();

    void SetGoalId(int a_iGoalId){m_iGoalId = a_iGoalId;}
    int GetGoalId() const{return m_iGoalId;}
    void SetGoalName(QString a_qstrGoalName){m_qstrGoalName = a_qstrGoalName;}
    QString GetGoalName() const{return m_qstrGoalName;}
    void SetGoalColor(QRgb a_EColor){m_EColorTag = a_EColor;}
    QRgb GetGoalColor() const{return m_EColorTag;}
    void AddTask(int a_iTaskId, QString a_qstrTaskTag, QString a_qstrTaskDesc,\
                 bool a_blFinished = false);
    void UpdateTask(int a_iTaskId, QString a_qstrTaskTag, QString a_qstrTaskDesc,\
                    bool a_blFinished);
    void DeleteTask(int a_iTaskId);
    void FinishTask(int a_iTaskId, bool a_blIsFinished);
    int TaskNumber(){return m_iTaskCounter;}
    const STask* GetTaskById(int a_iTaskId) const;
    const STask* GetTaskByTag(QString a_qstrTaskTag);
    const STask* GetTaskByIndex(int a_iTaskIndex);
    int GetTaskIndex(int a_iTaskId);
    bool HasTask(int a_iTaskId) const;
    const STask* GetTaskListHead()const{return m_pTaskListHead;}

private:
    STask* TaskExist(int a_iTaskId) const;

    int m_iGoalId;
    QString m_qstrGoalName;
    QRgb m_EColorTag;
    STask* m_pTaskListHead;
    STask* m_pTaskListTail;
    int m_iTaskCounter;
};

#endif // CPLANGOAL_H
