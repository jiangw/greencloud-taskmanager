#ifndef CWORKSPACE_H
#define CWORKSPACE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

#include "globalconst.h"

class CWorkSpace : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CWorkSpace(QObject *parent = 0);
    virtual ~CWorkSpace();

    void SysLabelDraw(QPointF a_cPos);

    GLOBALCONST::EStatus m_eStatus;
    GLOBALCONST::ESysLabel m_eSysLabel;
    
signals:
    void Signal_SysLabelDrawn();
    
public slots:
    
};

#endif // CWORKSPACE_H
