// KillDBG.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "KillDBG.h"
#include "MainFrm.h"
#include <Scintilla.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKillDBGApp

BEGIN_MESSAGE_MAP(CKillDBGApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CKillDBGApp::OnAppAbout)
	//	ON_COMMAND(ID_BUTTONDISWND, &CKillDBGApp::OnButtondiswnd)
END_MESSAGE_MAP()


// CKillDBGApp construction

CKillDBGApp::CKillDBGApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CKillDBGApp object

CKillDBGApp theApp;


// CKillDBGApp initialization

BOOL CKillDBGApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	//��ʼ��Scintilla
	Scintilla_LinkLexers();
	Scintilla_RegisterClasses(AfxGetApp()->m_hInstance);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("KillDBG"));
	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object

	//�������� Debug Privilege
	HANDLE hProcess=GetCurrentProcess();
	HANDLE hToken;
	BOOL bRet=FALSE;

	if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		LUID luid;

		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			TOKEN_PRIVILEGES tp;

			tp.PrivilegeCount=1;
			tp.Privileges[0].Luid=luid;
			tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(hToken, FALSE, &tp, NULL, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
			{
				bRet=(GetLastError() == ERROR_SUCCESS);
			}
		}
		CloseHandle(hToken);
	}

	if (!bRet)
	{
		MessageBox(NULL,_T("��������Ȩ�޵�SE_DEBUG_NAMEʧ�ܣ����ܻᵼ�²��ֽ����޷�����"),NULL,MB_OK | MB_ICONWARNING);
	}

	//����������
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,NULL);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


// CKillDBGApp message handlers




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CKillDBGApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CKillDBGApp message handlers



//void CKillDBGApp::OnButtondiswnd()
//{
//	MessageBox(NULL,_T("Dissasmbly"),NULL,MB_OK);
//}


int CKillDBGApp::ExitInstance()
{
	// �ͷ�Scintilla
	Scintilla_ReleaseResources();

	return CWinApp::ExitInstance();
}
