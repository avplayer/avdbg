#include "StdAfx.h"
#include "OutputWnd.h"


COutputWnd::COutputWnd(void)
{
}


COutputWnd::~COutputWnd(void)
{
}

BOOL COutputWnd::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_CHAR
		||pMsg->message == WM_KEYDOWN
		||pMsg->message == WM_KEYUP)		//�������û����룬���˵����м�����Ϣ
	{
		return TRUE;
	}
	return FALSE;
}