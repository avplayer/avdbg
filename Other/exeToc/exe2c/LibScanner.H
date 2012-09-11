///////////////////////////////////////////////////////////////
//
// LibScanner.h
// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com
//
///////////////////////////////////////////////////////////////
//#include "..\..\LibScanner\LibScanner.H"

#pragma once

#include "..\I_KSUNKNOWN\KsFrame.h"

#pragma warning(disable:4200)

typedef struct REFSYMBOL
{
	CHAR    RefSymbol[_MAX_PATH];	//���÷���
	WORD	RefType;				//such as IMAGE_REL_I386_REL32
	DWORD	RefOffset;				//�����ں����е�ƫ��
} *PREFSYMBOL;

typedef struct tagFUNCTION_SYMBOL
{
	DWORD       dwFuncLen;
	BYTE*		FunRawData;
	CHAR		ObjName[_MAX_PATH];		// ��������
	CHAR		FunctionName[_MAX_PATH];// ��������
	DWORD		RefCount;			// ���õķ��Ŵ���
	REFSYMBOL	RefInfo[];			// ������Ϣ
}FUNCTION_SYMBOL,* PFUNCTION_SYMBOL;

class I_LIBSCANNER : public I_KSUNKNOWN
{
public:
	//Add interface here
    
	virtual BOOL __stdcall ScanLib(LPCSTR szLib)=0;
	virtual PFUNCTION_SYMBOL __stdcall GetFunctionInfo(LPCSTR szFun)=0;
    virtual PSTR __stdcall CheckIfLibFunc(PCBYTE phead) = 0;
};

KS_DECLARE_INTERFACE(LibScanner, LIBSCANNER)

extern I_LIBSCANNER* g_LIBSCANNER;

/*	����Žӿں�������ϸ����
	
*/	