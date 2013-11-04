#ifndef CTESTBOX_H
#define CTESTBOX_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <QString>

class CTestBox
{
public:
    enum MsgType{LOGMSG, WARNINGMSG, ERRORMSG};
private:
    CTestBox();
    static bool m_blFlag;
    static CTestBox* m_pTestBox;

    std::string m_strPTYName;
public:
    static CTestBox* GetTestBox();
    static void InitTestBox();
    ~CTestBox(){CTestBox::m_blFlag = false;}
    void ShowMsg(std::wstring a_wstrMsg, MsgType a_EMsgType = LOGMSG);
    void ShowMsg(QString a_qstrMsg, MsgType a_EMsgType = LOGMSG);
};

#endif // CTESTBOX_H
