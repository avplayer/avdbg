
#include "stdafx.h"
#include "cisc.h"
#include "INSTR.h"

#include "hxd_Lib.h"
#include "CApiManage.h"

#include "..\LibScanner.H"

// ȫ�ֱ���
extern HANDLE	hOutPutFile;

bool check_if_API(BYTE* dwAddr)			// �ж��Ƿ�����ת��API
{
	if (*(WORD *)dwAddr != 0x25ff)		// ����Ϊǰ��λ�Ƿ�Ϊ0x25FF
		return false;

	DWORD d = *(DWORD *)(dwAddr + 2);

	CApi* papi = g_ApiManage->get_api((ea_t)d);
	if (papi != NULL)
		return true;
	else
		return false;
}

void OutToTxt(const char * szText)
{
	// ��ʼ������ļ����
	if(hOutPutFile==NULL)
		hOutPutFile = CreateFile("c:\\OutTxt.txt",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hOutPutFile == INVALID_HANDLE_VALUE || hOutPutFile==NULL)
		return;
		//AfxMessageBox("����������Ϣ����ļ�ʧ��!");

	DWORD dwSize= GetFileSize(hOutPutFile,NULL);   
	SetFilePointer(hOutPutFile,0,NULL,FILE_END);

	DWORD  dwBytesWritten;
	WORD   wLineEnd  = 0x0A0D;

	WriteFile(hOutPutFile,szText,lstrlen(szText),&dwBytesWritten,NULL);
	WriteFile(hOutPutFile,&wLineEnd,sizeof(wLineEnd),&dwBytesWritten,NULL);

	if (hOutPutFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hOutPutFile);
		hOutPutFile = NULL;
	}
}

void nop()
{
}

void lib_init()	// ���ļ���ʼ��
{
    I_LIBSCANNER* pnew = NEW_LIBSCANNER();	// �����

    char buf[255];

    sprintf(buf,"%s\\LIB\\%s",GetMyExePath(),"LIBC.LIB");

    pnew->ScanLib(buf);

	//sprintf(buf,"%s\\LIB\\%s",GetMyExePath(),"LIBCD.LIB");
    //pnew->ScanLib(buf);

	//sprintf(buf,"%s\\LIB\\%s",GetMyExePath(),"msvcrt.lib");
	//pnew->ScanLib(buf);

    g_LIBSCANNER = pnew;
}

void lib_exit()
{
    if (g_LIBSCANNER != NULL)
    {
        g_LIBSCANNER->Release();
        g_LIBSCANNER = NULL;
    }
}

PSTR GetMyExePath()	// Exe2C�����·��
{
    static char lpFilename[256];

    GetModuleFileName(NULL, lpFilename, 255);
    char* p = strrchr(lpFilename, '\\');
    if (p != NULL)
        *p = 0;
    return lpFilename;
}

PSTR my_itoa(int i)
{
    static char buf[80];
    sprintf(buf, "%d", i);
    return buf;
}


PSTR CallConvToName(enum_CallC ec)
{
    switch (ec)
    {
    case enum_stdcall: return "__stdcall";
    case enum_cdecl:   return "__cdecl";
    case enum_pascal:  return "PASCAL";
    case enum_fastcall:return "__fastcall";
    default:
        return "__unknown";
    }
}
