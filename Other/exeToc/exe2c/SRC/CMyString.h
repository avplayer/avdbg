// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	CMyString.h
#ifndef	CMyString_H
#define	CMyString_H

const	int MEMORY_STEP	= 2048;	// ÿ�������ڴ�ռ�Ĵ�С

class CMyString
{
	UINT	m_maxlen;	// ���������ڴ�ռ��Ĵ�С
	UINT	m_curlen;	// ��ǰ���ݳ���
	PSTR	m_bufptr;	// ��ǰ���ݻ�����

public:
	CMyString();
	~CMyString();
	
	void strcat(PSTR str);	// ׷������
	void Clear();
	UINT	GetLength();	// ��ȡ��ǰ���ݳ���
	PSTR	GetString();	// ��ȡ����
	PSTR	GetWritableString();	// ��ȡ��д������
};

#endif	//	CMyString_H
