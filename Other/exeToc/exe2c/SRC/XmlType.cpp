// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	XmlType.cpp

#include "stdafx.h"
#include "Public_Fun.h"
#include "XmlType.h"

/*
enum XMLTYPE
{
	XT_invalid = 0,
	XT_blank,
	XT_Symbol,
	XT_Function,
	XT_Keyword,		//�ؼ��֣�����struct,union,for,while
	XT_Class,		//�Ǹ�class��union��struct������
	XT_K1,			//{} []
	XT_Comment,		//ע��
	XT_DataType,	//��������
	XT_Number,		//һ����
	XT_AsmStack,	//��ջֵ
	XT_AsmOffset,	//�����ʾʱ��seg:offset
	XT_AsmLabel,	//�����ʾʱ��label name
    
};
*/
struct
{
	COLORREF color1;
	COLORREF color2;
} tbl_color[] =
{
	{COLOR_DEFAULT,	RGB(0,0,0)},			//0
	{COLOR_DEFAULT,	RGB(0,0,0)},			//blank
	{RGB(155,255,25),	RGB(0,0,0)},		//symbol
	{RGB(255,0,255),	RGB(0,0,0)},		//function
	{RGB(255,255,0),	RGB(0,0,0)},		//keyword
	{RGB(255,255,0),	RGB(0,0,0)},		//Class
	{RGB(163,70,255),	RGB(0,0,0)},		//K1 brace
	{RGB(0,245,255),	RGB(0,0,0)},		//comment
	{RGB(100,222,192),	RGB(0,0,0)},		//datatype
	{RGB(0,255,0),		RGB(0,0,0)},		//number
	{RGB(0,70,255),		RGB(0,0,0)},		//AsmStack
	{RGB(70,180,70),	RGB(0,0,0)},		//AsmOffset
	{RGB(255,180,70),	RGB(0,0,0)},		//AsmLabel
    

	{RGB(192,192,192),	RGB(0,0,0)},			//1
	{RGB(0,	255,0),	RGB(0,0,0)},			//2
	{RGB(0,	0,	255),	RGB(0,0,0)},		//3
	{RGB(255,255,0),	RGB(0,0,0)},		//preprocessor
	{RGB(0,	255,0),	RGB(0,0,0)},			//string
	{RGB(255,0,0),	RGB(0,0,0)},			//red
	{RGB(0,	0,	255),	RGB(0,0,0)},		//symbol name in ASM out
	
};

// ���ø���������ɫ
COLORREF XmlType_2_Color(XMLTYPE xmltype)
{
    switch (xmltype)
    {
    case XT_invalid  : return COLOR_DEFAULT;
    case XT_blank    : return COLOR_DEFAULT;
    case XT_Symbol   : return RGB(57,109,165);
    case XT_Function : return RGB(255,255,255);
    case XT_Keyword  : return RGB(255,255,0);
    case XT_Class    : return RGB(255,255,0);
    case XT_K1       : return RGB(163,70,255);
    case XT_Comment  : return RGB(0,245,255);
    case XT_DataType : return RGB(100,222,192);
    case XT_Number   : return RGB(0,255,0);
    case XT_AsmStack : return RGB(0,70,255);
    case XT_AsmOffset: return RGB(70,180,70);
    case XT_AsmLabel : return RGB(255,180,70);
    case XT_FuncName : return RGB(255,0,255);
    }
    return COLOR_DEFAULT;
}

UINT __stdcall PopUpKeys(PSTR msgtbl[])
{
	return 0;
}

PSTR tbl_Key_Function[] =
{
	"N: rename",
	"H: help",
	NULL
};
void XML_Clicked(XMLTYPE xmltype, PVOID p)
{
	switch (xmltype)
	{
	case XT_Function:
		{
			UINT key = PopUpKeys(tbl_Key_Function);
		}
		break;
	}
}

