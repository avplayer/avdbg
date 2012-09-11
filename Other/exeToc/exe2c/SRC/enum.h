// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	enum.h
#ifndef ENUM__H
#define ENUM__H

#define	BIT32_is_4	4
#define	ESP_UNKNOWN	11
#define	ESP_IGNORE	13
#define	Not_EBP_based	11
#define	INVALID_STACK	11

typedef	DWORD	ea_t;

enum JxxType
{
	JMP_jmp	=	1,
	JMP_jle	=	2,
	JMP_jng	=	2,
	JMP_jnl	=	3,	
	JMP_jge	=	3,	
	JMP_jl	=	4,	
	JMP_jg	=	5,
	JMP_jna	=	6,
	JMP_jbe	=	6,
	JMP_jnb	=	7,
	JMP_jae	=	7,
	JMP_jb	=	8,
	JMP_ja	=	9,

	JMP_jo,
	JMP_jno,
	JMP_jp,
	JMP_jnp,
	JMP_jnle,
	JMP_jz,
	JMP_jnz,
	JMP_js,
	JMP_jns,

	JMP_case,

};

enum enum_COMP
{
	COMP_unknown	=	0,
	COMP_for		=	1,
	COMP_if			=	2,
	COMP_if_else	=	3,
	COMP_while  	=	4,
	COMP_do_while  	=	5,
	COMP_long_if	=	6,
	COMP_for1,      //do while �͵�for��ʡ���˵�һ���ж�
	COMP_switch_case,
	COMP_switch_case_multcomp,	//	����compare��switch_case
	COMP_if_Or,
	COMP_if_else_if,
	COMP_break,
};


enum HLType		// α��������
{
    i_Nothing = 0,
	i_Jump	=	0x101,
	i_Label =	0x102,
	i_Begin	,		// ������ʼָ��
	i_End	,		// ��������ָ��
	i_Assign,		// ��ֵָ��
	i_Var	,
	i_Unknown,
	i_RetPar,		// �����ķ���ֵ��������i_Returnǰ
	i_Return,		// ����������־

	i_Rol, i_Ror,
	i_Add, i_Sub,  i_Sar,  i_Shl, i_Shr, i_Imul,
	i_Not, i_And,  i_Or, i_Xor, i_Test,
	i_Readpointto,
	i_Writepointto,
	
	i_Cmp,		// ��ʱ�ģ�����Ҫȥ����
	i_Lea,		// ��ʱ�ģ�����Ҫȥ����
	i_Address,	// ���ԶԸ� [eax+ebx*4+8888],�������ֳ�˭����pointer��Ҫ���i_GetAddr
	i_JmpAddr,	// ��ʱ������switch case

	i_GetAddr,
	i_Call,
	i_CallApi,
    i_CallThis, // ���call�Ǹ�ecx->func1��Ҫ�õ�ecx��
	i_CallPara,	// ��call�Ĳ��������ǽ���i_Call
	i_CallRet,	// ��call�ķ���ֵ�����ǽ���i_Call
	i_CplxBegin,
	i_CplxEnd,
	i_Nop,

	i_SignExpand,	//	for movsx
	i_NosignExpand,	//	for movzx

    i_EspReport,

	i_Setnz,
};
	
	
enum
{
	enum_EAX	=	0,
	enum_AL		=	0,
	enum_AH		=	1,
	enum_AX		=	0,
	enum_ECX	=	4,
	enum_CL		=	4,
	enum_CH		=	5,
	enum_CX		=	4,
	enum_EBX	=	8,
	enum_BL		=	8,
	enum_BH		=	9,
	enum_BX		=	8,
	enum_EDX	=	0x0c,
	enum_DL		=	0x0c,
	enum_DH		=	0x0d,
	enum_DX		=	0x0c,
	enum_EBP	=	0x10,
	enum_BP		=	0x10,
	enum_ESP	=	0x14,
	enum_SP		=	0x14,
	enum_ESI	=	0x18,
	enum_SI		=	0x18,
	enum_EDI	=	0x1c,
	enum_DI		=	0x1c,

};

enum XMLTYPE
{
	XT_invalid = 0,
	XT_blank,
	XT_Symbol,
	XT_Function,
	XT_Keyword,		//�ؼ��֣�����struct,union,for,while
	XT_Class,		//�Ǹ�class��union��struct������
	XT_K1,			//{} []
	XT_Comment,		//ע��
	XT_DataType,	//��������
	XT_Number,		//һ����
	XT_AsmStack,	//��ջֵ
	XT_AsmOffset,	//�����ʾʱ��seg:offset
	XT_AsmLabel,	//�����ʾʱ��label name
    XT_FuncName,	//��������
};

enum em_PRT_COLOR
{
    COLOR_0 = 0,    //default color
    COLOR_Func = 1,
    COLOR_Var,
    COLOR_VarH,     //�߼��ģ�cpp�õ�
    COLOR_Immed,
    COLOR_ea,
    COLOR_type,
    COLOR_label,
    COLOR_last
};

enum REF_ENUM
{
	enum_NOREF	= 0,
	enum_REF	= 1,
	enum_REUSE	= 2,	//	��Ȼ���������ң�����
};

enum em_STEP
{
	STEP_Init = 0,
	STEP_IDA_1 = 1,
	STEP_IDA_2 = 2,
	STEP_IDA_3 = 3,
	STEP_IDA_4 = 4,
	STEP_4 = 5,
	STEP_5 = 6,
	STEP_6 = 7,
	STEP_7 = 8,
	
    STEP_100 = 100,
};

enum ENUM_STRATEGY
{
    ES_Error = 0,
    ES_Instr_can_Delete,
    ES_Instr_can_Elim_63,
    ES_Instr_can_Elim_25E,
    ES_Instr_can_Elim_31E,
    ES_Instr_can_Elim_21E,
	ES_Instr_can_Elim_hxd_61,		// ��ǰ����һ����ֵ����ʱ������Ķ��붼�滻�� EAX= v_4,�����Ķ�eax����v_4����
};

// ��ת����
enum em_JumpType
{
    Jump_Down = 1,	// ������
	Jump_Up = 2,

	Form_Up = 3,	// ��������
	Form_Down = 4,
	Form_All = 5,	// ���Ϸ�����
    
};

#endif // ENUM__H
