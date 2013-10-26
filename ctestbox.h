#ifndef CTESTBOX_H
#define CTESTBOX_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTransform>
#include <QPointF>
#include <QRectF>

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#define TB_cout std::wcout
#define TB_string std::wstring

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
    void ShowMsg(TB_string a_wstrMsg, MsgType a_eMsgType = LOGMSG);

    static void PrintTransformMtx(QTransform* a_pMtx);
    static void PrintRect(QRectF a_cRect);
    static void PrintGraphicsItemLocation(QGraphicsItem* a_pItem);
    static void PrintMsg(TB_string a_wstrMsg);
};

#endif // CTESTBOX_H
