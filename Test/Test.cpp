// Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	char szBuffer[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH,szBuffer);
	printf("��ǰĿ¼��%s",szBuffer);

	OutputDebugStringA("ANSI DebugString");
	OutputDebugStringW(L"Unicode Debug String");

	system("pause");
	return 0;
}

