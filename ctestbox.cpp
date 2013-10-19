#include "ctestbox.h"

void CTestBox::PrintTransformMtx(QTransform *a_pMtx)
{
    TB_cout << "Transformation Matrix:" << std::endl;
    TB_cout << a_pMtx->m11() << " " << a_pMtx->m12() << " " << a_pMtx->m13() << std::endl;
    TB_cout << a_pMtx->m21() << " " << a_pMtx->m22() << " " << a_pMtx->m23() << std::endl;
    TB_cout << a_pMtx->m31() << " " << a_pMtx->m32() << " " << a_pMtx->m33() << std::endl;
}

void CTestBox::PrintRect(QRectF a_cRect)
{
    TB_cout << "x:" << a_cRect.x() << " y:" << a_cRect.y() << std::endl;
    TB_cout << "w:" << a_cRect.width() << " h:" << a_cRect.height() << std::endl;
}

void CTestBox::PrintGraphicsItemLocation(QGraphicsItem *a_pItem)
{
    // pos == center
    TB_cout << "pos (" << a_pItem->pos().x() << ", " << a_pItem->pos().y() << ")" << std::endl;
    QRectF l_cRect = a_pItem->boundingRect();
    TB_cout << "center (" << l_cRect.x() + l_cRect.width() / 2 << ", " << \
                 l_cRect.y() + l_cRect.height() / 2 << ")" << std::endl;
}

void CTestBox::PrintMsg(TB_string a_wstrMsg)
{
    TB_cout << a_wstrMsg << std::endl;
}
