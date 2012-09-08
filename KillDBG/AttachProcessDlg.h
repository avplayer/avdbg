#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CAttachProcessDlg �Ի���

class CAttachProcessDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAttachProcessDlg)

public:
	CAttachProcessDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAttachProcessDlg();

// �Ի�������
	enum { IDD = IDD_DIALOGATTACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListProcess;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_BtnOK;
	CButton m_BtnCancel;
	DWORD	m_dwPID;
	afx_msg void OnNMDblclkListprocess(NMHDR *pNMHDR, LRESULT *pResult);
};
