// CurrencyCommunicate.h : CurrencyCommunicate DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCurrencyCommunicateApp
// �йش���ʵ�ֵ���Ϣ������� CurrencyCommunicate.cpp
//

class CCurrencyCommunicateApp : public CWinApp
{
public:
	CCurrencyCommunicateApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
