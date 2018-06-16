#pragma once
#include "StructDef.h"
#include <afxmt.h>


struct JobItem
{
	CString m_strType;
	CString m_Data;
};

class CDealJob
{
public:
	CDealJob(void);
	~CDealJob(void);


public:
	static UINT WorkerThreadProc(LPVOID pParam);

public:
	CWinThread* GetWorker(WORD WorkerID);
	JobItem* GetJob();
	void ProcessJob(JobItem *pJob, CDealJob* pServer);
	void FreeJob(JobItem *pJob);
	BOOL AddJob(JobItem *pJob);

public:
	BOOL SetWorkers(int nThreads);

public:
	BOOL Startup();
	BOOL Start(int nOfWorkers=4);

	void ShutDown();
	void ShutDownWorkers();

	CString GetNewFileName();
	void	InitConfigInfo(structConfigInfo *pConfig);
	void GetMessageType(const CString &strFileName, CString &strMessageType);
	void GetMessageTypeMT(const CString &strFileName, CString &strMessageType);
	void GetMessageTypeCSM(const CString &strFileName, CString &strMessageType);
	void GetMessageTypeKy(const CString &strFileName, CString &strMessageType);


public:
	static int m_nSequenceNo;
	CCriticalSection m_SequenceLock;

	CMapWordToPtr m_WorkerThreadMap;
	CCriticalSection m_WorkerThreadMapLock;

	CCriticalSection m_JobQueueLock;
	CPtrList m_JobQueueList;

	volatile BOOL m_bAcceptJobs;
	int m_nOfWorkers;
	BOOL m_bServerStarted;

	volatile BOOL m_bShutDown;
	structConfigInfo m_config;
};
