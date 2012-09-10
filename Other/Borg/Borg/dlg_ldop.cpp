/************************************************************************
*                  dlg_ldopt.cpp                                        *
* Contains the dialog routines for the load file dialogboxes            *
************************************************************************/

#include <windows.h>
#include <stdio.h>

#include "resource.h"
#include "exeload.h"
#include "proctab.h"
#include "dasm.h"
#include "disasm.h"
#include "help.h"
#include "debug.h"

/************************************************************************
* forward declarations                                                  *
************************************************************************/
BOOL CALLBACK CheckOptDlgProc(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK moreoptions(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam);

/************************************************************************
* checktypebox                                                          *
* - after a file has been chosen to load and before the file is loaded  *
*   this is displayed for the user to set options for analysis, file    *
*   type, etc.                                                          *
************************************************************************/
//dialog proc for verifying type and
// initial options
BOOL CALLBACK CheckOptDlgProc(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	char segtext[20],offstext[20];
	static int exetype;
	int i;
	dword segd;
	switch(message)
	{
	case WM_INITDIALOG:
		exetype=g_floader.getexetype();
		i=0;
		g_options.loadaddr.segm=0x1000;
		g_options.loadaddr.offs=0x00;
		switch(exetype)		//����ǰ���жϳ������ͽ�����ʾ
		{
		case NE_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"NE Executable");
			CheckDlgButton(hdwnd,IDC_DEFAULTBUTTON,true);
			g_options.processor=PROC_80486;
			g_options.mode16=true;
			break;
		case COM_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"COM File");
			CheckDlgButton(hdwnd,IDC_DEFAULTBUTTON,true);
			g_options.processor=PROC_80386;
			g_options.mode16=true;
			g_options.loadaddr.offs=0x100;
			break;
		case SYS_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"SYS File");
			CheckDlgButton(hdwnd,IDC_DEFAULTBUTTON,true);
			g_options.processor=PROC_80386;
			g_options.mode16=true;
			g_options.loadaddr.offs=0x00;
			break;
		case PE_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"PE Executable");
			CheckDlgButton(hdwnd,IDC_DEFAULTBUTTON,true);
			g_options.processor=PROC_PENTIUM;
			g_options.mode16=false;
			break;
		case OS2_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"OS2 Executable");
			CheckDlgButton(hdwnd,IDC_DEFAULTBUTTON,true);
			g_options.processor=PROC_PENTIUM;
			g_options.mode16=false;
			break;
		case LE_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"LE Executable");
			CheckDlgButton(hdwnd,IDC_DEFAULTBUTTON,true);
			g_options.processor=PROC_80486;
			g_options.mode16=false;
			break;
		case MZ_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"COM File");
			CheckDlgButton(hdwnd,IDC_DOSBUTTON,true);
			g_options.processor=PROC_80386;
			g_options.mode16=true;
			break;
		default:
		case BIN_EXE:
			SetDlgItemText(hdwnd,IDC_DEFAULTBUTTON,"COM File");
			CheckDlgButton(hdwnd,IDC_BINBUTTON,true);
			g_options.processor=PROC_8086;
			g_options.mode16=true;
			break;
		}

		g_options.mode32=!g_options.mode16;
		CheckDlgButton(hdwnd,load_debug,g_options.loaddebug);
		CheckDlgButton(hdwnd,demangle_names,g_options.demangle);
		CheckDlgButton(hdwnd,IDC_16DASM,g_options.mode16);
		CheckDlgButton(hdwnd,IDC_32DASM,g_options.mode32);
		CheckDlgButton(hdwnd,IDC_LOADDATA,g_options.loaddata);
		CheckDlgButton(hdwnd,IDC_LOADRESOURCES,g_options.loadresources);

		while(procnames[i].num)		//ѭ����Ӵ���������
		{
			SendDlgItemMessage(hdwnd,IDC_LISTBOX1,LB_ADDSTRING,0,(LPARAM) (LPCTSTR)procnames[i].name);
			if(g_options.processor==procnames[i].num)
				SendDlgItemMessage(hdwnd,IDC_LISTBOX1,LB_SETCURSEL,i,0);
			i++;
		}

		//���ü��ضΣ�����ò�ƶ�PEûɶ�ã�
		segd=g_options.loadaddr.segm;
		wsprintf(segtext,"%x",segd);
		wsprintf(offstext,"%lx",g_options.loadaddr.offs);
		SendDlgItemMessage(hdwnd,IDC_SEGEDIT,WM_SETTEXT,0,(LPARAM)segtext);
		SendDlgItemMessage(hdwnd,IDC_OFFSEDIT,WM_SETTEXT,0,(LPARAM)offstext);
		return false;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			if(!IsDlgButtonChecked(hdwnd,IDC_DEFAULTBUTTON))
			{
				if(IsDlgButtonChecked(hdwnd,IDC_DOSBUTTON))
					g_floader.setexetype(MZ_EXE);
				else
					g_floader.setexetype(BIN_EXE);
			}
			else if((exetype==BIN_EXE)||(exetype==MZ_EXE))
				g_floader.setexetype(COM_EXE);
			g_options.processor=procnames[SendDlgItemMessage(hdwnd,IDC_LISTBOX1,LB_GETCURSEL,0,0)].num;
			EndDialog(hdwnd,NULL);
			return true;
		case IDC_SEGEDIT:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SendDlgItemMessage(hdwnd,IDC_SEGEDIT,WM_GETTEXT,(WPARAM)18,(LPARAM)segtext);
				sscanf(segtext,"%x",&g_options.loadaddr.segm);
			}
			return true;
		case IDC_OFFSEDIT:
			if(HIWORD(wParam)==EN_CHANGE)
			{
				SendDlgItemMessage(hdwnd,IDC_OFFSEDIT,WM_GETTEXT,(WPARAM)18,(LPARAM)offstext);
				sscanf(offstext,"%lx",&g_options.loadaddr.offs);
			}
			return true;
		case IDC_HELPBUTTON1:
			DialogBox(g_hInst,MAKEINTRESOURCE(HELPDIALOG_1),hdwnd,(DLGPROC)helpbox1);
			return true;
		case more_options:
			DialogBox(g_hInst,MAKEINTRESOURCE(Advanced_Options),hdwnd,(DLGPROC)moreoptions);
			return true;
		case load_debug:
			g_options.loaddebug=!g_options.loaddebug;
			CheckDlgButton(hdwnd,load_debug,g_options.loaddebug);
			return true;
		case demangle_names:
			g_options.demangle=!g_options.demangle;
			CheckDlgButton(hdwnd,demangle_names,g_options.demangle);
			return true;
		case IDC_16DASM:
			g_options.mode16=!g_options.mode16;
			CheckDlgButton(hdwnd,IDC_16DASM,g_options.mode16);
			return true;
		case IDC_32DASM:
			g_options.mode32=!g_options.mode32;
			CheckDlgButton(hdwnd,IDC_32DASM,g_options.mode32);
			return true;
		case IDC_LOADDATA:
			g_options.loaddata=!g_options.loaddata;
			CheckDlgButton(hdwnd,IDC_LOADDATA,g_options.loaddata);
			return true;
		case IDC_LOADRESOURCES:
			g_options.loadresources=!g_options.loadresources;
			CheckDlgButton(hdwnd,IDC_LOADRESOURCES,g_options.loadresources);
			return true;
		default:
			break;
		}
	}
	return false;
}

#ifdef __BORLANDC__
#pragma warn +par
#endif

/************************************************************************
* moreoptions                                                           *
* - advanced loading options                                            *
************************************************************************/
#ifdef __BORLANDC__
#pragma warn -par
#endif
BOOL CALLBACK moreoptions(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_COMMAND:
		{
			switch(wParam)
			{
			case IDOK:
				g_options.codedetect=0;
				if(IsDlgButtonChecked(hdwnd,advanced_pushbp))
					g_options.codedetect|=CD_PUSHBP;
				if(IsDlgButtonChecked(hdwnd,advanced_aggressive))
					g_options.codedetect|=CD_AGGRESSIVE;
				if(IsDlgButtonChecked(hdwnd,advanced_enter))
					g_options.codedetect|=CD_ENTER;
				if(IsDlgButtonChecked(hdwnd,advanced_movbx))
					g_options.codedetect|=CD_MOVBX;
				if(IsDlgButtonChecked(hdwnd,advanced_moveax))
					g_options.codedetect|=CD_MOVEAX;
				if(IsDlgButtonChecked(hdwnd,advanced_eaxfromesp))
					g_options.codedetect|=CD_EAXFROMESP;
				EndDialog(hdwnd,NULL);
				return true;
			default:
				break;
			}
		}
		break;
	case WM_INITDIALOG:
		CheckDlgButton(hdwnd,advanced_pushbp,g_options.codedetect&CD_PUSHBP);
		CheckDlgButton(hdwnd,advanced_aggressive,g_options.codedetect&CD_AGGRESSIVE);
		CheckDlgButton(hdwnd,advanced_enter,g_options.codedetect&CD_ENTER);
		CheckDlgButton(hdwnd,advanced_movbx,g_options.codedetect&CD_MOVBX);
		CheckDlgButton(hdwnd,advanced_moveax,g_options.codedetect&CD_MOVEAX);
		CheckDlgButton(hdwnd,advanced_eaxfromesp,g_options.codedetect&CD_EAXFROMESP);
		return false;
	default:
		break;
	}
	return false;
}
#ifdef __BORLANDC__
#pragma warn +par
#endif

/************************************************************************
* load                                                                  *
* - checks file header info, identifies the possible types of files,    *
*   gets the users file loading options and calls the specific exe      *
*   format loading routines.                                            *
************************************************************************/
bool loadexefile(char *fname)
{
	char mzhead[2],exthead[2];
	dword pe_offset;
	dword num;
	dword fsize;
	if(g_floader.efile!=INVALID_HANDLE_VALUE)	//�ж��Ƿ��Ѿ����ع�
		return false;

	// just grab the file size first
	//��ȡ�ļ���С
	g_floader.efile=CreateFile(fname,GENERIC_READ,1,NULL,OPEN_EXISTING,0,NULL);
	fsize=GetFileSize(g_floader.efile,NULL);
	CloseHandle(g_floader.efile);
	if(!fsize)		//��ȡ�ļ���Сʧ�ܻ�Ϊ��
	{
		MessageBox(g_hMainWnd,"���ļ�ʧ�ܻ��ļ���СΪ�㣡��","Borg Message",MB_OK);
		return false;
	}

	g_floader.efile=CreateFile(fname,GENERIC_READ|GENERIC_WRITE,1,NULL,OPEN_EXISTING,0,NULL);	//���ļ�
	if(g_floader.efile==INVALID_HANDLE_VALUE)		//�����ʧ��
	{
		g_floader.efile=CreateFile(fname,GENERIC_READ,1,NULL,OPEN_EXISTING,0,NULL);		//��ֻ����ʽ���´�һ��
		if(g_floader.efile==INVALID_HANDLE_VALUE)		//����ʧ��
			return false;

		g_options.readonly=true;		//��ֻ����ʽ�򿪳ɹ�
		MessageBox(g_hMainWnd,"�޷�����ļ���д��Ȩ��\n�ļ���ֻ���ģ��⽫�޷�������ļ����κ��޸�","Borg Message",MB_OK);
	}

	if(GetFileType(g_floader.efile)!=FILE_TYPE_DISK)	//������Ǵ����ϵ��ļ�����ʧ��
		return false;

	g_floader.exetype=BIN_EXE;
	if(ReadFile(g_floader.efile,mzhead,2,&num,NULL))	//��ȡMZ
	{
		if((num==2)&&(((mzhead[0]=='M')&&(mzhead[1]=='Z'))||	//�ж��Ƿ���PE�ļ���dos��exe�ļ�
			((mzhead[0]=='Z')&&(mzhead[1]=='M'))))
		{
			SetFilePointer(g_floader.efile,0x3c,NULL,FILE_BEGIN);
			if(ReadFile(g_floader.efile,&pe_offset,4,&num,NULL))		//��ȡIMAGE_DOS_HEADER�е�e_lfanew�ֶΣ���ȡ��PE�ļ�ͷ���ļ�ƫ��λ��
				SetFilePointer(g_floader.efile,pe_offset,NULL,FILE_BEGIN);
			if(ReadFile(g_floader.efile,exthead,2,&num,NULL))		//��ȡIMAGE_NT_HEADERS32�е�Signature�ֶΣ��ж��ļ������ͣ�һ�㶼��PE�ļ��˰ɣ�
			{
				if(((short int *)exthead)[0]==0x4550)		//IMAGE_NT_SIGNATURE
					g_floader.exetype=PE_EXE;
				else if(((short int *)exthead)[0]==0x454e)
					g_floader.exetype=NE_EXE;
				else if(((short int *)exthead)[0]==0x454c)
					g_floader.exetype=LE_EXE;
				else if(((short int *)exthead)[0]==0x584c)
					g_floader.exetype=OS2_EXE;
				else
					g_floader.exetype=MZ_EXE;
			}
		}
		else	//δ֪��ʽ��ֱ�ӵ��ɴ�bin�������
		{
			if(strlen(fname)>3)
			{
				if(!lstrcmpi(fname+strlen(fname)-3,"com"))	//�ж��Ƿ���dos��com����
				{
					SetFilePointer(g_floader.efile,0,NULL,FILE_BEGIN);
					g_floader.exetype=COM_EXE;
				}
				else if(!lstrcmpi(fname+strlen(fname)-3,"sys"))	//�ж��Ƿ���sys��sys��Ҳ��PE�ļ�ô����
				{
					SetFilePointer(g_floader.efile,0,NULL,FILE_BEGIN);
					g_floader.exetype=SYS_EXE;
				}
			}
		}
	}

	//������һ�����ļ���С��ͬ�Ļ����������ļ�ȫ������
	g_floader.fbuff=new byte[fsize];
	SetFilePointer(g_floader.efile,0x00,NULL,FILE_BEGIN);
	ReadFile(g_floader.efile,g_floader.fbuff,fsize,&num,NULL);

	//��ʾ�����ļ����õĶԻ���
	DialogBox(g_hInst,MAKEINTRESOURCE(D_checktype),g_hMainWnd,(DLGPROC)CheckOptDlgProc);

	if(!g_options.loadaddr.segm)
	{
		g_options.loadaddr.segm=0x1000;
		MessageBox(g_hMainWnd,"�޷�ʹ��0��Ϊ�λ���ַ��\n��ʹ��Ĭ�϶λ���ַ0x1000","Borg Message",MB_OK);
	}

	g_dsm.dissettable();	//ò�������÷�����õĴ�������
	switch(g_floader.exetype)
	{
	case BIN_EXE:
		g_floader.readbinfile(fsize);
		break;
	case PE_EXE:
		g_floader.readpefile(pe_offset);		//�ô�һ������
		break;
	case MZ_EXE:
		g_floader.readmzfile(fsize);
		break;
	case OS2_EXE:
		g_floader.reados2file();
		CloseHandle(g_floader.efile);
		g_floader.efile=INVALID_HANDLE_VALUE;
		g_floader.exetype=0;
		return false; // at the moment;
	case COM_EXE:
		g_floader.readcomfile(fsize);
		break;
	case SYS_EXE:
		g_floader.readsysfile(fsize);
		break;
	case LE_EXE:
		g_floader.readlefile();
		CloseHandle(g_floader.efile);
		g_floader.efile=INVALID_HANDLE_VALUE;
		g_floader.exetype=0;
		return false; // at the moment;
	case NE_EXE:
		g_floader.readnefile(pe_offset);
		break;
	default:
		CloseHandle(g_floader.efile);
		g_floader.efile=INVALID_HANDLE_VALUE;
		g_floader.exetype=0;
		return false;
	}
	return true;
}

/************************************************************************
* newfile                                                               *
* - handles selecting a new file and its messages, using the standard   *
*   routine GetOpenFileName                                             *
* - starts up the secondary thread when the file is loaded              *
************************************************************************/
bool newfile(void)
{
	// factor of 2 added for nt unicode
	getfiletoload(current_exe_name);	//��ʾ���ļ��Ի��򣬷����ļ�·��
	if(current_exe_name[0])
	{
		if(loadexefile(current_exe_name))
		{
			StatusMessage("File Opened");
			strcat(winname," : ");
			strcat(winname,current_exe_name);
			SetWindowText(g_hMainWnd,winname);
			InThread=true;
			ThreadHandle=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)Thread,0,0,&ThreadId);
			changemenus();
		}
		else
			MessageBox(g_hMainWnd,"File open failed ?",program_name,MB_OK|MB_ICONEXCLAMATION);
	}
	return 0;
}

