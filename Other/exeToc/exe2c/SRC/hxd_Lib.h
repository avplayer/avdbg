#pragma once

//bool check_if_API(BYTE* dwAddr);	// �ж��Ƿ�����ת��API

void OutToTxt(const char * szText);	// ��Ϣ���浽��c:\\OutTxt.txt���ļ���

void nop();	// �պ���

PSTR GetMyExePath();	// Exe2C�����·��

void lib_init();
void lib_exit();

PSTR my_itoa(int i);
