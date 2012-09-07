// FileOpenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KillDBG.h"
#include "FileOpenDlg.h"
#include "afxdialogex.h"


// CFileOpenDlg �Ի���

IMPLEMENT_DYNAMIC(CFileOpenDlg, CDialogEx)

CFileOpenDlg::CFileOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileOpenDlg::IDD, pParent)
{

}

CFileOpenDlg::~CFileOpenDlg()
{
}

void CFileOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileOpenDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFileOpenDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONBROWSE, &CFileOpenDlg::OnBnClickedButtonbrowse)
END_MESSAGE_MAP()


// CFileOpenDlg ��Ϣ�������


void CFileOpenDlg::OnBnClickedOk()
{
	if (!GetDlgItemText(IDC_EDITFILEPATH,m_strPath))
	{
		MessageBox(_T("�������ִ���ļ���·��"));
		return;
	}
	GetDlgItemText(IDC_EDITPARAM,m_strParam);
	GetDlgItemText(IDC_EDITRUNDIR,m_strRunDir);
	CDialogEx::OnOK();
}


void CFileOpenDlg::OnBnClickedButtonbrowse()
{
	CFileDialog	dlg(TRUE,_T(".exe"),NULL,OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY,
		_T("��ִ���ļ� (*.exe)|*.exe|�����ļ� (*.*)|*.*||"),this);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CString strExePath(dlg.GetPathName());
	SetDlgItemText(IDC_EDITFILEPATH,strExePath);

	SetDlgItemText(IDC_EDITRUNDIR,strExePath.Left(strExePath.ReverseFind('\\')+1));
}
