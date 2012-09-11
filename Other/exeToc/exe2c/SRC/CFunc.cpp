// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#include "stdafx.h"
#include "CISC.H"
#include "cexe2c.h"
#include "CFuncStep1.h"


void	fill_func_info( ea_t pos,CFunc* pfnc)		//	���� CFunc ��һЩ������Ϣ
{
	sprintf(pfnc->m_funcname,"sub_%x",pos);
}

void	Code_GetArgs(VAR* v,DWORD &maxesp)	
{
	if (v->type == v_Par)
	{
		if (v->par_off > maxesp)
			maxesp = v->par_off;
	}
}

bool	IfValideFuncName(PSTR pname)	// �ж��Ƿ�Ϊ��Ч�ĺ�������
{
	if (pname == NULL)
    	return false;
    if (*pname == 0 )
    	return false;

    char c = *pname;
    if ( c>='a' && c<='z' )
    	return true;
    if ( c>='A' && c<='Z' )
    	return true;
    if ( c=='_' || c=='~')
    	return true;
    return false;
}


VarTypeID GetMemDataType(VAR* pvar)
{
    /*
    assert(pvar);
    assert(pvar->thevar);
    assert(pvar->thevar->m_DataType);
    assert(pvar->thevar->m_DataType->m_type == vtt_class);
    */
    Class_st* pstruc = g_VarTypeManage->is_class(pvar->thevar->m_DataTypeID);
    assert(pstruc);

    if (pvar->part_flag == 0)
    {//ֻ��һ�ֿ��ܣ����ǽṹֻ��һ��Ԫ�أ�����ô��
        assert(pstruc->m_nDataItem == 1);
        return pstruc->GetClassItem(0)->m_vartypeid;
    }
    else
    {
        return pstruc->GetClassItem(pvar->part_flag-1)->m_vartypeid;
    }
}

#include "ParseHead.h"
CString GetToken(PSTR &p)
{
    CString s;
    while (*p != 0 && *p != ' ')
    {
        s += *p++;
    }
    while (*p == ' ')
        p++;
    return s;
}


bool VAR::IsSame(VAR* v1,VAR* v2)
{
	return (VarCompare(v1,v2) == 1);
}

int	VAR::VarCompare(VAR* v1,VAR* v2)
{
	//	0:	no relationship
	//	1:	same
	//	2:	v1 include v2
	//	3:	v2 include v1

	if (v1->type != v2->type)
		return 0;
	//if (v1->opsize != v2->opsize)
	//	return false;
	DWORD off1,off2;
	BYTE siz1 = v1->opsize;
	BYTE siz2 = v2->opsize;
	
	switch (v1->type)
	{
    case v_Volatile:
        return 1;   //same
	case v_Reg:
		off1 = v1->reg;
		off2 = v2->reg;
		break;
	case v_Immed:
    	off1 = v1->d;
        off2 = v2->d;
		break;
	case v_Global:
    	off1 = v1->off;
        off2 = v2->off;
		break;
	case v_Par:
    	off1 = v1->par_off;
        off2 = v2->par_off;
		break;
	case v_Var:
    	off1 = v1->var_off;
        off2 = v2->var_off;
		break;
	case v_Tem:
		if (v1->temno != v2->temno)
			return false;
        if (siz1 != siz2)
        	return false;
		return true;
	default:
		return false;
	}

	if (off1 > off2)
	{
		if (off2 + siz2 > off1)
		{
			return 3;	//	3:	v2 include v1
		}
	}
	else if (off1 < off2)
	{
		if (off1 + siz1 > off2)
		{
			return 2;	//	2:	v1 include v2
		}
	}
	else
	{	//	off1 == off2
		if (siz1 == siz2)
			return 1;	//	same
		else if (siz1 > siz2)
			return 2;	//	2:	v1 include v2
		else
			return 3;	//	3:	v2 include v1
	}
	return 0;
}

PINSTR CFunc_InstrList::skip_compl(PINSTR p)
{
    //p��һ��begin��������end��һ��
    assert(p->type == i_Begin);
    return instr_next_in_func(p->begin.m_end);
}


void	prt_partern(INSTR_LIST* list,PINSTR phead,PSTR partern_buf);


PINSTR	CFunc_InstrList::instr_next_in_func(PINSTR p)
{
	return instr_next(this->m_instr_list,p);
}

PINSTR	CFunc_InstrList::instr_prev_in_func(PINSTR p)
{
	return instr_prev(this->m_instr_list, p);
}

CFunc::CFunc(ea_t start)
{
    m_IfLibFunc = false;

    m_VarRange_L = 0;   // ����=-40h
    m_VarRange_H = 0;   // ����= 0

    m_head_off = start;

    m_nStep = STEP_Init;
	m_nStep_Two = STEP_Init;

    m_EBP_base = Not_EBP_based;   //invalid
    m_stack_purge = 0;

    this->ll.m_asmlist = new AsmCodeList;   //new_AsmCodeList
    this->m_exprs = new CExprManage;		//new_CExprManage
    
    m_functype = NULL;
}

CFunc::~CFunc()
{

	SAFE_DELETE(m_exprs);

	if (m_instr_list)
	{
		POSITION pos = m_instr_list->GetHeadPosition();
		while (pos)
		{
			POSITION savepos = pos;
			PINSTR p = m_instr_list->GetNext(pos);

			m_instr_list->RemoveAt(savepos);
			SAFE_DELETE(p);
		}
		SAFE_DELETE(m_instr_list);
	}

	if (ll.m_asmlist)
	{
		POSITION pos = ll.m_asmlist->GetHeadPosition();
		while (pos)
		{
			AsmCode* p = ll.m_asmlist->GetNext(pos);
			SAFE_DELETE(p);
		}
		SAFE_DELETE(ll.m_asmlist);
	}

    //delete m_instr_list;

    //if (m_exprs != NULL) 
	//	delete m_exprs;
	//if (ll.m_asmlist != NULL) 
	//	delete ll.m_asmlist;
}

UINT CFunc::GetVaryParaSize(POSITION pos)
{
    INSTR_LIST* list = this->m_instr_list;
    while (pos)
    {
		PINSTR pinstr = list->GetNext(pos);
        if (pinstr->type == i_EspReport)
        {
            return pinstr->espreport.howlen;
        }
    }
    return 0;
}

bool	CFunc::Func_FillCallParas()			// ��亯��������Ϣ
{
    INSTR_LIST* list = this->m_instr_list;
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
		PINSTR pinstr = list->GetNext(pos);
		if (pinstr->type == i_Call)			// �Ƿ�Ϊ��������
		{
            CFuncType* pfctype = pinstr->call.call_func->m_functype;

            if (pfctype != NULL && pfctype->m_class != NULL)
            {//����һ��ecx->func
                PINSTR p = new INSTR;   //new_INSTR
                p->type = i_CallThis;
                p->var_r1.type = v_Reg;
                p->var_r1.opsize = BIT32_is_4;
                p->var_r1.reg = enum_ECX;
                p->call_addon.p_thecall = pinstr;
                list->InsertBefore(pos, p); // ��i_Call�����i_CallPara
            }
            if (pfctype != NULL && pfctype->m_args != 0)
            {
                PINSTR p = new INSTR;   //new_INSTR
                p->type = i_CallPara;

                p->call_addon.p_thecall = pinstr;
                pinstr->call.p_callpara = p;

                p->var_r1.type = v_Var;
                UINT parasize = pfctype->para_total_size();
                if (pfctype->m_varpar)
                {
                    parasize = GetVaryParaSize(pos);
                }
                p->var_r1.opsize = parasize;
                p->var_r1.var_off = stack2varoff(pinstr->call.esp_level);

                list->InsertBefore(pos, p); //��i_Call�����i_CallPara
            }
            //��ô˵��call�ķ���ֵ��Ӱ��eax
            {//��ÿһ��call�����i_CallRet��û������ġ�����������û�з���ֵ��
                //���i_CallRet�϶��ᱻ�Ż���
				PINSTR p = new INSTR;   //new_INSTR
				p->type = i_CallRet;

                p->call_addon.p_thecall = pinstr;
                pinstr->call.p_callret = p;
				p->var_w.type = v_Reg;
				p->var_w.opsize = BIT32_is_4;
				p->var_w.reg = enum_EAX;

				list->InsertBefore(pos, p); //��i_Call�����i_CallRet
            }
		}
		else if (pinstr->type == i_CallApi)		// ��API����
		{
            CApi* papi = pinstr->call.papi;
            if (papi != NULL)
            {
                if (papi->m_functype->para_total_size() != 0)
                {
                    PINSTR p = new INSTR;   //new_INSTR
                    p->type = i_CallPara;

                    p->call_addon.p_thecall = pinstr;
                    pinstr->call.p_callpara = p;
                    p->var_r1.type = v_Var;
                    p->var_r1.opsize = papi->m_functype->para_total_size();
                    p->var_r1.var_off = stack2varoff(pinstr->call.esp_level);

                    list->InsertBefore(pos, p); //��i_Call����Ӷ��i_CallPara
                }

				//���ڷ���ֵ
  
                int n = GG_VarType_ID2Size(papi->m_functype->m_retdatatype_id);
                if (n == 4 || n == 2 || n == 1)
                {
                    PINSTR p = new INSTR;   //new_INSTR
                    p->type = i_CallRet;

                    p->call_addon.p_thecall = pinstr;
                    pinstr->call.p_callret = p;
                    p->var_w.type = v_Reg;
                    p->var_w.opsize = n;
                    p->var_w.reg = enum_EAX;

                    list->InsertBefore(pos, p); //��i_Call�����i_CallRet
                }
            }
		}
	}
	
	return true;
}

bool    CFunc::Step1_Disassembly()		// ����൱ǰ���������������ַ�����CALLָ���ĵ�ַΪ�º���
{
    bool bCreateNewFunc = true;
    if (this->m_IfLibFunc)				// ����ǿ⺯�����Ͳ�������
	{
		//log_prtl("�⺯�����账��");
        return false;
	}

    if (this->m_nStep != STEP_Init)		// ������Ǻ��������ĵ�һ�����˳�
        return false;

    CFuncStep1 the(this->ll.m_asmlist);

    if (the.Step_1(this->m_head_off))	// ����ຯ�������õ�����������ַ
    {
        this->m_end_off = the.Get_end_off();

        if (bCreateNewFunc)
            the.CreateNewFunc_if_CallNear();	// ��ȡ�����������У�CALLָ��ĺ�����Ϣ
        
        return true;
    }
    return false;
}

bool	CFunc::Step2_GetRetPurge()	// ����retָ��õ�retn��С
{
    if (this->m_IfLibFunc)			// ����ǿ⺯�����Ͳ�������
        return false;
	
    CFuncLL the(this->ll.m_asmlist);

    int retn = the.Get_Ret_Purge();
    if (retn == -1)		// û���ҵ�retָ��
        return false;
	else
	   m_stack_purge = retn;

	return true;
}

bool    CFunc::Step3_AddRemoveSomeInstr()	// ����_EH_prolog
{
    CFuncLL the(this->ll.m_asmlist);
    the.AddRemoveSomeInstr();
    return true;
}

// �����ջ��Ϣ�������ջ�б������������
bool	CFunc::Step4_FillStackInfo()
{
    m_EBP_base = Not_EBP_based;

    CFuncLL the(this->ll.m_asmlist);
    if (this->m_prepareTrue_analysisFalse == false)
    {
        the.Prepare_CallFunc();	// �ѱ�������call���ĺ�����prepareһ��
    }
    if (!the.Fill_Stack_Info())
    {
        return false;
    }
	
	// �����ջ�б������������
    m_EBP_base = the.Get_EBP_base();
    the.GetVarRange(this->m_VarRange_L, this->m_VarRange_H);

    if (this->m_VarRange_H - this->m_VarRange_L > 0)
    {
        this->m_varll.Init(this->m_VarRange_L, this->m_VarRange_H);
        the.VarLL_Analysis(&this->m_varll);
    }

    this->m_exprs->m_VarRange_H = this->m_VarRange_H;
    this->m_exprs->m_VarRange_L = this->m_VarRange_L;

	return true;
}

// �����ת���λ
bool CFunc::Step5_Label_Analysis()
{
    CJxxLabel the(this->ll.m_asmlist);
    the.Label_Analysis();

    return true;
}


bool	CFunc::Step7_GetArgs()	// ��ȡ��ǰ������������
{
	if (this->m_stack_purge != 0)
	{
		this->m_args = m_stack_purge / 4;
		return true;
	}
	DWORD maxesp = 0;
	POSITION pos = this->m_instr_list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = this->m_instr_list->GetNext(pos);
		Code_GetArgs(&p->var_w,maxesp);		//change maxesp if need
		Code_GetArgs(&p->var_r1,maxesp);
		Code_GetArgs(&p->var_r2,maxesp);
	}
	this->m_args = maxesp / 4;
	return true;
}

//	��phead��ָ��complex�У��ҵ���no��statement
PINSTR	CFunc::Get_no_Statement(PINSTR phead,int no)
{
    PINSTR p = phead;
	while (p)
	{
		p = instr_next(this->m_instr_list,p);
		if (p == NULL || p == phead->begin.m_end)
			return NULL;
		assert(p->type != i_CplxBegin);	//	�����������������
        if (p->type == i_Begin)
		{
			if ( no == 0 )
				return p;
			p = p->begin.m_end;
			no--;			//	��������һ��
		}

	}
	return NULL;
}

void CFunc::MakeDownInstr(void* hline)
{
    PINSTR p0 = (PINSTR)hline;
    
    INSTR_LIST* list = this->m_instr_list;
	POSITION pos = list->GetHeadPosition();
	while (pos)
	{
        POSITION savpos = pos;
		PINSTR pinstr = list->GetNext(pos);
        if (pinstr == p0 && pos != NULL)
        {
            list->RemoveAt(savpos);
            list->InsertAfter(pos, pinstr);
            return;
        }
    }
}
	

void CFunc::report_info()
{
    log_prtl("func name = %s", this->m_funcname);
    log_prtl("func linear address = %x", this->m_head_off);
    if (m_VarRange_L >= 0)
        log_prtl("func m_VarRange_L = %x", this->m_VarRange_L);
    else
        log_prtl("func m_VarRange_L = -%x", -this->m_VarRange_L);
    if (m_VarRange_H >= 0)
        log_prtl("func m_VarRange_H = %x", this->m_VarRange_H);
    else
        log_prtl("func m_VarRange_H = -%x", -this->m_VarRange_H);
}

void CFunc::DeleteUnusedVar()
{
	if (m_instr_list == NULL)
		return;
    this->m_exprs->ClearUse();
	assert(m_instr_list != NULL);
    POSITION pos = m_instr_list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = m_instr_list->GetNext(pos);

        M_t* pt;
        pt = p->var_r1.thevar; 
		if (pt != NULL) pt->tem_useno++;

        pt = p->var_r2.thevar; 
		if (pt != NULL) pt->tem_useno++;

        pt = p->var_w .thevar; 
		if (pt != NULL) pt->tem_useno++;
	}

    this->m_exprs->DeleteUnuse_VarList(this->m_exprs->vList);
}

extern CString PrtAddOn_internal(PSTR varname, Pst_InstrAddOn pAddOn);
CString CFunc::Instr_prt_simple(PINSTR p)
{
    CString s = hlcode_name(p->type);
    if (p->var_w.type != 0)
    {
        s += " w=";
        s += this->m_exprs->BareVarName(&p->var_w);
    }
    if (p->var_r1.type != 0)
    {
        s += " r1=";
        s += PrtAddOn_internal(this->m_exprs->BareVarName(&p->var_r1), p->va_r1.pao);
    }
    if (p->var_r2.type != 0)
    {
        s += " r2=";
        s += PrtAddOn_internal(this->m_exprs->BareVarName(&p->var_r2), p->va_r2.pao);
    }
    return s;
}

bool CFuncOptim::DataType_Check(VAR_ADDON* pva, CFuncType* pftype)
{
    M_t* pvar = pva->pv->thevar;
	if (pvar->m_DataTypeID == 0)
		return false;
    assert(pvar);
	if (!pftype->m_varpar)
	{
		int i1 = GG_VarType_ID2Size(pvar->m_DataTypeID);
		int i2 = pftype->para_total_size();
        if (pva->pao == NULL)
        {
            assert(i1 == i2);
        }
        else if (pva->pao->type == IA_GetAddress)
        {
            assert(i2 == 4);
        }
	}
    //���ǰ�i_CallPara�Ĳ������������͸���
    POSITION pos = Q->m_instr_list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = Q->m_instr_list->GetNext(pos);
        if (p->var_w.thevar == pvar && p->type == i_Assign)
        {
            if (p->var_r1.thevar != NULL 
                && g_VarTypeManage->is_simple(p->var_r1.thevar->m_DataTypeID)
                )
            {
                VarTypeID id;
                if (p->var_w.part_flag == 0)
                {//˵��ֻ��һ������
                    assert(pftype->m_args == 1);
                    id = pftype->m_partypes[0];
                }
                else
                {
                    id = pftype->SearchPara(p->var_w.part_flag - 1);
                }
				if (id != 0)
                {
                    if (p->va_r1.pao == NULL)
                    {
						if (p->var_r1.thevar->type != MTT_immed)
						{
							int size1 = p->var_r1.thevar->size;
							int size2 = GG_VarType_ID2Size(id);
							assert(size1 == size2);
						}
                        VarTypeID oldid = p->var_r1.thevar->m_DataTypeID;
                        p->var_r1.thevar->m_DataTypeID = id;

                        log_prtl("1: %s datatype %s -> %s",
                                 p->var_r1.thevar->GetName(),
                                 ::GG_VarType_ID2Name(oldid),
                                 ::GG_VarType_ID2Name(id));

                        return true;
                    }
                    else if (p->va_r1.pao->type == IA_GetAddress)
                    {
                        VarTypeID oldid = p->var_r1.thevar->m_DataTypeID;
                        VarTypeID id2 = g_VarTypeManage->GetPointTo(id);

                        p->var_r1.thevar->m_DataTypeID = id2;

                        UINT size1 = p->var_r1.thevar->size;    //ԭ����size
                        UINT size2 = GG_VarType_ID2Size(id2);   //�µ�size
                        if (size1 < size2)
                        {//�����
                            p->var_r1.thevar->size = size2;
                            this->Q->m_exprs->Enlarge_Var(p->var_r1.thevar, Q->m_instr_list);
                        }

                        log_prtl(this->Q->Instr_prt_simple(p).GetBuffer());
                        log_prtl("& X = TYPE *, so X = TYPE");

                        log_prtl("2: %s datatype %s -> %s",
                                 p->var_r1.thevar->GetName(),
                                 ::GG_VarType_ID2Name(oldid),
                                 ::GG_VarType_ID2Name(id2));

                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool CFuncOptim::SetParaType(UINT offset, UINT sizepara, enum_CallC conv,PSTR paraname, VarTypeID paraid)
{
    if (conv == enum_cdecl || conv == enum_stdcall)
    {
        UINT par_off = offset + 4;  //�Բ�����
        M_t* pmt = this->Q->m_exprs->SearchMT(MTT_par, par_off);
        if (pmt == NULL)
            return false;
        if (pmt->m_DataTypeID != 0 && !g_VarTypeManage->is_simple(pmt->m_DataTypeID))
            return false;   //�Ѿ��������ˣ��Ͳ���������
        
        strcpy(pmt->namestr, paraname);
        //���ڣ�Ҫ��pmt->m_DataType��Ϊparatype
        if (pmt->size == GG_VarType_ID2Size(paraid))
        {
            pmt->m_DataTypeID = paraid;
        }
        else
        {
            M_t* pnew = new M_t;    //new_M_t
            *pnew = *pmt;
            pnew->m_DataTypeID = paraid;

            Q->m_exprs->vList->AddTail(pnew);

            this->Q->m_exprs->Enlarge_Var(pnew, Q->m_instr_list);

            assert(pnew->size == GG_VarType_ID2Size(paraid));
        }
        return true;
    }
    else
        assert(0);
    return false;
}

bool CFuncOptim::VarDataType_analysis_mydefine()	//�ҵĺ��������Ѿ����ˣ�����ȡ������������
{
    if (this->Q->m_functype == NULL)
        return false;
    CFuncType* pftype = this->Q->m_functype;

    UINT sizepara = pftype->para_total_size();
    int offset = 0;
    for (int i=0; i<pftype->m_args; i++)
    {
        if (this->SetParaType(offset,
                              sizepara, 
                              this->Q->m_functype->m_callc,
                              pftype->m_parnames[i],
                              pftype->m_partypes[i]))
            return true;
        offset += GG_VarType_ID2Size(pftype->m_partypes[i]);
        while (offset % 4)
            offset++;
    }

    return false;
}

bool CFuncOptim::VarDataType_analysis()
{
    POSITION pos = Q->m_instr_list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = Q->m_instr_list->GetNext(pos);
        if (p->type == i_CallThis)
        {
            PINSTR pcall = p->call_addon.p_thecall;
            assert(pcall);
            if (pcall->type == i_Call)
            {
                CFuncType* pft = pcall->call.call_func->m_functype;
                assert(pft != NULL);
                assert(pft->m_class != NULL);

                if (p->var_r1.thevar != NULL 
                    && g_VarTypeManage->is_simple(p->var_r1.thevar->m_DataTypeID)
                    )
                {
                    VarTypeID id = g_VarTypeManage->Class2VarID(pft->m_class);
                    VarTypeID oldid = p->var_r1.thevar->m_DataTypeID;
                    if (p->va_r1.pao == NULL)
                    {
                        id = g_VarTypeManage->GetAddressOfID(id);
                        p->var_r1.thevar->m_DataTypeID = id;
                    }
                    else if (p->va_r1.pao->type == IA_GetAddress)
                    {
                        p->var_r1.thevar->m_DataTypeID = id;
                    }
                    log_prtl("3: %s datatype %s -> %s",
                             p->var_r1.thevar->GetName(),
                             ::GG_VarType_ID2Name(oldid),
                             ::GG_VarType_ID2Name(id));

                    return true;
                }
            }
        }
        if (p->type == i_CallPara)
        {
            PINSTR pcall = p->call_addon.p_thecall;
            assert(pcall);
            if (pcall->type == i_CallApi)
            {
                CFuncType* pft = pcall->call.papi->m_functype;
                if (pft != NULL)
                if (this->DataType_Check(&p->va_r1, pft))
                    return true; 
            }
            if (pcall->type == i_Call)
            {
                CFuncType* pft = pcall->call.call_func->m_functype;
                if (pft != NULL)
                if (this->DataType_Check(&p->va_r1, pft))
                    return true; 
            }
        }
        if (p->type == i_CallRet)
        {
            PINSTR pcall = p->call_addon.p_thecall;
            assert(pcall);
            if (pcall->type == i_Call)
            {
                CFuncType* pdf = pcall->call.call_func->m_functype;
                if (pdf != NULL
                    && p->var_w.thevar != NULL 
                    && g_VarTypeManage->is_simple(p->var_w.thevar->m_DataTypeID)
                    )
                {
                    VarTypeID oldid = p->var_w.thevar->m_DataTypeID;
                    VarTypeID id = pdf->m_retdatatype_id;
                    p->var_w.thevar->m_DataTypeID = pdf->m_retdatatype_id;

                    log_prtl("4: %s datatype %s -> %s",
                             p->var_w.thevar->GetName(),
                             ::GG_VarType_ID2Name(oldid),
                             ::GG_VarType_ID2Name(id));

                    return true;
                }
            }
            if (pcall->type == i_CallApi)
            {
                CApi* papi = pcall->call.papi;
                VarTypeID retid = papi->m_functype->m_retdatatype_id;
                assert(retid);
                if (p->var_w.thevar != NULL 
                    && g_VarTypeManage->is_simple(p->var_w.thevar->m_DataTypeID)
                    )
                {
                    VarTypeID oldid = p->var_w.thevar->m_DataTypeID;
                    p->var_w.thevar->m_DataTypeID = retid;

                    log_prtl("5: %s datatype %s -> %s",
                             p->var_w.thevar->GetName(),
                             ::GG_VarType_ID2Name(oldid),
                             ::GG_VarType_ID2Name(retid));

                    return true;
                }
            }
        }
    }
    return false;
}

bool CFunc::Step10_Var_analysis()	// ��������
{	
    //�⺯��ֻ�ܵ���һ��
    POSITION pos = m_instr_list->GetHeadPosition();
	while (pos)
	{
		PINSTR p = m_instr_list->GetNext(pos);

        if (p->type == i_EspReport)
        {
            this->m_exprs->EspReport(p->espreport.esp_level);
            continue;
        }

        this->m_exprs->AddVarRef(&p->var_r1);	// ��һ���������ı���
		this->m_exprs->AddVarRef(&p->var_r2);	// �ڶ����������ı���
		this->m_exprs->AddVarRef(&p->var_w);	// д�����ı���
	}
    
    return true;
}

void CFunc::ReType(M_t* p, PSTR newtype)
{
    VarTypeID vid = get_DataType(newtype);
    if (vid == 0)
        return;
    
    if (GG_VarType_ID2Size(vid) <= p->size)
    {//���С��һ�㲻�ᷢ��
        p->size = GG_VarType_ID2Size(vid);
        p->m_DataTypeID = vid;
    }
    else
    {//С�����ɸ�����
        p->size = GG_VarType_ID2Size(vid);
        p->m_DataTypeID = vid;

        this->m_exprs->Enlarge_Var(p, this->m_instr_list);
    }
}
    
void CFunc::Restart()  //Ҫ�ٷ��������� this->m_funcdefine
{
    this->m_nStep = STEP_IDA_4;
    //this->ll.m_asmlist = new AsmCodeList;   //new_AsmCodeList
    this->m_exprs = new CExprManage;    //new_CExprManage
    
    this->m_exprs->m_VarRange_H = this->m_VarRange_H;
    this->m_exprs->m_VarRange_L = this->m_VarRange_L;

    this->m_instr_list = NULL;
}

void CFunc::Fill_this_ECX(VarTypeID id)	 //	��˼��˵������һ��class���Ӻ�������ECX����Ϊ 'this'
{
    M_t* p = this->m_exprs->SearchMT(MTT_reg, enum_ECX);
    if (p == NULL || p->size != BIT32_is_4)
        return;		//	why ?
    strcpy(p->namestr, "this");
    p->m_DataTypeID = id;
}

