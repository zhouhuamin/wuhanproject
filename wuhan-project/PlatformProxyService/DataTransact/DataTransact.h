// DataTransact.h : DataTransact DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDataTransactApp
// �йش���ʵ�ֵ���Ϣ������� DataTransact.cpp
//

class CDataTransactApp : public CWinApp
{
public:
	CDataTransactApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
