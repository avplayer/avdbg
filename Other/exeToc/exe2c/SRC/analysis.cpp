// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	analysis.cpp
#include "stdafx.h"

#include "cisc.h"
#include "CStrategy.h"

extern bool g_f_Step_by_Step;
extern bool g_any1_return_TRUE;

bool	g_Hxd_Step;

bool Step_by_Step();

typedef bool (CFuncOptim::*OPTIMFUNC)();
typedef bool (CFunc::*PROGFUNC)();

PROGFUNC tbl_Progress[] =
{
    &CFunc::Step1_Disassembly,		//0 ����൱ǰ���������������ַ�����CALLָ���ĵ�ַΪ�º���
    &CFunc::Step2_GetRetPurge,
    &CFunc::Step3_AddRemoveSomeInstr,
    &CFunc::Step4_FillStackInfo,		//3  ����ջλ��
    &CFunc::Step5_Label_Analysis,	//4  ��ת����

	&CFunc::Step6_CreateInstrList,	//5  ����α����
	&CFunc::Step7_GetArgs,			//6	 ��ȡ��ǰ������������
    &CFunc::Step8_ana_RetType,		//7  ��麯���ķ���ֵ
    &CFunc::Func_FillCallParas,		//8  ��亯��������Ϣ

    &CFunc::Step10_Var_analysis,		//9	 ��������������ÿ��������thevar��Ϣ ��M_t�ṹ��
    0
};

OPTIMFUNC tbl_Ana[] =
{
	&CFuncOptim::Step1_Address_to_Add,			//0  ��i_Addressת����i_Add
	&CFuncOptim::Step2_Simplify_Instr,			//1  �Ѽ�ָ��ת���ɸ�ֵָ��i_Assign
    &CFuncOptim::ClassSubFuncProcess,            //2  �����һ��class���Ӻ���
	&CFuncOptim::DataType_Flow,	                //3  ��������������
	&CFuncOptim::pcode_1,	                    //4  ���cmp��jxx��������cmp��jxx�ϲ�
	&CFuncOptim::optim_once_new,	                //5  ���ɱ��������룬�Ա��������Ż�
	&CFuncOptim::ana_Flow,	                    //6	 ���̷���,������е�i_Begin��i_End

	&CFuncOptim::VarDataType_analysis_mydefine,	//7  �������ͷ���
	&CFuncOptim::VarDataType_analysis,	        //8  �������ͷ���
	//CFuncOptim::Step9_Var_Split,				//10  �Ĵ���������֣��ŵ����
	0
};


bool	CFunc::analysis_once_1()
{
    m_prepareTrue_analysisFalse = false;

	// ��һ����
	if(m_nStep<STEP_100)
	{
		for(;m_nStep < STEP_100;m_nStep++)	// α������Ҫ��10������������ſ�����ʾ
		{
			PROGFUNC fn = tbl_Progress[m_nStep];
			if (fn != NULL)
			{
				log_prtl("step %d",this->m_nStep);
				if (!(this->*fn)())
					return false;	// ����ʧ��
			}
			else					// �������
				break;
		}

		log_prtl("��һ���ִ�����ɣ�");
		this->m_nStep = STEP_100;   //˵�������ˣ�

		return true;	// ������ɺ�ͣһ��
	}

	// ��ʼ����ڶ�����
    CFuncOptim the(this);

	if(m_nStep_Two<STEP_100)
	{
		for (;m_nStep_Two<STEP_100;m_nStep_Two++)
		{
			OPTIMFUNC pfunc = tbl_Ana[m_nStep_Two];
			if (pfunc != NULL)
			{
				log_prtl("�ڶ����� step %d",m_nStep_Two);
				if ((the.*pfunc)())
					return true;		// �������һС��
				else
				{
					if(g_Hxd_Step)		// ÿ�������һ�󲽣�ͣһ��
					{
						m_nStep_Two++;
						return false;
					}
				}
			}
			else
				break;	// �������
		}
		log_prtl("�ڶ����ִ�����ɣ�");
		this->m_nStep_Two = STEP_100;   //˵�������ˣ�
	}

	return false;
}

bool	CFunc::analysis_once()	
{
    bool f = analysis_once_1();
    if (f)
    {
        if (g_CStrategy.IfAny())	// �����Ż�����
        {
			if(g_Hxd_Step)		// ����ʱ����ʾ
				g_CStrategy.PrintIt(this->m_instr_list, this);		// ��ӡ�Ż���Ϣ
            g_CStrategy.DoIt(this->m_instr_list, this->m_exprs);	// �Ż�����
        }
        DeleteUnusedVar();			// ɾ������Ҫ�ı���
    }
    return f;
}

void CFunc::analysis()	// һ��������
{
    for (;;)
    {
        g_f_Step_by_Step = true;
        g_any1_return_TRUE = false;
		g_Hxd_Step = false;
        if (!this->analysis_once())
            break;
    }
}

bool	CFunc::analysis_step_by_step() // ��������
{
	//	��������ʾ����Ϣ
	static CFunc* lastfunc = 0;

	if (this != lastfunc)
		lastfunc = this;

	g_f_Step_by_Step = true;
	g_any1_return_TRUE = false;

	g_Hxd_Step = true;

	bool f = analysis_once();

	g_Hxd_Step = false;

	g_f_Step_by_Step = false;
	f = f || g_any1_return_TRUE;

	return f;
}


bool	CFunc::Step8_ana_RetType()	//	��麯���ķ���ֵ
{
    VAR v;
    v.type = v_Reg;
    v.reg = enum_EAX;			//	enum_EAX = 0 = enum_AL = enum_AX 
    v.opsize = BIT32_is_4;

    if (this->m_functype != NULL)
    {
        int n = GG_VarType_ID2Size(this->m_functype->m_retdatatype_id);
        if (n == 0)
            return true;
        if (n == 2 || n == 4)
            v.opsize = n;
    }


    POSITION pos = m_instr_list->GetHeadPosition();
	while (pos)
	{
		POSITION savpos = pos;
		PINSTR p = m_instr_list->GetNext(pos);
		if (p->type == i_Return)
		{
			PINSTR pnew = new INSTR;    //  new_INSTR
			pnew->type = i_RetPar;		//	��ʱ��Ϊÿһ��func����ret DWORD
			pnew->var_r1 = v;
			m_instr_list->InsertBefore(savpos,pnew);
		}
	}
	return true;
}

void CFunc::PrepareFunc()				// ����tbl_Progressǰ����
{
    m_prepareTrue_analysisFalse = true;
    while (this->m_nStep < 3)
    {
        PROGFUNC fn = tbl_Progress[m_nStep];
        if (fn != NULL)
        {
            if ((this->*fn)())
            {
                m_nStep++;
                continue;
            }
            break;
        }
        else
            this->m_nStep = STEP_100;
    }
}
