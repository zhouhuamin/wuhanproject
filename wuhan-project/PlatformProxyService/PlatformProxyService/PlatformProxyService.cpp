// PlatformProxyService.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "PlatformProxyService.h"
#include "PlatformProxyServiceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
extern void __cdecl main(int argc, char **argv);
extern HANDLE					m_MainExit;
// CPlatformProxyServiceApp

BEGIN_MESSAGE_MAP(CPlatformProxyServiceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPlatformProxyServiceApp 构造

CPlatformProxyServiceApp::CPlatformProxyServiceApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPlatformProxyServiceApp 对象

CPlatformProxyServiceApp theApp;


VOID ServiceStart (DWORD dwArgc, LPTSTR *lpszArgv)
{
	ReportStatusToSCMgr( SERVICE_RUNNING, NO_ERROR, 0 );


	//	theApp.Init();
	CPlatformProxyServiceDlg dlg;
	m_MainExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	dlg.OnBnClickedButtonStart();
	WaitForSingleObject(m_MainExit,INFINITE);

	dlg.OnBnClickedButtonStop();



}

VOID ServiceStop()
{
	//	((CMonitorServerDlg*)theApp.m_pMainWnd)->OnMenuExit();
	SetEvent(m_MainExit);
}

// CPlatformProxyServiceApp 初始化

BOOL CPlatformProxyServiceApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	TCHAR szPath[MAX_PATH];
	GetModuleFileName( NULL, szPath, MAX_PATH );
	TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	_tsplitpath( szPath,drive,dir,fname,ext );
	strcpy( szPath, drive );
	strcat( szPath, dir );
	SetCurrentDirectory( szPath );

	main( __argc, __argv );
	//CPlatformProxyServiceDlg dlg;
	//m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: 在此处放置处理何时用“确定”来关闭
	//	//  对话框的代码
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: 在此放置处理何时用“取消”来关闭
	//	//  对话框的代码
	//}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
