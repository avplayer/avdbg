// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
//	ComplexInstr.cpp

#include	"CISC.H"
bool g_f_Step_by_Step = false;
bool g_any1_return_TRUE = false;
bool Step_by_Step();


const char finger_for[] 	= "0_jmp1_from2_0_from1_0_jxx3_0_jmp2_from3_";
const char finger_long_if [] 	= "0_jxx1_jmp2_from1_0_from2_";
const char finger_if [] 	= "0_jxx1_0_from1_";
const char finger_if_else[] = "0_jxx1_0_jmp2_from1_0_from2_";
const char finger_while[] 	= "from1_0_jxx2_0_jmp1_from2_";
const char finger_dowhile[] = "from1_0_jxx1_";
const char finger_dowhile_2[] = "from1_0_jxx1_from2_";	
						
//	���������break���ͻ�����������������ʱ���Ҳ������õİ취
	// for(1;3;2)
	// {
	//	4
	// }
// �ͷ��ڴ�
void Hxd_DeleteUsedJumpList(JUMPOPTM_LIST *used_list)
{
	if (used_list)
	{
		POSITION pos = used_list->GetHeadPosition();
		while (pos)
		{
			POSITION savpos = pos;
			st_JumpOptm * p = used_list->GetNext(pos);

			used_list->RemoveAt(savpos);
			SAFE_DELETE(p);
		}

	}
}

// ��ʼ��ת���̷���
bool CInstrList::Hxd_JumpFlow()
{
	JUMPOPTM_LIST used_list;	//	��������instr
	char hxd_tbl_c[256];

	Get_Jump_Use_Flow(used_list);//�ȵõ�����ʹ��ͼ
	while(Optim_Jump_flow_NT(used_list,hxd_tbl_c))
	{
		log_prtl("%s",hxd_tbl_c);
		Hxd_DeleteUsedJumpList(&used_list);
		return true;
	}

	log_prtl("%s",hxd_tbl_c);
	Hxd_DeleteUsedJumpList(&used_list);
	return false;
}

// �õ���ת��ʶ
char  CInstrList::HowJumpUse_Char(st_JumpOptm* the)
{
	PINSTR pinstr = the->pinstr;

	switch(pinstr->type)
	{
	case i_Jump:
		if(pinstr->jmp.jmp_type == JMP_jmp)
		{
			if(the->JumpType == Jump_Down)	// ������
				return 'J';		// Jump
			else
				return 'C';		// Continue
		}
		else
		{
			if(the->JumpType == Jump_Down)	// ������
				return 'I';		// If
			else
				return 'W';		// While
		}
		break;
	case i_Label:
		switch(the->iLableType)
		{
		case Form_Up:
			return 'E';	// end if
		case Form_Down:
			return 'D';	// do
		case Form_All:
			return 'T'; // two,����������
		}
		break;
	case i_Return:
		return 'R';

	}

	return 'U';

}


// �õ���תʹ��α�����б�
void CInstrList::Get_Jump_Use_Flow(JUMPOPTM_LIST& used_list)
{
	INSTR_LIST* list = this->m_list;		// α�����
    POSITION pos = list->GetHeadPosition();

    while (pos)		// ���浱ǰ����ʹ����Ϣ��i_Label��i_Return��i_Jump
    {
        POSITION savpos = pos;
        PINSTR p = list->GetNext(pos);

		if(p->bJmpFlowOk)
			continue;

		st_JumpOptm* the;
		PINSTR pJmp;

		bool bUp =false;
		bool bDown =false;

		switch(p->type)
		{
		case i_Jump:	// ��ת
			the = new st_JumpOptm;        
            the->pinstr = p;
			the->pos = savpos;
			the->bJxx = true;
			// ��ת�ķ���
			if(p->jmp.linear < p->jmp.jmpto_off)	// ��ǰ��ַС����ת��ĵ�ַ��˵����������
				the->JumpType = Jump_Down;		// ������
			else
				the->JumpType = Jump_Up;		// ������
            used_list.AddTail(the);
			break;

		case i_Label:	// ��ʶ
			the = new st_JumpOptm;
            the->pinstr = p;
			the->pos = savpos;
			the->bLable = true;
			// �õ�jump�����������ַ
			pJmp=p->label.ref_instr;
			the->iLableJmpFromNum = 0;
			the->dwJumpFrom[the->iLableJmpFromNum++]=pJmp->jmp.linear;	// i_Jumpָ��ĵ�ַ

			if(pJmp->jmp.linear < p->label.label_off)
				bUp = true;			// ������������
			else
				bDown = true;
			
			while(pJmp->jmp.next_ref_of_this_label)
			{
				pJmp = pJmp->jmp.next_ref_of_this_label;
				the->dwJumpFrom[the->iLableJmpFromNum++]=pJmp->jmp.linear;
				if(pJmp->jmp.linear < p->label.label_off)
					bUp = true;			// ������������
				else
					bDown = true;
			}
			if(p->label.bJmpFlowNum == the->iLableJmpFromNum)	// �Ѿ�ȫ���������
			{
				list->RemoveAt(savpos);		// ������ɺ�ɾ��
				//p->bJmpFlowOk =true;
			}
			else
			{
				if(bUp)
				{
					if(bDown)
						the->iLableType = Form_All;
					else
						the->iLableType = Form_Up;	// ������������
				}
				else
					the->iLableType = Form_Down;

				used_list.AddTail(the);
			}
			break;

		case i_Return:
			the = new st_JumpOptm;       
            the->pinstr = p;
			the->pos = savpos;
            used_list.AddTail(the);
			break;
		default:
			break;
		}

    }

}

// ������ת����
bool CInstrList::Optim_Jump_flow_NT(JUMPOPTM_LIST& Jumplist,char hxd_tbl_c[])
{
    int n = 0;
    char tbl_c[256];
    PJUMPOPTM tbl_JumpOptm[256];

    POSITION pos = Jumplist.GetHeadPosition();
    while (pos)
    {
        PJUMPOPTM the = Jumplist.GetNext(pos);
        char b = HowJumpUse_Char(the);

        tbl_c[n] = b; tbl_c[n+1] = '\0';
        tbl_JumpOptm[n] = the;
        n++;

		memcpy(hxd_tbl_c,tbl_c,n+1);

        //if (n >= 1)
        //{
        //    const char* pb = tbl_c + n - 1;		// ���һ��������
        //    PINSTR* pi = tbl_pinstr + n - 1;		// ���һ��α����

        //    if (strcmp(pb, "A") == 0 )
        //    {
  
        //    }

        //}

        if (n >= 2)		// �������������
        {
            const char* pb = tbl_c + n - 2;
            PJUMPOPTM* pi = tbl_JumpOptm + n - 2;

			if ( strcmp(pb, "RR") == 0 
			|| strcmp(pb, "RJ") == 0
			|| strcmp(pb, "JR") == 0
			|| strcmp(pb, "JJ") == 0
			)
			{
				pi[1]->pinstr->bJmpFlowOk =true;	// �ڶ������ù���
				return true;
			}

            if ( strcmp(pb, "IE") == 0 )
            {
				if(pi[0]->pinstr->jmp.jmpto_off == pi[1]->pinstr->label.label_off)
				{
					// �����Ѿ��������
					pi[0]->pinstr->bJmpFlowOk =true;
					pi[1]->pinstr->label.bJmpFlowNum++;

					// ����
					Hxd_If(pi[0]->pos,pi[1]->pos,COMP_if);
					return true;
				}

            }

			if ( strcmp(pb, "DW") == 0 )
            {
				if(pi[1]->pinstr->jmp.jmpto_off == pi[0]->pinstr->label.label_off)
				{
					// �����Ѿ��������
					pi[0]->pinstr->label.bJmpFlowNum++;
					pi[1]->pinstr->bJmpFlowOk =true;

					// ������ if��ʽ��ͬ
					Hxd_do_While(pi[0]->pos,pi[1]->pos);
					return true;
				}
            }

			if (strcmp(pb, "IW") == 0)		// while �е�һ��break
			{
				pi[0]->pinstr->bJmpFlowOk =true;

				Hxd_Break(pi[0]->pos,pi[1]->pos);
				return true;
			}

        }

		if (n >= 3)				// �����λ������
		{
			const char* pb = tbl_c + n - 3;
			PJUMPOPTM* pi = tbl_JumpOptm + n - 3;

			if (strcmp(pb, "IRE") == 0)	// Ҫ��������
			{
				if(pi[0]->pinstr->jmp.jmpto_off == pi[2]->pinstr->label.label_off)
				{
					// �����Ѿ��������
					pi[0]->pinstr->bJmpFlowOk =true;
					pi[1]->pinstr->bJmpFlowOk =true;
					pi[2]->pinstr->label.bJmpFlowNum++;

					// ����
					Hxd_If(pi[0]->pos,pi[2]->pos,COMP_if);

					return true;
				}

			}

		}

		if (n >= 4)
		{
			const char* pb = tbl_c + n - 4;
			PJUMPOPTM* pi = tbl_JumpOptm + n - 4;

			if (strcmp(pb, "DICE") == 0)	// while
			{
				bool bOne =	false;
				bool bTwo = false;

				if(pi[1]->pinstr->jmp.jmpto_off == pi[3]->pinstr->label.label_off)
					bOne = true;


				if(pi[2]->pinstr->jmp.jmpto_off == pi[0]->pinstr->label.label_off)
					bTwo = true;

				if(bOne &&	bTwo)
				{
					// �����Ѿ��������
					pi[0]->pinstr->label.bJmpFlowNum++;
					pi[1]->pinstr->bJmpFlowOk =true;
					pi[2]->pinstr->bJmpFlowOk =true;
					pi[3]->pinstr->label.bJmpFlowNum++;

					// ����
					Hxd_While(pi[0]->pos,pi[1]->pos,pi[2]->pos,pi[3]->pos);
					return true;
				}
			}

			if (strcmp(pb, "IIEE") == 0)	// IF_OR
			{
				bool bOne =	false;
				bool bTwo = false;

				if(pi[0]->pinstr->jmp.jmpto_off == pi[2]->pinstr->label.label_off)
					bOne = true;

				if(pi[1]->pinstr->jmp.jmpto_off == pi[3]->pinstr->label.label_off)
					bTwo = true;

				if(bOne &&	bTwo)
				{
					// �����Ѿ��������
					
					pi[0]->pinstr->bJmpFlowOk =true;
					pi[1]->pinstr->bJmpFlowOk =true;
					pi[2]->pinstr->label.bJmpFlowNum++;
					pi[3]->pinstr->label.bJmpFlowNum++;

					// ����
					Hxd_If_Or(pi[0]->pos,pi[1]->pos,pi[2]->pos,pi[3]->pos);
					return true;
				}

			}

			if (strcmp(pb, "IJEE") == 0)	// IF_else
			{
				bool bOne =	false;
				bool bTwo = false;

				if(pi[0]->pinstr->jmp.jmpto_off == pi[2]->pinstr->label.label_off)
					bOne = true;

				if(pi[1]->pinstr->jmp.jmpto_off == pi[3]->pinstr->label.label_off)
					bTwo = true;

				if(bOne &&	bTwo)
				{
					// �����Ѿ��������
					
					pi[0]->pinstr->bJmpFlowOk =true;
					pi[1]->pinstr->bJmpFlowOk =true;
					pi[2]->pinstr->label.bJmpFlowNum++;
					pi[3]->pinstr->label.bJmpFlowNum++;

					// ����
					Hxd_If_else(pi[0]->pos,pi[1]->pos,pi[2]->pos,pi[3]->pos);
					return true;
				}

			}

		}	// n> =4

    }	// while

    return false;
}

void	CInstrList::Hxd_Break(POSITION Jxx,POSITION While)
{
	Hxd_Add_CplxBegin_CplxEnd(Jxx,Jxx,COMP_break);

	// ���While ������Ƕ�Ӧ��i_Label �Ͳ����ˣ�����Ҫ����if

	INSTR_LIST* list = this->m_list;

	PINSTR pJxx = list->GetAt(Jxx);
	POSITION pos = While;
	list->GetNext(pos);
	PINSTR p2 = list->GetNext(pos);

	if( p2->type == i_Label)
	{
		if(pJxx->jmp.jmpto_off == p2->label.label_off)
		{
			p2->label.bJmpFlowNum++;
			return;
		}
	}

	// ����һ��i_Jxxα�������

	PINSTR pnew = new INSTR;
	pnew->type = i_Jump;
	//pnew->bJmpFlowOk = true;	// �Ѿ������

	pnew->jmp.jmp_type = pJxx->jmp.jmp_type;
	pnew->jmp.jmpto_off = pJxx->jmp.jmpto_off;
	pnew->jmp.linear = pJxx->jmp.linear;

	//pnew->var_w = pJxx->var_w;
	pnew->var_r1 = pJxx->var_r1;
	pnew->var_r2 = pJxx->var_r2;
	pnew->va_r1 = pJxx->va_r1;
	pnew->va_r2 = pJxx->va_r2;

	list->InsertAfter(While,pnew);


	return;
}

void	CInstrList::Hxd_While(POSITION firstpos, POSITION jxx,POSITION jump,POSITION endpos)
{
	Hxd_Add_Begin_End(firstpos,jxx);
	Hxd_Add_Begin_End(jxx,jump);
	Hxd_Add_CplxBegin_CplxEnd(firstpos,endpos,COMP_while);
	return;
}

void	CInstrList::Hxd_do_While(POSITION firstpos,POSITION endpos)
{
	Hxd_Add_Begin_End(firstpos,endpos);
	Hxd_Add_CplxBegin_CplxEnd(firstpos,endpos,COMP_do_while);
	return;
}

void	CInstrList::Hxd_If(POSITION firstpos,POSITION endpos,enum_COMP comptype)
{
	Hxd_Add_Begin_End(firstpos,endpos);
	Hxd_Add_CplxBegin_CplxEnd(firstpos,endpos,comptype);
	return;
}

void	CInstrList::Hxd_If_Or(POSITION firstpos, POSITION jxx,POSITION label,POSITION endpos)
{
	INSTR_LIST* list = this->m_list;

	POSITION pos = firstpos;

	list->GetNext(pos);

	if(jxx != pos)	// ���������������ж�
		Hxd_Add_Begin_End(firstpos,jxx);

	Hxd_Add_Begin_End(label,endpos);
	Hxd_Add_CplxBegin_CplxEnd(firstpos,endpos,COMP_if_Or);
	return;
}

void	CInstrList::Hxd_If_else(POSITION firstpos, POSITION jxx,POSITION label,POSITION endpos)
{
	Hxd_Add_Begin_End(firstpos,jxx);
	Hxd_Add_Begin_End(label,endpos);
	Hxd_Add_CplxBegin_CplxEnd(firstpos,endpos,COMP_if_else);
	return;
}


// ��� i_CplxBegin �� i_CplxEndָ��
// ����i_Labelα����
void CInstrList::Hxd_Add_CplxBegin_CplxEnd(POSITION firstpos, POSITION endpos,enum_COMP comptype)
{
	INSTR_LIST* list = this->m_list;

	PINSTR begin = new INSTR;
	PINSTR end = new INSTR;

	begin->type = i_CplxBegin;		// ��� i_CplxBegin ָ��
	end->type = i_CplxEnd;			// ��� i_CplxEnd   ָ��
	begin->begin.m_end = end;

	begin->begin.type=comptype;		// ��ת������

	PINSTR p = list->GetAt(firstpos);

	begin->begin.m_break = p->begin.m_break;	//	�̳�
	begin->begin.m_conti = p->begin.m_conti;	//	�̳�

	for (;;)
	{
		if (begin)
		{
			PINSTR p = list->GetAt(firstpos);
			if (p->type != i_Label)
			{
				list->InsertBefore(firstpos,begin);
				begin = NULL;	//	����������Ժ��ٴ���
			}
		}
		if (end)
		{
			PINSTR p = list->GetAt(endpos);
			if (p->type != i_Label)
			{
				list->InsertAfter(endpos,end);
				end = NULL;
			}

		}
		if (begin == NULL && end == NULL)
			return;	//	��������ˣ�ûʲô��������

		// ����һ��i_Labelα�������
		PINSTR pnew = new INSTR;
		pnew->type = i_Label;
		pnew->bJmpFlowOk = true;	// �Ѿ������
		pnew->label.ref_instr = 0;

		PINSTR p;
		if (begin)	//	����ȡһ����
		{
			p = list->GetAt(firstpos);
			list->InsertAfter(firstpos,pnew);
			list->InsertAfter(firstpos,begin);
			begin = NULL;	//	����������Ժ��ٴ���
		}
		else
		{
			p = list->GetAt(endpos);

			list->InsertBefore(endpos,pnew);
			list->InsertBefore(endpos,end);
			end = NULL;		//	����������Ժ��ٴ���
		}
		pnew->label.label_off = p->label.label_off;
	}

	return;
}

// ���i_Begin / i_End
bool	CInstrList::Hxd_Add_Begin_End(POSITION firstpos, POSITION endpos)
{
	INSTR_LIST* list = m_list;

	PINSTR begin = new INSTR;
	PINSTR end = new INSTR;
	begin->type = i_Begin;
	end->type = i_End;
	begin->begin.m_end = end;

	PINSTR p = list->GetAt(firstpos);
	begin->begin.m_break = p->begin.m_break;	//	�̳�
	begin->begin.m_conti = p->begin.m_conti;	//	�̳�

	list->InsertAfter(firstpos,begin);
	list->InsertBefore(endpos,end);

	return true;
}

bool Step_by_Step()
{	//	����Ϊ���ڷ�step_by_step״̬ʱ������ߵ��ٶ�
	//	����step_by_step״̬ʱ������ʹÿһ�����ֵú�ϸ
	if (g_f_Step_by_Step)
		return true;
	g_any1_return_TRUE = true;
	return false;
}

int CInstrList_Finger::search_and_add(DWORD* buf,DWORD val,int* pn)
{//static function
	int n = *pn;
	for (int i=0;i<n;i++)
	{
		if (buf[i] == val)
			return i+1;
	}
	buf[n] = val;
	*pn = n+1;
	return n+1;
}
bool	CInstrList_Finger::finger_compare(PSTR f1,const char* f2)
{//static function
	for (;;)
	{
		if (*f1 == *f2)
		{
			if (*f1 == 0)
				return true;
			f1++;
			f2++;
			continue;
		}
		if (*f2 == '0' && f2[1] == '_')
		{
			f2 += 2;
			continue;
		}
		return false;
	}
}

bool	CInstrList::if_Ly_In(PINSTR p, POSITION firstpos, POSITION endpos)
{	
    INSTR_LIST* list = m_list;

	assert(endpos);
	POSITION pos = firstpos;
	while (pos)
	{
		PINSTR pinstr = list->GetNext(pos);
		if (pinstr == p)
			return true;
		if (pos == endpos)
		{
			if (p->type != i_Label)
				return false;
			pinstr = list->GetNext(pos);
			if (p == pinstr)
				return true;
			return false;
		}
	}
	return false;
}


// ȷ��һ��ƥ���i_Jump �� i_Lable
bool	CInstrList::ifOneStatement(PINSTR pNode, POSITION firstpos, POSITION endpos)
{
    INSTR_LIST* list = this->m_list;		// α�����

	assert(firstpos);
	assert(endpos);

	bool ffirst = true;
	POSITION pos = firstpos;
	while (pos && pos != endpos)
	{
		PINSTR p = list->GetNext(pos);

		if (ffirst && p->type == i_Label)	// �ڶ������Ƕ�Ӧ��i_Label
		{
			ffirst = false;

			if (pos == endpos)
				return false;
			//continue;
		}
		
		if (p->type == i_Label)
		{
			PINSTR pr = p->label.ref_instr;
			while (pr)
			{
				if (! if_Ly_In(pr, firstpos, endpos) )
					return false;
				pr = pr->jmp.next_ref_of_this_label;
			}
		}
		else if (p->type == i_Jump)
		{
			if (p->jmp.jmp_type == JMP_jmp)
			{
				//if (p->jmp.the_label != pNode->begin.m_conti)
				//	alert("why?");
				if (p->jmp.the_label == pNode->begin.m_break)
					continue;
				if (p->jmp.the_label == pNode->begin.m_conti
					&& p != pNode->begin.m_not_conti)
					continue;
			}
			if (! if_Ly_In(p->jmp.the_label, firstpos, endpos) )
				return false;
		}
	}
	if (pos == endpos)
		return true;
	return false;
}

// ��һ������
PINSTR	instr_next(INSTR_LIST* list,PINSTR p)
{
	POSITION pos = list->Find(p);
	if (pos == NULL)
		return NULL;
	list->GetNext(pos);
	if (pos == NULL)
		return NULL;
	return list->GetAt(pos);
}

// ǰһ������
PINSTR	instr_prev(INSTR_LIST* list, PINSTR p)
{
	POSITION pos = list->Find(p);
	if (pos == NULL)
		return NULL;
	list->GetPrev(pos);
	if (pos == NULL)
		return NULL;
	return list->GetAt(pos);
}

void	CInstrList_Finger::prt_partern(PINSTR phead,PSTR partern_buf)
{
    INSTR_LIST* list = this->m_list;

	if (phead->type != i_CplxBegin)
		return;

	PINSTR p = instr_next(list,phead);

	int	t = 0;
	DWORD buf[20];
	int n = 0;
	
	while (p != NULL && p != phead->begin.m_end)
	{
		if (p->type == i_Jump)
		{
            int i = search_and_add(buf,(DWORD)p->jmp.the_label,&n);
			if (p->jmp.jmp_type == JMP_jmp)
			{
                t += sprintf(partern_buf+t,"jmp%d_",i);
			}
			else
				t += sprintf(partern_buf+t,"jxx%d_",i);

		}
		else if (p->type == i_Label)
		{
			int i = search_and_add(buf,(DWORD)p,&n);
			t += sprintf(partern_buf+t,"from%d_",i);

		}
		else if (p->type == i_Begin)
		{
            t += sprintf(partern_buf+t,"0_");
			p = p->begin.m_end;
		}
		else
			;//why here
		p = instr_next(list,p);

		if (t > 120)
		{
            t += sprintf(partern_buf+t,"...");
			t;	//	avoid warning
			break;
		}
	}
}
    
bool	CInstrList_Finger::Finger_check_partern_for1(PINSTR p)
{
    CFunc_InstrList instrl(this->m_list);

#if 0
    int i = 0;  //p1
    do
    {
        i++;    //p3
    }
    while (i < 100);    //p2
#endif
    INSTR_LIST* list = this->m_list;
    PINSTR p1 = instr_prev(list,p);	
    if (p1->var_r1.type != v_Immed)
        return false;
    if (p1->var_r2.type != v_Invalid)
        return false;   //��������������˼�ǣ�ֻ����ǰ����i=n�Ĳ���
    
    PINSTR p2;
    {
        PINSTR ptem = instr_next(list,p);
        if (ptem->type != i_Label)
            return false;
        p2 = ptem->label.ref_instr;
    }
    if (p1 == NULL || p2 == NULL)
        return false;
    
    if (VAR::IsSame(&p1->var_w, &p2->var_r1)
        || VAR::IsSame(&p1->var_w, &p2->var_r2))
    {
    }
    else
        return false;

    {
        list->RemoveAt(list->Find(p1));
        POSITION pos = list->Find(p);	//	insert after i_CplxBegin

        PINSTR begin = new INSTR;
        PINSTR end = new INSTR;
        begin->type = i_Begin;
        end->type = i_End;
        begin->begin.m_end = end;
        list->InsertAfter(pos,end);
        list->InsertAfter(pos,p1);
        list->InsertAfter(pos,begin);
    }

    {
        //p2������������ǰ��Ӧ����i_end
        PINSTR pend = instrl.instr_prev_in_func(p2);
        assert(pend->type == i_End);

        PINSTR begin = new INSTR;
        PINSTR end = new INSTR;
        begin->type = i_Begin;
        end->type = i_End;
        begin->begin.m_end = end;
        list->InsertAfter(list->Find(pend), end);
        list->InsertAfter(list->Find(pend), begin);

        for (;;)
        {
            PINSTR p3 = instrl.instr_prev_in_func(pend);
            if (p3->type != i_Add && p3->type != i_Sub)
                break;
            list->RemoveAt(list->Find(p3));
            list->InsertAfter(list->Find(begin), p3);
        }
    }

    return true;
}

bool	CInstrList_Finger::Finger_check_partern(PINSTR p)
	// ��� p ��partern
{
    INSTR_LIST* list = this->m_list;
	char buf[140];
	this->prt_partern(p,buf);
	
	if (finger_compare(buf, finger_if))
		p->begin.type = COMP_if;
	else if (finger_compare(buf, finger_long_if))	//	һ��Ҫ����if_elseǰ
		p->begin.type = COMP_long_if;
	else if (finger_compare(buf, finger_if_else))
		p->begin.type = COMP_if_else;
	else if (finger_compare(buf, finger_while))
		p->begin.type = COMP_while;
	else if (finger_compare(buf, finger_dowhile))
    {
		p->begin.type = COMP_do_while;
        //�����do-while���дΪfor
        if (this->Finger_check_partern_for1(p))
        {
            log_prtl("find for1");
            p->begin.type = COMP_for1;
        }
    }
	else if (finger_compare(buf, finger_dowhile_2))
		p->begin.type = COMP_do_while;
	else if (finger_compare(buf, finger_for))
	{	//const char finger_for[] 	= "0_jmp1_from2_0_from1_0_jxx3_0_jmp2_from3_";
		p->begin.type = COMP_for;
		//	���棬�����Ƕ� for ��һЩ����
		PINSTR p1 = instr_next(list,p);	//	p1ָ�� label
		assert(p1->type == i_Jump);
		p1 = instr_next(list,p1->jmp.the_label);	//p1ָ������
		if (p1->type == i_Jump
			&& p1->jmp.jmp_type != JMP_jmp
			&& p1->var_r1.type != 0)
		{
			VAR* pvar = &p1->var_r1;
			PINSTR p2 = instr_prev(list,p);
			if (VAR::IsSame(&p2->var_w,pvar))
			{
				list->RemoveAt(list->Find(p2));
				POSITION pos = list->Find(p);	//	insert after i_CplxBegin

				PINSTR begin = new INSTR;
				PINSTR end = new INSTR;
				begin->type = i_Begin;
				end->type = i_End;
				begin->begin.m_end = end;
				list->InsertAfter(pos,end);
				list->InsertAfter(pos,p2);
				list->InsertAfter(pos,begin);
			}
		}
	}
	else
		return false;
	return true;
}

void	CFunc::Finger_it()
{
	INSTR_LIST *list = this->m_instr_list;
    CInstrList_Finger the(list);
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p->type == i_CplxBegin)
		{
			the.Finger_check_partern(p);
		}
	}
}

bool	CInstrList::Flow_c(PINSTR phead)
{	//	�跨�����ҳ��� statement ������ס����call Flow_a
    INSTR_LIST* list = this->m_list;

	assert(phead->type == i_CplxBegin);

	POSITION s_pos = list->Find(phead);
	POSITION e_pos = list->Find(phead->begin.m_end);
	assert(s_pos);
	assert(e_pos);
	if (phead->begin.type == COMP_switch_case)
	{	//	��swith_case����Щ���⡣�ӵ�һ��label����
		POSITION pos = s_pos;
		while (pos)
		{
			PINSTR p = list->GetNext(pos);
			if (p->type == i_Label)
				break;
		}
		return Flow_cc(phead,pos,e_pos);
	}
    if (phead->begin.type == COMP_switch_case_multcomp)
    {
        bool f = false;
		POSITION pos = s_pos;
		while (pos)
		{
            POSITION savpos = pos;
			PINSTR p = list->GetNext(pos);
			if (p->type == i_Label)
				break;
            if (p->type == i_Jump && p->jmp.jmp_type == JMP_jz)
                f=true;
            if (f)
            {
                if (p->type == i_Begin)
                {
                    pos = list->Find(p->begin.m_end);
                    list->GetNext(pos);
                    continue;
                }
                if (p->type != i_Jump && p->type != i_Begin)
                {
                    //break;  //����û��jump��default
                    return Flow_cc(phead, savpos,e_pos);
                }
            }
		}
		return Flow_cc(phead,pos,e_pos);
    }
	
	list->GetNext(s_pos);				//	skip the i_CplxBegin
	PINSTR p = list->GetNext(s_pos);	//	skip first 1
	if (p->type == i_Begin)
	{	//	������for, ����ͷ����һ��С��statement
        if (this->Flow_a(p))
			return true;
		s_pos = list->Find(p->begin.m_end);
		list->GetNext(s_pos);	//	skip i_End
		list->GetNext(s_pos);	//	skip first 1
	}
	return Flow_cc(phead,s_pos,e_pos);
}

bool	CInstrList::Flow_cc(PINSTR pNode, POSITION firstpos, POSITION endpos)
{	//	����������cplx������ͼ�ҳ�����С��begin_end��
    INSTR_LIST* list = m_list;

//	���֣���ͷ��β��Ҫ�����ҵ�����ͷ������һ��statement������i_Begin,i_End������
//	�ٴ�i_End��β����
//	Flow_c(INSTR_LIST* list, POSITION firstpos,POSITION endpos);
//	���ҵ���i_Begin,i_End����Flow_a����

	//	pNode �Ǽ������������begin_end��ֻ��Ϊ���ṩ m_Break

	assert(firstpos);
	assert(endpos);
	if (firstpos == endpos)
		return false;

	PINSTR phead = list->GetAt(firstpos);
	if (phead->type == i_Label)
	{
		PINSTR p = instr_next(list,phead);
		if (p->type == i_Begin)	//	��Ϊi_label���i_begin��ʹ�������ѭ��
		{
			phead = p;	//	�ŵ���һ����д���
		}
	}
	if (phead->type == i_Begin)
	{
        if (this->Flow_a(phead))
			return true;
    	POSITION pos1 = list->Find(phead->begin.m_end);
		list->GetNext(pos1);	//	skip i_End
		return Flow_cc(pNode,pos1,endpos);
	}

	POSITION okpos;
	POSITION pos1;
	pos1 = okpos = firstpos;

    do
	{
		list->GetNext(pos1);
		if (ifOneStatement(pNode, firstpos,pos1))
			okpos = pos1;	// record the last success
	} while (pos1 != endpos);

	if (okpos == firstpos)	// not find anything
	{
		pos1 = firstpos;
		list->GetNext(pos1);
		if (pos1 && pos1 != endpos)
		{
			return Flow_cc(pNode, pos1, endpos);		//	start from next
		}
		return false;	
	}

	{
		PINSTR begin = new INSTR;
		PINSTR end = new INSTR;
		begin->type = i_Begin;
		end->type = i_End;
		begin->begin.m_end = end;

		Add_Begin_End(firstpos, okpos, begin, end);

		begin->begin.m_break = pNode->begin.m_break;	//	�̳�
		begin->begin.m_conti = pNode->begin.m_conti;	//	�̳�

        this->Flow_a(begin);
	}

	//if (Step_by_Step())
	//	return true;
	
	return Flow_cc(pNode, okpos, endpos);	//next part
}

void CInstrList::Add_Begin_End(POSITION firstpos, POSITION endpos, PINSTR begin, PINSTR end)
{
    this->Add_Begin_End_1(firstpos,endpos,begin,end);

    INSTR_LIST* list = this->m_list;
    POSITION pos = list->GetHeadPosition();
    while (pos)
    {
        POSITION savpos = pos;
        PINSTR p = list->GetNext(pos);
        if (p->type == i_Nop)
        {
            list->RemoveAt(savpos);
        }
    }
}

//	��Ϊ��Ҫ���ν���ͬһ�����ܣ��ֲ����������д����������дһ��for(;;)����

void CInstrList::Add_Begin_End_1(POSITION firstpos, POSITION endpos, PINSTR begin, PINSTR end)
{
    INSTR_LIST* list = this->m_list;


	for (;;)
	{
		if (begin)
		{
			PINSTR p = list->GetAt(firstpos);
			if (p->type != i_Label)
			{
				list->InsertBefore(firstpos,begin);
				begin = NULL;	//	����������Ժ��ٴ���
			}
		}
		if (end)
		{
			PINSTR p = list->GetAt(endpos);
			if (p->type != i_Label)
			{
				list->InsertAfter(endpos,end);
				end = NULL;
			}

		}
		if (begin == NULL && end == NULL)
			return;	//	��������ˣ�ûʲô��������

		// ����һ��i_Labelα�������
		PINSTR pnew = new INSTR;
		pnew->type = i_Label;
		pnew->bJmpFlowOk = true;	// �Ѿ������
		pnew->label.ref_instr = 0;

		PINSTR p;
		if (begin)	//	����ȡһ����
		{
			p = list->GetAt(firstpos);
			list->InsertAfter(firstpos,pnew);
			list->InsertAfter(firstpos,begin);
			begin = NULL;	//	����������Ժ��ٴ���
		}
		else
		{
			p = list->GetAt(endpos);

			list->InsertBefore(endpos,pnew);
			list->InsertBefore(endpos,end);
			end = NULL;		//	����������Ժ��ٴ���
		}
		pnew->label.label_off = p->label.label_off;
	
		// ��ʲô����
		PINSTR p_in = 0;
		PINSTR p_out = 0;
		PINSTR p1 = p->label.ref_instr;	// ��ת����Jumpα����

		while (p1)			
		{
			PINSTR pnext = p1->jmp.next_ref_of_this_label;
			if (if_Ly_In(p1,firstpos,endpos))
			{
				p1->jmp.the_label = pnew;
				p1->jmp.next_ref_of_this_label = p_in;
				p_in = p1;
			}
			else
			{
				//p1->jmp.the_label = p;	//need not
				p1->jmp.next_ref_of_this_label = p_out;
				p_out = p1;
			}
			p1 = pnext;
		}

		if (p_in)	//	it must be
			pnew->label.ref_instr = p_in;
		else
			pnew->type = i_Nop;		//	��Ϊԭ����label���Ͳ�����var��

		if (p_out)
			p->label.ref_instr = p_out;
		else
			p->type = i_Nop;
		
	}
}

bool CInstrList::IsSwitchCase_multcomp(PINSTR begin)
{	//	�ж��ǲ��Ƕ����жϵ���һ��switch_case
    INSTR_LIST* list = this->m_list;

	assert(begin->type == i_CplxBegin);
	POSITION pos = list->Find(begin);
	M_t* v;

	int first = 0;
	while (pos)
	{
        POSITION savpos = pos;
		PINSTR p = list->GetNext(pos);
		first++;
		if (first == 1)
			continue;	//	��һ�����϶���i_CplxBegin���Ͳ��ÿ���
		if (first == 2)
		{				//	��һ��ָ�������jz sth == n
			if (p->type != i_Jump || p->jmp.jmp_type != JMP_jz
				|| p->var_r1.type == 0)
				return false;
			v = p->var_r1.thevar;	//	���������Ժ��Ҫ�����һ������
			continue;
		}
		if (p->type == i_Jump)
		{
			if (p->jmp.jmp_type == JMP_jz)
			{
				if (v != p->var_r1.thevar)
					return false;
			}
			else if (p->jmp.jmp_type == JMP_jmp)	//	�ҵ�default��
			{
				if (first < 4)		//	���̫���ˣ�����switch case
					return false;
				return true;
			}
			else
				return false;

			continue;
		}
        //����Jump��
        if (first < 4)		//	���̫���ˣ�����switch case
            return false;
        return true;
	}
	return false;
}

bool CInstrList::IsSwitchCase(PINSTR begin)
{
    INSTR_LIST* list = m_list;

	assert(begin->type == i_CplxBegin);
	POSITION pos = list->Find(begin);

	bool first = true;
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p->type == i_JmpAddr)
		{
			return true;
		}
		if (p->type == i_Return)
			return false;
		if (p->type == i_Label)
			return false;
		if (p->type == i_Jump)
		{	//	ֻ������һ��jmp
			if (first)
				first = false;
			else
				return false;
		}
	}
	return false;
}

void	CInstrList::Flow_b(PINSTR pParentNode, POSITION firstpos, POSITION endpos)
{	//	���շ���
    INSTR_LIST* list = this->m_list;

	//	last not include
	if (firstpos == endpos)
	{
		//alert_prtf("why firstpos == endpos");
		return;
	}

	PINSTR begin = new INSTR;
	PINSTR end = new INSTR;
	begin->type = i_CplxBegin;		// ��� i_CplxBegin ָ��
	end->type = i_CplxEnd;			// ��� i_CplxEnd   ָ��
	begin->begin.m_end = end;

	Add_Begin_End(firstpos, endpos, begin, end);
	
	POSITION pos = endpos;
	list->GetPrev(pos);		//now it point to i_CplxEnd
	assert(list->GetAt(pos) == end);
	list->GetPrev(pos);		//now it point to last instr in body
	PINSTR plast = list->GetAt(pos);

	PINSTR plast2 = instr_prev(list,plast);	//ǰһ��ָ��

	PINSTR pNode = begin;
	pNode->begin.m_break = pParentNode->begin.m_break;	//	�̳�
	pNode->begin.m_conti = pParentNode->begin.m_conti;	//	�̳�

	PINSTR pfirst = instr_next(list,begin);
	PINSTR psecond = instr_next(list,pfirst);

	if (pfirst->type == i_Label
		|| (pfirst->type == i_Jump && pfirst->jmp.jmp_type == JMP_jmp && psecond->type == i_Label)
		)
	{	//	��������Ϊ break ������ ��
		PINSTR pconti;
		if (pfirst->type == i_Label)	//	����ǵ�һ�����
			pconti = pfirst;
		else
			pconti = psecond;
		pconti->label.f_conti = true;

		if (plast->type == i_Label)		//	������һ��ָ���Ǹ�label�������϶���break
			pNode->begin.m_break = plast;
		else
			pNode->begin.m_break = 0;
		
		pNode->begin.m_conti = pconti;

		if (plast->type == i_Label)
		{
			if (plast2->type == i_Jump
				&& plast2->jmp.jmp_type == JMP_jmp
				&& plast2->jmp.the_label == pconti)
			{	//	���� while !
				pNode->begin.m_not_conti = plast2;
			}
		}
		else if (plast->type == i_Jump
				 && plast->jmp.jmp_type == JMP_jmp
				 && plast->jmp.the_label == pconti)
		{
			pNode->begin.m_not_conti = plast;
		}
		else
		{
			//do_while��continue �Ժ��ٴ���
		}
	}
    else if (plast->type == i_Label && IsSwitchCase(begin))
	{
		pNode->begin.m_break = plast;
		//conti not change
		//	����switch_case ���ص㣬����ֻ��break,ȴ����continue,�ǳ����⡣
		pNode->begin.type = COMP_switch_case;
		//alert("case find 00");
	}
	else if (plast->type == i_Label && IsSwitchCase_multcomp(begin))
	{
		pNode->begin.m_break = plast;
		//conti not change
		//	����switch_case ���ص㣬����ֻ��break,ȴ����continue,�ǳ����⡣
		pNode->begin.type = COMP_switch_case_multcomp;
		//alert("case find 01");
	}
	//if (Step_by_Step())
	//	return;		//	�����Ѿ����˸�i_CplxBegin_End����ɹ�����

	Flow_c(begin);	//	�����i_CplxBegin����Щ����
}

//	���̷�����һ��
//	�����������з������Ժ���Եݹ�
bool	CInstrList::Flow_a(PINSTR pNode)
{
    INSTR_LIST* list = this->m_list;		// α�����б�
	assert(pNode->type == i_Begin);			// ����

	POSITION endpos = list->Find(pNode->begin.m_end);	// ��begin��Ӧ��end λ��
	
	POSITION firstpos = list->Find(pNode);	// beginλ��
	list->GetNext(firstpos);				// �ӵڶ���ָ�ʼ
	assert(firstpos);						// ����
	if (firstpos == endpos)					// �ڶ�������end �����ô�����
		return false;
	
	//	Flow_a ��ָ��һ��begin_end
	//	��Flow_aa��ָ��һ�����䣬����һ����begin_end

	return Flow_aa(firstpos,endpos);
}

// ��ɢ����
// return true��ʾ�����н�չ
// pBlockHeadNode �ǿ�ͷ���Ǹ�begin,���ڲ�ѯbreak��continue��ַ
// ���� i_Jump �� i_Labelָ��
bool	CInstrList::Flow_aa(POSITION firstpos, POSITION endpos)
{	

    INSTR_LIST* list = this->m_list;	// α�����б�
	
	assert(firstpos);
    assert(endpos);
	
	if (firstpos == endpos)
		return false;

	POSITION pos = firstpos;

	while(pos && pos != endpos)
	{
		POSITION savepos = pos;
		PINSTR pCur = list->GetNext(pos);		// ��һ��

		switch(pCur->type)
		{
		case i_Jump:
			{
				if (pCur->jmp.jmp_type == JMP_jmp)
				{
					if (pCur->jmp.the_label == pCur->begin.m_break)
					{
						return Flow_aa(savepos, endpos);
					}
					else if (pCur->jmp.the_label == pCur->begin.m_conti
						&& pCur != pCur->begin.m_not_conti)
					{
						return Flow_aa(savepos, endpos);
					}
				}
				else
				{
					PINSTR p1 = pCur->jmp.the_label;	// i_Label α����
					POSITION pos1 = list->Find(p1);		// i_Labelλ��
					assert(pos1);

					while (pos1)
					{
						if (pos1 == endpos)				// i_Labelλ�� �Ǿ�endposλ��
							break;
						else if (ifOneStatement(pCur, savepos,pos1))	//	�ҵ�һ�����սṹ
						{	
							Flow_b(pCur, savepos,pos1);
							//if (Step_by_Step())
							//	return true;	//	����������ֱ��return TRUE��
							return Flow_aa(firstpos,endpos);;
						}

						list->GetNext(pos1);	// ��һ��ָ��
					}
				}
			}
			break;

		case i_Label:
			{
				PINSTR p1 = pCur->label.ref_instr;
				POSITION pos1 = list->Find(p1);
				while (pos1 && pos1 != endpos)
				{
					list->GetNext(pos1);
					assert(pos1);
					if (ifOneStatement(pCur, savepos,pos1))
					{
						Flow_b(pCur, savepos,pos1);
						//if (Step_by_Step())
						//	return true;	//	����������ֱ��return TRUE��
						return Flow_aa(pos1,endpos);
					}
					//assert(pos1);
				}
			}
			break;

		case i_Begin:
			{
				if (this->Flow_a(pCur))			// �ݹ�
					return true;

				// ����������ʲô��
				POSITION pos1 = list->Find(pCur->begin.m_end);
				assert(pos1);
				return Flow_aa(pos1,endpos);
			}

		case i_CplxBegin:
			{
				POSITION pos1 = list->Find(pCur->begin.m_end);
				assert(pos1);

				if (Flow_c(pCur))
					return true;

				if (pCur->begin.type == COMP_unknown)	//	��ʱӦ����ͼʶ����
				{	
					CInstrList_Finger the(list);
					if (the.Finger_check_partern(pCur))
						return true;
				}

				return Flow_aa(pos1, endpos);	
			}
		}
		
	}

	return false;
}
/*
	�����ַ�����һ��Ҫ�������

	һ���ǣ���ͷ��β����֪����һ����ɢ��statement,Ҫ�������ҵ�һ������complex statement
	Flow_a(PINSTR phead, INSTR_LIST* list);
	���ҵ���complex,��Flow_b����
	
	���֣���ͷ��β������֪����һ����һ��complex statement����Ҫ�Լ����Լ�
	Flow_b(INSTR_LIST* list, POSITION firstpos,POSITION endpos);
	��ס�Լ���callһ��Flow_c

	���֣�����һ����һ��complex statement,�跨Ҫ�����ҵ�����ͷ������һ��statement������i_Begin,i_End������
	�ٴ�i_End��β����
	Flow_c(PINSTR phead, INSTR_LIST* list);
	���ҵ���i_Begin,i_End����Flow_a����
	
*/

// --------------------------------------------------------------
void	CFunc_Prt::add_default_entry(CasePrt_List* list, PINSTR thelabel)
{	//	delete all same as default
    //static function

    if (thelabel->type == i_Label)
    {
        POSITION pos = list->GetHeadPosition();
        while (pos)
        {
            POSITION savpos = pos;
            OneCase* p = list->GetNext(pos);
            if (p->thelabel->label.label_off == thelabel->label.label_off)
            {
                //alert_prtf("delete case %d",p->case_n);
                list->RemoveAt(savpos);
            }
        }
    }
	OneCase* pnew = new OneCase;
	pnew->case_n = 0xffffffff;		//	����ֻ�����һ�����������һ����������Ҫ��
	pnew->thelabel = thelabel;
	list->AddTail(pnew);
}

void	CFunc_Prt::Add_case_entry(CasePrt_List* list, int case_n, PINSTR thelabel)
{	//	�����Ȱ�switch case������������������һ���ӡ
	//alert("add case entry");
    //static function

	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		POSITION savpos = pos;
		OneCase* p = list->GetNext(pos);
		if (p->case_n == case_n)
			error("why same case");
        if (p->thelabel->label.label_off >= thelabel->label.label_off)
		{
			pos = savpos;
			break;
		}
	}

	OneCase* pnew = new OneCase;
	pnew->case_n = case_n;
	pnew->thelabel = thelabel;
	if (pos)
		list->InsertBefore(pos, pnew);
	else
		list->AddTail(pnew);
	return;
}
