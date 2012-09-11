// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
//	CFunc_CreateInstrList.cpp

#include "cisc.h"

//  ��ȡ��������
//	��ȡintel�������У�ת��Ϊα���룬����CFunc��α�������б�instr_list
//  ͬʱ����CFunc���ڲ������� expr_list��
//  �����Ҫ�Ļ�����ȫ�ֱ����� g_expr_list �������Ŀ
//  ������ɹ���α�������б�instr_list�Ĺ���
//  ���� stack, expr, call �ȷ�����������
bool CFunc::Step6_CreateInstrList()
{
	m_instr_list = new INSTR_LIST;  //new_INSTR_LIST α���

    CCodeList the;
	the.CreateInstrList_raw(m_instr_list, this->ll.m_asmlist, this->m_EBP_base);

	// ����һ���� label ������,��Ϊ��ÿ��ָ��ǰ����label��Ҫȥ��û�����õ�
	Create_Labels_backend();

	// ���ƶ����� call ������
//	CreateInstrList_welldone_call();   	// find call, and fill its args

	return true;

}

PINSTR findlabel(INSTR_LIST* list, ea_t off)
{
	assert(off);	//	һ������£����ǲ�Ϊ���
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p->type == i_Label && p->label.label_off == off)
			return p;
	}
	assert_msg(0,"why here");
	return NULL;
}

void CFunc::Create_Labels_backend()	// ��ź��
{
	INSTR_LIST* list = this->m_instr_list;
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p->type == i_Jump)
		{
			PINSTR thelabel = findlabel(list,p->jmp.jmpto_off);
			if (thelabel->label.ref_instr)
				p->jmp.next_ref_of_this_label = thelabel->label.ref_instr;	// save old ref list
			thelabel->label.ref_instr = p;	// tell the label it was referred
			p->jmp.the_label = thelabel;	// tell the Jxx the label it need
		}
	}
	
	pos = list->GetHeadPosition();

	while (pos)
	{	// remove all label not referred
		POSITION savpos = pos;
		PINSTR p = list->GetNext(pos);
		if (p->type == i_Label && p->label.ref_instr == 0)
		{
			list->RemoveAt(savpos);		// ���������ת���ı��
		}
	}
}


