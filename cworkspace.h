#ifndef CWORKSPACE_H
#define CWORKSPACE_H

#include <QGraphicsScene>

#include "gconfig.h"

class CWorkSpace : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CWorkSpace(QObject *parent = 0);
    virtual ~CWorkSpace();
    
signals:
    
public slots:
    
};

#endif // CWORKSPACE_H
