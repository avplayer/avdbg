// Copyright(C) 1999-2005 LiuTaoTao��bookaa@rorsoft.com

#pragma once

// �ѳ����ļ����뻺����

class CCbuf
{
public:
	char f_str;		// ��Ϊ��'��"����string��ʽ
	PSTR m_p;		// ���ݻ�����
	SIZEOF	m_len;

	CCbuf();
	~CCbuf();

	void LoadFile(FILE *f);
	void OneChar(int c);

};

