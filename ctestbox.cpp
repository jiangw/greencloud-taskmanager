#include "ctestbox.h"

bool CTestBox::m_blFlag = false;
CTestBox* CTestBox::m_pTestBox = NULL;

CTestBox::CTestBox()
{
    std::cout << "[CTestBox] Initiate TestBox." << std::endl;
    int l_iNextPtsNum = 0;
    while(true)
    {
        char l_pBuffer[100];
        sprintf(l_pBuffer, "/dev/pts/%d", l_iNextPtsNum);
        m_strPTYName = std::string(l_pBuffer);

        std::cout << "[CTestBox] Test: " << m_strPTYName << std::endl;

        std::ofstream l_cTest(m_strPTYName.c_str());
        if(!l_cTest.is_open())
        {
            break;
        }
        l_cTest.close();
        l_iNextPtsNum++;
    }
    std::cout << "[CTestBox] Get PTY name: " << m_strPTYName << std::endl;
    if(-1 == system("gnome-terminal"))
    {
        std::cerr << "[CTestBox] Failed to open terminal." << std::endl;
    }
}

CTestBox* CTestBox::GetTestBox()
{
    if(!CTestBox::m_blFlag)
    {
        CTestBox::m_pTestBox = new CTestBox();
        CTestBox::m_blFlag = true;
    }
    return CTestBox::m_pTestBox;
}

void CTestBox::InitTestBox()
{
    GetTestBox();
}

void CTestBox::ShowMsg(std::wstring a_wstrMsg, MsgType a_eMsgType)
{
    std::wofstream l_cOut(m_strPTYName.c_str());
    if(l_cOut.is_open())
    {
        switch(a_eMsgType)
        {
        case LOGMSG:
            l_cOut << std::endl << L"\033[1;32m" << a_wstrMsg;
            break;
        case WARNINGMSG:
            l_cOut << std::endl << L"\033[1;33m" << a_wstrMsg;
            break;
        case ERRORMSG:
            l_cOut << std::endl << L"\033[1;31m" << a_wstrMsg;
            break;
        default:
            break;
        }
    }
    else
    {
        std::cout << "Cannot connect to PTY." << std::endl;
    }
    l_cOut.close();
}

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
    TB_cout << "pos (" << a_pItem->pos().x() << ", " << a_pItem->pos().y() << ")" << std::endl;
    QRectF l_cRect = a_pItem->boundingRect();
    TB_cout << "center (" << l_cRect.x() + l_cRect.width() / 2 << ", " << \
                 l_cRect.y() + l_cRect.height() / 2 << ")" << std::endl;
}

void CTestBox::PrintMsg(TB_string a_wstrMsg)
{
    TB_cout << a_wstrMsg << std::endl;
}
