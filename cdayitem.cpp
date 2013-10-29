#include "cdayitem.h"

CDayItem::CDayItem(QGraphicsItem *a_pParent, const QDate &a_pDate)
    :QGraphicsItem(a_pParent)
{
    m_iHoursPerDay = 24;
    m_iRadius = 80;
    m_iExtRad = 25;
    m_iExtHeight = 30;
    m_CBR.setRect(0, 0, this->ItemWidth(), this->ItemHeight());
    this->Render();

    QTimer* l_pTimer1 = new QTimer(this);
    connect(l_pTimer1, SIGNAL(timeout()), this, SLOT(SLOT_HourChangeProc()));
    l_pTimer1->start(60000);

    m_pHourSelMask = new bool[m_iHoursPerDay];
    for(int i=0; i<m_iHoursPerDay; i++)
    {
        m_pHourSelMask[i] = false;
    }

    m_pDateLabel = new QGraphicsSimpleTextItem(this);
    m_pDateLabel->setPos(6, this->ItemHeight() - m_iExtHeight);
    this->SetDate(a_pDate);

    m_iCurrHour = this->CurrHour();
}

CDayItem::~CDayItem()
{
    Clear();
    delete [] m_pHourSelMask;
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

    for(int i=0; i<m_iHoursPerDay; i++)
    {
        m_pHourSelMask[i] = false;
    }
}

void CDayItem::SetRadius(int a_iRadius)
{
    this->prepareGeometryChange();
    m_iRadius = a_iRadius;
    m_pDateLabel->setPos(6, this->ItemHeight() - m_iExtHeight);
    m_CBR.setWidth(this->ItemWidth());
    m_CBR.setHeight(this->ItemHeight());
    this->Clear();
    this->Render();
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

void CDayItem::SetDate(const QDate &a_pDate)
{
    m_CDate = a_pDate;
    m_pDateLabel->setText(m_CDate.toString("yyyy.MM.dd dddd"));
    m_iCurrHour = this->CurrHour();
}

void CDayItem::SetHoursPerDay(int a_iHoursPerDay)
{
    m_iHoursPerDay = a_iHoursPerDay;
    this->Clear();
    this->Render();
}

int CDayItem::GetHoursPerDay()
{
    return m_iHoursPerDay;
}

bool* CDayItem::GetHourSelMask()
{
    return m_pHourSelMask;
}

QRectF CDayItem::boundingRect() const
{
    return m_CBR;
}

void CDayItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //draw background
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QBrush(Qt::white));
    painter->setPen(QPen(QBrush(Qt::black), 4));
    painter->drawRoundedRect(m_CBR, m_CBR.width() * 0.05, m_CBR.height() * 0.05);

    painter->restore();

    //draw foreground
    painter->save();

    QPen l_CHourSlicePen(QBrush(Qt::darkGreen), 2);
    QPen l_CDeadHourSlicePen(QBrush(Qt::gray), 2);
    QBrush l_CDeadHourSliceBrush(Qt::darkGray);
    QBrush l_CCurrHourSliceBrush(Qt::yellow);
    QBrush l_CSelHourSliceBrush(Qt::cyan);
    QPen l_CBlackTextPen(QBrush(Qt::black), 1);
    QPen l_CRedTextPen(QBrush(Qt::red), 1);

    painter->setRenderHint(QPainter::Antialiasing);

    for(int i=0; i<m_pHourSliceList.length(); i++)
    {
        painter->setPen(l_CHourSlicePen);

        if(i == this->CurrHour())
        {
            painter->fillPath(*(m_pHourSliceList[i]), l_CCurrHourSliceBrush);
        }
        if(i < this->CurrHour())
        {
            painter->fillPath(*(m_pHourSliceList[i]), l_CDeadHourSliceBrush);
            painter->setPen(l_CDeadHourSlicePen);
        }
        if(m_pHourSelMask[i])
        {
            painter->fillPath(*(m_pHourSliceList[i]), l_CSelHourSliceBrush);
        }

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
    if(m_CDate < QDate::currentDate())
    {
        m_iCurrHour = 24; //set all time as past
    }
    else if(m_CDate > QDate::currentDate())
    {
        m_iCurrHour = -1; //set all time as future
    }
    else
    {
        m_iCurrHour = QTime::currentTime().hour();
    }
    return m_iCurrHour;
}

int CDayItem::ItemWidth()
{
    return 2 * (m_iRadius + m_iExtRad);
}

int CDayItem::ItemHeight()
{
    return 2 * (m_iRadius + m_iExtRad) + m_iExtHeight;
}

void CDayItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_CMouseLastPos = event->pos();
    }
}

void CDayItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton &&
            QLineF(m_CMouseLastPos, event->pos()).length() < \
            TASKMANAGER::g_iMouseClickDistThreshold)
    {//left button clicked
        this->prepareGeometryChange();
        for(int i=0; i<m_iHoursPerDay; i++)
        {
            QPainterPath* l_pHourSlice = m_pHourSliceList[i];
            if(l_pHourSlice->contains(event->pos()) && i >= this->CurrHour())
            {
                m_pHourSelMask[i] = !m_pHourSelMask[i];
                break;
            }
        }
    }
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
    if(QTime::currentTime().hour() != m_iCurrHour && \
            m_CDate == QDate::currentDate())
    {
        update(QRectF(m_CBR.x(), m_CBR.y(), m_CBR.width(), m_CBR.height() - m_iExtHeight));
    }
}
