#include "ctestbox.h"

void CTestBox::AddTextItemToScene(QGraphicsScene *a_pScene)
{
    QGraphicsTextItem* l_pTextItem = new QGraphicsTextItem("Welcome to use Task Manager.", \
                                                           NULL, a_pScene);
    l_pTextItem->setDefaultTextColor(Qt::black);
}
