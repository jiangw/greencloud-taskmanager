#ifndef CTESTBOX_H
#define CTESTBOX_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTransform>
#include <QPointF>
#include <QRectF>
#include <iostream>

#define TB_cout std::wcout
#define TB_string std::wstring

class CTestBox
{
public:
    static void PrintTransformMtx(QTransform* a_pMtx);
    static void PrintRect(QRectF a_cRect);
    static void PrintGraphicsItemLocation(QGraphicsItem* a_pItem);
    static void PrintMsg(TB_string a_wstrMsg);
};

#endif // CTESTBOX_H
