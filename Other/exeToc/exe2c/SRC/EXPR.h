// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

// EXPR.h
#ifndef EXPR__H
#define EXPR__H
/*
class EXPR;
typedef	EXPR*	PEXPR;

class EXPR
{
public:
	BYTE    type;		// EXPR_???

	BYTE    len;		// var size in BYTE
	BYTE    no1;
	BYTE    f_G;		// 0: local 1: global

	BYTE	fUserDefineName;	//if it already has a user defined name, then we should not optim it
	BYTE	f_part;		// this expr is part of another, AL to EAX
	BYTE	no2;
	BYTE	no3;

	char	str_type[20];	//string of EXPR type, "unsigned long"


	union
	{
		DWORD d;
		struct
		{
			DWORD ESP_off;	//-04 for first push
		} v1;
		struct
		{
			BYTE REG_index;
		} v2;
		struct
		{
			DWORD MEM_off;
		} v3;
		struct
		{
			//BYTE    base_reg;
			//BYTE    index_reg;
			PEXPR   expr_base;
			PEXPR   expr_index;
			DWORD   off;
		} reg_ptr;
	};
	//VALUEE   value;
	char    name[20];

	EXPR();		// ������ CFunc::expr_new() ������һ�� EXPR
	~EXPR();

	PSTR Get_TypeDefStr();	// ���� EXPR �Ķ��壬�� "unsigned long"
};

typedef	CList<PEXPR,PEXPR>   EXPR_LIST;
*/
enum VARType
{
	v_Invalid = 0,
	v_Immed,    //  ������
	v_Reg,		//r_
	//v_Stack, ��ջ���������ֱ�Ϊ�������ڲ������������ģ����Էֿ�
	v_Par,		//v_ ����
	v_Var,		//a_ ����
	v_Global,	//g_ ȫ�ֱ���
	v_Tem,		//t_ ��ʱ����
    v_Volatile, //����ֻ���� fs:0
};

struct VAR
{
	VARType	type;	// ���������� v_Reg,v_Stack,v_Global,v_Immed

	UINT	opsize;	// ������С��0:void,1:BYTE,2:WORD,4:DWORD,...
    struct M_t* thevar;

    int part_flag;  //һ��Ϊ0�������Ϊ0����ʾ��var��thevar��һ����
                    //part_flag-1���Ǳ�var��thevar�е�offset

	union
	{
		DWORD	reg;		//  for v_Reg, 0:eax,1:ah,4:ecx....
		DWORD	par_off;	//	ƫ����������������Ҫ
		DWORD	var_off;	//  ƫ������������Ҫ
		ea_t	off;		//  ƫ������ȫ�ֱ�����Ҫ
		BYTE	b;
		WORD	w;
		DWORD	d;			//  ������������ֵ
		DWORD	temno;		//  for v_Tem
	};

    static bool IsSame(VAR* v1,VAR* v2);
    static int	VarCompare(VAR* v1,VAR* v2);
    	//	0:	no relationship
    	//	1:	same
    	//	2:	v1 include v2
    	//	3:	v2 include v1
    VAR()
    {
        thevar = NULL;
        part_flag = 0;
    }
};


DWORD	stack2varoff(signed long stackoff);

#endif // EXPR__H
