
// Filter.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CFilterApp:
// � ���������� ������� ������ ��. Filter.cpp
//

class CFilterApp : public CWinApp
{
public:
	CFilterApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CFilterApp theApp;