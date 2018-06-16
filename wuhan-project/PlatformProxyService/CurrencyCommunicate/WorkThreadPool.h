// WorkThreadPool.h: interface for the CWorkThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKTHREADPOOL_H__30AF8906_EE6D_4FAD_B0F3_5F4F8AEFB15E__INCLUDED_)
#define AFX_WORKTHREADPOOL_H__30AF8906_EE6D_4FAD_B0F3_5F4F8AEFB15E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxmt.h"

typedef void lpCallBack (LPVOID lpParam);

class CWorkThreadPool  
{
public:
	CWorkThreadPool();
	virtual ~CWorkThreadPool();

public:
	int GetWorkItemCount();
	void RemoveFirstWorkItem(LPVOID _pItem);
	LPVOID GetNextIFisrtWorktem();
	void RegisterFirstWorkItem(LPVOID item);
	void InitThreadInfo(int num = 3);
	void SetThreadNum(int num = 3);
	void Stop();
	void SetPF(lpCallBack *f);
	void RegisterWorkItem(LPVOID item);
	void Run();
	
private:
	static UINT WORK(LPVOID lp);
	void RemoveWorkItem(LPVOID _pItem);
	UINT Work();
	LPVOID GetNextItem();

private:
	CEvent m_exit;
	CCriticalSection m_lock, m_Threadlock,m_firstworklock;
	CEvent m_signal;
	CPtrArray m_workItems,m_firstworkintems;
	lpCallBack *pf;
	int m_threadNum;
};

#endif // !defined(AFX_WORKTHREADPOOL_H__30AF8906_EE6D_4FAD_B0F3_5F4F8AEFB15E__INCLUDED_)
