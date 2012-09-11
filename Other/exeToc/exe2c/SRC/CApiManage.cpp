// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	CApiManage.cpp
#include "stdafx.h"
#include "cisc.h"
#include "cexe2c.h"

CApiManage* g_ApiManage = NULL;

class CApiManage_cpp
{
public:
	CApiManage_cpp();
	~CApiManage_cpp();
};

CApiManage_cpp self;

CApiManage_cpp::CApiManage_cpp()
{
	g_ApiManage = new CApiManage;   //new_CApiManage
}

CApiManage_cpp::~CApiManage_cpp()
{
	SAFE_DELETE(g_ApiManage);

}
//	--------------------------------------------------

bool CApiManage::new_api(ea_t address,int stacksub)
{
	CApi *p = new CApi;     //new_CApi
	p->address = address;
	p->m_stack_purge = stacksub;
	
	sprintf(p->name,"api_%x",address);
    
	this->apilist->AddHead(p);
	return true;
}

CApi*	CApiManage::get_api(ea_t address)	// ͨ����ַ��ȡCApi��Ϣ��û���ҵ�����NULL
{
	//if(address<0x10000)
	//	return NULL;

	POSITION pos = this->apilist->GetHeadPosition();	// ��ȡAPI�б���׵�ַ
	while (pos)
	{
		CApi* p = this->apilist->GetNext(pos);
		if (p->address == address)						// �жϺ�����ַ�Ƿ����API��ַ
		{
			return p;
		}
	}
	if(g_Cexe2c->GetFunc(address)==NULL)
	{
		PBYTE temp01=ea2ptr(address);
		ea_t addr= *(DWORD*)temp01;

		if(g_Cexe2c->GetFunc(addr)==NULL)
			log_prtl("0x%08X ����δ����ĺ���", address);
	}
    //assert(0);
	return NULL;
}

// ע����ʱ��apiaddrʵ�����Ǹ�ptr������ea_t,��Ϊ��ʱea2ptr������������������
// ��������е�API��Ϣ��ӵ�apilist
void CApiManage::New_ImportAPI(PSTR pstr, DWORD apiaddr)
{
    //if (!stricmp(pstr, "RegisterClassExA"))
    //log_prtl("Finded ImportAPI %s 0x%x", pstr, apiaddr);
    
    CFuncType* pf = Get_FuncDefine_from_name(pstr);
	if (pf == NULL)
	{
		log_prtl("Finded ImportAPI 0x%08X %s δ����",apiaddr, pstr);
		return;
	}
	else
		log_prtl("Finded ImportAPI 0x%08X %s ", apiaddr, pstr);

	CApi *p = new CApi;     //new_CApi
	p->address = apiaddr;
    assert(pf);
    p->m_functype = pf;
	p->m_stack_purge = pf->get_stack_purge(); //g_FuncDefineMng.API_stack(pstr);
	
	strcpy(p->name, pstr);
    
	this->apilist->AddHead(p);
}


PSTR check_if_jmp_api(PCBYTE phead)	// �ж��Ƿ�����ת��API
{
	if (*(WORD *)phead != 0x25ff)		// ����Ϊǰ��λ�Ƿ�Ϊ0x25FF
		return NULL;

    phead += 2;
	DWORD d = *(DWORD *)phead;

	CApi* papi = g_ApiManage->get_api((ea_t)d);
	if (papi == NULL)
	{
		log_prtl("error!!! %x", d);

		return NULL;
	}

	PSTR name = papi->name;
	//alert_prtf("I find jmp api %s",name);
	return name;
}

