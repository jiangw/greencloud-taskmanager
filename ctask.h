/****************************************************************************
** here is for description
****************************************************************************/

#ifndef CTASK_H
#define CTASK_H

#include <QDateTime>

class CTask
{
public:
    CTask();
    ~CTask();

    enum ETaskStatus {NOTSTART, ONGOING, FINISHED, ABANDONED};

    void SetTitle(QString a_strTitle){m_strTitle = a_strTitle;}
    void SetDescription(QString a_strDesc){m_strDescription = a_strDesc;}
    void SetStatus(enum ETaskStatus a_eStatus){m_eStatus = a_eStatus;}

    QString GetTitle(){return m_strTitle;}
    QString GetDescription(){return m_strDescription;}
    const QDateTime& GetPreStartTime(){return m_cPreStartTime;}
    const QDateTime& GetPreEndTime(){return m_cPreEndTime;}
    const QDateTime& GetStartTime(){return m_cStartTime;}
    const QDateTime& GetEndTime(){return m_cEndTime;}
    enum ETaskStatus GetStatus(){return m_eStatus;}

    bool SetPreAllocTime(QDateTime a_cStartTime, QDateTime a_cEndTime);
    bool TaskStart();
    bool TaskEnd();
    bool TaskAbandon();

private:
    QString m_strTitle;
    QString m_strDescription;
    QDateTime m_cPreStartTime;
    QDateTime m_cPreEndTime;
    QDateTime m_cStartTime;
    QDateTime m_cEndTime;
    enum ETaskStatus m_eStatus;

};

#endif // CTASK_H
