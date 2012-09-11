// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
#include "CISC.H"
#include "cexe2c.h"

#include "CCodeList.h"

int g_hxd_instr;

// ��������instr_list��expr_list
void CCodeList::CreateInstrList_raw(INSTR_LIST* instr_list, AsmCodeList* asmlist, int EBP_base)
{
    this->m_instr_list = instr_list;
    this->m_asmlist = asmlist;
    this->m_EBP_base = EBP_base;

	PINSTR p_begin = new INSTR;     // new_INSTR
	p_begin->type = i_Begin;

	PINSTR p_end = new INSTR;		// new_INSTR	����ͬʱnew
	p_end->type = i_End;

	p_begin->begin.m_end = p_end;

	InstrAddTail(p_begin);	// �ڿ�ʼ�����i_Begin ��ʶ

    signed int esp_level = 3;

	g_hxd_instr = 0;		// ��ʼ

	POSITION pos = this->m_asmlist->GetHeadPosition();
	while (pos)
	{
		AsmCode* cur = this->m_asmlist->GetNext(pos);
        assert(cur);
		assert(cur->linear);

        CCodeList_Maker the(this,cur);	// ��ʼ��CCodeList_Maker
		
		//	��ÿ��ָ��ǰ��һ��label,Ψһ�����⣬C_JCASEǰ����label
        if (cur->xcpu.opcode != C_JCASE)
		{	
            PINSTR p = new INSTR;   //new_INSTR
            p->type = i_Label;
            p->label.label_off = cur->linear;
            InstrAddTail(p);
		}
		// ��ӳ�ջ���
        if (esp_level != 3 
			&& esp_level < cur->esp_level
			)
        {
            //cur->xcpu.opcode;
            PINSTR	p = new INSTR;  //new_INSTR
            p->type = i_EspReport;
            p->espreport.esp_level = cur->esp_level;
            p->espreport.howlen = cur->esp_level - esp_level;

            InstrAddTail(p);
        }
		

		the.AddTail_Cur_Opcode();		// �ѵ�ǰ��xcpuת��Ϊα�룬ͬʱ��intr_list��addtail

        esp_level = cur->esp_level;
    }

	// ������û��ret ָ��,���һ��i_Return
	AsmCode* Last=this->m_asmlist->GetTail();

	if(Last->xcpu.opcode != C_RET)
	{
		PINSTR	p = new INSTR;  // new_INSTR
		p->type = i_Return;
		InstrAddTail(p);
	}
	
	InstrAddTail(p_end);	//	�ں����һ��i_End ��ʶ
}

void CCodeList::InstrAddTail(PINSTR p)
{
	/*
	if (p->var_w.type)
    	if (p->var_w.opsize == 0)
			nop();//assert(p->var_w.opsize);
	if (p->var_r1.type)
    	if (p->var_r1.opsize == 0)
			nop();//assert(p->var_r1.opsize);
	if (p->var_r2.type)
    	if (p->var_r2.opsize == 0)
			nop();//assert(p->var_r2.opsize);
	*/

	p->id= g_hxd_instr++;
	this->m_instr_list->AddTail(p);
}

void	set_address(OPERITEM* op,PINSTR p)	// �����Ϣ����[eax + ebx*4 + 8888]
{
	if (op->addr.base_reg_index != _NOREG_)	// �Ĵ�������Ӧ��Ϊ eax
	{
		p->var_r1.type = v_Reg;
		p->var_r1.opsize = BIT32_is_4;
		p->var_r1.reg = regindex_2_regoff(op->addr.base_reg_index);
	}

	if (op->addr.off_reg_index != _NOREG_) // �Ĵ��� ����Ӧ��Ϊ ebx
	{
		p->var_r2.type = v_Reg;
		p->var_r2.opsize = BIT32_is_4;
		p->var_r2.reg = regindex_2_regoff(op->addr.off_reg_index);
	}

	p->i1 = op->addr.off_reg_scale;			// ��Ӧ��Ϊ 4
	p->i2 = op->addr.off_value;				// ��Ӧ��Ϊ 8888
}
//-------------------------------------------------------
#define m_instr_list Q->m_instr_list
#define InstrAddTail Q->InstrAddTail
#define m_EBP_base Q->m_EBP_base

ea_t FindApiAddress_Reg(DWORD regindex, XCPUCODE* pxcpu1, AsmCodeList* asmlist);

//	�ѵ�ǰ��xcpuת��Ϊα�룬ͬʱ��intr_list��addtail
void	CCodeList_Maker::AddTail_Cur_Opcode()
{
    XCPUCODE* pxcpu = &cur->xcpu;

	switch (pxcpu->opcode)
	{
		// �߼�ָ���
	case C_NOT:		
		Code_general(enum_A, i_Not); 
		// ��Ӱ����ת
		break;
	case C_AND:
		{
			PINSTR	p = new INSTR;
			p->type = i_Assign;
			
			TransVar(&p->var_r1, 0);	//	0 means	xcpu.op[0]
			TransVar(&p->var_r2, 1);	//	1 means	xcpu.op[1]

			if ( pxcpu->op[0].mode == OP_Register
              && pxcpu->op[1].mode == OP_Register
			  && VAR::IsSame(&p->var_r1,&p->var_r2)
			  )	//	and eax,eax �����Լ����Լ���ֵ
			{	
				OPERITEM* op0 = &pxcpu->op[0];
				OPERITEM* op1 = &pxcpu->op[1];
				op1->mode = OP_Immed;
				op1->opersize = op0->opersize;
				op1->immed.immed_value = 0;
				Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
			}
			else
			{
				PINSTR p = Code_general(enum_AR, i_And);;   // һ��αָ��

				// ����Ƿ�Ϊ 0 �ıȽ�ָ��
				OPERITEM* op0 = &pxcpu->op[0];
				OPERITEM* op1 = &pxcpu->op[1];
				op1->mode = OP_Immed;
				op1->opersize = op0->opersize;
				op1->immed.immed_value = 0;
				Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
			
			}
			break;
		}
		
	case C_OR: 
		{
			//PINSTR p = Code_general(enum_AR, i_Or);;   // һ��αָ��

			PINSTR	p = new INSTR;
			p->type = i_Assign;
			
			TransVar(&p->var_r1, 0);	//	0 means	xcpu.op[0]
			TransVar(&p->var_r2, 1);	//	1 means	xcpu.op[1]

			if ( pxcpu->op[0].mode == OP_Register
              && pxcpu->op[1].mode == OP_Register
			  && VAR::IsSame(&p->var_r1,&p->var_r2)
			  )	//	or eax,eax �����Լ����Լ���ֵ
			{	
				OPERITEM* op0 = &pxcpu->op[0];
				OPERITEM* op1 = &pxcpu->op[1];
				op1->mode = OP_Immed;
				op1->opersize = op0->opersize;
				op1->immed.immed_value = 0;
				Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
			}
			else
			{
				PINSTR p = Code_general(enum_AR, i_Or);;   // һ��αָ��

				// ����Ƿ�Ϊ 0 �ıȽ�ָ��
				OPERITEM* op0 = &pxcpu->op[0];
				OPERITEM* op1 = &pxcpu->op[1];
				op1->mode = OP_Immed;
				op1->opersize = op0->opersize;
				op1->immed.immed_value = 0;
				Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
			
			}

			break;
		}

	case C_XOR: 
		{
			PINSTR p = new INSTR;   // һ��αָ��

			TransVar(&p->var_r1, 0);	//	0 means	xcpu.op[0]
			TransVar(&p->var_r2, 1);	//	0 means	xcpu.op[0]

			if ( pxcpu->op[0].mode == OP_Register
              && pxcpu->op[1].mode == OP_Register
			  && VAR::IsSame(&p->var_r1,&p->var_r2)
			  )	//	xor eax,eax means mov eax,0 �����������������
			{	
				PINSTR	p = new INSTR;
				p->type = i_Assign;

				TransVar(&p->var_w, 0);		//	0 means	xcpu.op[0]

				p->var_r1.type = v_Immed;	//  ����Ϊ������
				p->var_r1.opsize = BIT32_is_4;
				p->var_r1.d=0;				//  ����Ϊ0

				InstrAddTail(p);

			}
			else
			{
				Code_general(enum_AR, i_Xor);

				// ����Ƿ�Ϊ 0 �ıȽ�ָ��
				OPERITEM* op0 = &pxcpu->op[0];
				OPERITEM* op1 = &pxcpu->op[1];
				op1->mode = OP_Immed;
				op1->opersize = op0->opersize;
				op1->immed.immed_value = 0;
				Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
			
			}

			break;
		}

	case C_TEST:
		{
			VAR v;
			new_temp(&v);			// ���һ������

			PINSTR p = new INSTR;   // һ��αָ��
			p->type = i_And;
			p->var_w = v;			// Ҫд��һ����ʱ����������

			TransVar(&p->var_r1, 0);	//	0 means	xcpu.op[0]
			TransVar(&p->var_r2, 1);	//	0 means	xcpu.op[0]

			if ( pxcpu->op[0].mode == OP_Register
              && pxcpu->op[1].mode == OP_Register
			  && VAR::IsSame(&p->var_r1,&p->var_r2)
			  )	//	xor eax,eax means mov eax,0 �����������������
			{	
				OPERITEM* op0 = &pxcpu->op[0];
				OPERITEM* op1 = &pxcpu->op[1];
				op1->mode = OP_Immed;
				op1->opersize = op0->opersize;
				op1->immed.immed_value = 0;
				Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
			}
			else
			{
				InstrAddTail(p);

				// ������һ���Ƚϵ�αָ��
				PINSTR p2 = new INSTR;   // һ��αָ��

				p2->type = i_Cmp;

				p2->var_r1 =v;

				p2->var_r2.type = v_Immed;
				p2->var_r2.opsize = 4;
				p2->var_r2.d=0;

				InstrAddTail(p2);

			}

			break;
		}

		// �߼�ָ��� end

    case C_INC:
        {
            OPERITEM* op0 = &pxcpu->op[0];	// ��һ��������
            OPERITEM* op1 = &pxcpu->op[1];

            op1->mode = OP_Immed;
            op1->opersize = op0->opersize;
            op1->immed.immed_value = 1;
        }

		Code_general(enum_AR, i_Add);    //2005.2.1�ӣ�������ʱһ����1��Add ָ���
        break;
    case C_DEC:
        {
            OPERITEM* op0 = &pxcpu->op[0];
            OPERITEM* op1 = &pxcpu->op[1];
//            OPERITEM* op2 = &this->cur->xcpu.op[2];
            op1->mode = OP_Immed;
            op1->opersize = op0->opersize;
            op1->immed.immed_value = 1;
        }

		Code_general(enum_AR, i_Sub);    //2005.2.1�ӣ���������ʱһ����1��SUB ָ���
        {	// ����Ƿ�Ϊ 0 �ıȽ�ָ��
            OPERITEM* op0 = &pxcpu->op[0];
            OPERITEM* op1 = &pxcpu->op[1];
            op1->mode = OP_Immed;
            op1->opersize = op0->opersize;
            op1->immed.immed_value = 0;
            Code_general(enum_RR, i_Cmp);	// ���һ��op0�Ƿ�Ϊ��ıȽ�ָ��
        }
        break;
	case C_JCASE:
		{
			//alert("C_JCASE find ");
			PINSTR p = new INSTR;   //new_INSTR
			p->type = i_Jump;
			p->jmp.jmp_type = JMP_case;
			p->jmp.jmpto_off = pxcpu->op[0].nearptr.offset;

			PINSTR plast = m_instr_list->GetTail();	//	���ǿ�����ǰһ����ʲô
			if (plast->type == i_JmpAddr)
			{	//	means this is case 0
				p->var_r1 = plast->var_r2;	// index reg
				p->var_r2.type = v_Immed;
				p->var_r2.d = 0;
			}
			else if (plast->type == i_Jump && plast->jmp.jmp_type == JMP_case)
			{
				p->var_r1 = plast->var_r1;
				p->var_r2.type = v_Immed;
				p->var_r2.d = plast->var_r2.d + 1;	//	next case
			}
			else
			{
				alert_prtf("type is %s", hlcode_name(plast->type));
				assert(0);
			}
			InstrAddTail(p);
		}
		break;

	case C_LEA:
		Code_general(enum_WR, i_Lea);
		break;
	case C_MOV:
		Code_general(enum_WR, i_Assign);
		break;
	case C_MOVZX:
		Code_general(enum_WR, i_NosignExpand);
		break;
	case C_MOVSX:
		Code_general(enum_WR, i_SignExpand);
		break;
	
	case C_ADD: Code_general(enum_AR, i_Add);    break;
	case C_IMUL: Code_general(enum_AR, i_Imul);    break;
	case C_SUB: 
        Code_general(enum_AR, i_Sub);    
		// ���һ���Ƚ�����
        {
            OPERITEM* op0 = &pxcpu->op[0];
            OPERITEM* op1 = &pxcpu->op[1];
//            OPERITEM* op2 = &this->cur->xcpu.op[2];
            op1->mode = OP_Immed;
            op1->opersize = op0->opersize;
            op1->immed.immed_value = 0;
            Code_general(enum_RR, i_Cmp);
        }
        break;
	case C_ROL: Code_general(enum_AR, i_Rol);    break;
	case C_ROR: Code_general(enum_AR, i_Ror);    break;

	case C_SAR: Code_general(enum_AR, i_Sar);    break;
	case C_SHL: Code_general(enum_AR, i_Shl);    break;
	case C_SHR: Code_general(enum_AR, i_Shr);    break;

	case C_CMP:
		Code_general(enum_RR, i_Cmp);
		break;
	case C_PUSH:
		{
			PINSTR	p = new INSTR;
			p->type = i_Assign;

			p->var_w.type = v_Var;
			p->var_w.opsize = BIT32_is_4;
			p->var_w.var_off = stack2varoff(cur->esp_level - 4);	// or esp_level_next

			TransVar(&p->var_r1, 0);	//	0 means	xcpu.op[0]
			InstrAddTail(p);
		}
		break;
	case C_POP:
		{
			PINSTR	p = new INSTR;  //new_INSTR
			p->type = i_Assign;

			p->var_r1.type = v_Var;
			p->var_r1.opsize = BIT32_is_4;
			p->var_r1.var_off = stack2varoff(cur->esp_level);

			TransVar(&p->var_w, 0);	//	0 means	xcpu.op[0]
			InstrAddTail(p);
		}
		break;
	case C_ENTER:	// ���ڶ�ջƽ�⣬����ָ��
		break;	
	case C_LEAVE:	// ���ڶ�ջƽ�⣬����ָ��
		break;

	case C_JO: 	Code_Jxx(JMP_jo);	break;
	case C_JNO: Code_Jxx(JMP_jno);	break;
	case C_JB: 	Code_Jxx(JMP_jb);	break;
	case C_JNB: Code_Jxx(JMP_jnb);	break;
	case C_JZ:	Code_Jxx(JMP_jz);	break;
	case C_JNZ: Code_Jxx(JMP_jnz);	break;
	case C_JNA: Code_Jxx(JMP_jna);	break;
	case C_JA:	Code_Jxx(JMP_ja);	break;
	case C_JS: 	Code_Jxx(JMP_js);	break;
	case C_JNS:	Code_Jxx(JMP_jns);	break;
	case C_JP: 	Code_Jxx(JMP_jp);	break;
	case C_JNP: Code_Jxx(JMP_jnp);	break;
	case C_JL: 	Code_Jxx(JMP_jl);	break;
	case C_JNL: Code_Jxx(JMP_jnl);	break;
	case C_JLE:	Code_Jxx(JMP_jle);	break;
	case C_JNLE:Code_Jxx(JMP_jnle);	break;
	case C_JMP:
		if (pxcpu->op[0].mode == OP_Near)
			Code_Jxx(JMP_jmp);
		else
		{
			if (pxcpu->op[0].mode == OP_Address)
			{
				PINSTR p = new INSTR;   //new_INSTR
				p->type = i_JmpAddr;	//	����˵�����Ȱ����jmp [edx*4+402000]������
				//	������������ C_JCASE

				void	set_address(OPERITEM* op,PINSTR p);

				set_address(&pxcpu->op[0], p);

				InstrAddTail(p);
			}
		}
		break;
	case C_CALL:
		if (pxcpu->op[0].mode == OP_Near)
		{
			PINSTR p = new INSTR;  //new_INSTR
			p->type = i_Call;
			p->call.esp_level = cur->esp_level;
			p->call.call_func = g_Cexe2c->GetFunc(pxcpu->op[0].nearptr.offset);
            p->call.p_callpara = NULL;
            p->call.p_callret = NULL;
			InstrAddTail(p);
		}
        else if (pxcpu->op[0].mode == OP_Address)
		{
			if (pxcpu->op[0].addr.base_reg_index == _NOREG_ &&
				pxcpu->op[0].addr.off_reg_index == _NOREG_)
			{
				ea_t address = pxcpu->op[0].addr.off_value;
				CApi* papi = g_ApiManage->get_api(address);	//find it
				if (papi)
				{
					PINSTR	p = new INSTR;  //new_INSTR
					p->type = i_CallApi;
					p->call.papi = papi;
					p->call.esp_level = cur->esp_level;
                    p->call.p_callpara = NULL;
                    p->call.p_callret = NULL;
					InstrAddTail(p);
				}
				else
				{
					//Code_general(0, i_Unknown);

					PBYTE temp01=ea2ptr(address);
					ea_t addr= *(DWORD*)temp01;

					PINSTR p = new INSTR;  //new_INSTR
					p->type = i_Call;
					p->call.esp_level = cur->esp_level;
					p->call.call_func = g_Cexe2c->GetFunc(addr);
					p->call.p_callpara = NULL;
					p->call.p_callret = NULL;
					InstrAddTail(p);
				}
			}
			else if (pxcpu->op[0].addr.base_reg_index != _NOREG_
				&& pxcpu->op[0].addr.off_reg_index == _NOREG_)
			{	// CALL [ESI]
				ea_t address = FindApiAddress_Reg(pxcpu->op[0].addr.base_reg_index, pxcpu, this->Q->m_asmlist);

				if(address<0x10000)
				{
					Code_general(0, i_Unknown);
					break;			// û���ҵ�
				}
				CApi* papi = g_ApiManage->get_api(address);	//find it
				if (papi)
				{
					PINSTR	p = new INSTR;  //new_INSTR
					p->type = i_CallApi;
					p->call.papi = papi;
					p->call.esp_level = cur->esp_level;
					p->call.p_callpara = NULL;
					p->call.p_callret = NULL;
					InstrAddTail(p);
				}
				else // ����API����
				{
					PINSTR p = new INSTR;  //new_INSTR
					p->type = i_Call;
					p->call.esp_level = cur->esp_level;

					if(g_Cexe2c->GetFunc(address))
						p->call.call_func = g_Cexe2c->GetFunc(address);
					else
						p->call.call_func = g_Cexe2c->func_new(address);
					p->call.p_callpara = NULL;
					p->call.p_callret = NULL;
					InstrAddTail(p);
				}
			}
			else
				Code_general(0, i_Unknown);
		}
        else if (pxcpu->op[0].mode == OP_Register)
        {
            ea_t address = FindApiAddress_Reg(pxcpu->op[0].reg.reg_index, pxcpu, this->Q->m_asmlist);
            CApi* papi = g_ApiManage->get_api(address);	//find it
            if (papi)
            {
                PINSTR	p = new INSTR;  //new_INSTR
                p->type = i_CallApi;
                p->call.papi = papi;
                p->call.esp_level = cur->esp_level;
                p->call.p_callpara = NULL;
                p->call.p_callret = NULL;
                InstrAddTail(p);
            }
            else
                Code_general(0, i_Unknown);
        }
        else
            Code_general(0, i_Unknown);
		break;
	case C_RET:	
		Code_general(0, i_Return);	
		break;

	case C_RDTSC:
		break;

	case C_NOP:    
		break;

	case C_LODSB:  
		break;

	case C_SETNZ:	
		Code_general(enum_W, i_Setnz);
		break;

	default:	
		Code_general(0, i_Unknown);	break;

	}
}

void	CCodeList_Maker::Code_Jxx(JxxType t)
{
	PINSTR p = new INSTR;   // new_INSTR
	p->type = i_Jump;
	p->jmp.jmp_type = t;	// ��ת������
	p->jmp.jmpto_off = cur->xcpu.op[0].nearptr.offset;
	p->jmp.linear = cur->linear ;
	InstrAddTail(p);
}

//	ֻ��type == enum_RRʱ������ֵ������
PINSTR	CCodeList_Maker::Code_general(int type, HLType t)
{	
	PINSTR	p = new INSTR;  // new_INSTR
	p->type = t;			// αָ�����ͣ���i_Assign��i_Add

	switch (type)
	{
	case enum_00:	// û�в�������αָ��
		InstrAddTail(p);
		return p;
	case enum_RR:	// �������������Ƕ�����
		{
			TransVar(&p->var_r1, 0);	//	0 means	xcpu.op[0]
			TransVar(&p->var_r2, 1);	//	1 means	xcpu.op[1]
			VarRead(p->va_r1);
			VarRead(p->va_r2);
		}
		InstrAddTail(p);
		return p;
	case enum_WR:	// ��һ��д����
		{
			TransVar(&p->var_w, 0);		//	0 means	xcpu.op[0]
			TransVar(&p->var_r1, 1);	//	1 means	xcpu.op[1]
			if (t == i_Lea)
			{
				p->type = i_Assign;
				if (p->var_r1.type != v_Tem)
				{	//	������ lea eax,[ebp]
					p->type = i_GetAddr;
				}
			}
			else
			{
				VarRead(p->va_r1);
				//VarWrite(&p->var_w);
				if (p->var_w.type == v_Tem)
				{
					WriteToAddress(p);
					return p;
				}
			}
		}
		InstrAddTail(p);
		return p;
	case enum_AR:	// ��һ���������ȶ�Ҳд��������ֻ��ȡ
		{
			VAR v;
			TransVar(&v, 0);
			TransVar(&p->var_r2, 1);
			p->var_r1 = v;
			p->var_w = v;
			VarRead(p->va_r2);
			VarRead(p->va_r1);
			//VarWrite(&p->var_w);
			if (p->var_w.type == v_Tem)
			{
				WriteToAddress(p);
				return p;
			}
		}
		InstrAddTail(p);
		return p;
	case enum_A:	 // һ��������������Ҳд���� notָ��
		{
			VAR v;
			TransVar(&v, 0);
			p->var_w = v;
			p->var_r1 = v;
			VarRead(p->va_r1);
			if (p->var_w.type == v_Tem)
			{
				WriteToAddress(p);
				return p;
			}
		}
		InstrAddTail(p);
		return p;
	case enum_W:	 // һ����������ֻдSetnz
		{
			VAR v;
			TransVar(&v, 0);
			p->var_w = v;
		}
		InstrAddTail(p);
		return p;
	default:
		alert("CCodeList_Maker::Code_general ����");	// û�ж�������ͱ���
		return NULL;
	}
	//return NULL;
}
void	CCodeList_Maker::TransVar(VAR* pvar,int no)
{
	TransVar_(pvar,no);
	if (pvar->type)
	{
		assert(pvar->opsize);
	}
}
// �ѵ�no��������תΪα������Ϣ
void	CCodeList_Maker::TransVar_(VAR* pvar,int no)
//SuperC_func: ֻ�ڣ�CCodeList_Maker::TransVar����ʹ��
{
	OPERITEM* op = &this->cur->xcpu.op[no];
	switch (op->mode)
	{
	case OP_Register:	// �Ĵ���
		pvar->type = v_Reg;
		pvar->opsize = op->opersize;
		pvar->reg = regindex_2_regoff(op->reg.reg_index);
		return;
	case OP_Immed:		// ������
		pvar->type = v_Immed;
		pvar->opsize = op->opersize;
		pvar->d = op->immed.immed_value;
		return;
	case OP_Address:	// []��ʽ
		if (op->addr.base_reg_index == _NOREG_
			&& op->addr.off_reg_index == _NOREG_)
		{
            if (op->addr.off_value == 0 && op->addr.seg_index == _FS_)
            {//�ж� fs:[0]
                pvar->type = v_Volatile;    //����ֻ���� fs:0
                pvar->opsize = op->opersize;
                pvar->temno = 222;			//ֻҪ��ż������
                return;
            }
			pvar->type = v_Global;
			pvar->opsize = op->opersize;
			pvar->off = op->addr.off_value;
			return;
		}
		if (op->addr.base_reg_index == _ESP_		// ��ַΪesp
			&& op->addr.off_reg_index == _NOREG_)	// ���ǼĴ���
		{
			pvar->opsize = op->opersize;
			signed long l = this->cur->esp_level + (signed int)op->addr.off_value;
			if (l >= 0)
			{
				pvar->par_off = l;
				pvar->type = v_Par;
			}
			else
			{
				pvar->var_off = stack2varoff(l);
				pvar->type = v_Var;
			}
			return;
		}
		if (op->addr.base_reg_index == _EBP_		// ��ַΪebp
			&& this->m_EBP_base != Not_EBP_based
			&& op->addr.off_reg_index == _NOREG_)	// ���ǼĴ���
		{
			pvar->opsize = op->opersize;
			signed long l = this->m_EBP_base + (signed int)op->addr.off_value;
			if (l >= 0)
			{
				pvar->par_off = l;
				pvar->type = v_Par;
			}
			else
			{
				pvar->var_off = stack2varoff(l);
				pvar->type = v_Var;
			}
			return;
		}
		//	��������ʽ���� [ebx+8]
		{
			VAR v;
			new_temp(&v);

			PINSTR p = new INSTR;   //new_INSTR
			p->type = i_Address;
			
			p->var_w = v;

			set_address(op, p);		// 

			InstrAddTail(p);

			*pvar = v;
		}
		return;
	default:
		//warn_msg(0,"op mode unknown");
		break;
	}
}

//	�����������˼�ǣ�������Ǹ�v_Tem,���һ�� i_Readpointto
void	CCodeList_Maker::VarRead(VAR_ADDON& va)
{	
    VAR* pvar = va.pv;
    Pst_InstrAddOn &pAddOn = va.pao;
	if (pvar->type != v_Tem)
		return;

    Pst_InstrAddOn pnew = new st_InstrAddOn;
    pnew->type = IA_ReadPointTo;
    pnew->pChild = pAddOn;
    pAddOn = pnew;

    return;
}

void	CCodeList_Maker::WriteToAddress(PINSTR p)
{	//	�����������˼�ǣ��������һ��addressд�����һ�� i_Writepointto
	
//	����add [ebx+4],6,��Ϊ
//		tem_1 = i_addr(ebx,4);
//		tem_2 = i_readpointto(tem_1);
//		tem_3 = tem_2 + 6;
//		i_writepointto(tem_1, tem_3);

	
	//	��ǰ������ǣ�
	//		tem1 addr eax,ebx*4,401000
	//		tem1 = ????
	//	Ҫ�ĳ�
	//		tem1 addr eax,ebx*4,401000
	//		tem2 = ????
	//		Writepointto(tem1, tem2);
	if (p->var_w.type != v_Tem)
	{	//	ʵ���ϲ���������
		InstrAddTail(p);
		return;
	}
	
	VAR tem1 = p->var_w;	//	sav it
	VAR tem2;

	new_temp(&tem2);

	p->var_w = tem2;
	InstrAddTail(p);	//	add this

	PINSTR pnew = new INSTR;    //new_INSTR
	pnew->type = i_Writepointto;
	pnew->var_r1 = tem1;				// the pointer
	pnew->var_r2 = tem2;					// the value
		//	�� i_Writepointto, ��var_r1��ָ�룬var_r2��ֵ
	InstrAddTail(pnew);
	
}

extern int g_newtemno;
void	CCodeList_Maker::new_temp(VAR* pvar)
{
    pvar->type = v_Tem;
    pvar->temno = g_newtemno;
    g_newtemno += 2;
	pvar->opsize = BIT32_is_4;	//	temp var always DWORD
}
