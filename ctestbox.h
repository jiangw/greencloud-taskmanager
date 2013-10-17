#ifndef CTESTBOX_H
#define CTESTBOX_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTransform>
#include <QPointF>
#include <QRectF>
#include <iostream>

class CTestBox
{
public:
    static void PrintTransformMtx(QTransform* a_pMtx);
    static void PrintRect(QRectF a_cRect);
    static void PrintGraphicsItemLocation(QGraphicsItem* a_pItem);
    static void PrintMsg(std::string a_strMsg);
};

#endif // CTESTBOX_H
