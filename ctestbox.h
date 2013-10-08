#ifndef CTESTBOX_H
#define CTESTBOX_H

#include <QGraphicsScene>
#include <QTransform>
#include <iostream>

#include "cgoalview.h"

class CTestBox
{
public:
    static void AddTextItemToScene(QGraphicsScene* a_pScene);
    static void PrintTransformMtx(QTransform* a_pMtx);
    static void PrintRect(QRectF a_cRect);
};

#endif // CTESTBOX_H
