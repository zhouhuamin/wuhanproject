#include "StdAfx.h"
#include "DealJob.h"
#include "CLM10041.h"
#include "CLM10011.h"
#include "CLMNullOrErrorObject.h"
#include "tinyxml/tinyxml.h"
#include "MessageType.h"
#include "MT2101.h"
#include "MT3101.h"
#include "MT4101.h"
#include "MT5101.h"
#include "MT5102.h"
#include "MTXXXX.h"
#include "CSM001.h"
#include "CSM002.h"
#include "CSM003.h"
#include "CSM004.h"
#include "CSM005.h"
#include "CSM006.h"
#include "CSM007.h"
#include "CSM301.h"
#include "CLM10012.h"
#include "CLM10021.h"
#include "CLM10022.h"
#include "CLM10031.h"
#include "CLM10032.h"
#include "CLM10051.h"
#include "CLM10052.h"
#include "CLM10061.h"
#include "CLM10062.h"
#include "CLM20011.h"
#include "CLM20012.h"
#include "CLM20013.h"
#include "CLM20014.h"
#include "CLM20021.h"
#include "CLM20022.h"
#include "CLM40011.h"
#include "CLM40012.h"
#include "CLM40021.h"
#include "CLM40022.h"
#include "CLM40031.h"
#include "CLM40032.h"
#include "CLM50011.h"
#include "CLM50012.h"
#include "CLM50031.h"
#include "CLM50032.h"
#include "CLM50033.h"
#include "CLM50034.h"
#include "CLM50041.h"
#include "CLM50042.h"
#include "CLM60011.h"
#include "CLM60012.h"
#include "CLM60013.h"
#include "CLM60014.h"

#include "CLM90011.h"
#include "CLM90012.h"
#include "CLM90021.h"
#include "CLM90022.h"
#include "CLM90031.h"
#include "CLM90032.h"
#include "CLM90041.h"
#include "CLM90042.h"
#include "CLM10042.h"
#include "REL10010.h"
#include "CLM32101.h"



#include <memory>
#include <string>
#include <algorithm>

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
	JobItem *pTmpJob = pJob;
	if (pTmpJob == NULL)
		return;

	CString strTypeTmp	= pTmpJob->m_strType;
	CString strLeft		= strTypeTmp.Left(3);

	std::string strPreFlag = "";
	std::string strMessageType = "";
	if (strLeft == "WEB")
	{
		strPreFlag = "WEB";
		//CString strLeft2 = strTypeTmp.Left(strTypeTmp.ReverseFind('-'));
		CString strMid = strTypeTmp.Mid(strTypeTmp.Find('-') + 1);
		strMessageType = (LPCTSTR)strMid;
	}
	else
	{
		strPreFlag = "";
		strMessageType = (LPCTSTR)pTmpJob->m_strType;
	}

	//std::string strMessageType((LPCTSTR)pTmpJob->m_strType);
	std::string strXmlData = (LPCTSTR)pTmpJob->m_Data;


	{
		strTypeTmp = "";

		CString strFileNameTmp = pTmpJob->m_Data;
		if (strTypeTmp.IsEmpty())
		{
			GetMessageType(strFileNameTmp, strTypeTmp);
		}

		if (strTypeTmp.IsEmpty())
		{
			GetMessageTypeMT(strFileNameTmp, strTypeTmp);
		}
		if (strTypeTmp.IsEmpty())
		{
			GetMessageTypeCSM(strFileNameTmp, strTypeTmp);
		}

		if (strTypeTmp.IsEmpty())
		{
			GetMessageTypeKy(strFileNameTmp, strTypeTmp);
		}

		if (strTypeTmp.IsEmpty())
		{
			return;
		}

		strMessageType = strTypeTmp;
	}





	std::vector<std::string> requestVect;
	requestVect.push_back("CLM20021");
	requestVect.push_back("CLM20031");
	requestVect.push_back("CLM20014");
	requestVect.push_back("CLM30011");
	requestVect.push_back("CLM40011");
	requestVect.push_back("CLM40021");
	requestVect.push_back("CLM40031");
	requestVect.push_back("CLM50012");
	requestVect.push_back("CLM50031");
	requestVect.push_back("CLM50034");
	requestVect.push_back("CLM50041");
	requestVect.push_back("CLM60011");
	requestVect.push_back("CLM60014");
	requestVect.push_back("CLM90011");
	requestVect.push_back("CLM90021");
	requestVect.push_back("CLM90032");
	requestVect.push_back("CLM90041");
	requestVect.push_back("CLM90052");
	requestVect.push_back("CLM80012");
	requestVect.push_back("CLM20024");
	requestVect.push_back("CLM20034");
	requestVect.push_back("CLM74102");

	std::vector<std::string> responseVect;
	responseVect.push_back("CLM20022");
	responseVect.push_back("CLM20032");
	responseVect.push_back("CLM20013");
	responseVect.push_back("CLM30012");
	responseVect.push_back("CLM40012");
	responseVect.push_back("CLM40022");
	responseVect.push_back("CLM40032");
	responseVect.push_back("CLM50011");
	responseVect.push_back("CLM50032");
	responseVect.push_back("CLM50033");
	responseVect.push_back("CLM50042");
	responseVect.push_back("CLM60012");
	responseVect.push_back("CLM60013");
	responseVect.push_back("CLM90012");
	responseVect.push_back("CLM90022");
	responseVect.push_back("CLM90031");
	
	responseVect.push_back("CLM90042");
	responseVect.push_back("CLM90051");
	responseVect.push_back("CLM80011");
	responseVect.push_back("CLM20023");
	responseVect.push_back("CLM20033");
	responseVect.push_back("LHT10010");
	responseVect.push_back("CLM24101");


	std::vector<std::string>::iterator requestIter = std::find(requestVect.begin(), requestVect.end(), strMessageType);

	if (requestIter != requestVect.end())
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM20021());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
		return;
	}

	std::vector<std::string>::iterator responseIter = std::find(responseVect.begin(), responseVect.end(), strMessageType);
	if (responseIter != responseVect.end())
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM20022());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
		return;
	}



	if (strMessageType == CMessageType::MT2101)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CMT2101());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::MT3101)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CMT3101());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::MT4101)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CMT4101());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::MT5101)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CMT5101());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::MT5102)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CMT5102());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::MTXXXX)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CMTXXXX());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM001)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM001());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM002)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM002());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM003)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM003());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM004)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM004());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM005)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM005());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM006)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM006());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM007)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM007());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CSM301)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCSM301());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10041)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10041());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10042)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10042());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10011)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10011());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10012)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10012());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10021)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10021());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10022)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10022());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10031)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10031());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10032)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10032());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10051)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10051());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10052)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10052());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10061)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10061());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM10062)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM10062());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM20011)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM20011());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM20012)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM20012());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == CMessageType::CLM32101)
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLM32101());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	else if (strMessageType == "REL10010")
	{
		std::auto_ptr<CCLMBasic> apBasic(new CREL10010());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}
	//else if (strMessageType == CMessageType::CLM20013)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM20013());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM20014)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM20014());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM40011)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM40011());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM40012)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM40012());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM40021)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM40021());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM40022)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM40022());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM40031)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM40031());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM40032)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM40032());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50011)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50011());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50012)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50012());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50031)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50031());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50032)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50032());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50033)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50033());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50034)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50034());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50041)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50041());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM50042)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM50042());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM60011)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM60011());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM60012)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM60012());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM60013)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM60013());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM60014)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM60014());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90011)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90011());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90012)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90012());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90021)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90021());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90022)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90022());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90031)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90031());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90032)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90032());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90041)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90041());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	//else if (strMessageType == CMessageType::CLM90042)
	//{
	//	std::auto_ptr<CCLMBasic> apBasic(new CCLM90042());
	//	apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	//}
	else
	{
		std::auto_ptr<CCLMBasic> apBasic(new CCLMNullOrErrorObject());
		apBasic->ProcessMessage(&m_config, strXmlData, strPreFlag);
	}

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
	int iNumberOfThreads = (int)m_WorkerThreadMap.GetCount();
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

void CDealJob::InitConfigInfo(structConfigInfo *pConfig)
{
	m_config.strQueueIP		= pConfig->strQueueIP;
	m_config.strQueueName	= pConfig->strQueueName;
	m_config.strQueueLabel	= pConfig->strQueueLabel;

	m_config.strWebQueueIP		= pConfig->strWebQueueIP;
	m_config.strWebQueueName	= pConfig->strWebQueueName;
	m_config.strWebQueueLabel	= pConfig->strWebQueueLabel;

	m_config.strFrontQueueIP		= pConfig->strFrontQueueIP;
	m_config.strFrontQueueName		= pConfig->strFrontQueueName;
	m_config.strFrontQueueLabel		= pConfig->strFrontQueueLabel;

	m_config.strBackerQueueIP		= pConfig->strBackerQueueIP;
	m_config.strBackerQueueName		= pConfig->strBackerQueueName;
	m_config.strBackerQueueLabel	= pConfig->strBackerQueueLabel;

	m_config.strSqlServerDataUser	= pConfig->strSqlServerDataUser;
	m_config.strSqlServerDataPwd	= pConfig->strSqlServerDataPwd;
	m_config.strSqlServerDataName	= pConfig->strSqlServerDataName;
	m_config.strSqlServerDataServer	= pConfig->strSqlServerDataServer;
	m_config.strFilePath			= pConfig->strFilePath;
	m_config.strErrorFilePath		= pConfig->strErrorFilePath;

}

void CDealJob::GetMessageType(const CString &strFileName, CString &strMessageType)
{
	try
	{
		if (strFileName.IsEmpty())
			return;

		std::string strFileName((LPCTSTR)strFileName);
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlNode*    node = 0;
		TiXmlElement* itemElement = 0;
		TiXmlElement* xmlRootElement = NULL;
		TiXmlElement* xmlListElement = NULL;
		TiXmlElement* xmlSubElement = NULL;
		TiXmlElement* xmlSonElement = NULL;
		TiXmlNode * pNode = NULL;
		TiXmlNode *pNodeTmp = NULL;

		pNode = doc.FirstChild("MESSAGE");
		if (pNode == NULL)
			return ;
		xmlRootElement = pNode->ToElement();
		if(xmlRootElement)
		{
			if ((pNode = xmlRootElement->FirstChild("MESSAGE_HEAD")) != NULL)
			{
				std::string MESSAGE_TYPE = "";

				xmlSubElement = pNode->ToElement() ;
				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MESSAGE_TYPE");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_TYPE = pNodeTmp->ToElement()->GetText();
				strMessageType = MESSAGE_TYPE.c_str();
			}
		}
	}
	catch(...)
	{
		return ;
	}
	return ;
}

void CDealJob::GetMessageTypeMT(const CString &strFileName, CString &strMessageType)
{
	try
	{
		if (strFileName.IsEmpty())
			return;

		std::string strFileName((LPCTSTR)strFileName);
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlNode*    node = 0;
		TiXmlElement* itemElement = 0;
		TiXmlElement* xmlRootElement = NULL;
		TiXmlElement* xmlListElement = NULL;
		TiXmlElement* xmlSubElement = NULL;
		TiXmlElement* xmlSonElement = NULL;
		TiXmlNode * pNode = NULL;
		TiXmlNode *pNodeTmp = NULL;

		pNode = doc.FirstChild("Manifest");
		if (pNode == NULL)
			return ;
		xmlRootElement = pNode->ToElement();
		if(xmlRootElement)
		{
			if ((pNode = xmlRootElement->FirstChild("Head")) != NULL)
			{
				std::string MESSAGE_TYPE = "";

				xmlSubElement = pNode->ToElement() ;
				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MessageType");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_TYPE = pNodeTmp->ToElement()->GetText();
				strMessageType = MESSAGE_TYPE.c_str();
			}
		}
	}
	catch(...)
	{
		return ;
	}
	return ;
}

void CDealJob::GetMessageTypeCSM(const CString &strFileName, CString &strMessageType)
{
	try
	{
		if (strFileName.IsEmpty())
			return;

		std::string strFileName((LPCTSTR)strFileName);
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlNode*    node = 0;
		TiXmlElement* itemElement = 0;
		TiXmlElement* xmlRootElement = NULL;
		TiXmlElement* xmlListElement = NULL;
		TiXmlElement* xmlSubElement = NULL;
		TiXmlElement* xmlSonElement = NULL;
		TiXmlNode * pNode = NULL;
		TiXmlNode *pNodeTmp = NULL;

		pNode = doc.FirstChild("Message");
		if (pNode == NULL)
			return ;
		xmlRootElement = pNode->ToElement();
		if(xmlRootElement)
		{
			if ((pNode = xmlRootElement->FirstChild("MessageHead")) != NULL)
			{
				std::string MESSAGE_TYPE = "";

				xmlSubElement = pNode->ToElement() ;
				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MessageType");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_TYPE = pNodeTmp->ToElement()->GetText();
				strMessageType = MESSAGE_TYPE.c_str();
			}
		}
	}
	catch(...)
	{
		return ;
	}
	return ;
}

void CDealJob::GetMessageTypeKy(const CString &strFileName, CString &strMessageType)
{
	try
	{
		if (strFileName.IsEmpty())
			return;

		std::string strFileName((LPCTSTR)strFileName);
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlNode*    node = 0;
		TiXmlElement* itemElement = 0;
		TiXmlElement* xmlRootElement = NULL;
		TiXmlElement* xmlListElement = NULL;
		TiXmlElement* xmlSubElement = NULL;
		TiXmlElement* xmlSonElement = NULL;
		TiXmlNode * pNode = NULL;
		TiXmlNode *pNodeTmp = NULL;

		pNode = doc.FirstChild("AIR_ENTRY_LIST");
		if (pNode == NULL)
			return ;
		xmlRootElement = pNode->ToElement();
		if(xmlRootElement)
		{
			if ((pNode = xmlRootElement->FirstChild("MESSAGE_TYPE")) != NULL)
			{
				std::string MESSAGE_TYPE = "";

				//xmlSubElement = pNode->ToElement() ;
				//if (xmlSubElement != NULL)
				//	pNodeTmp = xmlSubElement->FirstChildElement("MessageType");
				if (pNode != NULL && pNode->ToElement() != NULL && pNode->ToElement()->GetText() != 0)
					MESSAGE_TYPE = pNode->ToElement()->GetText();
				strMessageType = MESSAGE_TYPE.c_str();
			}
		}
	}
	catch(...)
	{
		return ;
	}
	return ;
}


