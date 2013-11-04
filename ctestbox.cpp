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
    std::cout << "[CTestBox] Get PTY device: " << m_strPTYName << std::endl;
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

void CTestBox::ShowMsg(std::wstring a_wstrMsg, MsgType a_EMsgType)
{
    std::wofstream l_cOut(m_strPTYName.c_str());
    if(l_cOut.is_open())
    {
        switch(a_EMsgType)
        {
        case LOGMSG:
            l_cOut << std::endl << L"\033[1;32m" << a_wstrMsg;
            std::wcout << a_wstrMsg << std::endl; //log messages will show in std output
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

void CTestBox::ShowMsg(QString a_qstrMsg, MsgType a_EMsgType)
{
    this->ShowMsg(a_qstrMsg.toStdWString(), a_EMsgType);
}
