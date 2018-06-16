// DataTransact.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DataTransact.h"
#include "TransactFactory.h"

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

CTransactFactory  gTransactFactory;
// CDataTransactApp

BEGIN_MESSAGE_MAP(CDataTransactApp, CWinApp)
END_MESSAGE_MAP()


// CDataTransactApp ����

CDataTransactApp::CDataTransactApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDataTransactApp ����

CDataTransactApp theApp;


// CDataTransactApp ��ʼ��

BOOL CDataTransactApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}

extern  "C"  _declspec(dllexport) int InitInfo()
{
	return gTransactFactory.InitInfo();
}

extern  "C"  _declspec(dllexport) int InfoDispose(CBasicConfigInfo &PackInfo)
{
	return gTransactFactory.InfoTransact(PackInfo);
}
