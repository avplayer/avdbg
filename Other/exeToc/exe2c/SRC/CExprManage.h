// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	CExprManage.h
#ifndef CExprManage__H
#define CExprManage__H

enum en_MTTYPE
{
    MTT_invalid = 0,
    MTT_tem,
    MTT_reg,
    MTT_var,
    MTT_par,
    MTT_immed,  //�Դˣ�namestr��s_off�ǲ��õ�
    MTT_global,
};

struct M_t
{
    en_MTTYPE type; // MTT_tem ..
	DWORD s_off;	// start offset
	DWORD size;
    
	//H_NAMEID nameid;
    char namestr[80];
    int  tem_useno;  // �м��������ң�һ�㲻Ҫ���������
    
    bool bTem;		//�Ƿ���ʱ����
    int  iThrowned;

    union
    {
        struct
        {
            DWORD temno;
        }tem;
        struct
        {
            DWORD d;
        } immed;
    };

    VarTypeID m_DataTypeID;

    PSTR GetName()
    {
        return namestr;
    }

    M_t()
    {
        ZeroMemory(this, sizeof(M_t));
        bTem = false;
        iThrowned = 0;
        strcpy(namestr, "");
        this->m_DataTypeID = 0;
    }

    DWORD end_off()
    {
        return s_off + size;
    }

    bool IfInclude(UINT off)
    {
        if (this->s_off <= off && 
            this->s_off + this->size >= off)
            return true;
        return false;
    }

    bool AnyOverlay(M_t* p);
    void Expand(M_t* p);
} ;

typedef CList<M_t*,M_t*> MLIST;    // MLIST�������������

class CNameMng;

class	CExprManage		// ����������
{
    M_t* GetVarByName_1(MLIST* list, PSTR varname);
public:

	MLIST*	vList;

    void DeleteUnuse_VarList(MLIST* vlist);
    
    signed int m_VarRange_L;
    signed int m_VarRange_H;

    M_t* CreateNewTemVar(UINT size);

	CExprManage();
	~CExprManage();

    void	EspReport(signed int esplevel);
	void	AddVarRef(VAR* pvar);
    M_t*	AddRef_with_name(en_MTTYPE type, DWORD off, DWORD size, PSTR tj_name);
    M_t*	AddRef_tem(DWORD temno, DWORD size);
    M_t*	AddRef_immed(DWORD d, DWORD size);

    M_t*	SearchMT(en_MTTYPE type, DWORD off);

    void 	prt_var(VAR* var, CXmlOutPro* out);
    void 	prt_var_Immed(VAR* var, CXmlOutPro* out);

    PSTR	VarDataType(VAR* v);	//	�������ͣ�����DWORD,int,unsigned long�ȵ�
    PSTR	VarName(VAR* v);
    PSTR	VarName_Immed(VAR* v);
	PSTR	BareVarName(VAR* v);

    void	prt_parameters(CXmlOutPro* out);
    void	prt_var_declares(CXmlOutPro* out);
    M_t*	GetVarByName(PSTR varname);
    void	Enlarge_Var(M_t* p, INSTR_LIST* instr_list);

    void	ClearUse();
};

extern CExprManage g_GlobalExpr;
#endif // CExprManage__H
