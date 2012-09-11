// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com
//------------------------//
//
//  ע�����ڣ�20110615
//  �� д �ߣ��쳾����
//  ��    �ܣ�����INCĿ¼�е�.h�ļ�����
//  ��    �����𲽷���ÿһ���ַ�������ע�͡����з�������
//
//------------------------//

#include "stdafx.h"
#include "Public_Fun.h"
#include "CCbuf.h"

#include <io.h>

//	----------------------------------------------------------
CCbuf::CCbuf()
{
	f_str = 0;
}

CCbuf::~CCbuf()
{

}

void CCbuf::LoadFile(FILE *f)
{
	long flen = _filelength(_fileno(f));	// �ļ�����
	char *buf = new char[flen];
	fread(buf,1,flen,f);				// �ļ����뻺����
	
	m_p = new char[flen+1];				//thats enough
	m_len = 0;

	for (long i = 0; i < flen; i++)		// ��ȡÿһ���ֽ�
	{
		OneChar(buf[i]);
	}
	OneChar(EOF);

	SAFE_DELETE(buf);

	assert((long)m_len <= flen+1);
}

// ����ע������
// ������
void CCbuf::OneChar(int c3)
{	
	if (c3 == '\r')
		c3 = '\n';

	//	c3 �� input char �� EOF
	//	�����Ч�����c3 �ӵ�p��ָ��buf��ͬʱ len++

	int i = m_len;	// �ļ���ǰ����λ��
	PSTR p = m_p;

	if (c3 == EOF)	// �ļ�����
	{
		assert(f_str == 0);
		OneChar(0);
		return;
	}

	char c = (char)c3;

	// ----��һ�������Ե���ע�������ע������----
	if ( f_str == 0 && i >= 2)	
	{
		if (p[i-2] == '/' && p[i-1] == '/')	// ������ע��
		{
			if (c == '\n' || c == 0)	// һֱ������
			{
				m_len -= 2;	// �ѵ���ע�ͷ������
				OneChar(0);
				return;
			}
			else
				return;	 // ����ע�͡�//��������ݣ����ñ��浽pָ����ڴ��У�ֱ�Ӻ��ԣ���������
		}

		if (p[i-2] == '/' && p[i-1] == '*')	// �������ע��
		{
			static char c1 = 0;
			if (c1 == '*' && c == '/')		// ע�ͽ���
			{
				m_len -= 2;
				return;
			}
			c1 = c;		// ���浽c1�У�����*����
			return;
		}
	}

	// --- �ڶ����������ַ���-----
	if ( f_str )	// �����Ƿ����ַ�����
	{
	
		if (c == 0 || c == '\n')	// �ַ����ڲ��ܻ���
			assert(("error",0));

		static BOOL f = FALSE;	//	for '\\'

		if (f)
		{
			f = FALSE;
		}
		else
		{
			if (c == '\\')		// ת���ַ������һ���ַ��������� \t
				f = TRUE;
			if (c == f_str)		// �ַ������
				f_str = 0;
		}
	}
	else
	{
		if (c == '\'' || c == '\"')
			f_str = c;			// �����Ϊ�ַ���
	}
	
	
	switch (c)
	{
	case 0:
	case '\n':
		if(i==0)	// ��һ���ַ�Ϊ���У������
			break;
		p[i] = 0;
		m_len++;
		if (i > 0)
		{
			//	��ǰ��Ŀո�����г���
			if (p[i-1] == ' ' || p[i-1] == '\0')
			{
				p[i-1] = 0;
				m_len--;
				return;
			}
			//	���ǰ���һ���ַ����ǲ��ǹ��з�
			if ( p[i-1] == '\\')
			{
				m_len -= 2;
				return;		//	������
			}
		}
		break;
	case '\t':
	case ' ':
		if (i == 0 || p[i-1] == '\0')	// ����һ�ַ� ���� ǰ�������
			break;

		if (f_str == 0 && p[i-1] == ' ')
			break;	//	���ǰ���Ѿ���һ���ո񣬾�����

		c = ' ';
		//continue to next
	default:		// ����������ģ�ֱ�Ӹ���
		p[i] = c;
		m_len++;
		break;
	}
}

