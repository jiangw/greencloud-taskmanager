#ifndef CTEXTINPUTITEM_H
#define CTEXTINPUTITEM_H

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QTextEdit>
#include <QRectF>
#include <QPainter>
#include <QKeyEvent>

#include "gconfig.h"
#include "citemanimation.h"

class CTextInputItem : public CItemAnimation, public QGraphicsItem
{
    Q_OBJECT
public:
    CTextInputItem(QGraphicsItem* a_pParent);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void SetInputTip(QString a_strTip);
    QString GetPlainText();

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void SLOT_SubmitTextEmit(); //emit SIGNAL_SubmitText

    void SLOT_RemoveItemEmit();
    void SLOT_DeleteItemEmit();
    void SLOT_DisappearItemProc();

signals:
    void SIGNAL_SubmitText(QTextDocument* a_pDoc);

private:
    QTextDocument m_cDoc;
    QGraphicsTextItem* m_pTextItem;
};

#endif // CTEXTINPUTITEM_H
