// MyLock.h: interface for the CMyLock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLOCK_H__64FB80B1_5639_4565_8FEE_6DB5669678A2__INCLUDED_)
#define AFX_MYLOCK_H__64FB80B1_5639_4565_8FEE_6DB5669678A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <process.h>
class CMyLock  
{
public:
	void Unlock();
	void Lock();
	CMyLock(CRITICAL_SECTION& cs, const CString& strFunc);
	virtual ~CMyLock();
protected:
	CRITICAL_SECTION*	m_pcs;
	CString				m_strFunc;


};

#endif // !defined(AFX_MYLOCK_H__64FB80B1_5639_4565_8FEE_6DB5669678A2__INCLUDED_)
