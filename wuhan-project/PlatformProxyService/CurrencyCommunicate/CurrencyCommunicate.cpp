// CurrencyCommunicate.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CurrencyCommunicate.h"
#include "../PlatformProxyService/public/BasicConfigInfo.h"
#include "CommunicateModule.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CCurrencyCommunicateApp

BEGIN_MESSAGE_MAP(CCurrencyCommunicateApp, CWinApp)
END_MESSAGE_MAP()


CCommunicateModule *gCommunicateModule=NULL;
// CCurrencyCommunicateApp ����

CCurrencyCommunicateApp::CCurrencyCommunicateApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCurrencyCommunicateApp ����

CCurrencyCommunicateApp theApp;


// CCurrencyCommunicateApp ��ʼ��

BOOL CCurrencyCommunicateApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}

extern  "C"  _declspec(dllexport) int StartListening(const UINT& port, CBasicConfigInfo &ConfigInfo)
{

	if (gCommunicateModule == NULL)
	{

		gCommunicateModule = new CCommunicateModule();
		gCommunicateModule->OnSetConfigInfo(ConfigInfo);
		return gCommunicateModule->StartListening(port);
	}
	else
	{
		return -2;
	}
}



extern  "C"  _declspec(dllexport) int StopListening()

{

	int nRet=0;

	if (gCommunicateModule != NULL)
	{
		nRet=gCommunicateModule->StopListening();
		delete  gCommunicateModule;
		gCommunicateModule=NULL;
		return nRet;
	}
	else
	{
		return 0;
	}

}

