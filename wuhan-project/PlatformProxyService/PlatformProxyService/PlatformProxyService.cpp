// PlatformProxyService.cpp : ����Ӧ�ó��������Ϊ��
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


// CPlatformProxyServiceApp ����

CPlatformProxyServiceApp::CPlatformProxyServiceApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPlatformProxyServiceApp ����

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

// CPlatformProxyServiceApp ��ʼ��

BOOL CPlatformProxyServiceApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


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
	//	// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
	//	//  �Ի���Ĵ���
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
	//	//  �Ի���Ĵ���
	//}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
