// TestMultiThread.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestMultiThreadApp:
// �йش����ʵ�֣������ TestMultiThread.cpp
//

class CTestMultiThreadApp : public CWinApp
{
public:
	CTestMultiThreadApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestMultiThreadApp theApp;