// TestSqlite3.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestSqlite3.h"

#include "DbSqlite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

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
		//int nFields = stmt->Fields();  //取得数据库表的列数
		//szText = stmt->FieldName(0);        //得到列名

		while (stmt->NextRow())
		{
			CString strText = "";
			strText = stmt->ValueString(0);        //得到列值
			mqPathArray.Add(strText);
			// TRACE("%s\n", strText);
		}
	}

	delete stmt;





	return nRetCode;
}
