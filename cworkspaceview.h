#ifndef CWORKSPACEVIEW_H
#define CWORKSPACEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPoint>
#include <QMouseEvent>

#include "cworkspace.h"

class CWorkSpaceView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit CWorkSpaceView(CWorkSpace* a_pWorkSpace, QWidget* a_pParent);
    virtual ~CWorkSpaceView();
    
signals:
    
public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    CWorkSpace* m_pWorkSpace;
    QPoint m_cLastMousePos;
};

#endif // CWORKSPACEVIEW_H
