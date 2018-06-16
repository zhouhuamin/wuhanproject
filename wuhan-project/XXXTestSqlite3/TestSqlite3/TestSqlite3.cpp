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

	//{
	//	bool fTest;
	//	CDbSQLite sqlite;
	//	//连接打开SQLite数据库
	//	fTest = sqlite.Open(_T("C:\\Njsamples\\CGS_DB\\WEB_PROXY_DB.db"));
	//	if (!fTest)
	//	{
	//		AfxMessageBox(_T("打不开WEB_PROXY_DB.db"));
	//		return 0;
	//	}
	//	//执行创建表语句
	//	fTest = sqlite.DirectStatement(_T("create table WH_MQPATH_CONFIG(ID integer primary key autoincrement, MQPATH			varchar(255), MQMESSAGETYPE	varchar(10), CUSTOMS_CODE	varchar(4), ENTERPRISE_CODE	varchar(50), INOROUTFLAG		int default 1)"));
	//	if (!fTest)
	//		AfxMessageBox(_T("不能创建表WH_MQPATH_CONFIG"));
	//	//新增数据
	//	CString szQuery;
	//	szQuery = _T("insert into WH_MQPATH_CONFIG(MQPATH,MQMESSAGETYPE,CUSTOMS_CODE,ENTERPRISE_CODE) values('formatname:direct=tcp:192.168.32.24\\private$\\whhg_InToOut', 'ALL', '4708', '4708000001');");
	//	fTest = sqlite.DirectStatement(szQuery);
	//	if (!fTest)
	//		AfxMessageBox(_T("插入数据失败！"));
	//	szQuery = _T("insert into WH_MQPATH_CONFIG(MQPATH,MQMESSAGETYPE,CUSTOMS_CODE,ENTERPRISE_CODE) values('formatname:direct=tcp:192.168.32.24\\private$\\whhg_InToOut', 'ALL', '4708', '4708000002');");
	//	fTest = sqlite.DirectStatement(szQuery);
	//	if (!fTest)
	//		AfxMessageBox(_T("插入数据失败！"));
	//	AfxMessageBox(_T("创建表成功，插入数据成功！"));
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
		int nFields = stmt->Fields();  //取得数据库表的列数
		szText = stmt->FieldName(0);        //得到列名

		while (stmt->NextRow())
		{
			CString strText = "";
			strText = stmt->ValueString(0);        //得到列值
			TRACE("%s\n", strText);
		}
	}

	delete stmt;





	return nRetCode;
}
