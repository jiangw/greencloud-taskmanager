#ifndef GCONFIG_H
#define GCONFIG_H

#include <iostream>

//set programming flag
//#define PF_TEST
#define PF_RUN

#include <QtCore>
#include <qmath.h>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QDate>
#include <QTime>
#include <QRectF>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QList>
#include <QTransform>

namespace TASKMANAGER
{
    //math
    const qreal g_dPI = 3.1415926;
    //properties of graphic items
    const int g_iMouseClickDistThreshold = 3;
    const int g_iItemBorderWidth = 4;
    const int g_iItemFontSizeLarge = 25;
    const int g_iItemFontSizeMid = 18;
    const int g_iItemFontSizeSmall = 8;
    const int g_iItemDisappearFrames = 10;
    const int g_iItemAppearFrames = 5;
    const int g_iItemIntervalX = 8; //standard distance between two items in X direction
    const int g_iItemIntervalY = 10; //standard distance between two items in Y direction
}

#endif // GCONFIG_H
