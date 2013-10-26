#include "cdayitem.h"

CDayItem::CDayItem(QGraphicsItem *a_pParent)
    :QGraphicsItem(a_pParent)
{
    m_iHoursPerDay = 24;
    m_iRadius = 100;
    m_iExtRad = 30;
    m_CBR.setRect(0, 0, 2 * (m_iRadius + m_iExtRad), 2 * (m_iRadius + m_iExtRad));
    Render();

    m_iCurrHour = QTime::currentTime().hour();
    QTimer* l_pTimer = new QTimer(this);
    connect(l_pTimer, SIGNAL(timeout()), this, SLOT(SLOT_HourChangeProc()));
    l_pTimer->start(60000);
    std::cout << "Test" << std::endl;
}

CDayItem::~CDayItem()
{
    Clear();
}

void CDayItem::Clear()
{
    for(int i=0; i<m_pHourSliceList.length(); i++)
    {
        delete (m_pHourSliceList[i]);
        delete (m_pHourIdRectList[i]);
    }
    m_pHourSliceList.clear();
    m_pHourIdRectList.clear();
}

QRectF CDayItem::boundingRect() const
{
    return m_CBR;
}

void CDayItem::SetRadius(int a_iRadius)
{
    this->prepareGeometryChange();
    m_iRadius = a_iRadius;
    m_CBR.setWidth(2 * (m_iRadius + m_iExtRad));
    m_CBR.setHeight(2 * (m_iRadius + m_iExtRad));
    Clear();
    Render();
}

void CDayItem::Render()
{
    QRectF l_CClockRect;
    l_CClockRect.setX(m_CBR.x() + m_iExtRad);
    l_CClockRect.setY(m_CBR.y() + m_iExtRad);
    l_CClockRect.setWidth(m_iRadius * 2);
    l_CClockRect.setHeight(m_iRadius * 2);
    QPoint l_CCenter;
    l_CCenter.setX(l_CClockRect.x() + l_CClockRect.width() / 2);
    l_CCenter.setY(l_CClockRect.y() + l_CClockRect.height() / 2);

    qreal l_dArcPerHour =  2 * TASKMANAGER::g_dPI / m_iHoursPerDay;
    qreal l_dAnglePerHour = 360 / m_iHoursPerDay;
    int l_iTranslate = 8;
    for(int i=m_iHoursPerDay; i>0; i--)
    {
        QPainterPath* l_pNewHour = new QPainterPath(l_CCenter);
        l_pNewHour->arcTo(l_CClockRect, 270 - (l_dAnglePerHour * (m_iHoursPerDay - i)), \
                          -l_dAnglePerHour);
        l_pNewHour->closeSubpath();
        m_pHourSliceList.append(l_pNewHour);

        QPoint l_CEndPoint;
        l_CEndPoint.setX(m_iRadius * qSin(l_dArcPerHour * i) + l_CCenter.x());
        l_CEndPoint.setY(m_iRadius * qCos(l_dArcPerHour * i) + l_CCenter.y());
        QRectF* l_pHourIdRect = new QRectF;
        l_pHourIdRect->setX(l_CEndPoint.x() + l_iTranslate * qSin(l_dArcPerHour * i) - 8);
        l_pHourIdRect->setY(l_CEndPoint.y() + l_iTranslate * qCos(l_dArcPerHour * i) - 10);
        l_pHourIdRect->setWidth(16);
        l_pHourIdRect->setHeight(20);
        m_pHourIdRectList.append(l_pHourIdRect);

    }
}

void CDayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //draw background
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QBrush(Qt::white));
    painter->setPen(QPen(QBrush(Qt::black), 4));
    painter->drawRoundedRect(m_CBR, m_CBR.width() * 0.05, m_CBR.height() * 0.05);

    painter->restore();

    //draw foreground
    painter->save();

    QPen l_CHourSlicePen(QBrush(Qt::blue), 2);
    QBrush l_CCurrHourSliceBrush(Qt::yellow);
    QPen l_CBlackTextPen(QBrush(Qt::black), 1);
    QPen l_CRedTextPen(QBrush(Qt::red), 1);

    painter->setRenderHint(QPainter::Antialiasing);

    for(int i=0; i<m_pHourSliceList.length(); i++)
    {
        if(i == this->CurrHour())
        {
            painter->fillPath(*(m_pHourSliceList[i]), l_CCurrHourSliceBrush);
        }

        painter->setPen(l_CHourSlicePen);
        painter->drawPath(*(m_pHourSliceList[i]));

        if(0 == i || 6 == i || 12 == i || 18 == i)
        {
            painter->setPen(l_CRedTextPen);
        }
        else
        {
            painter->setPen(l_CBlackTextPen);
        }
        painter->drawText(*(m_pHourIdRectList[i]), Qt::AlignCenter, QString("%1").arg(i));
    }

    painter->restore();
}

int CDayItem::CurrHour()
{
    m_iCurrHour = QTime::currentTime().hour();
    return m_iCurrHour;
}

void CDayItem::SLOT_RemoveItemEmit()
{
    emit this->SIGNAL_RemoveItem(this, this);
}

void CDayItem::SLOT_DeleteItemEmit()
{
    emit this->SIGNAL_DeleteItem(this);
}

void CDayItem::SLOT_HourChangeProc()
{
    if(QTime::currentTime().hour() != m_iCurrHour)
    {
        update(m_CBR);
    }
}
