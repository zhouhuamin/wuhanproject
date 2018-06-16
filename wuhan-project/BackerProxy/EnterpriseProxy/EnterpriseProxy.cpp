// EnterpriseProxy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "EnterpriseProxy.h"

#include "StructDef.h"
#include "CxLog.h"

#include "tinyxml/tinyxml.h"
//#include "CLM10041.h"
#include "msmq/MSMQApiWrapper.h"
#include "GUITransaction.h"
#include "DealJob.h"

#include <direct.h>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <boost/timer.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)

#define SZAPPNAME            "BackerProxy"
// internal name of the service
#define SZSERVICENAME        "BackerProxy_Service"
// displayed name of the service
#define SZSERVICEDISPLAYNAME "BackerProxy  Service"
// list of service dependencies - "dep1\0dep2\0\0"
#define SZDEPENDENCIES       ""
#define SZSERVICECOMMENT "后置机代理"

// internal variables
SERVICE_STATUS          ssStatus;       // current status of the service
SERVICE_STATUS_HANDLE   sshStatusHandle;
DWORD                   dwErr = 0;
BOOL                    bDebug = FALSE;
TCHAR                   szErr[256] = {0};
int nServiceFlag=1;	//	in service mode or debug mode
int GlobalStopFlag=0;

CDealJob g_job;
// 唯一的应用程序对象

CWinApp theApp;

using namespace std;
using namespace boost;



VOID ServiceStop();
int InstallService();
int UninstallService();
int DebugService(int argc, char ** argv);

LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize );
BOOL WINAPI ControlHandler ( DWORD dwCtrlType );
VOID ServiceStart (DWORD dwArgc, LPTSTR *lpszArgv);
VOID AddToMessageLog(LPTSTR lpszMsg);
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI ServiceCtrl(DWORD dwCtrlCode);
void ShutDownSystem();
void ReadConfigInfo(struct structConfigInfo &configInfo);

UINT __stdcall MainThread(LPVOID pParam);
UINT __stdcall GetFilesThread(LPVOID pParam);
UINT __stdcall ProcessGUIThread(LPVOID pParam);

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		if(argc>=2)
		{
			if((strcmp(argv[1],"/install")==0)||
				(strcmp(argv[1],"-install")==0))
				return InstallService();
			if((strcmp(argv[1],"/remove")==0)||
				(strcmp(argv[1],"-remove")==0))
				return UninstallService();
			if((strcmp(argv[1],"/debug")==0)||
				(strcmp(argv[1],"-debug")==0))
				return DebugService(argc,argv);
		}
		SERVICE_TABLE_ENTRY dispatchTable[] =
		{
			{ TEXT(SZSERVICENAME), (LPSERVICE_MAIN_FUNCTION)ServiceMain },
			{ NULL, NULL }
		};
		if (!StartServiceCtrlDispatcher(dispatchTable))
			AddToMessageLog(TEXT("StartServiceCtrlDispatcher failed."));
	}

	return nRetCode;
}

int InstallService()
{
	SC_HANDLE   schService;
	SC_HANDLE   schSCManager;
	SERVICE_DESCRIPTION localServiceDesc;

	TCHAR szPath[512];

	if ( GetModuleFileName( NULL, szPath, 512 ) == 0 )
	{
		_tprintf(TEXT("Unable to install %s - %s\n"), TEXT(SZSERVICEDISPLAYNAME), GetLastErrorText(szErr, 256));
		return -1;
	}

	schSCManager = OpenSCManager(
		NULL,                   // machine (NULL == local)
		NULL,                   // database (NULL == default)
		SC_MANAGER_ALL_ACCESS |SERVICE_CHANGE_CONFIG   // access required
		);
	if ( schSCManager )
	{
		schService = CreateService(
			schSCManager,               // SCManager database
			TEXT(SZSERVICENAME),        // name of service
			TEXT(SZSERVICEDISPLAYNAME), // name to display
			SERVICE_ALL_ACCESS,         // desired access
			SERVICE_WIN32_OWN_PROCESS,  // service type
			SERVICE_AUTO_START,       // start type
			SERVICE_ERROR_NORMAL,       // error control type
			szPath,                     // service's binary
			NULL,                       // no load ordering group
			NULL,                       // no tag identifier
			TEXT(SZDEPENDENCIES),       // dependencies
			NULL,                       // LocalSystem account
			NULL);                      // no password

		if ( schService )
		{
			_tprintf(TEXT("%s installed.\n"), TEXT(SZSERVICEDISPLAYNAME) );
			localServiceDesc.lpDescription=TEXT(SZSERVICECOMMENT);
			ChangeServiceConfig2(schService,SERVICE_CONFIG_DESCRIPTION,&localServiceDesc);
			CloseServiceHandle(schService);
		}
		else
		{
			_tprintf(TEXT("CreateService failed - %s\n"), GetLastErrorText(szErr, 256));
		}
		CloseServiceHandle(schSCManager);
	}
	else
		_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));

	return 0;
}
int UninstallService()
{
	SC_HANDLE   schService;
	SC_HANDLE   schSCManager;

	schSCManager = OpenSCManager(
		NULL,                   // machine (NULL == local)
		NULL,                   // database (NULL == default)
		SC_MANAGER_ALL_ACCESS   // access required
		);
	if ( schSCManager )
	{
		schService = OpenService(schSCManager, TEXT(SZSERVICENAME), SERVICE_ALL_ACCESS);

		if (schService)
		{
			// try to stop the service
			if ( ControlService( schService, SERVICE_CONTROL_STOP, &ssStatus ) )
			{
				_tprintf(TEXT("Stopping %s."), TEXT(SZSERVICEDISPLAYNAME));
				Sleep( 1000 );

				while( QueryServiceStatus( schService, &ssStatus ) )
				{
					if ( ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
					{
						_tprintf(TEXT("."));
						Sleep( 1000 );
					}
					else
						break;
				}

				if ( ssStatus.dwCurrentState == SERVICE_STOPPED )
					_tprintf(TEXT("\n%s stopped.\n"), TEXT(SZSERVICEDISPLAYNAME) );
				else
					_tprintf(TEXT("\n%s failed to stop.\n"), TEXT(SZSERVICEDISPLAYNAME) );

			}

			// now remove the service
			if( DeleteService(schService) )
				_tprintf(TEXT("%s removed.\n"), TEXT(SZSERVICEDISPLAYNAME) );
			else
				_tprintf(TEXT("DeleteService failed - %s\n"), GetLastErrorText(szErr,256));


			CloseServiceHandle(schService);
		}
		else
			_tprintf(TEXT("OpenService failed - %s\n"), GetLastErrorText(szErr,256));

		CloseServiceHandle(schSCManager);
	}
	else
		_tprintf(TEXT("OpenSCManager failed - %s\n"), GetLastErrorText(szErr,256));

	return 0;
}

LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize )
{
	DWORD dwRet;
	LPTSTR lpszTemp = NULL;

	dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		GetLastError(),
		LANG_NEUTRAL,
		(LPTSTR)&lpszTemp,
		0,
		NULL );

	// supplied buffer is not long enough
	if ( !dwRet || ( (long)dwSize < (long)dwRet+14 ) )
		lpszBuf[0] = TEXT('\0');
	else
	{
		lpszTemp[lstrlen(lpszTemp)-2] = TEXT('\0');  //remove cr and newline character
		_stprintf( lpszBuf, TEXT("%s (0x%x)"), lpszTemp, GetLastError() );
	}

	if ( lpszTemp )
		LocalFree((HLOCAL) lpszTemp );

	return lpszBuf;
}

int DebugService(int argc, char ** argv)
{
	DWORD dwArgc;
	LPTSTR *lpszArgv;
	nServiceFlag=0;
	dwArgc   = (DWORD) argc;
	lpszArgv = (LPTSTR *)argv;
	_tprintf(TEXT("Debugging %s.\n"), TEXT(SZSERVICEDISPLAYNAME));

	SetConsoleCtrlHandler( ControlHandler, TRUE );

	ServiceStart( dwArgc, lpszArgv );
	return 0;
}

BOOL WINAPI ControlHandler ( DWORD dwCtrlType )
{
	switch( dwCtrlType )
	{
	case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate
	case CTRL_C_EVENT:      // SERVICE_CONTROL_STOP in debug mode
	case CTRL_CLOSE_EVENT:	//	close console	
		_tprintf(TEXT("Stopping %s.\n"), TEXT(SZSERVICEDISPLAYNAME));
		ServiceStop();
		return TRUE;
		break;
	}
	return FALSE;
}

VOID ServiceStop()
{
	GlobalStopFlag=1;
}

VOID AddToMessageLog(LPTSTR lpszMsg)
{
	TCHAR   szMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[2];


	if ( !bDebug )
	{
		dwErr = GetLastError();

		// Use event logging to log the error.
		//
		hEventSource = RegisterEventSource(NULL, TEXT(SZSERVICENAME));

		_stprintf(szMsg, TEXT("%s error: %d"), TEXT(SZSERVICENAME), dwErr);
		lpszStrings[0] = szMsg;
		lpszStrings[1] = lpszMsg;

		if (hEventSource != NULL) {
			ReportEvent(hEventSource, // handle of event source
				EVENTLOG_ERROR_TYPE,  // event type
				0,                    // event category
				0,                    // event ID
				NULL,                 // current user's SID
				2,                    // strings in lpszStrings
				0,                    // no bytes of raw data
				(LPCSTR *)lpszStrings,          // array of error strings
				NULL);                // no raw data

			(VOID) DeregisterEventSource(hEventSource);
		}
	}
}

BOOL ReportStatusToSCMgr(DWORD dwCurrentState,
						 DWORD dwWin32ExitCode,
						 DWORD dwWaitHint)
{
	static DWORD dwCheckPoint = 1;
	BOOL fResult = TRUE;


	if ( !bDebug ) // when debugging we don't report to the SCM
	{
		if (dwCurrentState == SERVICE_START_PENDING)
			ssStatus.dwControlsAccepted = 0;
		else
			ssStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

		ssStatus.dwCurrentState = dwCurrentState;
		ssStatus.dwWin32ExitCode = dwWin32ExitCode;
		ssStatus.dwWaitHint = dwWaitHint;

		if ( ( dwCurrentState == SERVICE_RUNNING ) ||
			( dwCurrentState == SERVICE_STOPPED ) )
			ssStatus.dwCheckPoint = 0;
		else
			ssStatus.dwCheckPoint = dwCheckPoint++;


		// Report the status of the service to the service control manager.
		//
		if (!(fResult = SetServiceStatus( sshStatusHandle, &ssStatus))) {
			AddToMessageLog(TEXT("SetServiceStatus"));
		}
	}
	return fResult;
}

VOID ServiceStart (DWORD dwArgc, LPTSTR *lpszArgv)
{
	if(nServiceFlag)
	{
		if (!ReportStatusToSCMgr(
			SERVICE_RUNNING,       // service state
			NO_ERROR,              // exit code
			0))                    // wait hint
		{
			return;
		}
	}

	//AfxBeginThread(ProcessDataThread, NULL);
	//int nErrCount=0;
	//MSG msg;
	//do
	//{
	//	try
	//	{
	//		//CMSMQApiWrapper ob;
	//		//CString szLabel = "test",szMessage;
	//		////ob.ReadStringMessage(_T("abcd"),szLabel,szMessage);
	//		//BYTE *pMessage = NULL;
	//		//ob.ReadStreamMessage(_T("test"),szLabel,pMessage);
	//		////if (szMessage != "")
	//		//if (pMessage != NULL)
	//		//{
	//		//	g_AISDataCritical.Lock();
	//		//	g_AISDataList.AddTail(szMessage);
	//		//	g_AISDataCritical.Unlock();
	//		//}
	//	}
	//	catch(...)
	//	{
	//		;
	//	}	


	//	if(GlobalStopFlag==0)
	//	{
	//		while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
	//		{
	//			if(GetMessage(&msg,NULL,0,0)!=-1)
	//			{
	//				TranslateMessage(&msg); 
	//				DispatchMessage(&msg);
	//			}
	//		}

	//		Sleep(1000);
	//	}
	//}
	//while(GlobalStopFlag==0);
	struct structConfigInfo configInfo;
	struct structConfigInfo *pConfigInfo = &configInfo;
	ReadConfigInfo(configInfo);
	g_job.InitConfigInfo(pConfigInfo);
	HANDLE hChildThread = NULL;
	HANDLE hChildThreadFiles = NULL;
	//HANDLE hGUIChildThread	 = NULL;
	UINT uId = 0;
	UINT uIdFiles = 0;
	UINT uGUIId   = 0;
	hChildThreadFiles = (HANDLE)::_beginthreadex(NULL, 0, GetFilesThread, (void*)pConfigInfo, 0, &uIdFiles);
	::WaitForSingleObject(hChildThreadFiles, INFINITE);
	::CloseHandle(hChildThreadFiles);

	hChildThread = (HANDLE)::_beginthreadex(NULL, 0, MainThread, (void*)pConfigInfo, 0, &uId);

	//hGUIChildThread = (HANDLE)::_beginthreadex(NULL, 0, ProcessGUIThread, (void*)pConfigInfo, 0, &uGUIId);


	g_job.Start(4);
	::WaitForSingleObject(hChildThread, INFINITE);
	//::WaitForSingleObject(hGUIChildThread, INFINITE);
	::CloseHandle(hChildThread);
	//::CloseHandle(hGUIChildThread);

	g_job.ShutDown();
	return;
}

void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{

	// register our service control handler:
	//
	sshStatusHandle = RegisterServiceCtrlHandler( TEXT(SZSERVICENAME), ServiceCtrl);

	if ((!sshStatusHandle)&&(nServiceFlag))
	{
		(VOID)ReportStatusToSCMgr(
			SERVICE_STOPPED,
			dwErr,
			0);
		return ;
	}
	// SERVICE_STATUS members that don't change in example
	//
	ssStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	ssStatus.dwServiceSpecificExitCode = 0;


	// report the status to the service control manager.
	//
	if (!ReportStatusToSCMgr(
		SERVICE_START_PENDING, // service state
		NO_ERROR,              // exit code
		3000))                 // wait hint
	{	if (sshStatusHandle)
	(VOID)ReportStatusToSCMgr(
	SERVICE_STOPPED,
	dwErr,
	0);
	}


	ServiceStart( dwArgc, lpszArgv );

	//
	if (sshStatusHandle)
		(VOID)ReportStatusToSCMgr(
		SERVICE_STOPPED,
		dwErr,
		0);

	return;
}

VOID WINAPI ServiceCtrl(DWORD dwCtrlCode)
{
	// Handle the requested control code.
	//
	switch(dwCtrlCode)
	{
		// Stop the service.
	case SERVICE_CONTROL_STOP:
		ReportStatusToSCMgr(SERVICE_STOP_PENDING, NO_ERROR, 0);
		ServiceStop();
		return;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
	ReportStatusToSCMgr(ssStatus.dwCurrentState, NO_ERROR, 0);
}

void ReadConfigInfo(struct structConfigInfo &configInfo)
{
	char szAppName[MAX_PATH] = {0};
	size_t  len = 0;
	::GetModuleFileName(AfxGetInstanceHandle(), szAppName, sizeof(szAppName));
	len = strlen(szAppName);
	for(size_t i=len; i>0; i--)
	{
		if(szAppName[i] == '\\')
		{
			szAppName[i+1]='\0';
			break;
		}
	}

	CString strFileNameTmp = szAppName;
	strFileNameTmp += "WHMessageFile";
	_mkdir(strFileNameTmp);

	CString strErrorFileNameTmp = szAppName;
	strErrorFileNameTmp += "WHErrorMessageFile";
	_mkdir(strErrorFileNameTmp);

	CString strFileName=szAppName;
	strFileName += "SysConfig.xml";

	TiXmlDocument doc((LPCTSTR)strFileName);
	doc.LoadFile();

	TiXmlNode* node = 0;
	TiXmlElement* areaInfoElement = 0;
	TiXmlElement* itemElement = 0;

	TiXmlHandle docHandle( &doc );
	TiXmlHandle ConfigInfoHandle = docHandle.FirstChildElement("ConfigInfo");
	TiXmlHandle QueueIPHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("QueueIP", 0).FirstChild();
	TiXmlHandle QueueNameHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("QueueName", 0).FirstChild();
	TiXmlHandle QueueLabelHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("QueueLabel", 0).FirstChild();


	TiXmlHandle WebQueueIPHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("WEB_MQ_QueueIP", 0).FirstChild();
	TiXmlHandle WebQueueNameHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("WEB_MQ_QueueName", 0).FirstChild();
	TiXmlHandle WebQueueLabelHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("WEB_MQ_QueueLabel", 0).FirstChild();

	TiXmlHandle FrontQueueIPHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("FRONT_MQ_QueueIP", 0).FirstChild();
	TiXmlHandle FrontQueueNameHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("FRONT_MQ_QueueName", 0).FirstChild();
	TiXmlHandle FrontQueueLabelHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("FRONT_MQ_QueueLabel", 0).FirstChild();

	TiXmlHandle BackerQueueIPHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("GLW_MQ_QueueIP", 0).FirstChild();
	TiXmlHandle BackerQueueNameHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("GLW_MQ_QueueName", 0).FirstChild();
	TiXmlHandle BackerQueueLabelHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("GLW_MQ_QueueLabel", 0).FirstChild();



	TiXmlHandle SqlServerDataUserHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("SqlServerDatabase", 0).ChildElement("DataUser", 0).FirstChild();
	TiXmlHandle SqlServerDataPwdHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("SqlServerDatabase", 0).ChildElement("DataPwd", 0).FirstChild();
	TiXmlHandle SqlServerDataNameHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("SqlServerDatabase", 0).ChildElement("DataName", 0).FirstChild();
	TiXmlHandle SqlServerDataServerHandle = docHandle.FirstChildElement("ConfigInfo").ChildElement("SqlServerDatabase", 0).ChildElement("DataServer", 0).FirstChild();

	string strQueueIPHandle = "";
	string strQueueNameHandle = "";
	string strQueueLabelHandle = "";
	string strSSDataUserHandle = "";
	string strSSDataPwdHandle = "";
	string strSSDataNameHandle = "";
	string strSSDataServerHandle = "";

	if (QueueIPHandle.Node() != NULL)
		configInfo.strQueueIP = strQueueIPHandle 		= QueueIPHandle.Text()->Value();
	if (QueueNameHandle.Node() != NULL)
		configInfo.strQueueName = strQueueNameHandle 		= QueueNameHandle.Text()->Value();
	if (QueueLabelHandle.Node() != NULL)
		configInfo.strQueueLabel = strQueueLabelHandle 		= QueueLabelHandle.Text()->Value();


	if (WebQueueIPHandle.Node() != NULL)
		configInfo.strWebQueueIP 		= WebQueueIPHandle.Text()->Value();
	if (WebQueueNameHandle.Node() != NULL)
		configInfo.strWebQueueName 		= WebQueueNameHandle.Text()->Value();
	if (WebQueueLabelHandle.Node() != NULL)
		configInfo.strWebQueueLabel		= WebQueueLabelHandle.Text()->Value();



	if (FrontQueueIPHandle.Node() != NULL)
		configInfo.strFrontQueueIP 			= FrontQueueIPHandle.Text()->Value();
	if (FrontQueueNameHandle.Node() != NULL)
		configInfo.strFrontQueueName 		= FrontQueueNameHandle.Text()->Value();
	if (FrontQueueLabelHandle.Node() != NULL)
		configInfo.strFrontQueueLabel 	= FrontQueueLabelHandle.Text()->Value();


	if (BackerQueueIPHandle.Node() != NULL)
		configInfo.strBackerQueueIP 			= BackerQueueIPHandle.Text()->Value();
	if (BackerQueueNameHandle.Node() != NULL)
		configInfo.strBackerQueueName 		= BackerQueueNameHandle.Text()->Value();
	if (BackerQueueLabelHandle.Node() != NULL)
		configInfo.strBackerQueueLabel 	= BackerQueueLabelHandle.Text()->Value();






	if (SqlServerDataUserHandle.Node() != NULL)
		configInfo.strSqlServerDataUser = strSSDataUserHandle 		= SqlServerDataUserHandle.Text()->Value();
	if (SqlServerDataPwdHandle.Node() != NULL)
		configInfo.strSqlServerDataPwd = strSSDataPwdHandle 		= SqlServerDataPwdHandle.Text()->Value();
	if (SqlServerDataNameHandle.Node() != NULL)
		configInfo.strSqlServerDataName = strSSDataNameHandle 		= SqlServerDataNameHandle.Text()->Value();
	if (SqlServerDataServerHandle.Node() != NULL)
		configInfo.strSqlServerDataServer = strSSDataServerHandle 	= SqlServerDataServerHandle.Text()->Value();

	configInfo.strFilePath = (LPCTSTR)strFileNameTmp;
	configInfo.strErrorFilePath	= (LPCTSTR)strErrorFileNameTmp;

	LOG4("%s, %s, %s, %s", configInfo.strQueueName.c_str(), configInfo.strWebQueueName.c_str(), configInfo.strFrontQueueName.c_str(), configInfo.strBackerQueueName.c_str());
}

UINT __stdcall MainThread(LPVOID pParam)
{
	struct structConfigInfo *pConfigInfo = NULL;
	pConfigInfo = (struct structConfigInfo *)pParam;
	if (pConfigInfo == NULL)
	{
		return 0;
	}

	MSG msg;
	do
	{
		try
		{
			CMSMQApiWrapper ob;
			CString szLabel = "";

			//ob.ReadStringMessage(_T("abcd"),szLabel,szMessage);
			BYTE *pMessage = NULL;
			int iLen = 0;
			ob.ReadStreamMessage2(pConfigInfo->strFilePath.c_str(), pConfigInfo->strQueueIP.c_str(), pConfigInfo->strQueueName.c_str(), pConfigInfo->strQueueLabel.c_str(), szLabel, &pMessage, iLen);
			//if (pMessage != NULL)
			//{
			//	//auto_ptr<CCLMBasic> apBasic(new CCLM10041());
			//	//apBasic->ProcessMessage(pConfigInfo, szLabel, pMessage, iLen);
			//	//CCLM10041 clm10041;
			//	//clm10041.ProcessMessage(pConfigInfo, szLabel, pMessage, iLen);

			//	//CCLMBasic *pBasic = new CCLM10041();
			//	//pBasic->ProcessMessage(pConfigInfo, szLabel, pMessage, iLen);

			//	//clm10041.InitConfigInfo(pConfigInfo);
			//	//clm10041.SaveFileName(szLabel);
			//	//clm10041.SaveLocal(pMessage, iLen);
			//	//clm10041.ParseMessage(messageHead, billInfo, contaInfoVect);
			//	//clm10041.SaveToDB(messageHead, billInfo, contaInfoVect);
			//	//clm10041.SendResponse(messageHead, respondInfo);
			//	//clm10041.UpdateDB(messageHead);
			//}
		}
		catch(...)
		{
			;
		}	


		if(GlobalStopFlag==0)
		{
			while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
			{
				if(GetMessage(&msg,NULL,0,0)!=-1)
				{
					TranslateMessage(&msg); 
					DispatchMessage(&msg);
				}
			}

			int nTimes = 3;
			for (; nTimes > 0; --nTimes)
			{
				if (GlobalStopFlag == 1)
					break;

				Sleep(5 * 1000);
			}

		}
	}while(GlobalStopFlag==0);

	return 0;
}

UINT __stdcall GetFilesThread(LPVOID pParam)
{
	struct structConfigInfo *pConfigInfo = NULL;
	pConfigInfo = (struct structConfigInfo *)pParam;
	if (pConfigInfo == NULL)
	{
		return 0;
	}

	do
	{
		try
		{
			CString strFilePath = pConfigInfo->strFilePath.c_str();

			char Buffer[MAX_PATH] = {0};
			WIN32_FIND_DATA finder;
			HANDLE hFileFind;
			char search[MAX_PATH] = {0};
			strcpy(search, strFilePath);
			strcat(search, "\\*.xml");

			hFileFind = FindFirstFile(search, &finder);

			if (hFileFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					char path[MAX_PATH] = {0};
					strcpy(path, (LPCTSTR)strFilePath);
					strcat(path, "\\");
					strcat(path, finder.cFileName);
					CString strFileName = path;
					CString strMessageType = "";

					if (!(finder.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					{
						g_job.GetMessageType(strFileName, strMessageType);
						if (strMessageType.IsEmpty())
						{
							g_job.GetMessageTypeMT(strFileName, strMessageType);
						}

						if (strMessageType.IsEmpty())
						{
							g_job.GetMessageTypeCSM(strFileName, strMessageType);
						}

						if (strMessageType.IsEmpty())
						{
							continue;
						}

						JobItem *pJob = new JobItem;
						pJob->m_strType = strMessageType;
						pJob->m_Data = path;
						g_job.AddJob(pJob);

					}
				} while (FindNextFile(hFileFind, &finder) != 0);

				FindClose(hFileFind);
			}
		}
		catch(...)
		{
			;
		}	

	}while(0);

	return 0;
}

UINT __stdcall ProcessGUIThread(LPVOID pParam)
{
	struct structConfigInfo *pConfigInfo = NULL;
	pConfigInfo = (struct structConfigInfo *)pParam;
	if (pConfigInfo == NULL)
	{
		return 0;
	}

	MSG msg;
	do
	{
		try
		{
			std::string strPreFlag = "WEB";
			const std::string strFileName = "";
			auto_ptr<CCLMBasic> apBasic(new CGUITransaction());
			apBasic->ProcessMessage(pConfigInfo, strFileName, strPreFlag);

			std::string strRet = apBasic->GetFileName();

			if (strRet.empty())
			{
				int nTimes = 29;
				for (; nTimes > 0; --nTimes)
				{
					if (GlobalStopFlag == 1)
						break;

					Sleep(2 * 1000);
				}
			}
		}
		catch(...)
		{
			;
		}	


		if(GlobalStopFlag==0)
		{
			while(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
			{
				if(GetMessage(&msg,NULL,0,0)!=-1)
				{
					TranslateMessage(&msg); 
					DispatchMessage(&msg);
				}
			}

			int nTimes = 3;
			for (; nTimes > 0; --nTimes)
			{
				if (GlobalStopFlag == 1)
					break;

				Sleep(2 * 1000);
			}

		}
	}while(GlobalStopFlag==0);

	return 0;
}
