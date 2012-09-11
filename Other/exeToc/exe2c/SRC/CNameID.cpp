// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
#include	"CISC.H"


CNameMng::CNameMng()
{
	list = new CNameList;
}

CNameMng::~CNameMng()
{
	if (list)
	{
		POSITION pos = list->GetHeadPosition();
		while (pos)
		{
			st_NameID * p = list->GetNext(pos);
			SAFE_DELETE(p->name);
		}

		SAFE_DELETE(list);
	}

	//list->RemoveAll();
	//SAFE_DELETE(list);
}


H_NAMEID CNameMng::savname(PSTR name)	// ��������
{
    H_NAMEID h = this->LookupName(name);
    if (h != 0)
        return h;   //��Ҫ�ظ���

	st_NameID* p = new st_NameID;
	p->name = new_str(name);
	p->id = (H_NAMEID)p->name;	//	������������Ҳ��Ψһ��

	list->AddTail(p);

	return p->id;
}

PSTR CNameMng::id2name(H_NAMEID id)	// ͨ��ID��ȡ����
{
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		st_NameID* p = list->GetNext(pos);
		if (p->id == id)
			return p->name;
	}
	return NULL;
}

H_NAMEID CNameMng::LookupName(PSTR name)	// ͨ�����ƻ�ȡID
{
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		st_NameID* p = list->GetNext(pos);
		if (!strcmp(name, p->name))
		{
			return p->id;
		}
	}
	return 0;
}

void CNameMng::Rename(H_NAMEID id, PSTR newname)	// ������
{
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		st_NameID* p = list->GetNext(pos);
		if (p->id == id)
        {
            strcpy(p->name, newname);
            return;
        }
	}
}
