// CommunicateModule.cpp: implementation of the CCommunicateModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommunicateModule.h"
#include "CxLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define HUERISTIC_VALUE 2
extern  "C"  _declspec(dllimport) int InitInfo();
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL FisrstWorkFlags[255];
CCommunicateModule::CCommunicateModule()
{
	memset(FisrstWorkFlags,FALSE,sizeof(FisrstWorkFlags));
    InitInfo();
	LOG("CCommunicateModule init success\n");
}

CCommunicateModule::~CCommunicateModule()
{
	POSITION pos=m_ListTable.GetHeadPosition();
    while(pos != NULL)
	{
		delete m_ListTable.GetNext(pos);
	}

	m_ListTable.RemoveAll();
}

int CCommunicateModule::StartListening(const UINT& port)
{
	int nRet;
	WSADATA wsaData;
	SOCKADDR_IN ServerAddr;

	//�������ȴ�����Ϣ����
	FisrstWorkFlags[_MonitorLoginCmd]=TRUE;
	FisrstWorkFlags[_MonitorLoginResult]=TRUE;
	FisrstWorkFlags[_MonitorQueryChannelCmd]=TRUE;
	FisrstWorkFlags[_MonitorSubChannelCmd]=TRUE;
	FisrstWorkFlags[_MonitorUnSubChannelCmd]=TRUE;
	FisrstWorkFlags[_MonitorCallPictCmd]=TRUE;
	FisrstWorkFlags[_ChannelRetPicInfo]=TRUE;
	FisrstWorkFlags[_ManCtrlCmd]=TRUE;
	FisrstWorkFlags[_ExcuteManManCtrlCmdResualt]=TRUE;
	FisrstWorkFlags[_MonitorCtrlChannelResetCmd]=TRUE;
	FisrstWorkFlags[_MonitorCtrlChannelBeginCmd]=TRUE;
	FisrstWorkFlags[_MonitorCtrlChannelPauseCmd]=TRUE;
	FisrstWorkFlags[_MonitorQueryRecordCmd]=TRUE;
	FisrstWorkFlags[_ExcuteManManCtrlCmdResualt]=TRUE;

	FisrstWorkFlags[_ChannelRegCmd]=TRUE;
	FisrstWorkFlags[_ChannelRegResault]=TRUE;
	FisrstWorkFlags[_ChannelUnRegCmd]=TRUE;
//	FisrstWorkFlags[_WLJKChannlRelCmd]=TRUE;

//	FisrstWorkFlags[_WLJKChannlDeviceExcuteCmd]=TRUE;
//	FisrstWorkFlags[_ChannelDeviceExcuteBack]=TRUE;
	m_cpu.Init();
	
   	nRet=WSAStartup(MAKEWORD(2,2),&wsaData);                     
	if(nRet!=0)
	{
		WSACleanup();
		return -1;
	}
	
	m_ExitEvent  = CreateEvent(NULL, TRUE, FALSE, NULL);
	if ((m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		return -1;
	}
	
	//�����̳߳�.
	if(m_ConfigInfo.nThreadNumber>2)
	   CreateWorkerThreads(m_ConfigInfo.nThreadNumber);
	else
		CreateWorkerThreads(2);
	
	//���������׽���(��ʽ).
	if ((m_SockListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0,
		WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		LOG_ERR1("WSASocket() failed with error %d\n", WSAGetLastError());
		return -1;
	} 

	// 2014-3-31 add
	const int on = 1;
	if (setsockopt(m_SockListen, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)))
	{
		LOG_ERR1("setsockopt() failed with error %d\n", WSAGetLastError());
		return -1;
	}
	
	//����˿ڼ�Э���岢��.
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port = htons(port);
	
	if (bind(m_SockListen, (PSOCKADDR) &ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		return -1;
	}
	
	//��ʼ�����������ü����ͻ�������.
	if (listen(m_SockListen, 10000) == SOCKET_ERROR)
		return -1;
	else
		AfxBeginThread(_ServerListenThread,this);    
	
	m_WorkThreadPool.InitThreadInfo(m_ConfigInfo.nThreadNumber);
	m_WorkThreadPool.Run();

	////////////////////////////////////////////

	return 0;
}

//ֹͣ�����̡߳�
int CCommunicateModule::StopListening()
{
	SYSTEM_INFO  sysinfo;
    DWORD        dwThreads;
    DWORD        i;

	OVERLAPPED Overlapped = {0, 0, 0, 0, NULL};

    GetSystemInfo(&sysinfo);
	
	if(m_ConfigInfo.nThreadNumber>2)
		dwThreads = sysinfo.dwNumberOfProcessors + m_ConfigInfo.nThreadNumber;
	else
        dwThreads = sysinfo.dwNumberOfProcessors + 2;

	Sleep(40);
    for (i=0; i<dwThreads; i++)
    {
       PostQueuedCompletionStatus(
		   m_CompletionPort,            
		   NULL,            
		   NULL,            
		   &Overlapped);
    }

	SetEvent(m_ExitEvent);
	SOCKET clientSocket=INVALID_SOCKET;

    OnSetupConnect(clientSocket, "127.0.0.1", m_ConfigInfo.nLocalPort, 1000, 2000) ;
	
	int j = m_ConfigInfo.nThreadNumber*10;

	m_WorkThreadPool.Stop();
	Sleep(200+j);
	closesocket(clientSocket);
	closesocket(m_SockListen);

	LOG("ֹͣ�����߳�.");
	
	return 0;
}

//�����̳߳ء�
void CCommunicateModule::CreateWorkerThreads(int nThreadNumber)
{
    SYSTEM_INFO  sysinfo;
    DWORD        dwThreadId;
    DWORD        dwThreads;
    DWORD        i;

    GetSystemInfo(&sysinfo);
    dwThreads = sysinfo.dwNumberOfProcessors +nThreadNumber;
    for (i=0; i<dwThreads; i++)
    {
        HANDLE hThread;
        hThread = CreateThread(
            NULL, 0, _CompletionPortWorkerThread, m_CompletionPort, 0, &dwThreadId
            );
        CloseHandle(hThread); 
    }
} 

// ��ñ���IP
CString CCommunicateModule::GetLocalIP()
{
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData)==SOCKET_ERROR)
	{
		return "0.0.0.0";
	}

	int nLen=20;
	char hostname[20];

	// ��ñ���������
	gethostname(hostname,nLen);        

	CString strHostName(hostname);

	struct hostent FAR* lpHostEnt = gethostbyname(strHostName);

	if(lpHostEnt == NULL)
	{
		return "0.0.0.0";
	}

	LPSTR lpAddr = lpHostEnt->h_addr_list[0];  
	struct in_addr inAddr;
	memmove(&inAddr,lpAddr,4);

	return inet_ntoa(inAddr);         
}

//����
BOOL CCommunicateModule::OnSetupConnect(SOCKET &clientSocket, CString strIP, int nPort, int nSendTimeout, int nRecvTimeout) 
{	
	sockaddr_in ServAddr;

	try
	{
		closesocket(clientSocket);
		clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
		
		::setsockopt(
			clientSocket, 
			SOL_SOCKET,SO_RCVTIMEO,
			(char*)&nRecvTimeout,
			sizeof(nRecvTimeout));
		
		::setsockopt(
			clientSocket, 
			SOL_SOCKET,SO_SNDTIMEO,
			(char*)&nSendTimeout,
			sizeof(nSendTimeout));
		
		if (clientSocket != INVALID_SOCKET)	
		{ 
			ServAddr.sin_family = AF_INET;
			ServAddr.sin_addr.s_addr = inet_addr(strIP);//strIP
			ServAddr.sin_port = htons(nPort);
			if (connect(clientSocket,(LPSOCKADDR)&ServAddr,sizeof(ServAddr))
				== SOCKET_ERROR)
			{
				closesocket(clientSocket);
				clientSocket= INVALID_SOCKET;
				return FALSE;
			}
			else
			{	
				return TRUE;
			}
			
		}
	}catch(...)
	{
		return FALSE;
	}
	return FALSE;
}

BOOL CCommunicateModule::OnSetConfigInfo(CBasicConfigInfo &ConfigInfo)
{
    m_ConfigInfo.hNotifyWnd    = ConfigInfo.hNotifyWnd;
	m_ConfigInfo.nFlatRoofPort = ConfigInfo.nFlatRoofPort;
	m_ConfigInfo.nMTFlatRoofPort = ConfigInfo.nMTFlatRoofPort;
	m_ConfigInfo.nLocalPort    = ConfigInfo.nLocalPort;
	m_ConfigInfo.nMonitorPort  = ConfigInfo.nMonitorPort;
	m_ConfigInfo.nProxyPort    = ConfigInfo.nProxyPort;
	m_ConfigInfo.nRecvTimeOut  = ConfigInfo.nRecvTimeOut;
	m_ConfigInfo.nSendTimeOut  = ConfigInfo.nSendTimeOut;
	m_ConfigInfo.nThreadNumber = ConfigInfo.nThreadNumber;

	m_ConfigInfo.strDataPaw    = ConfigInfo.strDataPaw;
	m_ConfigInfo.strDataServer = ConfigInfo.strDataServer;
	m_ConfigInfo.strDataUser   = ConfigInfo.strDataUser;
	m_ConfigInfo.strMonitorServerIp = ConfigInfo.strMonitorServerIp;
	m_ConfigInfo.strMTFlatRoofIp = ConfigInfo.strMTFlatRoofIp;
	m_ConfigInfo.strFilePath=    ConfigInfo.strFilePath;
	m_ConfigInfo.nIsNeedActiveTest=ConfigInfo.nIsNeedActiveTest;
	m_ConfigInfo.nCheckLoginOutTime=ConfigInfo.nCheckLoginOutTime;
	m_ConfigInfo.nLoginOutTime=ConfigInfo.nLoginOutTime;

	m_ConfigInfo.nCtrlSpeedFlag=ConfigInfo.nCtrlSpeedFlag;
	m_ConfigInfo.nCheckContaFlag=ConfigInfo.nCheckContaFlag;

	m_ConfigInfo.nChnlLoginMode=ConfigInfo.nChnlLoginMode;

	m_ConfigInfo.nUserLoginMode=ConfigInfo.nUserLoginMode;
	m_ConfigInfo.nESealFlag=ConfigInfo.nESealFlag;
	m_ConfigInfo.nESealTime=ConfigInfo.nESealTime;
	m_ConfigInfo.nESealInfoDownFlag=ConfigInfo.nESealInfoDownFlag;
	m_ConfigInfo.m_iSaveRunDataPacket = ConfigInfo.m_iSaveRunDataPacket;
	m_ConfigInfo.m_iQueryLicenseID = ConfigInfo.m_iQueryLicenseID;

    return TRUE;
}

void CCommunicateModule::Test(LPVOID lpParam)
{
	CModuleInfo * pInfo = (CModuleInfo* )lpParam;
//	DWORD id = GetCurrentThreadId();
//	CString str;
//	str.Format("work thread is working %x\n",id);
//	pInfo->OnDealInfo(m_ConfigInfo);

	Sleep(100);
}

CModuleInfo* CCommunicateModule::GetInfoType(char *strIp, int nPort)
{
	try
	{
		CModuleInfo* p = NULL;
		POSITION pos = m_ListTable.GetHeadPosition();
		while(pos != NULL)
		{
			p = m_ListTable.GetNext(pos);
			if(p->OnIsExist(strIp, nPort))
			   return p;
		}
	}catch(...)
	{
		return NULL;
	}
	
	return NULL;
}

// �����߳�
UINT _ServerListenThread(LPVOID lParam)
{	
	DWORD Flags;
	DWORD RecvBytes;
	
	CCommunicateModule *pModule = (CCommunicateModule *) lParam;
	
	while(TRUE)
	{
		try
		{
			SOCKET Accept;
			
			LPPER_IO_OPERATION_DATA PerIoData;
			
			SOCKADDR_IN ClientAddr;                   
			int addr_length = sizeof(ClientAddr);
			
			if ((Accept = WSAAccept(pModule->m_SockListen, (SOCKADDR*)&ClientAddr,&addr_length, NULL, 0)) == SOCKET_ERROR)
			{
				LOG_ERR("_ServerListenThread WSAAccept����");
				continue;
			}
			
			//�ȴ������¼���
			if (WaitForSingleObject(pModule->m_ExitEvent,0) == WAIT_OBJECT_0)	
				break;
			
			// ȡ�ÿͻ��˵�IP�Ͷ˿�
			UINT  nPort   = ClientAddr.sin_port;
			LPCTSTR strIP = inet_ntoa(ClientAddr.sin_addr);
	//		char *p=malloc(10);
			
			if ((PerIoData = (LPPER_IO_OPERATION_DATA) GlobalAlloc(GPTR,sizeof(PER_IO_OPERATION_DATA))) == NULL)
			{
				LOG_ERR1("�����ڴ�ʧ��: %d\n", GetLastError());
				//TRACE("_ServerListenThread�����ڴ�ʧ��: %d",GetLastError());
	
	//			CXMLLogfile::Instance()->Flush();
				continue;
			//	return -1;
			}
			
			PerIoData->Socket      = Accept;
			PerIoData->bIsStart    = TRUE;
			PerIoData->nClientPort = nPort;
			PerIoData->pVoid       = lParam;
			strcpy_s(PerIoData->szClientIp, sizeof(PerIoData->szClientIp), strIP);

			TRACE("************!@#$*******������SOCKET��%d\n",Accept);
			
			if (CreateIoCompletionPort((HANDLE) Accept, pModule->m_CompletionPort, (DWORD) PerIoData,
				0) == NULL)
			{
				LOG_ERR("_ServerListenThread CreateIoCompletionPort����");
				continue;
				//	return -1;
			}
			
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->szBuffer;
	
			Flags = 0;
			
 
			//��ȡ�ڴ��ʹ�����
	//		MEMORYSTATUS memStatus;  
	//		memStatus.dwLength=sizeof(MEMORYSTATUS);
//			::GlobalMemoryStatus(&memStatus);
			
	//		TRACE("-)-)-)-)�����ڴ�Ϊ%d%% \n", memStatus.dwMemoryLoad);

 
			//��������
			if(pModule->m_ConfigInfo.nCtrlSpeedFlag==TRUE&&pModule->m_WorkThreadPool.GetWorkItemCount()>10000&&strcmp(pModule->m_ConfigInfo.strMonitorServerIp,strIP)!=0)
			{
				Sleep(1);
				PerIoData->m_ioType=IOWrite;
				int sleeptime=1000;
				memcpy(PerIoData->DataBuf.buf,&sleeptime,4);
				PerIoData->DataBuf.len=4;
				unsigned int SendBytes=4;
				if (WSASend(Accept, &(PerIoData->DataBuf), 1, &PerIoData->DataBuf.len, Flags,
					&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
				{
					int nError=WSAGetLastError();
					if (nError != ERROR_IO_PENDING)
					{
						
						LOG_ERR1("_ServerListenThread WSASend����: %d",nError);
				
				//		return -1;
					}
				}
			}
			else
			{

				PerIoData->m_ioType=IORead;
				if (WSARecv(Accept, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
					&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
				{
					if (WSAGetLastError() != ERROR_IO_PENDING)
					{
						int nError=WSAGetLastError();
						if (nError != ERROR_IO_PENDING)
							LOG_ERR1("_ServerListenThread WSARecv����: %d",nError);


					}
				}
			}	
		}
		catch(...)
		{
			LOG_ERR("_ServerListenThread �쳣�˳�");
			return -1;
		}
	}	
	
	LOG("�˳������߳� \n");

	return 0;
}

DWORD WINAPI _CompletionPortWorkerThread(LPVOID CompletionPortID)
{
	HANDLE CompletionPort = (HANDLE) CompletionPortID;
	DWORD BytesTransferred;
	LPPER_IO_OPERATION_DATA PerIoData, PerIoDataTmp;
	DWORD RecvBytes;
	DWORD Flags;
	char szclientip[64];
	int nclientport;
	
	
	PerIoData = NULL;
	while(TRUE)
	{
		try
		{
			BOOL bIORet;
			memset(szclientip,0,sizeof(szclientip));
			nclientport=0;
			try
			{
				bIORet=GetQueuedCompletionStatus(CompletionPort, &BytesTransferred,
				(LPDWORD)&PerIoDataTmp, (LPOVERLAPPED *) &PerIoData, INFINITE);
			}
			catch(...)
			{
				LOG1("GetQueuedCompletionStatus �쳣����:%d",WSAGetLastError());
			}
			DWORD dwIOError = WSAGetLastError();
			 if (!bIORet && dwIOError != WAIT_TIMEOUT )
			 {
#ifdef DEBUF				 
				LPVOID lpMsgBuf; 
				FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS, 
					NULL, 
					GetLastError(), 
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					(LPTSTR) &lpMsgBuf, 
					0, 
					NULL 
					); 
				TRACE((LPCTSTR)lpMsgBuf); 
				LocalFree(lpMsgBuf); 
#endif
				continue;
	//			return 0;
			 }
		
		
			
	//		if(!bIORet&&dwIOError == WAIT_TIMEOUT)
			{
				if(PerIoDataTmp == NULL) 
				{

					break;
				}
			}
			if(	PerIoData->m_ioType==IOWrite)
			{
				//Flags = 0;
				//ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
				//memset(PerIoData->szBuffer, 0, sizeof(PerIoData->szBuffer));
				//PerIoData->DataBuf.len = DATA_BUFSIZE;
				//PerIoData->DataBuf.buf = PerIoData->szBuffer;
				//PerIoData->m_ioType=IORead;
				//
				//if (WSARecv(PerIoDataTmp->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
				//	&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
				//{
				//	if (WSAGetLastError() != ERROR_IO_PENDING)
				//	{
				//		TRACE("��������ʧ��: %d\n", WSAGetLastError());
		
				//	}
				//	else
				//	{			
				//	}
				//}

				TRACE("�������ݳɹ�\n");
				closesocket(PerIoDataTmp->Socket);
				if(PerIoData!=NULL)
					GlobalFree(PerIoData);
				PerIoData=NULL;
				continue;
			}
			
			CCommunicateModule *pModule = (CCommunicateModule *) PerIoData->pVoid; 


			CModuleInfo *pModuleInfo = NULL;
			CModuleInfo* p = NULL;
			POSITION pos;
		
			strcpy_s(szclientip, sizeof(szclientip), PerIoData->szClientIp);
			nclientport=PerIoData->nClientPort;
			try
			{
				pModule->m_lock.Lock();
				pos = pModule->m_ListTable.GetHeadPosition();
				while(pos != NULL)
				{
					p = pModule->m_ListTable.GetNext(pos);
					if(p->OnIsExist(szclientip, nclientport))
					{					
						pModuleInfo = p;
						break;
					}
			
				}
				pModule->m_lock.Unlock();
			
			}
			catch(...)
			{
				LOG("pModule �쳣����");
			}
				 
			
			if (BytesTransferred == 0)
			{
				//�ر�.
				TRACE("�ر� socket %d\n", PerIoDataTmp->Socket);
				
				if (closesocket(PerIoDataTmp->Socket) == SOCKET_ERROR)
				{
					LOG1("�ر� socket ʧ��,����: %d\n", WSAGetLastError());
			//		return 0;
				}
				
				if(pModuleInfo != NULL)
				{
					pModule->m_lock.Lock();					
					POSITION pos = pModule->m_ListTable.Find(pModuleInfo, NULL);
				
					if(pos!=NULL)
					{
						
						delete  pModuleInfo;
						pModuleInfo=NULL;
						pModule->m_ListTable.RemoveAt(pos);
						
					}				
					pModule->m_lock.Unlock();						
				}
//TRACE("���ܲɼ���Ϣ�ر�\n");	
				if(PerIoData!=NULL)
					GlobalFree(PerIoData);
				PerIoData=NULL;
				continue;
			}
			
			PerIoData->DataBuf.len = BytesTransferred;
			
			if(pModuleInfo == NULL)
			{
			
//TRACE("���ܲɼ���Ϣ2\n");
				pModuleInfo = new CModuleInfo();
				if(!pModuleInfo->OnDealInfo(PerIoData, pModule->m_ConfigInfo))//�Ƿ����ݰ�
				{
					delete pModuleInfo;
					pModuleInfo=NULL;
					closesocket(PerIoDataTmp->Socket);
					if(PerIoData!=NULL)
						GlobalFree(PerIoData);
					PerIoData=NULL;
					continue;
				}
			
			
				if(pModuleInfo->GetStart())
				{

					if(!pModuleInfo->GetIsFirstWorkItem())
						pModule->m_WorkThreadPool.RegisterWorkItem(pModuleInfo);
					else
						pModule->m_WorkThreadPool.RegisterFirstWorkItem(pModuleInfo);
				//���յ������İ��������Ͽ�����
					{
						TRACE("�ر�1 socket %d\n", PerIoDataTmp->Socket);
						closesocket(PerIoDataTmp->Socket);
						if(PerIoData!=NULL)
							GlobalFree(PerIoData);
						PerIoData=NULL;
						continue;
					}
				}
				else
				{
					pModule->m_lock.Lock();
					pModule->m_ListTable.AddHead(pModuleInfo);
					pModule->m_lock.Unlock();
				}
			}
			else
			{

				try
				{
					pModuleInfo->OnDealInfo(PerIoData, pModule->m_ConfigInfo);

					if(pModuleInfo->GetStart())
					{
						pModule->m_lock.Lock();
						try
						{
							POSITION pos = pModule->m_ListTable.Find(pModuleInfo, NULL);
													
							pModule->m_ListTable.RemoveAt(pos);
						}
						catch(...)
						{
							
							LOG("pModule �쳣����1");
						}
						pModule->m_lock.Unlock();
		

						if(!pModuleInfo->GetIsFirstWorkItem())
							pModule->m_WorkThreadPool.RegisterWorkItem(pModuleInfo);
						else
							pModule->m_WorkThreadPool.RegisterFirstWorkItem(pModuleInfo);
						//���յ������İ��������Ͽ�����
						{
							TRACE("�ر� 2 socket %d\n", PerIoDataTmp->Socket);
							closesocket(PerIoDataTmp->Socket);
							if(PerIoData!=NULL)
								GlobalFree(PerIoData);
							PerIoData=NULL;
							continue;
						}
					}
				}
				catch(...)
				{
						LOG("pModule �쳣����2");
				}
				
			}
			
			Flags = 0;
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			memset(PerIoData->szBuffer, 0, sizeof(PerIoData->szBuffer));
			PerIoData->DataBuf.len = DATA_BUFSIZE;
			PerIoData->DataBuf.buf = PerIoData->szBuffer;
			PerIoData->m_ioType=IORead;
			
			if (WSARecv(PerIoDataTmp->Socket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,
				&(PerIoData->Overlapped), NULL) == SOCKET_ERROR)
			{
				int nError=WSAGetLastError();
				if(nError!=ERROR_IO_PENDING)
					LOG1("_CompletionPortWorkerThread ��������ʧ��:%d",nError);
	
			}
		}catch(...)
		{			
			Sleep(2);
		}
	}
	
	LOG("�˳������߳� \n");
	return 0;
}
