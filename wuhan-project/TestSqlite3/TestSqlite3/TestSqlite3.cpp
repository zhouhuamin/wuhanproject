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



	BOOL bRet = FALSE;
	CDbSQLite sqlite; 
	bRet = sqlite.Open("C:\\Njsamples\\CGS_DB\\WEB_PROXY_DB.db");
	if (!bRet)
	{
		return 0;
	}

	CSqlStatement *stmt = sqlite.Statement("SELECT MQPATH FROM WH_MQPATH_CONFIG");
	if (stmt != NULL)
	{
		//CString szText  = "";
		//int nFields = stmt->Fields();  //ȡ�����ݿ�������
		//szText = stmt->FieldName(0);        //�õ�����

		while (stmt->NextRow())
		{
			CString strText = "";
			strText = stmt->ValueString(0);        //�õ���ֵ
			mqPathArray.Add(strText);
			// TRACE("%s\n", strText);
		}
	}

	delete stmt;





	return nRetCode;
}
