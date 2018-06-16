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

	//监听线程。
	friend UINT _ServerListenThread(LPVOID lParam);
	//I/O处理线程。
	friend DWORD WINAPI _CompletionPortWorkerThread(LPVOID lParam);
	
public:
	//启动服务器。
	int StartListening(const UINT&);
	//停止服务器。
    int StopListening();
	
	BOOL OnSetConfigInfo(CBasicConfigInfo &ConfigInfo);
public:
	HANDLE m_ExitEvent;
	//监听SOCKET。
    SOCKET m_SockListen;
	//完成端口
	HANDLE m_CompletionPort;
	
	//
	CList<CModuleInfo*, CModuleInfo*> m_ListTable;
	//配置信息
	CBasicConfigInfo m_ConfigInfo;

	CCriticalSection m_lock;
	void Test(LPVOID lpParam);//static 
	CWorkThreadPool m_WorkThreadPool;
	CCpuUsage m_cpu;

private:
	//创建线程池。
	void CreateWorkerThreads(int nThreadNumber);
	//获得本机IP。
	static CString GetLocalIP();
	
	CModuleInfo* GetInfoType(char * strIp, int nPort);
	virtual BOOL OnSetupConnect(SOCKET &clientSocket, CString strIP, int nPort, int nSendTimeout, int nRecvTimeout); 	
};

#endif // !defined(AFX_COMMUNICATEMODULE_H__A16C54BA_28D5_467C_87DE_EB29B23B3393__INCLUDED_)
