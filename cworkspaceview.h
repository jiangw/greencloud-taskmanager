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
    void Slot_DragModeSwitched(bool a_blFlag);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    CWorkSpace* m_pWorkSpace;
    QPoint m_cLastMousePos;
    GLOBALCONST::SWorkSpaceFlag m_sLastFlag;
};

#endif // CWORKSPACEVIEW_H
