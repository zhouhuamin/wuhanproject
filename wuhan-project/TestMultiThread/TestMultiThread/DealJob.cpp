#include "StdAfx.h"
#include "DealJob.h"

int CDealJob::m_nSequenceNo = 0;

CDealJob::CDealJob(void)
{
	m_bAcceptJobs	= TRUE;
	m_nOfWorkers	= 4;
	m_bServerStarted= FALSE;
	m_bShutDown		= FALSE;
}

CDealJob::~CDealJob(void)
{
}

UINT CDealJob::WorkerThreadProc(LPVOID pParam)
{
	CDealJob* pPoolServer= reinterpret_cast<CDealJob*>(pParam);
	CWinThread* pThis=NULL;
	if(pPoolServer)
		pThis=pPoolServer->GetWorker((WORD)::GetCurrentThreadId());

	if(pThis)
	{
		TRACE("Thread %i is alive.\r\n",::GetCurrentThreadId());
		JobItem *pJob=NULL;
		while(pThis->m_hThread!=INVALID_HANDLE_VALUE)	
		{
			pJob=NULL;	
			pJob=pPoolServer->GetJob();
			if(pJob) 
			{
				pPoolServer->ProcessJob(pJob,pPoolServer);
				pPoolServer->FreeJob(pJob);
			}else
				::SuspendThread(::GetCurrentThread());
		}


	}
	TRACE("Thread %i RIP.\r\n",::GetCurrentThreadId());
	return 0x01;
}

CWinThread* CDealJob::GetWorker(WORD WorkerID)
{
	CWinThread* pWorker=NULL;
	m_WorkerThreadMapLock.Lock();
	pWorker=(CWinThread*)m_WorkerThreadMap[WorkerID];
	m_WorkerThreadMapLock.Unlock();
	return pWorker;
}

JobItem* CDealJob::GetJob()
{
	JobItem* pJob=NULL;
	m_JobQueueLock.Lock();
	if(!m_JobQueueList.IsEmpty())
		pJob=(JobItem*)m_JobQueueList.RemoveTail();
	m_JobQueueLock.Unlock();
	return pJob; 
}

void CDealJob::ProcessJob(JobItem *pJob, CDealJob* pServer)
{
}

void CDealJob::FreeJob(JobItem *pJob)
{
	if(pJob)
	{
		delete pJob;
		pJob = NULL;
	}
}

BOOL CDealJob::AddJob(JobItem *pJob)
{
	BOOL bRetur=FALSE;
	if(m_bAcceptJobs&&pJob!=NULL)
	{
		m_JobQueueLock.Lock();
		bRetur= (m_JobQueueList.AddHead((void*)pJob)!=NULL);
		m_JobQueueLock.Unlock();
		//
		// Wake someone upp to do the job..
		if(bRetur)
		{
			m_WorkerThreadMapLock.Lock();
			POSITION pos = m_WorkerThreadMap.GetStartPosition ();
			while (pos != NULL) 
			{
				WORD strKey;
				CWinThread* pThread=NULL;
				m_WorkerThreadMap.GetNextAssoc (pos, strKey,(void *&)pThread);
				if(pThread)
				{
					if(pThread->ResumeThread()==1) // Some one wake up. 
						break;
				}	
			}
			m_WorkerThreadMapLock.Unlock();

		}
		return bRetur; 
	}
	return FALSE;
}


BOOL CDealJob::SetWorkers(int nThreads)
{
	int iNumberToKill=0;
	int iNumberToStart=0;

	m_WorkerThreadMapLock.Lock();
	int iNumberOfThreads = m_WorkerThreadMap.GetCount();
	m_WorkerThreadMapLock.Unlock();

	if(nThreads<iNumberOfThreads)
		iNumberToKill=iNumberOfThreads-nThreads;
	else
		iNumberToStart=nThreads-iNumberOfThreads;

	// No interference while admin the threads. 
	BOOL bAcceptJobs=m_bAcceptJobs;
	m_bAcceptJobs=FALSE;





	//
	// if nThreads is bigger than our current thread count, remove all excess threads
	//

	//
	// Kill some of the workers. 
	// ej helt klart... 

	m_WorkerThreadMapLock.Lock();
	POSITION pos = m_WorkerThreadMap.GetStartPosition ();
	while (pos != NULL&&iNumberToKill>0) 
	{
		WORD strKey;
		CWinThread* pThread=NULL;
		m_WorkerThreadMap.GetNextAssoc (pos, strKey,(void *&)pThread);
		if(pThread)
		{

			HANDLE hThread = pThread->m_hThread;

			// notify the thread that it should die. 
			pThread->m_hThread=INVALID_HANDLE_VALUE;  
			// now let the thread terminate itself

			//::GetExitCodeThread(hThread, &dwExit) && (dwExit != 0xdead)

			::ResumeThread(hThread);


			DWORD dwExit = NULL;
			while(::GetExitCodeThread(hThread, &dwExit) && (dwExit != 0x01))
			{
				::Sleep(50);	// give it a chance to finish
			}
			::CloseHandle (hThread);
			iNumberToKill--;
			m_WorkerThreadMap.RemoveKey(strKey);
			delete[] pThread;
		}	
	}
	m_WorkerThreadMapLock.Unlock();

	//
	// Start some Workers. 
	//
	m_WorkerThreadMapLock.Lock();
	while (iNumberToStart>0) 
	{
		CWinThread* pWorkerThread=AfxBeginThread(CDealJob::WorkerThreadProc, (void*)this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);  
		pWorkerThread->m_bAutoDelete=FALSE;  
		if(pWorkerThread)
		{
			pWorkerThread->ResumeThread();
			m_WorkerThreadMap[(WORD)pWorkerThread->m_nThreadID]=(void*)pWorkerThread;
			iNumberToStart--;
		}else
			return FALSE;
	}

	m_WorkerThreadMapLock.Unlock();
	m_bAcceptJobs=bAcceptJobs;
	return TRUE;
}

BOOL CDealJob::Startup()
{
	BOOL bRet=TRUE;
	CString msg;

	if ( !m_bServerStarted )
	{
		m_bShutDown=FALSE;

		// Start the logical Workers. (SetWorkes can be callen in runtime..). 
		bRet&=SetWorkers(m_nOfWorkers);

		if ( bRet )
		{
			msg.Format("Successfully started %i logical Worker(s).\n",m_nOfWorkers);
			TRACE(msg);
		}

		// Accept incoming Job. 
		m_bAcceptJobs=TRUE;
		m_bServerStarted=TRUE;

	}

	return bRet; 	
}

BOOL CDealJob::Start(int nOfWorkers)
{
	m_nOfWorkers=nOfWorkers;
	m_bShutDown=FALSE;
	return Startup();
}

void CDealJob::ShutDown()
{
	CString msg;
	if(m_bServerStarted)
	{
		TRACE("Shutdown initialized.");

		TRACE("Sending shutdown signal to logical worker threads.");
		ShutDownWorkers();

		m_bShutDown=TRUE;
		m_bServerStarted=FALSE;
	}
}

void CDealJob::ShutDownWorkers()
{
	//Close The Workers. 
	m_bAcceptJobs=FALSE;
	SetWorkers(0);
	m_WorkerThreadMapLock.Lock();
	m_WorkerThreadMap.RemoveAll();
	m_WorkerThreadMapLock.Unlock();

	// Empty the JobQueue. 

	JobItem *pJob=NULL;
	m_JobQueueLock.Lock();
	POSITION pos = m_JobQueueList.GetHeadPosition ();
	while (pos != NULL) 
	{
		pJob = (JobItem *)m_JobQueueList.GetNext (pos);
		FreeJob(pJob);
	}
	m_JobQueueList.RemoveAll();
	m_JobQueueLock.Unlock();

}

CString CDealJob::GetNewFileName()
{
	CString str;
	static int nSequenceNo = 0;

	m_SequenceLock.Lock();
	nSequenceNo += 1;
	m_SequenceLock.Unlock();

	str.Format("%d",nSequenceNo);

	int len = str.GetLength();
	for(int i = 0; i < 6 - len; i++)
	{
		str="0" + str;
	}

	if (len > 6)
	{
		str = str.Left(6);
	}

	CString strCurTime;
	CTime tm = CTime::GetCurrentTime();
	strCurTime = tm.Format("%Y%m%d%H%M%S");
	str = strCurTime + str;
	return str;
}

