// CommunicateModule.h: interface for the CCommunicateModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMUNICATEMODULE_H__A16C54BA_28D5_467C_87DE_EB29B23B3393__INCLUDED_)
#define AFX_COMMUNICATEMODULE_H__A16C54BA_28D5_467C_87DE_EB29B23B3393__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>
#include "ModuleInfo.h"
#include "WorkThreadPool.h"
#include "cpuusage.h"
class CServerHeatBeat;


class CCommunicateModule : public CObject  
{
public:
	CCommunicateModule();
	virtual ~CCommunicateModule();

	//�����̡߳�
	friend UINT _ServerListenThread(LPVOID lParam);
	//I/O�����̡߳�
	friend DWORD WINAPI _CompletionPortWorkerThread(LPVOID lParam);
	
public:
	//������������
	int StartListening(const UINT&);
	//ֹͣ��������
    int StopListening();
	
	BOOL OnSetConfigInfo(CBasicConfigInfo &ConfigInfo);
public:
	HANDLE m_ExitEvent;
	//����SOCKET��
    SOCKET m_SockListen;
	//��ɶ˿�
	HANDLE m_CompletionPort;
	
	//
	CList<CModuleInfo*, CModuleInfo*> m_ListTable;
	//������Ϣ
	CBasicConfigInfo m_ConfigInfo;

	CCriticalSection m_lock;
	void Test(LPVOID lpParam);//static 
	CWorkThreadPool m_WorkThreadPool;
	CCpuUsage m_cpu;

private:
	//�����̳߳ء�
	void CreateWorkerThreads(int nThreadNumber);
	//��ñ���IP��
	static CString GetLocalIP();
	
	CModuleInfo* GetInfoType(char * strIp, int nPort);
	virtual BOOL OnSetupConnect(SOCKET &clientSocket, CString strIP, int nPort, int nSendTimeout, int nRecvTimeout); 	
};

#endif // !defined(AFX_COMMUNICATEMODULE_H__A16C54BA_28D5_467C_87DE_EB29B23B3393__INCLUDED_)
