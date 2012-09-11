// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#pragma once

enum enum_CallC
{	//	0 for invalid or unknown
	enum_unknown=	0,
	enum_cdecl	=	1,
	enum_stdcall,
	enum_pascal,
	enum_fastcall,
};	//	calling convention

enum_CallC if_CallC(PSTR p);

class CFuncType
{
public:
	enum_CallC	m_callc;
	VarTypeID	m_retdatatype_id;	//	����ֵ����������
	BOOL		m_extern_c;
	BOOL		m_varpar;			//	ΪTRUE�����������ɱ�
	int			m_args;				//	�������������m_varpar==TRUE�����������ٸ���
									//	not include last "..."
	VarTypeID*	m_partypes;			//	ָ��һ��m_args��С�����飬�Ǹ�����������������
									//	VarTypeID[m_args] *
	PSTR*		m_parnames;			//	ָ��һ��m_args��С�����飬�Ǹ���������name,����argc,argv
									//	PSTR[m_args] *
	PSTR		m_pname;
	PSTR		m_internal_name;	//  can be "_printf", "printf$CRS"
	class Class_st*	m_class;		//	if not null, means this func is a subfunc of this class

	CFuncType();
	~CFuncType();

	void create_internal_funcname();
	CFuncType* ft_clone();

	BYTE get_stack_purge();
    UINT para_total_size();
    VarTypeID SearchPara(UINT off);
};
    

typedef CList<CFuncType*,CFuncType*> FuncTypeList;

void func_1(CFuncType* pfunc,PSTR p);
void func_define_2(CFuncType* pfunc,PSTR &p);


//	--------------------------------------------------------

CFuncType* Get_FuncDefine_from_internal_name(PSTR pmyinternalname);
CFuncType* Get_FuncDefine_from_name(PSTR pmyinternalname);
//	�Կ⺯��������internal_name����api��������func_name


