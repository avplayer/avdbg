// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	XML.h

#ifndef	XML_H
#define XML_H

#include "XMLTYPE.h"

class I_COLOROUT
{
public:
    virtual void __stdcall SetBKColor(COLORREF textcolor) = 0;
    virtual void __stdcall ColorOut(PSTR str, long len, COLORREF textcolor) = 0;
    //str �Ǹ��ܳ���string�����ֻ��ʾǰ�泤��Ϊlen�Ĳ���
    //color ���ַ�ǰ��ɫ
    //������� '\n' ���Զ�����
};
typedef void (_stdcall *Func_ColorOut)(PSTR str, long len, COLORREF color, long pos);

class CXmlList
{
	CXmlList();
	CXmlList(int){}
    friend class CXmlPrt;

	XMLTYPE m_xmltype;
	PVOID	m_p;
	UINT	m_posfrom;
	UINT	m_posto;

	CXmlList* m_sub;
	CXmlList* m_next;
    CXmlList* GetLast_willbegin();
    CXmlList* GetLast_willend();
public:
    static CXmlList* new_CXmlList(XMLTYPE xmltype, PVOID p, UINT posfrom);
	void Clicked(long x1,long x2);
	BOOL GetRightWord(long curpos, long &posfrom, long &posto);
	BOOL GetLeftWord(long curpos, long &posfrom, long &posto);
	CXmlList* GetCurWord(UINT curpos, long &posfrom, long &posto);
	~CXmlList();
	XMLTYPE XMLend(XMLTYPE xmltype, UINT pos);
	void XMLbegin(XMLTYPE xmltype, PVOID p, UINT pos);

    void prtprtout(PSTR str, class CXmlOutPro* prt);
	void Display(PSTR pstr, I_COLOROUT* iColorOut, XMLTYPE curw_xmltype, PVOID curw_p);
};

//void Del_XmlRec_List(XMLREC* pfirst);

#endif	//	XML_H
