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
		MessageBox(L"�������ִ���ļ���·��");
		return;
	}
	GetDlgItemText(IDC_EDITPARAM,m_strParam);
	CDialogEx::OnOK();
}


void CFileOpenDlg::OnBnClickedButtonbrowse()
{
	CFileDialog	dlg(TRUE,L".exe",NULL,OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_FORCESHOWHIDDEN | OFN_HIDEREADONLY,L"��ִ���ļ� (*.exe)|*.exe|�����ļ� (*.*)|*.*||",this);
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	SetDlgItemText(IDC_EDITFILEPATH,dlg.GetPathName());
}
