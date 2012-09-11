// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com


#pragma once

typedef DWORD ea_t;

//#include "../CXmlPrt/CXmlPrt.h"
#include "CFuncType.h"

enum enumClassMemberAccess
{
	nm_unknown	=	0,
	nm_private,
	nm_protected,
	nm_public,
	
	nm_substruc,
	nm_subunion,
	nm_sub_end,
};

struct st_Var_Declare
{	//	�Ƕ�һ�����Ķ��壬�����������ͺͱ�����
	//	������struct_struct�е�item���壬��functoin parameter�Ķ���ȵ�
	VarTypeID	m_vartypeid;
	SIZEOF		m_size;
	DWORD		m_offset_in_struc;
	char		m_name[80];
	enumClassMemberAccess	m_access;	//	ΪclassԤ��
};

//	----------------------------------------------------
//	class

class Class_st
{
public:
	BOOL	m_TclassFstruc;	//	TRUE means class, FALSE means struct
	char	m_name[80];		//	class��
	SIZEOF	m_size;			//	class��size,���ݲ���
	int		m_nDataItem;	//	�������ĸ���
	st_Var_Declare* m_DataItems;	//	sizeof = m_nDataItem ��һ��buffer
	BOOL	m_Fstruc_Tunion;	//TRUE = union

	ea_t	m_Vftbl;		//	���ĵ�ַ������еĻ�
	int		m_nSubFuncs;		//	�Ӻ����ĸ���
	CFuncType**		m_SubFuncs;	//	�����Ӻ���

public:
	Class_st();
	~Class_st();

	CFuncType* LookUp_SubFunc(PSTR name);
	BOOL	is_GouZ(CFuncType* pft);        //�ǹ���
	BOOL	is_GouX(CFuncType* pft);        //�ǹ���
	BOOL	is_GouZX(CFuncType* pft);       //�ǹ������
	PSTR	getclassitemname(DWORD off);
	st_Var_Declare* GetClassItem(DWORD off);
//	void	prtout(CXmlPrt* prt);
	void	set_subfuncs();
	BOOL	IfThisName(PSTR name);
	PSTR	getname();
};

typedef	CList<Class_st *, Class_st *> CLASS_LIST;

class CClassManage
{
	CLASS_LIST*	m_classlist;
public:

	CClassManage();
	~CClassManage();

	CFuncType* Get_SubFuncDefine_from_name(PSTR classname, PSTR funcname);
	void add_class(Class_st* pnew);
	Class_st* LoopUp_class_by_name(PSTR name);
	
	VarTypeID if_StrucName(PSTR &pstr);
	void	new_struc(Class_st* pnew);
};


extern	CClassManage* g_ClassManage;

