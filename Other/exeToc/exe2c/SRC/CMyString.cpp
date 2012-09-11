// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

//	CMyString.cpp

#include "stdafx.h"
#include "Public_Fun.h"
#include "CMyString.h"

CMyString::CMyString()	// ��ʼ������
{
	this->m_bufptr = NULL;
	this->m_curlen = 0;
	this->m_maxlen = 0;
}

CMyString::~CMyString()
{
	this->Clear();
}

void CMyString::Clear()
{
	if (this->m_bufptr)
	{
		delete this->m_bufptr;
	}
	this->m_bufptr = NULL;
	this->m_curlen = 0;
	this->m_maxlen = 0;
}

void CMyString::strcat(PSTR str)
{
	UINT len = strlen(str);
	while (m_curlen + len + 1 > m_maxlen)	// ���������ڴ�ռ�
	{
		PSTR pnew = new char[m_maxlen + MEMORY_STEP];
		memcpy(pnew, m_bufptr, m_maxlen);
		delete m_bufptr;
		m_bufptr = pnew;
		m_maxlen += MEMORY_STEP;
	}
	memcpy(m_bufptr + m_curlen, str, len+1);	//	include last EOS
	m_curlen += len;
}

PSTR	CMyString::GetString()
{
	if (m_bufptr != NULL)
        return m_bufptr;
    static char* p = "";
    return p;
}

PSTR	CMyString::GetWritableString()
{
	if (m_bufptr != NULL)
        return m_bufptr;

    char* p = "";
    return p;

}

UINT	CMyString::GetLength()
{
	return this->m_curlen;
}
