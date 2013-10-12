#ifndef CTESTBOX_H
#define CTESTBOX_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTransform>
#include <QPointF>
#include <QRectF>
#include <iostream>

#include "cgoalitem.h"

class CTestBox
{
public:
    static void AddTextItemToScene(QGraphicsScene* a_pScene);
    static void PrintTransformMtx(QTransform* a_pMtx);
    static void PrintRect(QRectF a_cRect);
    static void PrintGraphicsItemLocation(QGraphicsItem* a_pItem);
};

#endif // CTESTBOX_H
