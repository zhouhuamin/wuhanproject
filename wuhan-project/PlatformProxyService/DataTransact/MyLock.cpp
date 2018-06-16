// MyLock.cpp: implementation of the CMyLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "datatransact.h"
#include "MyLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyLock::CMyLock(CRITICAL_SECTION& cs, const CString& strFunc)
{
	m_strFunc = strFunc;
	m_pcs = &cs;
	Lock();
}

CMyLock::~CMyLock()
{
	Unlock();
}

void CMyLock::Lock()
{
	TRACE(_T("EC %d %s\n") , GetCurrentThreadId(), m_strFunc);
	EnterCriticalSection(m_pcs);
}

void CMyLock::Unlock()
{
	LeaveCriticalSection(m_pcs);
	TRACE(_T("LC %d %s\n") , GetCurrentThreadId() , m_strFunc);
}
