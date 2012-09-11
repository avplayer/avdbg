// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	CCodeList
#ifndef CCodeList__H
#define CCodeList__H

class	CCodeList
{
    friend class CCodeList_Maker;
private:
	//CFunc*	m_owner;

    AsmCodeList* m_asmlist;
    int m_EBP_base;
	INSTR_LIST* m_instr_list;

public:

	CCodeList(){};

	void InstrAddTail(PINSTR p);
	void CreateInstrList_raw(INSTR_LIST* instr_list, AsmCodeList* asmlist, int EBP_base);

	
};

enum
{
	enum_00 = 0,    // û�в����� ���� ret��nop
	enum_RR = 1,    // ���������������Ƕ�������cmp
	enum_WR = 2,    // ������������һ��д��һ����
	enum_AR = 3,    // ������������һ������Ҳд��һ����
	enum_A  = 4,    // һ��������������Ҳд���� notָ��
	enum_W =  5,	// һ����������ֻд
};

class CCodeList_Maker
{
	AsmCode*	cur;	//temp used by instrlist maker
    CCodeList* Q;
	DWORD		m_tem_var_no;
	
    void	Code_Jxx(JxxType t);
	PINSTR	Code_general(int type, HLType t);
	void	TransVar(VAR* pvar,int no);
	void	TransVar_(VAR* pvar,int no);
	void	VarRead(VAR_ADDON& va);
	void	WriteToAddress(PINSTR p);
	void	new_temp(VAR* pvar);
public:
    CCodeList_Maker(CCodeList* p_owner, AsmCode* p_cur)
    {
        Q = p_owner;
        cur = p_cur;
        m_tem_var_no = 0;
    }
	void AddTail_Cur_Opcode();
};


#endif // CCodeList__H
