// DataInterviewClass.h: interface for the DataInterviewClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAINTERVIEWCLASS_H__6DCC5DF0_A8B8_40EE_9A7B_D26E01A04F00__INCLUDED_)
#define AFX_DATAINTERVIEWCLASS_H__6DCC5DF0_A8B8_40EE_9A7B_D26E01A04F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SPStruct.h"
//#include "Common.h"
//#include "interfacedefine.h"

class DataInterviewClass  
{
public:
	DataInterviewClass();
	virtual ~DataInterviewClass();
public:
	BOOL ExecSQL_NoRecord(LPCTSTR lpszSql);
	//设置访问数据库的用户、密码、服务器
	void			SetConnectParam(LPCTSTR strUser, LPCTSTR strPwd,LPCTSTR strServer);	

	//根据SQL语句操作数据
	_RecordsetPtr	exeSqlForAllData(LPCTSTR StrCMD);
	
	//插入采集数据
	BOOL			InsertGateGatherData(_CHNL_GATHER_DATA &_struGatherData);


	BOOL			OpenDB();
	BOOL			CloseDB();
private:
	void			BeginTrans(); // 
	void			CommitTrans(); //  
	void			RollbackTrans(); //	数据回滚
	
	void		    FuncInsertOneImage(unsigned char  * ImageData,int iImageSize,CString field);
	
	_RecordsetPtr	WorkFunc(LPCTSTR adCmd);

	BOOL PutCollectInfo(_RecordsetPtr Recordset, CString strField, char * pValue);
	BOOL PutCollectInfo(_RecordsetPtr Recordset, CString strField, int lValue);
	GUID GetGUID(void);

private:
	char			strConnection[1000];//连接字符串
	_ConnectionPtr	m_pConnection;
   	_RecordsetPtr	m_pUserSet;
	_CommandPtr		m_commandptr;
	BOOL            m_ConnectFlag;
	static char			strDataBaseName[100];
	static int      m_iFlag;
	
};

#endif // !defined(AFX_DATAINTERVIEWCLASS_H__6DCC5DF0_A8B8_40EE_9A7B_D26E01A04F00__INCLUDED_)



















