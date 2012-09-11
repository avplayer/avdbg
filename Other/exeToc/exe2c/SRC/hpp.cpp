// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com


//	exe2c project

#include "stdafx.h"

#include "Public_Fun.h"
#include "HPP.h"

#include "CClassManage.h"
#include "CEnumMng.h"
#include "CCbuf.h"
#include "strparse.h"
#include "SVarType.h"

#include "io.h"
#include "hxd_Lib.h"

// �����������

//#define HXD_OUTTEXT

char g_incpath[256];

//---ȫ�ֺ���------------------
VarTypeID Get_Var_Declare(PSTR &p, OUT PSTR name);

bool LoadIncFile(PSTR fname);
void prt_defines();
PSTR get_define(PSTR partern);
void define_replace(PSTR buf);

void One_Line_pre(PSTR lbuf); // ���������� # ��ʼ����
VarTypeID do_struct(PSTR &p);
VarTypeID do_union(PSTR &p);
void skip_string(char c1, PSTR & p);

//------------------------------

BOOL hpp_init()
{
	g_Hpp = new CHpp;

	g_DefineList = new DefineList;
	g_ClassManage = new CClassManage;
	g_enum_mng = new Enum_mng;
	
	strcpy(g_incpath, GetMyExePath());// �õ�����·��
	strcat(g_incpath, "\\inc\\");	  // ����inc��ȡ·��

//	if (g_EXEType == enum_PE_sys)
//		strcat(g_incpath, "\\ntddk\\");

	LoadIncFile("MY.H");

	return TRUE;
}

BOOL hpp_exit()
{
	SAFE_DELETE(g_Hpp);
	SAFE_DELETE(g_enum_mng);
	SAFE_DELETE(g_ClassManage);

	if (g_DefineList)
	{
		POSITION pos = g_DefineList->GetHeadPosition();
		while (pos)
		{
			define_t* p = g_DefineList->GetNext(pos);
			SAFE_DELETE(p);
		}

		SAFE_DELETE(g_DefineList);
	}

	return TRUE;
}

//------------------------------

CHpp::CHpp()
{
	m_FuncTypeList = new FuncTypeList;	// ���������б�
}

CHpp::~CHpp()
{
	if (m_FuncTypeList)
	{
		POSITION pos = m_FuncTypeList->GetHeadPosition();
		while (pos)
		{
			CFuncType* p = m_FuncTypeList->GetNext(pos);

			SAFE_DELETE(p);
		}

		SAFE_DELETE(m_FuncTypeList);
	}
}

define_t::define_t()
{
	src = NULL;
	dst = NULL;
}

define_t::~define_t()
{
	delete src;
	delete dst;
}

//	----------------------------------------------------------

CCInfo::CCInfo()
{
	comma1 = 0;
	comma2 = 0;
	extern_c = 0;
	m_default_callc = enum_stdcall;

	m_len = 0;
	m_buf = NULL;
}

CCInfo::~CCInfo()
{
	SAFE_DELETE(m_buf);
}

void CCInfo::LoadFile(FILE *f)
{
	CCbuf ccbuf;
	ccbuf.LoadFile(f);
	m_buf = ccbuf.m_p;
	m_len = ccbuf.m_len;
}

//  ����#��ʼ���У���#include "stdio.h" �� #define One,1
//	�Ѿ�ȥ�������Ļ��з�
//	�Ѿ���\���ӵ��н�������
void CCInfo::OneLine( PSTR lbuf, PSTR &pnext )
{
	PSTR p = lbuf;
	assert(p);

	skip_space(p);	// һ�п�ʼ���ֵĿո�ȫ������

	if (*p == '#')	// ����#��ʼ����
	{
		One_Line_pre(p);
		return;
	}

	//_CRTIMP int __cdecl printf(const char *, ...);
	PSTR p1 = p;
	char part1[80];
	get_1part(part1,p1);
	skip_eos(p1);
	
	if (strcmp(part1,"__inline") == 0)
	{
		pnext = p1;
		OneLine(p1,pnext);
		return;
	}
	if (strcmp(part1,"extern") == 0)
	{
		char part2[80];
		assert(*p1);
		PSTR p2 = p1;
		get_1part(part2,p2);
		if (strcmp(part2,"\"C\"") == 0)
		{	//extern "C"
			//printf("extern C find\n");
			assert(*p2);			
			if (*p2 == '{')
			{
				assert(this->comma1 == 0);
				assert(this->comma2 == 0);
				this->extern_c++;
				this->OneLine(p2+1, pnext);
				return;
			}
			else
			{	//	just 1 line
				this->extern_c++;
				this->OneLine(p2, pnext);
				this->extern_c--;
				return;
			}
		}
	}
	else if (strcmp(part1,"typedef") == 0)
	{
		do_typedef(p1);
		pnext = p1;
		return;
	}
	else if (strcmp(part1,"struct") == 0)
	{
		do_struct(p1);
		pnext = p1;
		return;
	}
	else if (strcmp(part1,"union") == 0)
	{
		do_union(p1);
		pnext = p1;
		return;
	}
	else if (strcmp(part1,"class") == 0)
	{
		do_class(p1,pnext);
		return;
	}
	else if (strcmp(part1,"enum") == 0)
	{
		do_enum(p1);
		pnext = p1;
		return;
	}
	{	//	����ǲ��Ƕ��е� func define
		skip_eos(p);

		PSTR pf = p;
		VarTypeID id = get_DataType(pf);	// �õ������ķ�������
		if (id)	//	��һ�����϶�����������
		{
			char buf1[80];
			skip_eos(pf);
			get_1part(buf1,pf);
			enum_CallC cc = if_CallC(buf1);
			if (cc == enum_unknown)
				cc = enum_stdcall;			// �������÷�ʽĬ��Ϊ _stdcall
			else
			{
				skip_eos(pf);
				get_1part(buf1,pf);
			}
			skip_eos(pf);
			if (*pf == '(')		//	���£��϶���func define ��
			{
				CFuncType* pnewfunc = new CFuncType;
				if (this->extern_c)
					pnewfunc->m_extern_c = TRUE;
				pnewfunc->m_callc = cc;				// �������÷�ʽ
				pnewfunc->m_retdatatype_id = id;	// ������������
				pnewfunc->m_pname = new_str(buf1);	// ��������
				//this->do_func_proto
				func_define_2(pnewfunc,pf);
				g_Hpp->newfunc_addlist(pnewfunc);
				if (*pf == '{')
				{
					int n = 1;
					for (;;)
					{
						pf++;
						if (*pf == '{') n++;
						if (*pf == '}') n--;
						if (n == 0) break;
					}
				}
				else if (*pf == ';')
				{
				}
				else
#ifndef HXD_OUTTEXT
					assert(0);
#endif
				pf++;
				skip_eos(pf);
				pnext = pf;
				return;
			}
		}
	}
	int old_comma1 = this->comma1;
//	int old_comma2 = this->comma2;
	for(;;)
	{
		char c = *p++;
		if (c == '\0')
			break;
		if (c == '\'' || c == '\"')
		{
			skip_string(c,p);
			continue;
		}
		if (c == '(')
		{
			this->comma1++;
			continue;
		}
		if (c == '{')
		{
			this->comma2++;
			continue;
		}
		if (c == ')')
		{
			this->comma1--;
			if (old_comma1 == 0	&& this->comma1 == 0)
			{
				PSTR p1 = p;
				if (*p1 == ' ')
					p1++;
				if (*p1 == ';')
				{	//	���ڣ�����Ϊ���Ѿ��ҵ�һ����������
					//printf(":: %s\n",lbuf);
					g_Hpp->func_define(lbuf, this);
				}
			}
			continue;
		}
		if (c == '}')
		{
			if (this->comma2)
				this->comma2--;
			else if (this->extern_c != 0)
				this->extern_c--;
			else
				assert(0);	//extra '}' find			
			continue;
		}
			
	}
}

//	----------------------------------------------------------

bool LoadIncFile(PSTR fname)
{
	char path[256];
	strcpy(path,g_incpath);
	strcat(path, fname);

	//alert_prtf("begin file %s\n",fname);
	FILE* f = fopen(path,"rb");
	if (f == NULL)
	{
		alert_prtf("File open error: %s",path);
		return FALSE;
	}

	CCInfo *pInfo = new CCInfo;

	pInfo->LoadFile(f);
	
	fclose(f);

	PSTR p = pInfo->m_buf;
	PSTR plast = p + pInfo->m_len;	// �ļ�������λ��
	while (p < plast)
	{
		
		PSTR pnext = NULL;
		pInfo->OneLine(p, pnext);	//	һ������£�OneLine���ᶯpnext,������Ƕ��У��ͻ��pnextָ�����
		
		if (pnext == NULL)
		{
			p += strlen(p) + 1;
		}
		else
		{
			assert( memcmp(pnext,"E ",2) );
			p = pnext;
		}
	}

	assert(pInfo->comma1 == 0);
	assert(pInfo->comma2 == 0);
	assert(pInfo->extern_c == 0);

	if(pInfo->m_buf)
	{
		delete pInfo->m_buf;
		pInfo->m_buf = NULL;
	}

	if(pInfo)
	{
		delete pInfo;
		pInfo = NULL;
	}

	//printf("end   file %s\n",fname);
	return TRUE;
}
void LoadIncBuffer(PSTR p,PSTR plast)
{
	CCInfo* pInfo = new CCInfo;
	
	while (p < plast)
	{
		PSTR pnext = NULL;
		pInfo->OneLine(p, pnext);	//	һ������£�OneLine���ᶯpnext
									//	������Ƕ��У��ͻ��pnextָ�����
		if (pnext == NULL)
		{
			p += strlen(p) + 1;
		}
		else
		{
			p = pnext;
		}
	}

	delete pInfo;
}
void prt_defines()
{
	POSITION pos = g_DefineList->GetHeadPosition();
	while (pos)
	{
		define_t* p = g_DefineList->GetNext(pos);
		p;
		//printf("#define | %s | %s\n",p->src,p->dst);
	}
}

// ��ӵ��궨���б���
void do_define(PSTR p1,PSTR p2)
{	//	means #define p1 p2
	POSITION pos = g_DefineList->GetHeadPosition();
	while (pos)
	{
		define_t* p = g_DefineList->GetNext(pos);
		if (strcmp(p->src,p1) == 0)
		{
			if (strcmp(p->dst,p2) == 0)
				return;
			break;
		}
	}
	//	not find
	define_t* pnew = new define_t;
	pnew->src = new_str(p1);
	pnew->dst = new_str(p2);
	if (pos)
		g_DefineList->InsertBefore(pos,pnew);
	else
		g_DefineList->AddTail(pnew);
}

// ���������� # ��ʼ����
void One_Line_pre(PSTR lbuf)
{	
	PSTR p = lbuf;
	assert(*p == '#');
	p++;

	if (memcmp(p,"define",6) == 0)	// �궨��
	{
		p+=6;
		skip_space(p);
		assert(*p);	//#define ���治��ʲôҲû��

		char buf[280];
		get_1part(buf,p);
		skip_space(p);
		do_define(buf,p);	// ��ӵ�g_DefineList�б���
		return;
	}
	if (memcmp(p,"include",7) == 0)
	{
		p += 7;
		skip_space(p);
		if (*p == '\"' || *p == '<')
		{
			char path[80];
			p++;
			int i=0;
			while (*p != '\"' && *p != '>')
			{
				path[i++] = *p++;
			}
			path[i] = '\0';
			log_prtf("Load include file: %s\n",path);
			LoadIncFile(path);
		}
		return;
	}
}

void skip_string(char c1, PSTR & p)
{
	//c1 is ' or "
	for(;;)
	{
		char c = *p++;
		if (c == '\\')
		{
			p++;
			continue;
		}
		assert(c != '\0');
		if (c == c1)
			return;
	}
}
BOOL Read_Var_Declare(PSTR pstr, st_Var_Declare* pvardcl)
{
	VarTypeID id = Get_Var_Declare(pstr, pvardcl->m_name);
	if (id == 0)
	{
		alert("Read Struct Item error");
		return FALSE;
	}
	pvardcl->m_size = GG_VarType_ID2Size(id);
	pvardcl->m_vartypeid = id;
	return TRUE;
}
void log_display_structure(Class_st* p)
{
	log_prtt("struct ");
	log_prtt(p->m_name);
	log_prtf("\n{    \\\\sizeof = 0x%x\n",p->m_size);
	int nident = 1;
	for (int i=0;i<p->m_nDataItem;i++)
	{
		st_Var_Declare* pv = &p->m_DataItems[i];
		if (pv->m_access == nm_sub_end)
			nident--;
		for (int j=0;j<nident;j++)
			log_prtt("    ");
		if (pv->m_access == nm_substruc)
		{
			log_prtt("struct {\n");
			nident++;
			continue;
		}
		if (pv->m_access == nm_subunion)
		{
			log_prtt("union {\n");
			nident++;
			continue;
		}
		if (pv->m_access == nm_sub_end)
		{
			log_prtt("} ");
			if (pv->m_name[0])
				log_prtt(pv->m_name);
			log_prtt(";\n");
            continue;
		}

//		log_prtl("%s\t%s;\t//+%02x",
//				 GG_VarType_ID2Name(pv->m_vartypeid),
//				 pv->m_name,
//				 pv->m_offset_in_struc);
	}
	log_prtt("}\n");
}

VarTypeID do_struct_after_name(PSTR strucname, PSTR &p, BOOL Fstruc_Tunion);
VarTypeID do_struct(PSTR &p)
{
	PSTR savp = p;

	char name[80];
	get_1part(name,p);

	skip_eos(p);

	if (*p == '{')
		return do_struct_after_name(name,p,FALSE);
	VarTypeID id = g_VarTypeManage->NewUnknownStruc(name);
	return id;
}

VarTypeID do_union(PSTR &p)
{
	PSTR savp = p;

	char name[80];
	get_1part(name,p);

	skip_eos(p);

	if (*p == '{')
		return do_struct_after_name(name,p,TRUE);
	VarTypeID id = g_VarTypeManage->NewUnknownStruc(name);
	return id;
}

VarTypeID do_struct_after_name(PSTR strucname, PSTR &p, BOOL Fstruc_Tunion)
{
	assert(*p == '{');
	p++;
	skip_eos(p);

	int n = 0;
	st_Var_Declare items[150];	//50����������
	SIZEOF size = 0;
	ZeroMemory(items,150 * sizeof(st_Var_Declare));

	BOOL f_tbl[20];
	BOOL f = Fstruc_Tunion;	//FALSE means struct, TRUE means union
	SIZEOF size_tbl[20];
	SIZEOF maxsize_tbl[20];
	SIZEOF maxsize_in_union = 0;
	int	substruc_stack = 0;

	while (*p != '}' || substruc_stack != 0)
	{
		assert(n < 150);
		st_Var_Declare* pvar = &items[n];
		{
			PSTR savp = p;
			char buf[80];
			get_1part(buf,p);
			skip_eos(p);
			if (strcmp(buf,"struct") == 0 && *p == '{')
			{	//	����һ��struct �е�struct����
				size_tbl[substruc_stack] = size;
				maxsize_tbl[substruc_stack] = maxsize_in_union;
				f_tbl[substruc_stack++] = f;
				assert(substruc_stack<20);
				f = FALSE;
				pvar->m_access = nm_substruc;
				n++;

				p++;
				skip_eos(p);
				continue;
			}
			if (strcmp(buf,"union") == 0 && *p == '{')
			{	//	����һ��struct �е�union����
				size_tbl[substruc_stack] = size;
				maxsize_tbl[substruc_stack] = maxsize_in_union;
				f_tbl[substruc_stack++] = f;
				assert(substruc_stack<20);
				f = TRUE;
				maxsize_in_union = size;
				pvar->m_access = nm_subunion;
				n++;

				p++;
				skip_eos(p);
				continue;
			}
			
			p = savp;
		}
		if (substruc_stack > 0 && *p == '}')
		{
			if (f)	//union
			{
				size = maxsize_in_union;
			}
			substruc_stack--;
			f = f_tbl[substruc_stack];
			if (f)
			{
				maxsize_in_union = maxsize_tbl[substruc_stack];
				if (size > maxsize_in_union)
					maxsize_in_union = size;
				size = size_tbl[substruc_stack];	//	�˻�ԭ����size
			}
			pvar->m_access = nm_sub_end;
			n++;

			p++;
			skip_eos(p);
			if (*p != ';')
			{	//	˵��������
				get_1part(pvar->m_name,p);
				skip_eos(p);
				assert(*p == ';');
			}
			p++;
			skip_eos(p);
			continue;
		}
		Read_Var_Declare(p,pvar);
		pvar->m_offset_in_struc = size;

		//alert_prtf("var_item %s",pvar->m_name);

		if (!f)
			size += pvar->m_size;
		else
		{	//union
			if (size + pvar->m_size > maxsize_in_union)
				maxsize_in_union = size + pvar->m_size;
		}
		n++;
		p += strlen(p)+1;
	}
	

	p++;
	skip_space(p);
	if (*p == ';')
	{
		p++;
		skip_eos(p);
	}

	//	------------------------------------------
	Class_st* pnew = new Class_st;
	strcpy(pnew->m_name, strucname);
	pnew->m_nDataItem = n;
	pnew->m_DataItems = new st_Var_Declare[n];
	memcpy(pnew->m_DataItems, items, sizeof(st_Var_Declare)*n);
	if (Fstruc_Tunion)
		pnew->m_size = maxsize_in_union;
	else
		pnew->m_size = size;
	pnew->m_Fstruc_Tunion = Fstruc_Tunion;

	log_display_structure(pnew);

	g_ClassManage->new_struc(pnew);

	return g_VarTypeManage->Class2VarID(pnew);
}


void CCInfo::do_typedef(PSTR &p)
{
	PSTR savp = p;
	assert(p);
	assert(*p);
	assert(*p != ' ');

	VarTypeID id = get_DataType_bare(p);
	if (id == 0)
	{
		// �����������͵�����ļ���

#ifdef HXD_OUTTEXT
		char buff[255];
		wsprintf(buff,"typedef DWORD %s",savp);
		OutToTxt(buff);
#else
		alert_prtf("unknown datatype : %s",savp);
		return;
#endif
	}

	skip_eos(p);

	do_typedef_(id,p);
	
	assert(*p == ';');	// һ������Ѵ�����
	p++;
	skip_eos(p);
}

void CCInfo::do_typedef_(VarTypeID baseid, PSTR &p)
{
	PSTR savp = p;
	//	����һ�� typedef char *LPSTR, *PSTR;
	//	������ֻʣһ�� *LPSTR, *PSTR;
	VarTypeID id = Get_Additional_id(baseid, p);

	if (*p == '(')
	{	//	it must be a function point
		p++;
		skip_space(p);
		
		enum_CallC callc = this->m_default_callc;
		if (*p != '*')
		{
			char part1[80];
			get_1part(part1,p);
			callc = if_CallC(part1);
			assert(*p == '*');
		}
		assert(callc != enum_unknown);
		p++;
		skip_space(p);

		char name[80];
		get_1part(name,p);
		assert(*p == ')');
		p++;
		skip_space(p);
		assert(*p == '(');
		
		CFuncType* pnewfunc = new CFuncType;
		if (this->extern_c)
		{
			pnewfunc->m_extern_c = TRUE;
		}

		func_define_2(pnewfunc,p);

		pnewfunc->m_callc = callc;
		pnewfunc->m_retdatatype_id = id;
		pnewfunc->m_pname=new_str(name);
		//	�����ڲ�������
		pnewfunc->create_internal_funcname();

		VarTypeID id_f = g_VarTypeManage->FuncType2VarID(pnewfunc);
		g_VarTypeManage->NewTypeDef(id_f, name);
		
		return;
	}

	char name[80];
	get_1part(name, p);

	{	//	���һ���Ƿ��Ѿ��������
		PSTR p1 = name;
		VarTypeID id1 = get_DataType_bare(p1);
		if (id1 != 0)
		{
			if (id1 >= 10)
				alert_prtf("already typedef : %s", name);
			//	����Ԥ������BYTE,WORD,DWORD,PVOID,������
			return;
		}
	}

//	log_prtl("^^ typedef %s === %s",
//			 GG_VarType_ID2Name(id),
//			 name);
	g_VarTypeManage->NewTypeDef(id, name);

	skip_eos(p);
	if (*p == ';')
		return;
	else if (*p == ',')
	{
		p++;
		skip_eos(p);
		do_typedef_(baseid,p);
		return;
	}
	else
	{
		nop();
		alert_prtf("expect , or ; in typedef: %s",savp);
	}
}
CFuncType* CCInfo::do_func_proto_void(PSTR lbuf)
{	//	����û�к�������ֵ�����������class�Ĺ��캯��
	CFuncType* pnewfunc = new CFuncType;
	if (this->extern_c)
	{
		pnewfunc->m_extern_c = TRUE;
	}

	//printf(":: %s\n",lbuf);

	char name[128];
	PSTR p = lbuf;

	get_1part(name,p);

	assert(*p == '(');
	
	//	--------------------------

	pnewfunc->m_pname = new_str(name);
	pnewfunc->m_retdatatype_id = id_void;
    pnewfunc->m_callc = enum_stdcall;

	//	���棬�������Ĳ���

	func_define_2(pnewfunc,p);

	//	�����ڲ�������
	pnewfunc->create_internal_funcname();

	return pnewfunc;
}
CFuncType* CCInfo::do_func_proto(PSTR lbuf)
{
	CFuncType* pnewfunc = new CFuncType;
	if (this->extern_c)
	{
		pnewfunc->m_extern_c = TRUE;
	}

	//printf(":: %s\n",lbuf);

	char buf1[128];
	PSTR p = lbuf;
	PSTR p1 = buf1;
	while (*p != '\0' && *p != '(')
		*p1++ = *p++;
	*p1 = '\0';

	assert(*p == '(');
	
	//	--------------------------

	func_1(pnewfunc, buf1);

	//	���棬�������Ĳ���

	func_define_2(pnewfunc,p);

	//	�����ڲ�������
	pnewfunc->create_internal_funcname();

	return pnewfunc;
}
void CHpp::func_define(PSTR lbuf, CCInfo* pCCInfo)
{
	CFuncType* pnewfunc = pCCInfo->do_func_proto(lbuf);
	newfunc_addlist(pnewfunc);
}
void CHpp::newfunc_addlist(CFuncType* pnewfunc)
{

	pnewfunc->create_internal_funcname();
	//	�������뵽 g_FuncTypeList ��
	POSITION pos = m_FuncTypeList->GetHeadPosition();
	while (pos)
	{	//	��Ҫ��һ���Ƿ��Ѿ�����
		CFuncType* pft = m_FuncTypeList->GetNext(pos);
		if (pnewfunc->m_internal_name && pft->m_internal_name)
		if (strcmp(pft->m_internal_name,pnewfunc->m_internal_name) == 0)
		{
			delete pnewfunc;
			return;	//	�ҵ��ˣ��Ѿ����ˣ�������
		}
	}
	//alert_prtf("func: %s\ninternal: %s",pnewfunc->m_name,pnewfunc->m_internal_name);

	m_FuncTypeList->AddHead(pnewfunc);
}
VarTypeID Get_Var_Declare(PSTR &p, OUT PSTR name)
{	//	���Ǹ���һ��
	//	const char * pstr1,
	//	char const * pstr2,
	//	char * const pstr3,
	//	�ó�����������������ͺ�����

	//	�ر�Ҫ֧�� struct _NewStruc * name;
	

	VarTypeID id = get_DataType(p);
	if (id == 0)
	{
#ifdef HXD_OUTTEXT
		// �����������͵�����ļ���
		char buff[255];
		wsprintf(buff,"typedef DWORD %s",p);
		OutToTxt(buff);
#else
		alert_prtf("unknown datatype : %s",p);
		return 0;
#endif
	}

	get_1part(name,p);	//	ȡ�ò�����
	if (*p == '[')
	{
		//alert("[ find");
		p++;
		if (*p == ']')
		{
			p++;
			id = g_VarTypeManage->NewArray_id_id(id, SIZE_unknown);
		}
		else
		{
			DWORD d = Str2Num(p);			
			id = g_VarTypeManage->NewArray_id_id(id, d);
		}
	}
	return id;
}
void func_define_2(CFuncType* pfunc,PSTR &p)
{	//	����func�Ĳ�������"(int argc, char* argv[])"
	//	���ָ��')'���';'
	assert(*p == '(');
	p++;
	skip_space(p);
	skip_eos(p);

	int parnum = 0;
	VarTypeID pars[100];	//	100�������˰�?
	PSTR parnames[100];

	while (*p && *p != ')')
	{
		if (memcmp(p,"...",3) == 0)
		{
			pfunc->m_varpar = TRUE;
			p += 3;
			skip_space(p);
			skip_eos(p);
			assert(*p == ')');
			break;	//	��֪���������ٲ�����������������
		}

		char parname[80];
		VarTypeID id = Get_Var_Declare(p,parname);
		if (id == 0)
		{
#ifndef HXD_OUTTEXT
		alert_prtf("unknown datatype : %s",p);
		break;
#endif
		}
		if (id == id_void)
		{
			skip_eos(p);
			assert(*p == ')');
			assert(parnum == 0);
			break;
		}
		if (parname[0])
			parnames[parnum] = new_str(parname);
		else
			parnames[parnum] = NULL;

		pars[parnum++] = id;

		if (*p == ',')
			p++;
        skip_space(p);
		skip_eos(p);
	}
	p++;	//skip ')'
	skip_space(p);
	skip_eos(p);
	pfunc->m_args = parnum;
	pfunc->m_partypes = new VarTypeID[parnum];
	pfunc->m_parnames = new PSTR[parnum];
	for (int i=0;i<parnum;i++)
	{
		pfunc->m_partypes[i] = pars[i];
		pfunc->m_parnames[i] = parnames[i];
	}

}

//	pָ����Ǻ����Ķ��壬������������������������
//	p������"int","__cdecl","DWORD",....
//	Ҫ��pָʾ����Ϣ�pfunc��ȥ

void func_1(CFuncType* pft,PSTR p)
{
    assert(pft->m_retdatatype_id == 0);

	char part[80];

	pft->m_callc = enum_stdcall;	//	����һ��ȱʡֵ

	skip_space(p);
	//alert(p);

	while (*p)
	{
        if (pft->m_retdatatype_id == 0)
        {
            VarTypeID id = get_DataType(p);
            if (id != 0)
            {
                pft->m_retdatatype_id = id;
                continue;
            }
        }

		get_1part(part,p);
		assert(part[0]);
		if (*p == 0)
		{	//	���һ���֣��Ǻ�����
			//printf("func name = %s\n",part);
			pft->m_pname = new_str(part);
			break;
		}
		PSTR p1 = get_define(part);
		if (p1 == NULL)
			p1 = part;	//	�������һ��de fin��ת��һ��

		enum_CallC callc = if_CallC(p1);
		if (callc != 0)
		{
			pft->m_callc = callc;
			continue;
		}
		alert_prtf("-> %s",p1);
		assert(("I do not know this part",0));
	}
}

enum_CallC if_CallC(PSTR p)
{
	if (strcmp(p,"__cdecl") == 0)
		return enum_cdecl;
	if (strcmp(p,"__stdcall") == 0)
		return enum_stdcall;
	if (strcmp(p,"__pascal") == 0)
		return enum_pascal;
	if (strcmp(p,"__fastcall") == 0)
		return enum_fastcall;
	if (strcmp(p,"_fastcall") == 0)
		return enum_fastcall;
	return enum_unknown;
}
void define_replace(PSTR buf)
{
	PSTR p = get_define(buf);
	if (p)
		strcpy(buf,p);
}
PSTR get_define(PSTR partern)
{
	if (*partern == '\0')
		return NULL;
	POSITION pos = g_DefineList->GetHeadPosition();
	while (pos)
	{
		define_t* p = g_DefineList->GetNext(pos);
		if (strcmp(partern,p->src) == 0)
		{
			return p->dst;
		}		
	}
	return NULL;
}

CFuncType* Get_FuncDefine_from_internal_name(PSTR pmyinternalname)
{
	return g_Hpp->Get_FuncDefine_from_internal_name_(pmyinternalname);
}

CFuncType* CHpp::Get_FuncDefine_from_internal_name_(PSTR pmyinternalname)
{
	assert(m_FuncTypeList);
	POSITION pos = m_FuncTypeList->GetHeadPosition();
	while (pos)
	{
		CFuncType* pft = m_FuncTypeList->GetNext(pos);
		if (strcmp(pft->m_internal_name,pmyinternalname) == 0
		|| strcmp(pft->m_pname,pmyinternalname) == 0)
		{
			return pft;
		}
	}
	return NULL;
}

CFuncType* Get_FuncDefine_from_name(PSTR pmyname)
{
	return g_Hpp->Get_FuncDefine_from_name_(pmyname);
}

CFuncType* CHpp::Get_FuncDefine_from_name_(PSTR pmyname)
{
	//	?AfxWinMain@@YGHPAUHINSTANCE__@@0PADH@Z
	char name[128];
	{
		if (*pmyname == '?')
			pmyname++;
		name[127] = '\0';
		strcpy(name,pmyname);
		assert(name[127] == '\0');
        PSTR p = strchr(name,'@');
		if (p)
			*p = '\0';
	}
	assert(m_FuncTypeList);
	POSITION pos = m_FuncTypeList->GetHeadPosition();
	while (pos)
	{
		CFuncType* pft = m_FuncTypeList->GetNext(pos);
		if (pft->m_pname != NULL)
		{
            if (strcmp(pft->m_pname, name) == 0
                || strcmp(pft->m_internal_name, name) == 0)
            {
                return pft;
            }
		}
	}
	return NULL;
}

VarTypeID define_enum(PSTR &p)
{
	char name[80];
	get_1part(name,p);
	skip_eos(p);
	if (*p == '{')
	{
		p++;
		skip_space(p);
		skip_eos(p);

		enum_st* pnew = new enum_st;
		strcpy(pnew->m_name,name);
		pnew->m_pfirst = NULL;
		int nextnnn = 0;		//	first enum = 0
		while (*p != '}')
		{
			char itemname[80];
			get_1part(itemname,p);
			if (*p == '=')
			{
				p++;
				skip_space(p);
				nextnnn = Str2Num(p);
				while (*p != '\0' && *p != ',' && *p != '}')
					p++;

			}
			NumStr_st* newitem = new NumStr_st;
			newitem->n = nextnnn++;
			newitem->name = new_str(itemname);
			newitem->next = pnew->m_pfirst;
			pnew->m_pfirst = newitem;

			skip_space(p);
			skip_eos(p);
			if (*p == ',')
			{
				p++;
				skip_eos(p);
			}
			else if (*p == '}')
				break;
			else
				assert(0);
		}
		p++;
		g_enum_mng->Add_New_Enum(pnew);
		return g_VarTypeManage->Enum2VarID(pnew);
	}
	return 0;
}

VarTypeID do_enum(PSTR &p)
{
	VarTypeID id = define_enum(p);
	skip_space(p);
	if (*p == ';')
		p++;
    skip_eos(p);
	return id;
}
void CCInfo::do_class(PSTR p, PSTR &pnext)
{
	Class_st theclass;
	ZeroMemory(&theclass,sizeof(Class_st));

	get_1part(theclass.m_name,p);

	if (theclass.m_name[0] == '\0')
		return;
    if (!strcmp(theclass.m_name, "CTest1"))
    {
        strcmp(theclass.m_name, "CTest1");
    }

	skip_eos(p);

	if (*p != '{')
		return;
    p++;
	skip_eos(p);


	enumClassMemberAccess cma = nm_private;
	
	int n = 0;
	st_Var_Declare items[50];	//50����������
	ZeroMemory(items, 50 * sizeof(st_Var_Declare));
	SIZEOF size = 0;

	int nfunc = 0;
	CFuncType* funcs[50];
	ZeroMemory(funcs,50 * sizeof(CFuncType*));

	while (*p && *p != '}')
	{
		if (strcmp(p,"private:") == 0)
		{
			cma = nm_private;
			p += 9;
		}
		if (strcmp(p,"protected:") == 0)
		{
			cma = nm_private;
			p += 11;
		}
		if (strcmp(p,"public:") == 0)
		{
			cma = nm_public;
			p += 8;
		}
		if (strchr(p,'('))	//	�Ǻ���
		{
			CFuncType* pft = NULL;
			int sz = strlen(theclass.m_name);
			if (memcmp(p,theclass.m_name, sz) == 0
				&& if_split_char(p[sz]))	//	�ǹ��캯��
			{
				pft = do_func_proto_void(p);
			}
			else if (p[0] == '~')
			{
				assert(memcmp(p+1,theclass.m_name, sz) == 0);
				assert(if_split_char(p[sz+1]));	//	�ǹ�������
				pft = do_func_proto_void(p);
			}
			else
			{
				pft = do_func_proto(p);
			}
			assert(pft);
			funcs[nfunc++] = pft;
			assert(nfunc < 50);
			p+= strlen(p);	//	������
			p++;
		}
		else
		{	//	��������
			assert(n < 50);
			st_Var_Declare* pvar = &items[n];
			Read_Var_Declare(p, pvar);
			pvar->m_offset_in_struc = size;
			pvar->m_access = cma;

			size += pvar->m_size;
			n++;
			p += strlen(p)+1;
		}
	}
	theclass.m_nDataItem = n;
	theclass.m_size = size;
	theclass.m_DataItems = new st_Var_Declare[n];
	memcpy(theclass.m_DataItems, items, n * sizeof(st_Var_Declare));

	theclass.m_nSubFuncs = nfunc;
	theclass.m_SubFuncs = new CFuncType *[nfunc];
	memcpy(theclass.m_SubFuncs, funcs, nfunc * sizeof(CFuncType *));

	Class_st* pnew = new Class_st;
	*pnew = theclass;

    ZeroMemory(&theclass, sizeof(Class_st)); //��ֹtheclass����ʱɾ����

	pnew->set_subfuncs();
	g_ClassManage->add_class(pnew);

	//	���� p Ӧ��ָ�� "};"
	p++;
	skip_space(p);
	if (*p == ';')
		p++;
	p++;
	pnext = p;
}
