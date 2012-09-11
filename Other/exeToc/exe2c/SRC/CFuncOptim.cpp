// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
#include	"CISC.H"
#include "cexe2c.h"
#include "CStrategy.h"

//#define VarName  Q->VarName
#define m_instr_list Q->m_instr_list
#define m_exprs Q->m_exprs
#define m_head_off Q->m_head_off
#define m_funcname Q->m_funcname

extern bool	g_Hxd_Step;

// �ͷ��ڴ�
void Hxd_DeleteUsedList(VAROPTM_LIST *used_list)
{
	if (used_list)
	{
		POSITION pos = used_list->GetHeadPosition();
		while (pos)
		{
			POSITION savpos = pos;
			st_VarOptm * p = used_list->GetNext(pos);

			used_list->RemoveAt(savpos);
			SAFE_DELETE(p);
		}

	}
}

// ����i_Addressָ�����i_Address�ܲ��ܱ��i_Add
bool CFuncOptim::Step1_Address_to_Add()
{
	INSTR_LIST* list = this->m_instr_list;
	
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p->type == i_Address)				// ������ַ��ʽ��[eax + ebx*4 + 8888]
        {
            if (p->var_r2.type == v_Invalid)	// �ڶ�����ȡ�Ĳ�����������
            {	// �ڶ�������Ϊ�����ã��磺[eax + 8888]
                p->i1 = 0;
                if (p->i2 == 0)
                {	// �ڶ�����ֵΪ0����[ eax ]
                    p->type = i_Assign;			// ��ֵ
                }
				else
				{	// i2��Ϊ�㣬Ҫ��Ϊadd���� [eax + 8888]
					p->var_r2.type = v_Immed;
					p->var_r2.d = p->i2;		// ��ֵΪ p->i2
					p->var_r2.thevar = NULL;
					p->i2 = 0;
					p->type = i_Add;
				}
                //return true;
            }
            else if (p->i2 == 0 
                && VAR::IsSame(&p->var_r1, &p->var_r2) 
                && st_InstrAddOn::IsSame(p->va_r1.pao, p->va_r2.pao))
            {	// [eax + eax*4 ]
                p->var_r2.type = v_Invalid;
                p->va_r2.pao = NULL;

                p->var_r2.type = v_Immed;
                p->var_r2.d = p->i1 + 1;
                p->var_r2.thevar = NULL;

                p->type = i_Imul;

                p->i1 = 0;
                //return true;
            }
            else if (p->i1 > 1 && p->var_r2.type != v_Invalid)
            {	// [eax + ebx*4 + 8888]
                st_InstrAddOn* pnew = new st_InstrAddOn;
                pnew->type = IA_MulImmed;
                pnew->addimmed.iAddon = p->i1;
                pnew->pChild = p->va_r2.pao;

                p->va_r2.pao = pnew;

                p->i1 = 1;
                //return true;
            }
            else if (p->i1 == 1 && p->i2 == 0)
            {	// [eax + ebx]
                p->type = i_Add;
                //return true;
            }
        }
    }
    return false;
}

// �Ѽ�ָ��ת���ɸ�ֵָ��i_Assign
bool CFuncOptim::Step2_Simplify_Instr()
{
	INSTR_LIST* list = this->m_instr_list;
	
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
        if (p->type == i_GetAddr)	// ȡ��ַ����
        {
            p->type = i_Assign;
            
            st_InstrAddOn* pnew = new st_InstrAddOn;
            pnew->type = IA_GetAddress;
			
			pnew->pChild = p->va_r1.pao;

            p->va_r1.pao = pnew;

            //return true;
        }
        else if (p->type == i_Add && p->var_r2.type == v_Immed)	// i_Addָ�� ��:Add eax,2
        {
            p->type = i_Assign;

            st_InstrAddOn* pnew = new st_InstrAddOn;
            pnew->type = IA_AddImmed;
            pnew->addimmed.iAddon = p->var_r2.d;

			pnew->pChild = p->va_r1.pao;

            p->va_r1.pao = pnew;

            p->var_r2.type = v_Invalid;

            //return true;
        }
        else if (p->type == i_Imul && p->var_r2.type == v_Immed) // i_Imulָ��
        {
            p->type = i_Assign;
            
            st_InstrAddOn* pnew = new st_InstrAddOn;
            pnew->type = IA_MulImmed;
            pnew->addimmed.iAddon = p->var_r2.d;

			pnew->pChild = p->va_r1.pao;

            p->va_r1.pao = pnew;

            p->var_r2.type = v_Invalid;

            //return true;
        }
    }
    return false;
}


bool CFuncOptim::Step9_Var_Split()			// �Ĵ����������
{
    POSITION pos = this->m_exprs->vList->GetHeadPosition();
    while (pos)
    {
        M_t* pvar = this->m_exprs->vList->GetNext(pos);
		if (pvar->type == MTT_reg)			// ����Ĵ�������
        {
            VAROPTM_LIST used_list;						// ������var��Ϣ

            Get_Var_Use_Flow(used_list, pvar);

			if(g_Hxd_Step)		// ����ʱ����ʾ
				prt_var_uselist(used_list);

			
            if (TryDistinguishVar(used_list, pvar))		// ���ű�����
            {
                if(g_Hxd_Step)		// ����ʱ����ʾ
					prt_var_uselist(used_list);

				Hxd_DeleteUsedList(&used_list);
                return true;
            }
			
			Hxd_DeleteUsedList(&used_list);
        }
    }
    return false;
}

//	���cmp��jxx��������cmp��jxx�ϲ�
bool	CFuncOptim::pcode_1()
{
	// ȡ��cmp��jmp��i_Jump�б�
	// INSTR_LIST	hxd_list
	// Get_Var_Use_Flow(used_list, pvar);

	INSTR_LIST* list = this->m_instr_list;
	
	POSITION pos = list->GetHeadPosition();

	POSITION hxd_savpos = pos;		// ������һ��cmp����ʼ��Ϊ��һ

	while (pos)
	{
		POSITION savpos = pos;

		PINSTR pjxx = list->GetNext(pos);	// ��ǰָ��

		if ( pjxx->type == i_Jump 
		  && pjxx->jmp.jmp_type != JMP_jmp
		  && pjxx->i2 != 9					// ˵���Ѿ��������
				)
			{
				PINSTR pcmp = list->GetAt(hxd_savpos);	// ���������ָ��

				if(pcmp->type == i_Cmp)
				{
					pjxx->var_r1 = pcmp->var_r1;
					pjxx->va_r1.pao = pcmp->va_r1.pao;
					pjxx->var_r2 = pcmp->var_r2;
					pjxx->va_r2.pao = pcmp->va_r2.pao;
					pjxx->i2 = 9;						// ���һ��

				}
			}
			if ( pjxx->type == i_Setnz 
				&& pjxx->i2 != 9					// ˵���Ѿ��������
				)
			{
				PINSTR pcmp = list->GetAt(hxd_savpos);	// ���������ָ��

				if(pcmp->type == i_Cmp)
				{
					pjxx->var_r1 = pcmp->var_r1;
					pjxx->va_r1.pao = pcmp->va_r1.pao;
					pjxx->var_r2 = pcmp->var_r2;
					pjxx->va_r2.pao = pcmp->va_r2.pao;
					pjxx->i2 = 9;						// ���һ��

				}
			}

		// ��������i_Cmp��ֻ�������һ��
		if (pjxx->type == i_Cmp || pjxx->type == i_Return || pjxx->type == i_Jump)
		{
			PINSTR pcmp = list->GetAt(hxd_savpos);	// �ϴα�����ָ��

			if(pcmp->type == i_Cmp)			// ��������i_Cmp��ֻ�������һ��
			{
				list->RemoveAt(hxd_savpos);	// ɾ��cmp����
					return true;
			}

			hxd_savpos = savpos;
		};
	}

	// ɾ����icmp����ɾ��

	pos = list->GetHeadPosition();

	while (pos)
	{
		POSITION savpos = pos;
		PINSTR pcmp = list->GetNext(pos);	// ��ǰָ��

		if (pcmp->type == i_Cmp)
		{
			PINSTR pjxx = list->GetAt(pos);	// ������ָ��
			if(pjxx->type == i_EspReport)
				list->RemoveAt(pos);
			else
				list->RemoveAt(savpos);
				//return true;
		}
	}

	return false;
}

//	���̷���
//	�����i_Begin��i_End֮��Ľ��з���,������е�i_Begin��i_End
bool	CFuncOptim::ana_Flow()
{	
	INSTR_LIST *list = this->m_instr_list;
	PINSTR first = list->GetHead();

    CInstrList the(list);
	
	return the.Hxd_JumpFlow();

	//return the.Flow_a(first);
}

bool CFuncOptim::expr_never_use_after_this(VAR* pvar, PINSTR p0, INSTR_LIST* oldroad)
{	//	�ұ���֪��������֪����
	INSTR_LIST* list = this->m_instr_list;
	POSITION pos = list->Find(p0);
	while (pos)
	{
		PINSTR p = list->GetNext(pos);

		if (p->type == i_Label)	
		{	//	�������ÿ���
			oldroad->AddHead(p);
			continue;
		}
		if (VAR::IsSame(pvar, &p->var_w))
			return true;
		if (VAR::IsSame(pvar, &p->var_r1))
			return false;
		if (VAR::IsSame(pvar, &p->var_r2))
			return false;
        //	��Ϊ i_Jump Ҳ���Դ�����������Ҫ�ȼ��
		if (p->type == i_Jump)
		{	//	Ҫ��ֹ����
			if (! oldroad->Find(p->jmp.the_label))
			{
				oldroad->AddHead(p->jmp.the_label);
				if (! expr_never_use_after_this(pvar,p->jmp.the_label,oldroad))
					return false;
			}
			if (p->jmp.jmp_type == JMP_jmp)
				return true;
			continue;
		}
	}
	return true;
}

//	ȷ�ϴ�p1��p2û���ù�pvar
//	������p1����,Ҳ������p2
bool	CFuncOptim::MakeSure_NotRef_in_Range(VAR* pvar, PINSTR p1, PINSTR p2)
{
	INSTR_LIST* list = this->m_instr_list;
	POSITION pos = list->Find(p1);
	list->GetNext(pos);		//	skip p1
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p == p2)
			return true;
		if (VAR::IsSame(pvar, &p->var_w))
			return false;
		if (VAR::IsSame(pvar, &p->var_r1))
			return false;
		if (VAR::IsSame(pvar, &p->var_r2))
			return false;
	}
	error("why here 8u3");
	return false;
}

bool CFuncOptim::ClassSubFuncProcess()
{//return false ������һ���ķ���
    CFuncType* pfctype = this->Q->m_functype;
    if (pfctype == NULL || pfctype->m_class == NULL)
        return false;

    VarTypeID id = g_VarTypeManage->Class2VarID(pfctype->m_class);
    id = g_VarTypeManage->GetAddressOfID(id);

    this->Q->Fill_this_ECX(id);

    return false;
}

// ��������������
bool CFuncOptim::DataType_Flow()
{
    POSITION pos = m_instr_list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = m_instr_list->GetNext(pos);
        if (p->type == i_GetAddr
            && p->var_w.thevar != NULL
            && g_VarTypeManage->GetPointTo(p->var_w.thevar->m_DataTypeID) != 0
            && p->var_r1.thevar != NULL
            && g_VarTypeManage->is_simple(p->var_r1.thevar->m_DataTypeID)
            && GG_VarType_ID2Size(g_VarTypeManage->GetPointTo(p->var_w.thevar->m_DataTypeID)) 
                >= GG_VarType_ID2Size(p->var_r1.thevar->m_DataTypeID))
        {
            p->var_r1.thevar->m_DataTypeID = g_VarTypeManage->GetPointTo(p->var_w.thevar->m_DataTypeID);
            //������һ���ǲ�����
            p->var_r1.opsize = GG_VarType_ID2Size(p->var_r1.thevar->m_DataTypeID);
            p->var_r1.thevar->size = GG_VarType_ID2Size(p->var_r1.thevar->m_DataTypeID);
            this->m_exprs->Enlarge_Var(p->var_r1.thevar, this->m_instr_list);
            return true;
        }
    }
    return false;
}

//--------------------------------------------------
bool CFuncOptim::optim_once_new()
{
	// cmp��jxx�Ѿ��ϲ�
    POSITION pos = this->m_exprs->vList->GetHeadPosition();
    while (pos)
    {
        M_t* p = this->m_exprs->vList->GetNext(pos);
        //if (p->type == MTT_reg)
        if (p->type != MTT_var)		// �������������
        {
            if (Optim_var_NT(p))
                return true;
        }

		// ��ʱ����
		if (p->type == MTT_var
		&&  p->bTem ==true
			)		// ������ʱ����
        {
            if (Optim_var_NT_temp(p))
                return true;
        }
    }

#if 0
	INSTR_LIST* list = this->m_instr_list;

	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = list->GetNext(pos);
		if (p->var_w.type && p->var_w.thevar)
		{
			if (Optim_var_NT(p->var_w.thevar))
				return true;
		}
		if (p->var_r1.type && p->var_r1.thevar)
		{
			if (Optim_var_NT(p->var_r1.thevar))
				return true;
		}
		if (p->var_r2.type && p->var_r2.thevar)
		{
			if (Optim_var_NT(p->var_r2.thevar))
				return true;
		}
	}
#endif
	return false;
}

static st_VarOptm* used_list_Find(PINSTR pinstr, VAROPTM_LIST& used_list)
{
    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* p = used_list.GetNext(pos);

        if (p->pinstr == pinstr)
            return p;
    }
    return NULL;
}

//00: nothing with
//01: read
//02: write
//03: read and write
BYTE GetVarFinger_INSTR(M_t* pvar, PINSTR p)

{
    BYTE r = 0;
    if (pvar == p->var_w.thevar)
    {
        r |= 2;
        if (p->var_w.part_flag != 0)
            r|=4;
    }
    if (pvar == p->var_r1.thevar)
    {
        r |= 1;
        if (p->var_r1.part_flag != 0)
            r|=4;
    }
    if (pvar == p->var_r2.thevar)
    {
        r |= 1;
        if (p->var_r2.part_flag != 0)
            r|=4;
    }
    return r;
}

void CFuncOptim::prt_var_uselist(VAROPTM_LIST& used_list)	// ��ʾvar�б�
{
    log_prtl("%s","===========");
    INSTR_LIST *list = this->m_instr_list;
    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = used_list.GetNext(pos);
        PINSTR p = the->pinstr;
        if (p->type == i_Jump)
        {
            if (p->jmp.jmp_type == JMP_jmp)
            {
                log_prtl("    jmp %x", p->jmp.jmpto_off);
            }
            else
            {
                log_prtl("    jxx %x", p->jmp.jmpto_off);
            }
        }
        else if (p->type == i_Label)
        {
            log_prtl("    %x:", p->label.label_off);
        }
        else if (p->type == i_Return)
        {
            log_prtl("    ret");
        }
        else	// ��ʾα������Ϣ
        {
            CFunc_Prt theprt(this->Q);
            CString s = theprt.prt_the_instr(p);
            log_prtl("    %s", (PSTR)s.GetBuffer());
        }
    }
    log_prtl("%s","===========");
}

            
bool RemoveOneInstr_1(VAROPTM_LIST& used_list, PINSTR p)
{
    assert(p->type == i_Jump);
    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        POSITION savpos = pos;
        st_VarOptm* the = used_list.GetNext(pos);
        if (p == the->pinstr && the->bJxx)
        {
            used_list.RemoveAt(savpos);
			SAFE_DELETE(the);
            return true;
        }
    }
    return false;
}
bool RemoveOneInstr(VAROPTM_LIST& used_list, PINSTR p)
{
    bool rtn = false;
    rtn |= RemoveOneInstr_1(used_list, p);
    while (p->jmp.next_ref_of_this_label)
    {
        p = p->jmp.next_ref_of_this_label;
        rtn |= RemoveOneInstr_1(used_list, p);
    }
    return rtn;
}

bool CFuncOptim::Optim_Var_Flow_3(VAROPTM_LIST& used_list) // ��������EE��Ҫɾ��һ��
{
    
    bool FindE = false;

    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        POSITION savpos = pos;
        st_VarOptm* the = used_list.GetNext(pos);
        if (the->pinstr->type == i_End || the->pinstr->type == i_Return)
        {
            if (the->rw == 0)
            {
                if (FindE)
                {
                    used_list.RemoveAt(savpos);
					SAFE_DELETE(the);
                    return true;
                }
                FindE = true;
                continue;
            }
        }
        FindE = false;
            
    }

    return false;
}

   //�����һ��LE����ɾ�����ָ��LE��jmp
   //�����һ��L6����ɾ�����ָ��L6��jmp

bool CFuncOptim::Optim_Var_Flow_2(VAROPTM_LIST& used_list)
{
    PINSTR lastlabelinstr = NULL;

    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = used_list.GetNext(pos);
        if (lastlabelinstr != NULL)					// �����һ��i_Label
        {
            bool f = false;
            if (the->pinstr->type == i_End 
			 || the->pinstr->type == i_Return)
            {
                if (the->rw == 0)	// �ҵ�һ��LE
                    f=true;
            }
            if (the->rw == 2)		// i_Label ������д���������i_Label��������Ϊ����ı���һ��Ҫ��������
            {
                f=true;
            }

            if (f)
            {
                PINSTR p = lastlabelinstr->label.ref_instr;
                if (RemoveOneInstr(used_list, p))   // ɾ�����ָ��LE��jmp, ʹ������ɾ������ָ���jmp
                    return true;
            }
        }
            
        lastlabelinstr = NULL;
        if (the->pinstr->type == i_Label)
            lastlabelinstr = the->pinstr;
    }

    return false;
}
bool CFuncOptim::Optim_Var_Flow_1(VAROPTM_LIST& used_list)
{
    //��һ����������������ȥ��
    //������һ���ˣ���ȥ��
    
    POSITION pos = used_list.GetHeadPosition();

    POSITION headpos = pos;
    PINSTR head_label_instr = NULL;
    while (pos)
    {
        POSITION savpos = pos;
        st_VarOptm* the = used_list.GetNext(pos);
        PINSTR p = the->pinstr;

        if (headpos == savpos)	// ��һ��
        {
            if (p->type == i_Return)
            {
                used_list.RemoveAt(savpos); //remove me
				SAFE_DELETE(the);
                return true;    // ��һ������Return����ȥ��
            }
            if (the->IsJump())
            {
                used_list.RemoveAt(savpos); //remove me
				SAFE_DELETE(the);
                return true;    // ��һ����������������ȥ��
            }

           /* if (p->type == i_Label)
            {
                head_label_instr = p;
                continue;
            }*/
            break;
        }

    //    if (the->IsJump())
    //    {
    //        if (p->jmp.the_label == head_label_instr)
    //        {
    //            used_list.RemoveAt(savpos); //remove me
				//SAFE_DELETE(the);
    //            return true;    //������һ���ˣ���ȥ��
    //        }
    //    }
    }
    return false;
}

bool CFuncOptim::Optim_Var_Flow(VAROPTM_LIST& used_list)
{
    //��Щ��������ֻ����ɾused_list�е��������ɾinstr_list�е���
	switch(Hxd_Var_Split_Step)
	{
	case 0:
		if (this->Optim_Var_Flow_1(used_list))
			return true;
		Hxd_Var_Split_Step++;
	case 1:
		if (this->Optim_Var_Flow_2(used_list))	// ������Ҫ��jmp
			return true;
		Hxd_Var_Split_Step++;
	case 2:
		if (this->Optim_Var_Flow_3(used_list))	// ��������EE��Ҫɾ��һ��
			return true;
		Hxd_Var_Split_Step++;
	case 3:
		if (this->Optim_Var_Flow_4(used_list))
			return true;
		Hxd_Var_Split_Step++;
	case 4:
		if (this->Optim_Var_Flow_5(used_list))
        return true;
		Hxd_Var_Split_Step++;
	default:
		 return false;
	}

}
    
    
st_VarOptm* Find_Instr(VAROPTM_LIST& used_list, PINSTR pinstr)
{
    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = used_list.GetNext(pos);
        if (the->pinstr == pinstr)
            return the;
    }
    return NULL;
}

bool CFuncOptim::SureNotUse_1(VAROPTM_LIST& used_list, st_VarOptm* the)
{
    if (the->tem_1)
        return true;
    the->tem_1 = true;

    if (the->rw == 2)
        return true;
    if (the->rw & 1)
        return false;
    if (the->pinstr->type == i_Return)
        return true;

    if (the->bJxx)
    {
        if (!SureNotUse_1(used_list, 
                          Find_Instr(used_list, the->pinstr->jmp.the_label)
                          ))
            return false;
    }

    POSITION pos = used_list.Find(the);
    used_list.GetNext(pos);
    if (pos == NULL)
        return true;
    
    st_VarOptm* the1 = used_list.GetNext(pos);
    return SureNotUse_1(used_list,the1);
}

// 
bool CFuncOptim::SureNotUse(VAROPTM_LIST& used_list, st_VarOptm* j)
{
    //assert(j->bJxx);
    {
        POSITION pos = used_list.GetHeadPosition();
        while (pos)
        {
            st_VarOptm* the = used_list.GetNext(pos);
            the->tem_1 = false;
        }
    }

    return SureNotUse_1(used_list,j);
}

// �������ѵ�
bool CFuncOptim::Optim_Var_Flow_5(VAROPTM_LIST& used_list)
{
    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        POSITION savpos = pos;
        st_VarOptm* the = used_list.GetNext(pos);
        if (the->bJxx)	// ��ת
        {
            if (this->SureNotUse(used_list, the))	// ȷ������Ҫ��
            {
                used_list.RemoveAt(savpos);
				SAFE_DELETE(the);
                return true;
            }
        }
    }
    return false;
}

// �յ�������Ҫɾ
// �յ�������Ҫɾ
// û���õ�Label��Ҫɾ
bool CFuncOptim::Optim_Var_Flow_4(VAROPTM_LIST& used_list)
{
    POSITION last_pos;
    PINSTR last_jmp = NULL;     
    PINSTR last_label[256];   
    int n_label = 0;

    POSITION pos = used_list.GetHeadPosition();
    while (pos)
    {
        POSITION savpos = pos;
        st_VarOptm* the = used_list.GetNext(pos);
        PINSTR p = the->pinstr;
        if (the->IsJump())
        {
            for (int i=0; i<n_label; i++)
            {
                if (last_label[i] != NULL && p->jmp.the_label == last_label[i])
                {
                    used_list.RemoveAt(savpos); // remove me
					SAFE_DELETE(the);
                    return true;
                }
            }
            last_jmp = p;
            last_pos = savpos;
            n_label = 0;
        }
        else if (p->type == i_Label)
        {
            if (last_jmp != NULL && last_jmp->jmp.the_label == p)
            {
				st_VarOptm* the_last=used_list.GetAt(last_pos);
                used_list.RemoveAt(last_pos);

				SAFE_DELETE(the_last);
                return true;
            }

            last_label[n_label++] = p;
        }
        else
        {
            last_jmp = NULL;
            n_label = 0;
        }
    }

    //û���õĿյ�labelҪɾ��
    pos = used_list.GetHeadPosition();
    while (pos)
    {
        POSITION savpos = pos;
        st_VarOptm* the = used_list.GetNext(pos);
        PINSTR p = the->pinstr;
        if (p->type == i_Label)
        {
            bool used = false;
            POSITION pos1 = used_list.GetHeadPosition();
            while (pos1)
            {
                st_VarOptm* the1 = used_list.GetNext(pos1);
                PINSTR p1 = the1->pinstr;
                if (the1->IsJump())
                {
                    if (p1->jmp.the_label == p)
                    {
                        used = true;
                        break;
                    }
                }
            }
            if (!used)
            {
                used_list.RemoveAt(savpos);
				SAFE_DELETE(the);
                return true;
            }
        }
    }

    return false;
}

char HowVarUse_Char(st_VarOptm* the);
CString CFuncOptim::Get_var_finger_NT(VAROPTM_LIST& volist, M_t* pvar)
{
    CString tbl_c;
    POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);

        char b = HowVarUse_Char(the);
        tbl_c += b;
    }
    return tbl_c;
}

char HowVarUse_Char(st_VarOptm* the)
{
    if (the->bJxx)
        return 'J';
    
    PINSTR pinstr = the->pinstr;
    BYTE rw = the->rw;

    if (pinstr->type == i_Label) return 'L';
    if (pinstr->type == i_Return) return 'E';
	if (pinstr->type == i_CallRet) return 'F';	// �������أ����ܵ��ɼ򵥵ĸ�ֵ

    if (pinstr->type == i_Assign)
    {
        if (rw == 1) return '5';    //Read
        if (rw == 2) return '6';    //Write
        if (rw == 3) return '7';    //Read and Write
    }
    else
    {
        if (rw == 1) return '1';    //Read
        if (rw == 2) return '2';    //Write
        if (rw == 3) return '3';    //Read and Write
    }
    if (rw >= 4) 
		return 'u';
    assert(0);
    return 'U'; //��Ӧ�õ�����
}

// Ҫ��������
st_VarOptm* CFuncOptim::Hxd_GetVarUse_JmpChar(VAROPTM_LIST& volist,PINSTR pJmp)
{
	POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);

		if(the->pinstr->type == i_Label
		&& the->pinstr->label.label_off ==pJmp->jmp.jmpto_off
		)
		{
			the = volist.GetNext(pos);		// ����label�ļ�¼
			if(the)
				return the;
			else
				NULL;
		}
	}

	return NULL;	// û���ҵ�
}

bool CFuncOptim::Hxd_VarNoChange(PINSTR pStart, PINSTR pEnd)
{
	INSTR_LIST *list = this->m_instr_list;

    POSITION pos =  list->Find(pStart);
	VAR varFind = pStart->var_r1;

	POSITION lastpos =  list->Find(pEnd);
	
	while(pos)
	{
		PINSTR p = list->GetNext(pos);

		if(pos==lastpos)
			return true;
		else if(VAR::IsSame(&varFind, &p->var_w))
			return false;
	
	}
    return true;
}

// ������ʱ����
bool CFuncOptim::Optim_var_flow_NT_temp(VAROPTM_LIST& volist, M_t* pvar,char hxd_tbl_c[])		// volist��pvar��ʹ�ü�¼
{
    int n = 0;
    char tbl_c[256];
    PINSTR tbl_pinstr[256];

    POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);
        char b = HowVarUse_Char(the);

		while(b=='L')	// i_Label ����
		{
			the = volist.GetNext(pos);
			  b = HowVarUse_Char(the);
		}

        tbl_c[n] = b; tbl_c[n+1] = '\0';
        tbl_pinstr[n] = the->pinstr;
        n++;

		memcpy(hxd_tbl_c,tbl_c,n+1);

        if (n >= 1)		// �κ�ʱ���Լ���ֵ�Լ�����䶼����Ҫ
        {
            const char* pb = tbl_c + n - 1;		// ���һ��������
            PINSTR* pi = tbl_pinstr + n - 1;	// ���һ��α����
            if (strcmp(pb, "7") == 0 &&  pi[0]->va_r1.pao == NULL)	//�Լ�assign�Լ�
            {
                g_CStrategy.AddOne_CanDelete(pvar, pi[0], "assiang self, delete it");
                return true;
            }

        }

        if (n >= 2)		// �������������
        {
            const char* pb = tbl_c + n - 2;
            PINSTR* pi = tbl_pinstr + n - 2;

            if (   strcmp(pb, "2E") == 0		// д��Ͳ�����
				|| strcmp(pb, "3E") == 0		// д��Ͳ�����
				|| strcmp(pb, "6E") == 0		// ��ֵ��Ͳ�����
                || strcmp(pb, "7E") == 0		// ��ֵ��Ͳ�����
				)
            {
                g_CStrategy.AddOne_CanDelete(pvar, pi[0], "write and end, delete it");
                return true;
            }
        }

    }

    return false;
}


bool CFuncOptim::Optim_var_flow_NT(VAROPTM_LIST& volist, M_t* pvar,char hxd_tbl_c[])		// volist��pvar��ʹ�ü�¼
{
    int n = 0;
    char tbl_c[256];
    PINSTR tbl_pinstr[256];

	// �ڶ����Ż�
    POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);
        char b = HowVarUse_Char(the);

		while(b=='L')	// i_Label ����
		{
			the = volist.GetNext(pos);
			  b = HowVarUse_Char(the);
		}

        tbl_c[n] = b; tbl_c[n+1] = '\0';
        tbl_pinstr[n] = the->pinstr;
        n++;

		memcpy(hxd_tbl_c,tbl_c,n+1);

        if (n >= 1)		// �κ�ʱ���Լ���ֵ�Լ�����䶼����Ҫ
        {
            const char* pb = tbl_c + n - 1;		// ���һ��������
            PINSTR* pi = tbl_pinstr + n - 1;	// ���һ��α����
            if (strcmp(pb, "7") == 0 &&  pi[0]->va_r1.pao == NULL)	//�Լ�assign�Լ�
            {
                g_CStrategy.AddOne_CanDelete(pvar, pi[0], "assiang self, delete it");
                return true;
            }

        }

        if (n >= 2)		// �������������
        {
            const char* pb = tbl_c + n - 2;
            PINSTR* pi = tbl_pinstr + n - 2;

            if (   strcmp(pb, "2E") == 0		// д��Ͳ�����
				|| strcmp(pb, "3E") == 0		// д��Ͳ�����
				|| strcmp(pb, "6E") == 0		// ��ֵ��Ͳ�����
                || strcmp(pb, "7E") == 0		// ��ֵ��Ͳ�����
				)
            {
                g_CStrategy.AddOne_CanDelete(pvar, pi[0], "write and end, delete it");
                return true;
            }
            
            if (   strcmp(pb, "22") == 0 
				|| strcmp(pb, "26") == 0
                || strcmp(pb, "32") == 0 
				|| strcmp(pb, "36") == 0
                || strcmp(pb, "62") == 0 
				|| strcmp(pb, "66") == 0
				|| strcmp(pb, "FF") == 0
				|| strcmp(pb, "F2") == 0
				|| strcmp(pb, "F6") == 0
				|| strcmp(pb, "2F") == 0
				|| strcmp(pb, "6F") == 0
				)
            {
                g_CStrategy.AddOne_CanDelete(pvar, pi[0], "Write and Write, delete first");
                return true;
            }

			if (strcmp(pb, "63") == 0
			&& Hxd_VarNoChange(pi[0],pi[1]))			// ��ֵ�Ժ���д��
            {

				g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"63",ES_Instr_can_Elim_63);
                return true;
            }
			if (strcmp(pb, "23") == 0
				&& Hxd_VarNoChange(pi[0],pi[1]))
            {
				if(pvar->type!=MTT_tem)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"23",ES_Instr_can_Elim_21E);
                    return true;
				}
            }

			if (strcmp(pb, "61") == 0 )
			{
				if(pi[0]->va_r1.pao == NULL 
				&& Hxd_VarNoChange(pi[0],pi[1])
				)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"61",ES_Instr_can_Elim_hxd_61);
					return true;
				}
			}
			if (strcmp(pb, "65") == 0 )
			{
				
				if( (pi[0]->va_r1.pao == NULL  || pi[0]->va_r1.pao->type == IA_GetAddress)
				&& Hxd_VarNoChange(pi[0],pi[1])
				)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"65",ES_Instr_can_Elim_hxd_61);
					return true;
				}

			}

			if (strcmp(pb, "6J") == 0 
			&& pi[0]->va_r1.pao == NULL )	// ������ת
			{
				st_VarOptm* pP2;
				pP2=Hxd_GetVarUse_JmpChar(volist,pi[1]);

				if(pP2 != NULL)
				{
					char b = HowVarUse_Char(pP2);
					if(b=='5')
					{
						g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pP2->pinstr,"6J5",ES_Instr_can_Elim_hxd_61);
						return true;

					}
					else if(b=='1')
					{
						g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pP2->pinstr,"6J1",ES_Instr_can_Elim_hxd_61);
						return true;

					}
				}
			}

        }

		if (n >= 3)				// �����λ������
		{
			const char* pb = tbl_c + n - 3;
			PINSTR* pi = tbl_pinstr + n - 3;

			char fpt[4];    //finger point
			strcpy(fpt,pb);
			assert(strlen(fpt) == 3);

			if (strcmp(fpt, "6J1") == 0)	// Ҫ��������
			{
				if( pi[0]->va_r1.pao == NULL )
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[2],"6J1",ES_Instr_can_Elim_hxd_61);
					return true;
				}
			}

			if (strcmp(fpt, "6J5") == 0)
			{
				if( pi[0]->va_r1.pao == NULL 
				 || pi[0]->va_r1.pao->type == IA_GetAddress)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[2],"6J5",ES_Instr_can_Elim_hxd_61);
					return true;
				}
			}

			if (fpt[2] == '6' || fpt[2] == '2')		// д�롢��ֵд�롢���Ǻ�������ֵ
				fpt[2] = 'E';

			if (strcmp(fpt, "F1E") == 0)
			{
				if(pvar->type!=MTT_tem)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"F1E",ES_Instr_can_Elim_21E);
					return true;
				}

			}

			if (strcmp(fpt, "F5E") == 0)
			{
				if(pvar->type!=MTT_tem)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"F5E",ES_Instr_can_Elim_21E);
					return true;
				}

			}

			// �������ʹ�������
			if (fpt[2] == 'F')	// д�롢��ֵд�롢���Ǻ�������ֵ
				fpt[2] = 'E';

			if (strcmp(fpt, "6JE") == 0 && pi[0]->va_r1.pao == NULL )	// Ҫ��������
			{
				st_VarOptm* pP2;
				pP2=Hxd_GetVarUse_JmpChar(volist,pi[1]);

				if(pP2 != NULL)
				{
					char b = HowVarUse_Char(pP2);

					if (b == '6' || b == '2' || b == 'F' || b == 'E')
					{
						g_CStrategy.AddOne_CanDelete(pvar, pi[0], "6JEE,delete it");
						return true;
					}
					
				}
			}

			if (strcmp(fpt, "25E") == 0 && pi[1]->va_r1.pao == NULL)	// д�룬��ֵ�����ж�����д��
			{
				//g_CStrategy.AddOne_CanEliminate_25E(pvar,pi[0],pi[1],"25E");
				g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"25E",ES_Instr_can_Elim_25E);
				return true;
			}

			if (strcmp(fpt, "61E") == 0)
			{
				g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"61E",ES_Instr_can_Elim_63);
				return true;
			}

			if (strcmp(fpt, "31E") == 0)
			{
				if (pi[0]->type == i_Sub || pi[0]->type == i_Add)
					if (pi[0]->var_w.thevar == pi[0]->var_r1.thevar)
						if (pi[1]->type == i_Cmp 
						//&& pi[1]->var_r2.type == v_Immed
						&& pi[0]->var_w.thevar == pi[0]->var_r1.thevar)
						{
							//log_prtl("31E find");
							g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"31E",ES_Instr_can_Elim_31E);
							return true;
						}
			}
			if (strcmp(fpt, "21E") == 0)
			{
				if(pvar->type!=MTT_tem)
				{
					g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"21E",ES_Instr_can_Elim_21E);
					return true;
				}
			}

		}

        if (n >= 4)		// �����λ������
        {
            const char* pb = tbl_c + n - 4;
            PINSTR* pi = tbl_pinstr + n - 4;

            char fpt[5];    //finger point
            strcpy(fpt,pb);
            assert(strlen(fpt) == 4);

			if (strcmp(pb, "F1JE") == 0 )
			{
				if(pvar->type!=MTT_tem)
				{
					st_VarOptm* pP2;
					pP2=Hxd_GetVarUse_JmpChar(volist,pi[2]);

					if(pP2 != NULL)
					{
						char b = HowVarUse_Char(pP2);
						if(b=='6' || b=='2' || b=='F')
						{
							g_CStrategy.AddOne_CanEliminate(pvar,pi[0],pi[1],"F1JE",ES_Instr_can_Elim_21E);
							return true;

						}

					}
				}
			}


            if (fpt[3] == '6' || fpt[3] == '2' || fpt[3] == 'F')
                fpt[3] = 'E';

            if (strcmp(fpt, "311E") == 0)
            {
                if (pi[0]->type == i_Sub || pi[0]->type == i_Add)
                if (pi[0]->var_w.thevar == pi[0]->var_r1.thevar)
                if (pi[1]->type == i_Cmp && pi[0]->var_w.thevar == pi[1]->var_r1.thevar)
                if (pi[2]->type == i_Cmp && pi[0]->var_w.thevar == pi[2]->var_r1.thevar)
                {
                    log_prtl("311E find");
                    signed int d = pi[0]->var_r2.d;
                    if (pi[0]->type == i_Add)
                        d = -d;
                    pi[1]->var_r2.d += d;
                    pi[2]->var_r2.d += d;
                    m_instr_list->RemoveAt(m_instr_list->Find(pi[0]));
                    return true;
                }
            }
        }
    }

    return false;
}



bool CFuncOptim::Optim_var_NT_temp(M_t* pvar)
{
    if (pvar == NULL)
        return false;

    VAROPTM_LIST used_list;	//	��������instr
	char hxd_tbl_c[256];

    Get_Var_Use_Flow(used_list, pvar);//�ȵõ�����ʹ��ͼ

    // ���Ż����ٷ�
    if (Optim_var_flow_NT_temp(used_list, pvar,hxd_tbl_c))
    {
		if(g_Hxd_Step)		// ����ʱ����ʾ
		{
			prt_var_uselist(used_list);	  // ��ӡ��ǰ����ʹ�����
			log_prtl("%s: %s",pvar->GetName() ,hxd_tbl_c);
		}

		Hxd_DeleteUsedList(&used_list);
        return true;
    }
	Hxd_DeleteUsedList(&used_list);

    return false;
}


// ��ͼ�����var�Ż�
bool CFuncOptim::Optim_var_NT(M_t* pvar)
{
    if (pvar == NULL)
        return false;

    VAROPTM_LIST used_list;	//	��������instr
	char hxd_tbl_c[256];

    Get_Var_Use_Flow(used_list, pvar);//�ȵõ�����ʹ��ͼ

    // ���Ż����ٷ�
    if (Optim_var_flow_NT(used_list, pvar,hxd_tbl_c))
    {
		if(g_Hxd_Step)		// ����ʱ����ʾ
		{
			prt_var_uselist(used_list);	  // ��ӡ��ǰ����ʹ�����
			log_prtl("%s: %s",pvar->GetName() ,hxd_tbl_c);
		}

		Hxd_DeleteUsedList(&used_list);
        return true;
    }
	Hxd_DeleteUsedList(&used_list);

    return false;
}

static int static_n = 0;
static int static_tbl[500];
// �������Ҫ�ݹ��
void CFuncOptim::TryDistinguishVar_1(VAROPTM_LIST& volist, M_t* pvar, POSITION pos,int* pglobalstep)
{
    int step = *pglobalstep;

    //֮����Ҫ��һ��pglobalstep����Ϊ�˵õ���һ��step
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);
        if (the->bJxx)
        {
            st_VarOptm* plabel = used_list_Find(the->pinstr->jmp.the_label, volist);
            assert(plabel != 0);
            TryDistinguishVar_1(volist, pvar, volist.Find(plabel), pglobalstep);

            if (the->pinstr->jmp.jmp_type == JMP_jmp)	// jmpָ��
            {
                step = *pglobalstep;
                *pglobalstep = step+1;
            }
            continue;
        }

        if (the->pinstr->type == i_Label)
        {
            continue;   //����Ҫ��ʲô����
        }
        if (the->pinstr->type == i_Return)
        {
            step = *pglobalstep;
            *pglobalstep = step+1;
            continue;
        }

        //
        assert(the->rw != 0);

        if (the->rw & 1)    // ����Ϊ������
        {

			 //if (the->varstep_r != step)
			 //{
				//if (the->varstep_r != 0			// �����ȼ��Ѷ�
				//&& the->varstep_r != step
				//)
				//{//ײ���ˣ���ô�죿
				//	int i1 = min(the->varstep_r, step);
				//	int i2 = max(the->varstep_r, step);
				//	assert(static_n < 500);
				//	static_tbl[static_n++] = i1;
				//	static_tbl[static_n++] = i2;
				//	return;
				//}
				//the->varstep_r = step;
			 //}


            if (the->varstep_r == step)	
                return;
            
            if (the->varstep_r != 0			// �����ȼ��Ѷ�
			 && the->varstep_r != step
			 )
            {//ײ���ˣ���ô�죿
                int i1 = min(the->varstep_r, step);
                int i2 = max(the->varstep_r, step);
				assert(static_n < 500);
                static_tbl[static_n++] = i1;
                static_tbl[static_n++] = i2;
                return;
            }
            the->varstep_r = step;

        }

        if (the->rw & 2)    // write
        {
            if (the->varstep_w != 0)
                return;		//	���������ˣ���������
            
            step = *pglobalstep;
			step++;
			*pglobalstep = step;
            the->varstep_w = step;
        }
    }
}

bool CFuncOptim::IfAnyThisStep(int i, VAROPTM_LIST& volist)
{
    POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);
        if (the->varstep_r == i)
            return true;
        if (the->varstep_w == i)
            return true;
    }
    return false;
}

//��i2�����Ǹ�
bool Get_pair(int& out_i1, int& out_i2)
{
    int savi;
    int max_i2 = 0;
    for (int i=0; i<static_n; i+= 2)
    {
        int i1 = static_tbl[i];
        int i2 = static_tbl[i+1];
        if (i2 > max_i2)
        {
            max_i2 = i2;
            out_i2 = i2;
            out_i1 = i1;
            savi = i;
        }
    }
    if (max_i2 == 0)
        return false;
    static_tbl[savi] = 0;  //����������ظ�ʹ��
    static_tbl[savi+1] = 0;
    return true;
}
        
void Replace_i2_with_i1(VAROPTM_LIST& volist, int i1, int i2)
{
    POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* the = volist.GetNext(pos);
        if (the->varstep_r == i2)
            the->varstep_r = i1;
        if (the->varstep_w == i2)
            the->varstep_w = i1;
    }
}

            
void CFuncOptim::Replace_Var_vo(VAROPTM_LIST& volist, int step, M_t* pvar, M_t* pnew)
{
    POSITION pos = volist.GetHeadPosition();
    while (pos)
    {
        st_VarOptm* p = volist.GetNext(pos);
        if (p->varstep_r == step)
        {
            if (p->pinstr->var_r1.thevar == pvar)
                p->pinstr->var_r1.thevar = pnew;
            if (p->pinstr->var_r2.thevar == pvar)
                p->pinstr->var_r2.thevar = pnew;
        }
        if (p->varstep_w == step)
        {
            if (p->pinstr->var_w.thevar == pvar)
                p->pinstr->var_w.thevar = pnew;
        }
    }
}

//  �����ܲ��ܰ�һ��var���߼��Ϸ�Ϊ����
//  ��ʼддvarstep_r��varstep_w
PSTR my_itoa(int i);
bool CFuncOptim::TryDistinguishVar(VAROPTM_LIST& volist, M_t* pvar)
{
    if (pvar->type != MTT_reg)
        return false;   //��ֻ����reg����
    
	static_n = 0;
    int step = 1;
    TryDistinguishVar_1(volist, 
		                pvar,
                        volist.GetHeadPosition(), 
                        &step);

    for (;;)		// ������������
    {
        int i1, i2;
        if (!Get_pair(i1, i2))	// û�г�ͻ���ݣ����ô���
            break;
        Replace_i2_with_i1(volist, i1, i2);
    }

    int n = 0;
    int tblstep[500];			// һ���Ĵ��������������500��
    for (int i=1; i<step+1; i++)
    {
        if (IfAnyThisStep(i, volist))
        {
            tblstep[n++] = i;
        }
    }

    if (n > 1)
    {
		if(g_Hxd_Step)
			log_prtl("find var distinguish");

        for (int i=0; i<n; i++)
        {
            if (i == 0)
                continue;   //������һ��
            
            M_t* pnew = new M_t;
            *pnew = *pvar;
            strcat(pnew->namestr, "_");
            strcat(pnew->namestr, my_itoa(i));
            this->m_exprs->vList->AddTail(pnew);
            Replace_Var_vo(volist, tblstep[i], pvar, pnew);		// ����������
        }
        return true;
    }

    return false;
}

// ֻдused_list.pinstr �� used_list.rw����дvarstep_r��varstep_w

void CFuncOptim::Get_Var_Use_Flow(VAROPTM_LIST& used_list, M_t* pvar)
{
    PINSTR tbl_label[728];
    int num = 0;

    INSTR_LIST *list = this->m_instr_list;
    POSITION pos = list->GetHeadPosition();
    while (pos)		// ���浱ǰ����ʹ����Ϣ��i_Label��i_Return��i_Jump
    {
        POSITION savpos = pos;
        PINSTR p = list->GetNext(pos);

        BYTE c = GetVarFinger_INSTR(pvar, p);
        if (c != 0 
		|| p->type == i_Label
		|| p->type == i_Return
		)
        {

            st_VarOptm* the = new st_VarOptm;
            
            the->pinstr = p;
            the->rw = c;

            used_list.AddTail(the);

        }

        if (p->type == i_Jump)
        {
            tbl_label[num++] = p->jmp.the_label;	// ������ת��ַ
            
            st_VarOptm* the = new st_VarOptm;
            
            the->pinstr = p;
            the->rw = c;
            the->bJxx = true;

            used_list.AddTail(the);

            //������һ��jump���ܻ�����Σ�ǰһ�������õ��˱�����cmp(v,?)
            //��һ������jxx��
        }
    }

	//prt_var_uselist(used_list);	// �鿴�Ż�ǰ����Ϣ

    //----------------------------------
    //Ҫ�Ż�������У���û�õ�jmp��labelȥ��
	Hxd_Var_Split_Step = 0;
    while (Optim_Var_Flow(used_list))
    {
    }
}

void CFuncOptim::Prt_Var_Flow(PSTR varname)
{
    M_t* pvar = this->m_exprs->GetVarByName(varname);
    if (pvar == NULL)
    {
        log_prtl("not find var: %s", varname);
        return;
    }
    log_prtl("find var: %s", varname);
    VAROPTM_LIST used_list; //��������instr

    Get_Var_Use_Flow(used_list, pvar);
    prt_var_uselist(used_list);
    CString s = Get_var_finger_NT(used_list, pvar);

	Hxd_DeleteUsedList(&used_list);

    log_prtl("%s", (PSTR)s.GetBuffer());
}
