// PlatformProxyService.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPlatformProxyServiceApp:
// �йش����ʵ�֣������ PlatformProxyService.cpp
//

class CPlatformProxyServiceApp : public CWinApp
{
public:
	CPlatformProxyServiceApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()


public:
	CString SZAPPNAME;
	CString SZSERVICENAME;
	CString SZSERVICEDISPLAYNAME;
	CString SZDEPENDENCIES;
};

extern CPlatformProxyServiceApp theApp;