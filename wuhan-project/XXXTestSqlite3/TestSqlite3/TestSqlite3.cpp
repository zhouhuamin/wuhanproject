// TestSqlite3.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestSqlite3.h"

#include "DbSqlite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
	}

	//{
	//	bool fTest;
	//	CDbSQLite sqlite;
	//	//���Ӵ�SQLite���ݿ�
	//	fTest = sqlite.Open(_T("C:\\Njsamples\\CGS_DB\\WEB_PROXY_DB.db"));
	//	if (!fTest)
	//	{
	//		AfxMessageBox(_T("�򲻿�WEB_PROXY_DB.db"));
	//		return 0;
	//	}
	//	//ִ�д��������
	//	fTest = sqlite.DirectStatement(_T("create table WH_MQPATH_CONFIG(ID integer primary key autoincrement, MQPATH			varchar(255), MQMESSAGETYPE	varchar(10), CUSTOMS_CODE	varchar(4), ENTERPRISE_CODE	varchar(50), INOROUTFLAG		int default 1)"));
	//	if (!fTest)
	//		AfxMessageBox(_T("���ܴ�����WH_MQPATH_CONFIG"));
	//	//��������
	//	CString szQuery;
	//	szQuery = _T("insert into WH_MQPATH_CONFIG(MQPATH,MQMESSAGETYPE,CUSTOMS_CODE,ENTERPRISE_CODE) values('formatname:direct=tcp:192.168.32.24\\private$\\whhg_InToOut', 'ALL', '4708', '4708000001');");
	//	fTest = sqlite.DirectStatement(szQuery);
	//	if (!fTest)
	//		AfxMessageBox(_T("��������ʧ�ܣ�"));
	//	szQuery = _T("insert into WH_MQPATH_CONFIG(MQPATH,MQMESSAGETYPE,CUSTOMS_CODE,ENTERPRISE_CODE) values('formatname:direct=tcp:192.168.32.24\\private$\\whhg_InToOut', 'ALL', '4708', '4708000002');");
	//	fTest = sqlite.DirectStatement(szQuery);
	//	if (!fTest)
	//		AfxMessageBox(_T("��������ʧ�ܣ�"));
	//	AfxMessageBox(_T("������ɹ����������ݳɹ���"));
	//}



	BOOL bRet = FALSE;
	CDbSQLite sqlite; 
	bRet = sqlite.Open("C:\\Njsamples\\CGS_DB\\WEB_PROXY_DB.db");
	if (!bRet)
	{
		return 0;
	}

	CSqlStatement *stmt = sqlite.Statement(_T("SELECT MQPATH FROM WH_MQPATH_CONFIG"));
	if (stmt != NULL)
	{
		CString szText  = "";
		int nFields = stmt->Fields();  //ȡ�����ݿ�������
		szText = stmt->FieldName(0);        //�õ�����

		while (stmt->NextRow())
		{
			CString strText = "";
			strText = stmt->ValueString(0);        //�õ���ֵ
			TRACE("%s\n", strText);
		}
	}

	delete stmt;





	return nRetCode;
}
