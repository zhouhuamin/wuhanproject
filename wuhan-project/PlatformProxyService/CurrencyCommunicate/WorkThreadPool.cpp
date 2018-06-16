// WorkThreadPool.cpp: implementation of the CWorkThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WorkThreadPool.h"

#include "ModuleInfo.h"
#include "omp.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkThreadPool::CWorkThreadPool():m_exit(false,true),m_signal(false,true)
{
	pf = NULL;
	m_threadNum = 3;
}

CWorkThreadPool::~CWorkThreadPool()
{

}

void CWorkThreadPool::Run()
{
	for(int i=0;i<m_threadNum;i++)
		AfxBeginThread(WORK,this);
}

void CWorkThreadPool::RegisterWorkItem(LPVOID item)
{
	m_lock.Lock();
	m_workItems.Add(item);
	m_lock.Unlock();
//	TRACE("RegisterWorkItem 增加任务\n");

	m_signal.SetEvent();
}

LPVOID CWorkThreadPool::GetNextItem()
{
	LPVOID ret = NULL;

	m_lock.Lock();
	if(m_workItems.GetSize())
		ret = m_workItems[0];
	m_lock.Unlock();

	return ret;
}

void CWorkThreadPool::SetPF(lpCallBack *f)
{
	pf = f;
}

UINT CWorkThreadPool::Work()
{
	DWORD id = GetCurrentThreadId();
	try
	{
		//	ASSERT(pf);
		HANDLE Event[2];
		Event[0] = m_exit;
		Event[1] = m_signal;
	
		while(1)
		{
			Sleep(1);
			
	
			int ret = WaitForMultipleObjects(2,
				Event ,FALSE,INFINITE);	
			switch(ret - WAIT_OBJECT_0 ) 
			{
			case 0:
				{
				m_threadNum-- ;
			//	DWORD id1 = GetCurrentThreadId();
				TRACE("CWorkThreadPool  %d退出", id);
				return 0;
				break;
				}
			case 1:
				{
					m_Threadlock.Lock();
					LPVOID pItem = GetNextIFisrtWorktem();
					if(pItem)
						RemoveFirstWorkItem(pItem);
					else
					{
						pItem = GetNextItem();
						if(pItem)
								RemoveWorkItem(pItem);
					}
					m_Threadlock.Unlock();
				
					if(pItem != NULL)
					{
						CModuleInfo* ModuleInfo = (CModuleInfo*)pItem;

				
						ModuleInfo->OnDealInfo();
		//				TRACE("$$$$$$$$$$$$$$$工作线程处理开始^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		
			
					
						delete ModuleInfo;
						

					
		//				TRACE("$$$$$$$$$$$$$$$工作线程%x处理结束^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n", id);


		//				Sleep(1);
						
						//pf(pItem);
					}else
					{
						m_signal.ResetEvent();
					}
				
				}
				break;
			}
		}
	}
	catch(...)
	{
		TRACE("CWorkThreadPool %d 异常退出", id);
		return -1;
	}
	
	return 0;
	
}

UINT CWorkThreadPool::WORK(LPVOID lp)
{
	CWorkThreadPool* pWork = (CWorkThreadPool*)lp;
	ASSERT(pWork);
	#pragma omp parallel 
	{
	::CoInitialize(NULL);
	pWork->Work();
	::CoUninitialize();
	}
	return 0;
}

void CWorkThreadPool::Stop()
{
	m_signal.ResetEvent();
	m_exit.SetEvent();
	int nCount=0;
	while (m_threadNum)
	{
		nCount++;
		if(nCount>m_threadNum*100)
			break;
		Sleep(10);
	}
}

void CWorkThreadPool::RemoveWorkItem(LPVOID _pItem)
{
	LPVOID pItem = NULL;
	m_lock.Lock();
	for(int i=0;i<m_workItems.GetSize();i++)
	{
		pItem = (LPVOID)m_workItems[i];
		if(_pItem == pItem)
		{
	//		TRACE("RemoveWorkItem 销毁\n");
			m_workItems.RemoveAt(i);
			break;
		}
	}
	m_lock.Unlock();	
}

void CWorkThreadPool::SetThreadNum(int num)
{
	Stop();
	ASSERT(num > 0);
	m_threadNum = num;
}

void CWorkThreadPool::InitThreadInfo(int num)
{
	m_threadNum = num;
}

void CWorkThreadPool::RegisterFirstWorkItem(LPVOID item)
{
	m_firstworklock.Lock();
	m_firstworkintems.Add(item);
	m_firstworklock.Unlock();
	m_signal.SetEvent();
}

LPVOID CWorkThreadPool::GetNextIFisrtWorktem()
{
	LPVOID ret = NULL;

	m_firstworklock.Lock();
	if(m_firstworkintems.GetSize()>0)
		ret = m_firstworkintems[0];
	m_firstworklock.Unlock();

	return ret;
}

void CWorkThreadPool::RemoveFirstWorkItem(LPVOID _pItem)
{
	LPVOID pItem = NULL;
	m_firstworklock.Lock();
	for(int i=0;i<m_firstworkintems.GetSize();i++)
	{
		pItem = (LPVOID)m_firstworkintems[i];
		if(_pItem == pItem)
		{
	//		TRACE("RemoveWorkItem 销毁\n");
			m_firstworkintems.RemoveAt(i);
	//		break;
		}
	}
	m_firstworklock.Unlock();	
}

int CWorkThreadPool::GetWorkItemCount()
{
	int nCount=0;
	m_lock.Lock();
	nCount=m_workItems.GetSize();
	m_lock.Unlock();
	return nCount;
}
