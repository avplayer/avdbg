// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
#include	"CISC.H"
#include "cexe2c.h"
#include "CFuncStep1.h"

static void	Add_in_order(EALIST *list, ea_t i)
{
	if (list->IsEmpty())
	{
		list->AddHead(i);
		return;
	}
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		POSITION savpos = pos;
		ea_t ea = list->GetNext(pos);
		if (ea == i)
			return;		//	�Ѿ����ˣ�������
		if (ea > i)
		{
			list->InsertBefore(savpos,i);
			return;
		}
	}
    list->AddTail(i);
}

//==========================================

//	�ж��Ƿ�Ϊenter,leave��mov esp,??֮���ָ��
static bool	isLeave(AsmCode* p)
{	
	if (p->xcpu.opcode == C_LEAVE)
		return true;
	if (p->xcpu.opcode == C_MOV
		&& (p->xcpu.op[0].mode == OP_Register)
		&& (p->xcpu.op[0].reg.reg_index == _ESP_)	//mov esp,?
		)
		return true;
	return false;
}

static bool	any_free_ea(EALIST *jxxlist, EALIST *usedlist, ea_t* pea)	// ���ز���usedlistg�б��еĵ�ַ
{
	POSITION pos = jxxlist->GetHeadPosition();
	while (pos)
	{
		ea_t ea = jxxlist->GetNext(pos);
		if (! usedlist->Find(ea))
		{
			*pea = ea;
			return true;
		}
	}
	return false;
}

void CFuncStep1::check_if_switch_case(ea_t cur_off, CaseList* pcaselist,EALIST* pjxxlist,XCPUCODE* pxcpu)
{//static function
	if (pxcpu->opcode != C_JMP)
		return;
	if (pxcpu->op[0].mode != OP_Address)
		return;
	if (pxcpu->op[0].addr.base_reg_index != _NOREG_)
		return;
	if (pxcpu->op[0].addr.off_reg_index == _NOREG_)
		return;
	if (pxcpu->op[0].addr.off_reg_scale != 4)
		return;
	if (pxcpu->op[0].addr.off_value <= 0x401000)
		return;
	//alert("switch case find 1");
	//Code_SwitchCase1();

	ea_t ptbl = pxcpu->op[0].addr.off_value;
	//alert_prtf("table is %x",ptbl);
	if (! IfInWorkSpace(ptbl))
		return;		//	ȷ�����table��Ч
	
	//alert("switch case find 2");

	ea_t d = Peek_D(ptbl);
	if (! IfInWorkSpace(d))
		return;		//	ȷ�ϵ�һ����Ч
	
	//alert("switch case find 3");

	ea_t break_off = 0;	//	���棬Ҫȷ��break_off��ֵ
	POSITION pos = pjxxlist->GetHeadPosition();
	while (pos)
	{
		ea_t ea = pjxxlist->GetNext(pos);
		if (ea > cur_off)
		{
			break_off = ea;
			break;
		}
	}
	if (break_off == 0)
		return;		//	û�ҵ�������?���Ծ�
	
	//alert("switch case find 4");


	//if (pjxx->jmp.jmp_type != JMP_ja)
	//	return;		//	����ja,Ҳ����
	if (d < cur_off || d > break_off)
		return;		//	��һ����û��׼������

	//alert("switch case find 5");

	//alert("rreally switch case");
	CASE_t *pnew = new CASE_t;
	pnew->jxx_opcode = cur_off;
	pnew->caselist = new EALIST;
	pcaselist->AddHead(pnew);

	for (int i=0;;i++)
	{
		d = Peek_D(ptbl+i*4);
//		if (! IfInWorkSpace(d))
		if (d < cur_off || d > break_off)
			break;
		Add_in_order(pjxxlist,d);
		pnew->caselist->AddTail(d);
	}
}

void CFuncStep1::CheckIfJustSwitchCase(CaseList& caselist, ea_t ea)
{
    POSITION pos1 = caselist.GetHeadPosition();
    while (pos1)
    {
        CASE_t *p1 = caselist.GetNext(pos1);
        if (p1->jxx_opcode == ea)	//	really
        {	//	now, add some jcase instruction
            POSITION pos2 = p1->caselist->GetHeadPosition();
            while (pos2)
            {
                ea_t case_ea = p1->caselist->GetNext(pos2);
                assert(case_ea);

                AsmCode *pnew = AsmCode::new_AsmCode();
                pnew->linear = ea;
                pnew->xcpu.opcode = C_JCASE;
                pnew->xcpu.op[0].mode = OP_Near;
                pnew->xcpu.op[0].nearptr.offset = case_ea;
                this->m_asmlist->AddTail(pnew);
                //alert("insert 1 C_JCAES");
            }
            break;	//	only one can be true
        }
    }
}

void CFuncStep1::CreateNewFunc_if_CallNear()
{
	POSITION pos1 = m_asmlist->GetHeadPosition();
	while (pos1)
	{
		AsmCode* pasm = m_asmlist->GetNext(pos1);
		XCPUCODE* pxcpu = &pasm->xcpu;
		if ( pxcpu->opcode == C_CALL && pxcpu->op[0].mode == OP_Near)
		{
				g_Cexe2c->func_new(pxcpu->op[0].nearptr.offset);	// ����������Ϣ
		}
	}
}

bool CFuncStep1::Step_1(ea_t head_off) // ��������б����溯��������ַ���趨�������3000��
{
	//assert(m_nStep == 0);

	CaseList caselist;
	EALIST jxxlist;		// ��ת����б�
	EALIST usedlist;	// �ѷ�����������б�

    ea_t ea = head_off;
    assert(ea < 0x10000000);	// 0x400000 <= �����ĵ�ַ < 0x10000000
    assert(ea >= 0x400000);

	jxxlist.AddHead(ea);

	// �ҵ���ǰ�������еĵ�ַ�����浽usedlist��
    while (any_free_ea(&jxxlist,&usedlist,&ea))							//	�߱����е�jxx
    {	
		for (;;)
		{
			Add_in_order(&usedlist,ea);									// ��ӵ��Ѵ�����б���

            CDisasm the;
			the.Disasm_OneCode(ea); //ea�������

			if (the.get_xcpu()->IsJxx() || the.get_xcpu()->IsJmpNear())	// �ж��Ƿ�Ϊ��תָ��
			{
				Add_in_order(&jxxlist, the.get_xcpu()->op[0].nearptr.offset);	// ��ӵ���ת�б���
			}
			else														// �ж��Ƿ�Ϊswitchָ��
			{
				check_if_switch_case(ea,&caselist,&jxxlist, the.get_xcpu());
			}

			if (the.get_xcpu()->opcode == C_RET || the.get_xcpu()->opcode == C_JMP )
				break;

			//if (the.get_xcpu()->opcode == C_CALL )	// ExitProcessҲҪ�˳�
			//	  break;

			if (usedlist.Find(ea))		// ��ַ�Ѿ������
				break;
		}
    }
	
	// �����usedlist,���3000�л��
	POSITION pos = usedlist.GetHeadPosition();
	int i_MaxLines=0;
	while (pos && i_MaxLines<3000)									//	����asm opcode list
	{
		if (this->m_asmlist == NULL)
		{
			this->m_asmlist = new AsmCodeList;
		}

		i_MaxLines++;
		
		ea_t ea = usedlist.GetNext(pos);
		AsmCode *p = AsmCode::new_AsmCode();
		p->linear = ea;								// ������ַ

        CDisasm the;
		p->opsize = the.Disasm_OneCode(ea);			// �����,ea�Զ�����
		p->xcpu = *the.get_xcpu();

		// �쳾�������
		// 0000 ADD [EAX], AL ָ���
		/*
		if(p->xcpu.opcode == C_ADD
		&& (p->xcpu.op[0].mode==OP_Address && p->xcpu.op[0].addr.base_reg_index==0x00)
		&& (p->xcpu.op[1].mode==OP_Register && p->xcpu.op[1].reg.reg_index==0x00)
			)				
		  continue;
		*/

		this->m_asmlist->AddTail(p);				// ���淴�����

        this->CheckIfJustSwitchCase(caselist, ea);	// �����ǲ��Ǹչ�һ��swith case

	}

	AsmCode *pasm = this->m_asmlist->GetTail();	// ��ȡ���һ������������Ϣ
	m_end_off = pasm->linear + pasm->opsize;	// �õ�����������ַ

	return true;
}

AsmCode* CFuncLL::ea2pasm(ea_t ea)
{
	POSITION pos = m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* p = m_asmlist->GetNext(pos);
		if (p->linear == ea)
			return p;
	}
	log_prtl("0x%08X �����ʧ��",ea);
	//assert_msg(0,"whye here");
	return NULL;
}

PSTR check_if_jmp_api(const BYTE* phead);	// �ж��Ƿ�����ת��API
// ������ǰ�������һ�����
bool	CFuncLL::stack_stack(AsmCode* p0, AsmCode* p1)
{
    signed int & esp0 = p0->esp_level;
    signed int & esp0_next = p0->esp_level_next;
    signed int & esp1 = p1->esp_level;

	//	���ȣ���p0����Ľ���-------
    if (isLeave(p0))					// �Ƿ�ΪLeave �� MOV esp,? ָ��
	{
		if (esp0_next == ESP_UNKNOWN)	// δ֪����
		{
			signed int esp = ESP_UNKNOWN;
			if (Asm_Code_Change_ESP(esp, &p0->xcpu))
			{
				esp0_next = esp;
				return true;
			}
		}
	}
	else
	{
		if (esp0_next == ESP_UNKNOWN)		// ��һ��ָ���λ����δ֪��
		{
			if (p0->xcpu.opcode == C_JMP || p0->xcpu.opcode == C_RET)	// ��ת�뷵��
			{
				esp0_next = ESP_IGNORE;
				return true;
			}
			if (esp0 != ESP_UNKNOWN)		// ��ǰָ��λ��Ϊ��֪����һָ��δ֪ʱ����
			{
				signed int esp = esp0;
				if ( Asm_Code_Change_ESP(esp, &p0->xcpu) )
				{
					esp0_next = esp;
					return true;
				}
			}
		}

		if (esp0_next != ESP_UNKNOWN
			&& (esp0_next != ESP_IGNORE)
			&& (esp0 == ESP_UNKNOWN)
			)	// ����
		{
			signed int esp = 0;
			if ( Asm_Code_Change_ESP(esp, &p0->xcpu) )
			{
				//esp0_next = esp0 + esp;
				esp0 = esp0_next - esp;
				return true;
			}
		}
		// call --------------------
		if (esp0 != ESP_UNKNOWN
			&& esp0_next != ESP_UNKNOWN
			&& p0->xcpu.opcode == C_CALL
			&& p0->xcpu.op[0].mode == OP_Address
			&& p0->xcpu.op[0].addr.base_reg_index == _NOREG_
			&& p0->xcpu.op[0].addr.off_reg_index == _NOREG_
			)
		{
			ea_t address = p0->xcpu.op[0].addr.off_value;
			
			CApi *papi = g_ApiManage->get_api(address);	//find it
			if (papi)
			{
				if (esp0_next != esp0 + papi->m_stack_purge)
					return false;	//find error
			}
			else	// ��API������û���ҵ�
			{	//not find, insert it
				//alert_prtf("error not find api %x", address);
				//g_ApiManage->new_api(address,esp0_next - esp0);

				PBYTE temp01=ea2ptr(address);
				ea_t addr= *(DWORD*)temp01;

				g_Cexe2c->func_new(addr);
			}
		}
		
	}

	// Jmp -------------------------
    if (p1->xcpu.IsJxx() || p1->xcpu.IsJmpNear())
	{
		ea_t jmpto = p1->xcpu.op[0].nearptr.offset;
		AsmCode* p = ea2pasm(jmpto);
		if (p->esp_level == ESP_UNKNOWN && esp1 != ESP_UNKNOWN)
		{
			p->esp_level = esp1;
			return true;
		}
		if (p->esp_level != ESP_UNKNOWN && esp1 == ESP_UNKNOWN)
		{
			esp1 = p->esp_level;
			return true;
		}
		if (p->esp_level != ESP_UNKNOWN && esp1 != ESP_UNKNOWN)
		{
			if (p->esp_level != esp1)
				return false;
		}
	}
	// ret ------------------------
	if (p1->xcpu.opcode == C_RET)
	{
		if (esp1 == ESP_UNKNOWN)
		{
			esp1 = 0;
			return true;
		}
		if (esp1 != 0)
			return false;
	}
	// -----------------------------------
	if (esp0_next == ESP_UNKNOWN && esp1 != ESP_UNKNOWN)
	{
		esp0_next = esp1;
		return true;
	}

	if (esp0_next != ESP_UNKNOWN
		&& esp0_next != ESP_IGNORE
		&& esp1 == ESP_UNKNOWN)
	{
		esp1 = esp0_next;
		return true;
	}
	if (esp0_next != ESP_UNKNOWN
		&& esp0_next != ESP_IGNORE
		&& esp1 != ESP_UNKNOWN)
	{	//do some check
		if (esp0_next != esp1)
			return false;
	}
	return false;
}

ea_t FindApiAddress_Reg(DWORD regindex, XCPUCODE* pxcpu1, AsmCodeList* asmlist)
{
    //��˼�ǣ���Ҫ��ȷ����call eax
    //��ǰ��ȥ��һ��mov eax, [405070] �� mov eax, 405070
    ea_t retn = 7;

	POSITION pos = asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* p = asmlist->GetNext(pos);
        XCPUCODE* pxcpu = &p->xcpu;
		if (pxcpu == pxcpu1)
			return retn;
        if (pxcpu->opcode == C_MOV
            && pxcpu->op[0].mode == OP_Register
            && pxcpu->op[0].reg.reg_index == regindex)
        {
            if (pxcpu->op[1].mode == OP_Address 
                && pxcpu->op[1].addr.base_reg_index == _NOREG_ 
                && pxcpu->op[1].addr.off_reg_index == _NOREG_
                )   //mov eax, [405070] // call [eax]
            {
                ea_t address = pxcpu->op[1].addr.off_value;
                retn = address;
            }
			else if (pxcpu->op[1].mode == OP_Immed )	//mov eax, 405070 //call [eax]
            {
                ea_t address = pxcpu->op[1].immed.immed_value;

				PBYTE temp01=ea2ptr(address);
				ea_t addr= *(DWORD*)temp01;
				return addr;
            }
        }
	}
    return retn;
}


//  �����esp,��return true
//	�����Ǽٶ�һ��func��ֻ����һ��mov ebp,esp

//	Ӱ��stack�����⼸�������
//	enter leave
//	push ebp,mov ebp,esp,.....,mov esp,ebp,pop ebp
//	push,pop
//	call near
//	retʱ����Ƿ���ƽ��
//	��call	[immed]�ȣ��ݲ�����s
bool	CFuncLL::Asm_Code_Change_ESP(int &esp, XCPUCODE* pxcpu)
{
	if (pxcpu->opcode == C_MOV					// mov esp,ebp
        && (pxcpu->op[0].mode == OP_Register)
        && (pxcpu->op[0].reg.reg_index == _ESP_)
        && (pxcpu->op[1].mode == OP_Register)
        && (pxcpu->op[1].reg.reg_index == _EBP_))
    {
        if (this->m_EBP_base != Not_EBP_based
            && esp == ESP_UNKNOWN)
        {
            esp = this->m_EBP_base;				// mov esp,ebp
            return true;
        }
    }
	if (pxcpu->opcode == C_MOV					// mov ebp,esp
        && (pxcpu->op[0].mode == OP_Register)
        && (pxcpu->op[0].reg.reg_index == _EBP_)
        && (pxcpu->op[1].mode == OP_Register)
        && (pxcpu->op[1].reg.reg_index == _ESP_))
    {
        if (esp != ESP_UNKNOWN
            && this->m_EBP_base == Not_EBP_based)
        {
            this->m_EBP_base = esp;				//mov ebp,esp
            return true;
        }
    }

	if (pxcpu->opcode == C_LEAVE)	//  leave
	{
		esp = m_EBP_base;			//	mov	esp,ebp
		esp += 4;					//	pop	ebp

		return true;
	}

	if (pxcpu->opcode == C_ENTER)	//  enter N,L
	{
		esp -= 4;					//	push ebp
		this->m_EBP_base = esp;		//  mov ebp,esp

		if(pxcpu->op[1].immed.immed_value == 0) // enter N,0
			esp -= pxcpu->op[0].immed.immed_value;

		return true;
	}

	if (pxcpu->opcode == C_PUSH)	// push ?
	{
		esp -= 4;
		return true;
	}

	if (pxcpu->opcode == C_POP)		// pop  ?
	{
		esp += 4;
		return true;
	}

	if (pxcpu->opcode == C_SUB		// sub esp,imm
		&&(pxcpu->op[0].mode == OP_Register)
		&&(pxcpu->op[0].reg.reg_index == _ESP_)
		&&(pxcpu->op[1].mode == OP_Immed))
	{
		esp -= pxcpu->op[1].immed.immed_value;
		return true;
	}

	if (pxcpu->opcode == C_ADD		// add esp,imm
		&&(pxcpu->op[0].mode == OP_Register)
		&&(pxcpu->op[0].reg.reg_index == _ESP_)
		&&(pxcpu->op[1].mode == OP_Immed))
	{
		esp += pxcpu->op[1].immed.immed_value;
		return true;
	}

	if (pxcpu->opcode == C_CALL)	// call
	{
		if (pxcpu->op[0].mode == OP_Near)
		{
			CFunc* pfunc = g_Cexe2c->GetFunc(pxcpu->op[0].nearptr.offset);
			if (pfunc == NULL)
                return false;
			if (pfunc->m_IfLibFunc)
			{
				//esp += pfunc->m_stack_purge;
				//assert(pfunc->m_functype);
				if(pfunc->m_functype)	// Ҫ����
					esp += pfunc->m_functype->get_stack_purge();
		        return true;
			}
			if (pfunc->m_nStep < STEP_IDA_1)
				return false;
			esp += pfunc->m_stack_purge;
            return true;
		}
        else if (pxcpu->opcode == C_CALL &&
            pxcpu->op[0].mode == OP_Address &&
            pxcpu->op[0].addr.base_reg_index == _NOREG_ &&
            pxcpu->op[0].addr.off_reg_index == _NOREG_
            )   //call [405070]
        {
            ea_t address = pxcpu->op[0].addr.off_value;
            CApi* papi = g_ApiManage->get_api(address);// find API ����
            if (papi)
            {
                esp += papi->m_stack_purge;
                return true;
            }
			else
			{
				PBYTE temp01=ea2ptr(address);
				ea_t addr= *(DWORD*)temp01;

				g_Cexe2c->func_new(addr);
			}
        }
        else if (pxcpu->opcode == C_CALL &&
            pxcpu->op[0].mode == OP_Register
            )   //call eax
        {
            ea_t address = FindApiAddress_Reg(pxcpu->op[0].reg.reg_index, pxcpu, this->m_asmlist);
		
			if(address<0x10000)
				return false;

			CApi* papi = g_ApiManage->get_api(address);// find it
			if (papi)
			{
				esp += papi->m_stack_purge;
				return true;
			}
			
        }
		else if (pxcpu->opcode == C_CALL &&
            pxcpu->op[0].mode == OP_Address &&
            pxcpu->op[0].addr.base_reg_index != _NOREG_
            )   // ��call [ESI]  �쳾������� 
		{
			return true;
		}
		else
			return false;
	}

	// �쳾������� 
	if (pxcpu->opcode == C_RDTSC)		// RDTSC
		return true;

	// �쳾������� end

    if (esp == ESP_UNKNOWN)
        return false;
	return true;
}

void CFuncLL::AddRemoveSomeInstr()
{
    POSITION pos = m_asmlist->GetHeadPosition();
    while (pos)
    {
        AsmCode* pasm = m_asmlist->GetNext(pos);
        if (pasm->iAddRemove != 0)				// �Ѿ�������ģ����ٴ���
            continue;
        
        XCPUCODE* pxcpu = &pasm->xcpu;
        if (pxcpu->opcode == C_CALL && pxcpu->op[0].mode == OP_Near)
        {
            CFunc* pfunc = g_Cexe2c->GetFunc(pxcpu->op[0].nearptr.offset);
            if (pfunc == NULL)
                continue;
            if (pfunc->m_IfLibFunc && pfunc->m_functype != NULL)	// �ж���Ŀ⺯��
            {
                CFuncType* fctype = pfunc->m_functype;
                if (!strcmp(fctype->m_internal_name, "_EH_prolog"))
                {
                    //strcmp(fctype->m_internal_name, "_EH_prolog");
                    pasm->iAddRemove = 1;

                    AsmCode* pnew;
                    //push ebp
                    pnew = AsmCode::new_AsmCode();
                    pnew->iAddRemove = 2;
                    pnew->xcpu.opcode = C_PUSH;
                    pnew->xcpu.op[0].mode = OP_Register;
                    pnew->xcpu.op[0].opersize = 4;
                    pnew->xcpu.op[0].reg.reg_index = _EBP_;
                    m_asmlist->InsertBefore(pos, pnew);

                    //mov ebp, esp
                    pnew = AsmCode::new_AsmCode();
                    pnew->iAddRemove = 2;
                    pnew->xcpu.opcode = C_MOV;
                    pnew->xcpu.op[0].mode = OP_Register;
                    pnew->xcpu.op[0].opersize = 4;
                    pnew->xcpu.op[0].reg.reg_index = _EBP_;
                    pnew->xcpu.op[1].mode = OP_Register;
                    pnew->xcpu.op[1].opersize = 4;
                    pnew->xcpu.op[1].reg.reg_index = _ESP_;
                    m_asmlist->InsertBefore(pos, pnew);

                }
            }
        }
    }
}

void CFuncLL::Prepare_CallFunc()	// �ѱ�������call���ĺ�����prepareһ��
{
    POSITION pos = m_asmlist->GetHeadPosition();
    while (pos)
    {
        AsmCode* pasm = m_asmlist->GetNext(pos);
        XCPUCODE* pxcpu = &pasm->xcpu;
        if (pxcpu->opcode == C_CALL && pxcpu->op[0].mode == OP_Near)
        {
            CFunc* pfunc = g_Cexe2c->GetFunc(pxcpu->op[0].nearptr.offset);
            if (pfunc )
               pfunc->PrepareFunc();
        }
    }
}

// ���ú�����stack�Ƿ�ƽ�⣬��leave����������Զ�ջ��ƽ
// ��������ջ�ǲ�ƽ��
bool	CFuncLL::Check_Stack()	
{
	POSITION pos = this->m_asmlist->GetHeadPosition();
	signed int lastesp = 0;	

	while (pos)
	{
		AsmCode* p = this->m_asmlist->GetNext(pos);
		signed int esp = p->esp_level;
		if (esp == ESP_UNKNOWN)		// ��ջλ�ò���Ϊδ֪
			return false;

		if (lastesp != ESP_UNKNOWN 	// ��ջλ�ò���Ϊδ֪
			&& lastesp != ESP_IGNORE 
			&& esp != lastesp
			)
			return false;

        if (p->xcpu.IsJxx() || p->xcpu.IsJmpNear())
		{
			ea_t jmpto = p->xcpu.op[0].nearptr.offset;
			AsmCode* p = ea2pasm(jmpto);
			if (esp != p->esp_level)
				return false;
		}
		if (p->xcpu.opcode == C_RET)
		{
			if (esp != 0)
				return false;
		}
		lastesp = p->esp_level_next;
	}
	return true;
}

bool	CFuncLL::Fill_Stack_Info()	// �����ջλ��
{
	POSITION pos = m_asmlist->GetHeadPosition();
	AsmCode* pasm = 0;
	if (pos)
		pasm = m_asmlist->GetNext(pos);// ��ǰ���
	else
		return false;

	pasm->esp_level = 0;		// ��ʼ��һ�еĶ�ջλ��Ϊ0

	int i;
	while (pos)
	{
		AsmCode* p0 = pasm;					// ��ǰ���
		pasm = m_asmlist->GetNext(pos);		// ��һ�����

		i=0;
		while(stack_stack(p0,pasm) && i<8)	// ѭ�����ã�����stackλ��
			;
	}

	//Fill_Stack_1();	
	// ��leave������������mov esp,ebp ����������Զ�ջ��ƽ��,���Բ������
	//if (! Check_Stack())
	//{
	//	log_prtl("�����ջƽ��ʧ��");
	//}

    return true;
}

int CFuncLL::Get_Ret_Purge()	// return -1 for fail
{
	int retn = -1;
	POSITION pos = this->m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* pasm = this->m_asmlist->GetNext(pos);
		XCPUCODE* pxcpu = &pasm->xcpu;
		if (pxcpu->opcode == C_RET)		//�ҵ���ret���
		{
			int r = 0;
			if (pxcpu->op[0].mode == OP_Immed)	// means RET n
			{
				r = pxcpu->op[0].immed.immed_value;
			}

			if (retn == -1)
				retn = r;
			else if (retn != r) //����Ret��һ�£�
				return -1;
		}
	}
	if (retn == -1)
	{
		log_prtl("����û�� Ret ָ��");
		return 0;	// û�о�Ĭ��Ϊret 0;
	}
    return retn;
}

CString CFuncLL::GetLabelName(ea_t ea)
{
    CString retn;

    CFunc* pfunc = g_Cexe2c->GetFunc(ea);
    if (pfunc != NULL)
    {
        retn = pfunc->m_funcname;
        return retn;
    }
    AsmCode* pasm = this->ea2pasm(ea);
    if (pasm != NULL && pasm->h.label.ref_j != NULL)
    {
        retn = pasm->h.label.label_name;
        return retn;
    }
    return retn;
}

void	CFuncLL::prtout_asm(CFunc* pfunc, CVarLL* pvarll, CXmlOutPro* out)
{
    out->XMLbegin(XT_Function, pfunc);

    out->XMLbegin(XT_FuncName, pfunc);
    out->prtt(pfunc->m_funcname);
    out->XMLend(XT_FuncName);

    out->prtspace();
    out->prtt("proc");
    out->endline();

    pvarll->prtout(out);

    out->endline();
    
    this->prtout_asm_1(pvarll, out);

    out->XMLbegin(XT_FuncName, pfunc);		// ��β�ĺ�������
    out->prtt(pfunc->m_funcname);
    out->XMLend(XT_FuncName);

    out->prtspace();
    out->prtt("endp");
    out->endline();

    out->XMLend(XT_Function);
}

void	CFuncLL::prtout_asm_1(CVarLL* pvarll, CXmlOutPro* out)
{   //	�����д�����ASM��ʾfunc�������е�GAP��ָ����

	ea_t last = 0xffffffff;
	POSITION pos = this->m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* pasm = this->m_asmlist->GetNext(pos);
		if (pasm->iAddRemove == 2)
			continue;
		ea_t ea = pasm->linear;

		char	buf[280];
		DWORD n;
		if (pasm->xcpu.opcode == C_JCASE)
		{
			n = 0;
			sprintf(buf,"case jmp to %x",pasm->xcpu.op[0].nearptr.offset);
		}
		else
        {
            st_IDA_OUT idaout;
            CDisasm the;
            //n = the.Disassembler(buf, ea2ptr(ea), ea);
            n = the.Disassembler_X(ea2ptr(ea), ea, &idaout);
            XCPUCODE* pxcpu = the.get_xcpu();
            if (pxcpu->op[0].mode == OP_Near)
            {
                ea_t linear = pxcpu->op[0].nearptr.offset;
                CString labelname = this->GetLabelName(linear);
                if (!labelname.IsEmpty())
                {
                    idaout.Par1Str = labelname;
                }
            }
            else if (pxcpu->op[0].mode == OP_Address)
            {
                OPERITEM* op = &pxcpu->op[0];
                if (op->addr.base_reg_index == _ESP_
                    || (op->addr.base_reg_index == _NOREG_
                        && op->addr.off_reg_index == _ESP_
                        && op->addr.off_reg_scale == 1))
                {
                    signed int level = pasm->esp_level + op->addr.off_value;
                    st_VarLL* p = pvarll->LookUp_VarLL(level- pvarll->m_VarRange_L);
                    if (p != NULL)
                    {
                        idaout.Par1Str += '.';
                        idaout.Par1Str += p->Name;
                    }
                }
            }
            else if (pxcpu->op[1].mode == OP_Address)
            {
                OPERITEM* op = &pxcpu->op[1];
                if (op->addr.base_reg_index == _ESP_
                    || (op->addr.base_reg_index == _NOREG_
                        && op->addr.off_reg_index == _ESP_
                        && op->addr.off_reg_scale == 1))
                {
                    signed int level = pasm->esp_level + op->addr.off_value;
                    st_VarLL* p = pvarll->LookUp_VarLL(level- pvarll->m_VarRange_L);
                    if (p != NULL)
                    {
                        idaout.Par2Str += '.';
                        idaout.Par2Str += p->Name;
                    }
                }
            }
            idaout.output(buf);
        }
		if (last != 0xffffffff && ea != last)
			out->prtl("//      gap here");
        
        if (pasm->h.label.ref_j != NULL)
        {
            //asm_prtl("%s:", pasm->h.label.label_name);
            out->prtf("%s:", pasm->h.label.label_name);
            out->endline();
        }

        //asm_prtl("%4x %x %s",-pasm->esp_level, ea, buf);
        if (pasm->esp_level == ESP_UNKNOWN)
        {
            out->prtt("    ");  //�ĸ��ո��λ��Ҫ��
        }
        else
            out->prtf("%4x", -pasm->esp_level);

        out->prtspace();

        out->XMLbegin(XT_AsmOffset, (void*)ea);
        out->prtf("%x", ea);
        out->XMLend(XT_AsmOffset);

        out->prtt(buf);
        out->endline();

		last = ea+n;
	}
}

/*
���һ��������ͷ�ǣ�
  0 401010 SUB    ESP,00000190
190 401016 LEA    ECX,[ESP+00]    
�� VarRange_L = -190h
�� VarRange_H = 0
д��
  0 401010 SUB    ESP,00000190
190 401016 LEA    ECX,[ESP+v_00]    
*/
// ��ѯ��������ķ�Χ
void CFuncLL::GetVarRange(signed int& VarRange_L, signed int& VarRange_H)
{
    signed int L = 0;
    signed int H = 0;

	POSITION pos = this->m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* pasm = this->m_asmlist->GetNext(pos);

        signed int last = pasm->esp_level;
        signed int here = pasm->esp_level_next;
        if (pasm->xcpu.opcode == C_SUB		// ����һ����ͨ��sub ��add �ڶ�ջ������ռ�
		 || pasm->xcpu.opcode == C_ADD
		 )
        {
            if (last - here > H - L)
            {
                H = last;
                L = here;
            }
        }
    }
    if (H - L > 0)
    {
        VarRange_H = H;
        VarRange_L = L;
    }
}


void CVarLL::prtout(CXmlOutPro* out)
{
    int curlevel = 0;
    int maxlevel = this->m_VarRange_H - this->m_VarRange_L;

    POSITION pos = this->m_varll_list.GetHeadPosition();
    while (pos)
    {
        st_VarLL* p = this->m_varll_list.GetNext(pos);
        if (curlevel > p->off)
        {
            out->prtl("error, var collapse!!!");
            curlevel = p->off;
        }
        else if (curlevel < p->off)
        {
            out->prtl("gap len = %x", p->off - curlevel);
            curlevel = p->off;
        }
        /*asm_prtl("%s equ %s %x", p->Name, 
                 (p->size == 1) ? "BYTE ptr" :
                 (p->size == 2) ? "WORD ptr" :
                 (p->size == 4) ? "DWORD ptr" : "",
                 p->off); */

        out->prtspace(4);
        out->XMLbegin(XT_Symbol, p);
        out->prtt(p->Name);
        out->XMLend(XT_Symbol);
        out->prtt("equ");
        out->prtspace();
        out->prtt((p->size == 1) ? "BYTE ptr" :
                          (p->size == 2) ? "WORD ptr" :
                          (p->size == 4) ? "DWORD ptr" : "");
        out->prtspace();
        if (p->array != 1)
        {
            out->XMLbegin(XT_Number, NULL);
            out->prtf("%xh", p->array);
            out->XMLend(XT_Number);
            out->prtt("dup");
            out->prtspace();
        }
        out->XMLbegin(XT_Number, NULL);
        out->prtf("%xh", p->off);
        out->XMLend(XT_Number);
        out->endline();

        curlevel += p->size * p->array;
    }
    
    if (curlevel < maxlevel)
    {
        out->prtl("    gap len = %xh", maxlevel - curlevel);
    }
}
st_VarLL* CVarLL::LookUp_VarLL(int off)
{
    POSITION pos = this->m_varll_list.GetHeadPosition();
    while (pos)
    {
        st_VarLL* p = this->m_varll_list.GetNext(pos);
        if (p->off == off)
            return p;
    }
    return NULL;
}
void CVarLL::AddRef(signed int level, int opersize)
{
    if (level < this->m_VarRange_H && level >= this->m_VarRange_L)
    {
    }
    else
        return;
    int off = level - this->m_VarRange_L; //�����>=0��
    st_VarLL* pnew = this->LookUp_VarLL(off);
    if (pnew != NULL)
        return;
    pnew = new st_VarLL;
    pnew->off = off;
    pnew->size = opersize;
    sprintf(pnew->Name, "v_%x", off);

    if (this->m_varll_list.IsEmpty())
    {
        this->m_varll_list.AddTail(pnew);
    }
    else
    {//�����б�
        POSITION pos = this->m_varll_list.GetHeadPosition();
        while (pos)
        {
            POSITION savpos = pos;
            st_VarLL* p = this->m_varll_list.GetNext(pos);
            if (p->off > off)
            {
                this->m_varll_list.InsertBefore(savpos, pnew);
                return;
            }
        }
        this->m_varll_list.AddTail(pnew);
    }

}
void CFuncLL::VarLL_Analysis_1(CVarLL* pvarll, OPERITEM* op, AsmCode* pasm)
{
    if (op->mode != OP_Address)
        return;
	if (op->addr.base_reg_index == _ESP_
        || (op->addr.base_reg_index == _NOREG_
                        && op->addr.off_reg_index == _ESP_
                        && op->addr.off_reg_scale == 1))
    {
        signed int level = pasm->esp_level + op->addr.off_value;
        pvarll->AddRef(level, op->opersize);
    }
	if (op->addr.base_reg_index == _EBP_)
    {
        //��ôд��
    }
}
void CFuncLL::VarLL_Analysis(CVarLL* pvarll)
{
    POSITION pos = this->m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* pasm = this->m_asmlist->GetNext(pos);
        if (pasm->xcpu.op[0].mode == OP_Address)
            this->VarLL_Analysis_1(pvarll, &pasm->xcpu.op[0], pasm);
        if (pasm->xcpu.op[1].mode == OP_Address)
            this->VarLL_Analysis_1(pvarll, &pasm->xcpu.op[1], pasm);
    }
}

AsmCode* ea2pasm(ea_t ea, AsmCodeList* m_asmlist)	// 
{
	POSITION pos = m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* p = m_asmlist->GetNext(pos);
		if (p->linear == ea)
			return p;
	}
	assert_msg(0,"ȡָ����ַ�ķ������Ϣ����");
	return NULL;
}

void CJxxLabel::Label_Analysis()
{
    POSITION pos = this->m_asmlist->GetHeadPosition();
    while (pos)
    {
		AsmCode* p = this->m_asmlist->GetNext(pos);
        assert(p->h.type == i_Nothing);				// ��ǰû��h������

        if (p->xcpu.IsJxx() || p->xcpu.IsJmpNear())
		{
			ea_t jmpto = p->xcpu.op[0].nearptr.offset;
			AsmCode* plabel = ea2pasm(jmpto, m_asmlist);

            p->h.type = i_Jump;
            if (p->xcpu.IsJmpNear())
                p->h.jmp.jmp_type = JMP_jmp;
            p->h.jmp.the_label = plabel;

            if (plabel->h.label.ref_j == NULL)
            {
                plabel->h.label.ref_j = p;
                sprintf(plabel->h.label.label_name, "loc_%x", plabel->linear);
            }
            else
            {
                p->h.jmp.next_ref_of_this_label = plabel->h.label.ref_j;
                plabel->h.label.ref_j = p;
            }
		}
    }
}
