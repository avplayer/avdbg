#pragma once


// CFileOpenDlg �Ի���

class CFileOpenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileOpenDlg)

public:
	CFileOpenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFileOpenDlg();

// �Ի�������
	enum { IDD = IDD_DIALOGOPEN };

	CString	m_strPath;
	CString	m_strParam;
	CString m_strRunDir;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonbrowse();
};
