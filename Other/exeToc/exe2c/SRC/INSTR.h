// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

// INSTR.h
#ifndef INSTR__H
#define INSTR__H

class	CFunc;

class INSTR;
typedef	INSTR*	PINSTR;

enum em_InstrAddOn
{
    IA_Nothing = 0,
    IA_AddImmed,    // �򵥵ؼ�������
    IA_MulImmed,    // �򵥵س�������
    IA_ReadPointTo, // *
    IA_GetAddress,  // &
};

// ��������ʲô�ô��أ�
struct st_InstrAddOn
{
    em_InstrAddOn type;

    union
    {
        struct
        {
            signed int iAddon;
        } addimmed; //for IA_AddImmed
    };

    st_InstrAddOn* pChild;

    st_InstrAddOn()
    {
        ZeroMemory(this, sizeof(st_InstrAddOn));

    }
	~st_InstrAddOn()
	{

		if(pChild)
		{
			delete pChild;
			pChild = NULL;
		}

	}
    static bool IsSame(st_InstrAddOn* p1, st_InstrAddOn* p2);
};

typedef st_InstrAddOn* Pst_InstrAddOn;

struct VAR_ADDON
{
	VAR*	pv;			// ������Ϣ
    st_InstrAddOn* pao; // point add on

    VAR_ADDON()
    {
        pv = NULL;
        pao = NULL;
    }
    ~VAR_ADDON()
    {
        //do not delete pv
        if (pao)
            delete pao;
    }
};

// ����α������
class INSTR
{
public:
	DWORD	id;			//  �Զ�����
	HLType	type;		//	α���������,��i_Assign ...
	bool	bJmpFlowOk;	// �����û
    union
	{
		struct
		{
			JxxType	jmp_type;	//��ת�����ͣ���JMP_???
			DWORD	linear;		//  ��תʱ����ǰָ��ĵ�ַ
			DWORD   jmpto_off;
			PINSTR the_label;
			PINSTR next_ref_of_this_label;	//������ɸ��������������һ��label������ref
		} jmp;					//for type = i_Jump only

		struct
		{
			PINSTR  ref_instr;		// for type = i_Label only
			ea_t	label_off;
			int		bJmpFlowNum;	// ���̷���ʱ���Ѿ��ɹ����������
			bool	f_conti;
		} label;

		struct
		{
			CFunc*  call_func;		// for i_Call
			CApi*	papi;			// for i_CallApi
			signed int	esp_level;
            PINSTR      p_callpara;
            PINSTR      p_callret;
		} call;

        struct
        {
            PINSTR      p_thecall;
        } call_addon;   //for i_CallPara and i_CallRet

		struct	// for,while i_CplxBegin������
		{
			PINSTR		m_end;			// ����i_Endα����
			enum_COMP	type;			// ��תָ�����ͣ���COMP_if
			PINSTR		m_break;		// ������� break �Ļ�������break����label
			PINSTR		m_conti;		// ������� continue �Ļ�������continue����label
			PINSTR		m_not_conti;	// �������jmpָ��m_conti������Ȼ����continue.
		} begin;						// ���������Ϊ���չ�while

        struct
        {
            signed int esp_level;
            UINT howlen;
        } espreport;		// for i_EspReport
	};

	VAR			var_w;		// д�����Ĳ�������Ϣ
    VAR_ADDON	va_r1;		// va=Var AddOn
    VAR_ADDON	va_r2;		// 
	VAR			var_r1;		// �������Ĳ�����1
	VAR			var_r2;		// �������Ĳ�����2
	DWORD	i1;				// =4,     ���磺��type == i_Addressʱ��[eax+ebx*4+8888]
	DWORD	i2;				// =8888

	INSTR();
	~INSTR();
};

typedef	CList<PINSTR,PINSTR> INSTR_LIST;

// ������תָ�������
struct st_JumpOptm
{
    PINSTR pinstr;
	POSITION pos;	// ��ǰ�����α�����е�λ��
	bool   bJxx;	// ��ת
	em_JumpType  JumpType;	// ��ת�ķ�������Ϊ 1,����Ϊ 2
	bool   bLable;
	int    iLableType;	// �����ķ��򣬴���������Ϊ 1,����������Ϊ 2�����¶�����3
    int	   iLableJmpFromNum;	// ��ת����i_Lable��Jmp����
	DWORD  dwJumpFrom[250];		// ��ת�ĵ�ַ

    st_JumpOptm()
    {
        ZeroMemory(this, sizeof(st_JumpOptm));
    }
    bool IsJump()
    {
        return (bJxx && pinstr->type == i_Jump);
    }
};
typedef CList<st_JumpOptm*, st_JumpOptm*> JUMPOPTM_LIST;
typedef	st_JumpOptm*	PJUMPOPTM;


class CInstrList
{
    bool	if_Ly_In(PINSTR p, POSITION firstpos, POSITION endpos);
    bool	IsSwitchCase_multcomp(PINSTR begin);
    bool	IsSwitchCase(PINSTR begin);
    bool	ifOneStatement(PINSTR pNode, POSITION firstpos, POSITION endpos);
    bool	Flow_c(PINSTR pNode);
    void	Flow_b(PINSTR pParentNode, POSITION firstpos, POSITION endpos);

    bool	Flow_aa(POSITION firstpos, POSITION endpos);

    bool	Flow_cc(PINSTR pNode, POSITION firstpos, POSITION endpos);
    void	Add_Begin_End(POSITION firstpos, POSITION endpos, PINSTR begin, PINSTR end);
    void	Add_Begin_End_1(POSITION firstpos, POSITION endpos, PINSTR begin, PINSTR end);

	// �������ݣ��쳾�������
	void	Get_Jump_Use_Flow(JUMPOPTM_LIST& used_list);		// �õ���תʹ��α�����б�
	char	HowJumpUse_Char(st_JumpOptm* the);
	bool	Optim_Jump_flow_NT(JUMPOPTM_LIST& Jumplist,char hxd_tbl_c[]);	// ������ת����

	void	Hxd_Add_CplxBegin_CplxEnd(POSITION firstpos, POSITION endpos,enum_COMP comptype);
	bool	Hxd_Add_Begin_End(POSITION firstpos, POSITION endpos);

	void	Hxd_While(POSITION firstpos, POSITION jxx,POSITION jump,POSITION endpos);
	void	Hxd_do_While(POSITION firstpos,POSITION endpos);
	void	Hxd_If(POSITION firstpos,POSITION endpos,enum_COMP comptype);
	void	Hxd_If_Or(POSITION firstpos, POSITION jxx,POSITION label,POSITION endpos);
	void	Hxd_If_else(POSITION firstpos, POSITION jxx,POSITION label,POSITION endpos);
	void	Hxd_Break(POSITION Jxx,POSITION While);

	// end �쳾����
    
    INSTR_LIST* m_list; //Ҫ��������private

public:
    CInstrList(INSTR_LIST* p)
    {
        m_list = p;
    }

    bool	Flow_a(PINSTR pNode);
	bool	Hxd_JumpFlow();
};

class CInstrList_Finger
{
    void	prt_partern(PINSTR phead, PSTR partern_buf);
    static int search_and_add(DWORD* buf,DWORD val,int* pn);
    static bool	finger_compare(PSTR f1,const char* f2);
public:
    INSTR_LIST* m_list; //Ҫ��������private
    CInstrList_Finger(INSTR_LIST* p)
    {
        m_list = p;
    }
    bool	Finger_check_partern(PINSTR p);
    bool	Finger_check_partern_for1(PINSTR p);
};




#endif // INSTR__H
