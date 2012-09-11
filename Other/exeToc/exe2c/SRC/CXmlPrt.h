// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	CXmlPrt.h

#ifndef	CXmlPrt_H
#define CXmlPrt_H

#include "CXmlList.h"
#include "CMyString.h"

class CXmlPrt : public I_XmlOut
{
	CXmlList* 	m_xmllist;
	CMyString*	m_str;

	XMLTYPE m_curword_type;	// �������ݵ�����
	PVOID	m_curword_p;

	CString GetLine(int nLine);
    int GetPosXY(int x, int y);
public:

	CXmlPrt();
	~CXmlPrt();
	
	virtual void __stdcall prtt(PSTR s);
	virtual void __stdcall XMLbegin(enum XMLTYPE xmltype, PVOID p);
	virtual void __stdcall XMLend(enum XMLTYPE xmltype);

	void	prtprtout(class CXmlOutPro* prt);

	void Clear();
	void Clicked(long x1,long x2);
	BOOL GetRightWord(long curpos, long &posfrom, long &posto);
	BOOL GetLeftWord(long curpos, long &posfrom, long &posto);
	BOOL GetCurWord(long curpos, long &posfrom, long &posto);
	int MoveHome(int nLine);
	int MoveLeftWord(int x, int y);
	int MoveRightWord(int x, int y);
	int WordToLeft(int x, int y);
	int WordToRight(int x, int y);
    CString GetText(int y1, int x1, int y2, int x2);
	
    void GetItem(int x, int y, OUT enum XMLTYPE& xmltype, OUT PVOID& p);
	void Display(I_COLOROUT* iColorOut);
    int GetLineCount();
    int GetLineLength(int nLine);
    CString GetString();
    void CommaLast();
    bool SetCurWord(int x, int y);
};

class CXmlOutPro
{
    I_XmlOut* m_out;
	UINT    m_nIdent;
    bool b_OneLine;
    bool fHasSpace;
public:
	BOOL	m_f_prt_in_1line;	//	Ϊtrue����Ҫ�������һ���ϣ������лس�
	BOOL	m_f_prt_in_comma;	//	Ϊ 1 ������','����';'

    CXmlOutPro(I_XmlOut* out)	//  ��ʼ��
    {
        fHasSpace = false;
        b_OneLine = false;
        m_out = out;
        m_f_prt_in_1line = FALSE;
        m_f_prt_in_comma = FALSE;
        m_nIdent = 0;
    }

	void prtt(PSTR s)
    {
        if (fHasSpace)
            this->m_out->prtt(" ");
        fHasSpace = false;
        
        this->m_out->prtt(s);
    }
	void prtslen(PSTR s, int len);

	void XMLbegin(enum XMLTYPE xmltype, PVOID p)
    {
        if (fHasSpace)
            this->m_out->prtt(" ");
        fHasSpace = false;
        this->m_out->XMLbegin(xmltype,p);
    }

	void XMLend(enum XMLTYPE xmltype)
    {
        this->m_out->XMLend(xmltype);
        this->fHasSpace = true;
    }

    void prtspace(int n = 1)
    {
        for (int i=0; i<n; i++) 
			prtt(" ");
    }

	void prtf(PSTR fmt,...);
	void prtl(PSTR fmt,...);
	void	EOL();		//	һ������£���";\n"
    void endline();

    void SetOneLine(bool b)
    {
        b_OneLine = b;
        if (b)
        {
            m_f_prt_in_1line = true;
            m_f_prt_in_comma = true;
        }
        else
        {
            m_f_prt_in_1line = false;
            m_f_prt_in_comma = false;
        }
    }

	
	void	ident_add1();
	void	ident_sub1();
	void    ident();
	void 	prtl_ident(PSTR fmt,...);
    void 	prtf_ident(PSTR fmt,...);

    void    nospace()
    {
        fHasSpace = false;
    }

    // ���class�������ǣ�����������ṩ��һ��I_XmlOut��ʹ������������
    // ��Ҫ��չһ��
};

#endif	//	CXmlPrt_H
